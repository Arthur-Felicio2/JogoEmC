# PLAN_FLUXO_E_DESIGN.md — Reestruturação de Fluxo, Paleta Preto & Roxo e UI Responsiva

> **Projeto:** Starlight Company (C++17, raylib)  
> **Objetivo deste documento:** Corrigir a ausência da função "Jogar novamente" nos jogos Par ou Ímpar e Jokenpô, ajustar o fluxo/rotas de navegação, modernizar a identidade visual para uma paleta temática **Preto e Roxo**, e reestruturar o layout dos botões e elementos para garantir alta responsividade visual.

---

## 1. Diagnóstico do Problema & Motivação

### 1.1 O que falhou no fluxo anterior?
Nos documentos anteriores (`PLAN.md` e `PLAN_AJUSTES.md`), a tela de fim de partida (`ResultScene`) foi projetada com múltiplas opções dispersas ou com acoplamento incorreto à reinicialização da `MatchScene`. Na prática:
1. **Perda de Contexto:** Ao clicar em reiniciar, os parâmetros da partida (`Player 1`, `Player 2` / `CPU`, `GameId`) eram perdidos ou a cena retornava para o menu principal em vez de recomeçar a disputa.
2. **Ciclo Incompleto:** O usuário ficava sem uma pergunta direta e assertiva de pós-jogo (**"Quer jogar novamente?"**) com respostas binárias (**SIM / NÃO**), gerando confusão na transição de estados.
3. **Duplicação / Falta de Gravação:** Sem uma máquina de estados explícita para o pós-partida, o registro de pontuação no `ScoreStore` ficava vulnerável a ser ignorado ou contabilizado múltiplas vezes ao tentar recomeçar.

### 1.2 O Fluxo Canônico Exigido

```
[ Menu Principal ]
       │
       ▼ (1. Escolhe o jogo: Par ou Ímpar / Jokenpô)
[ Modo 1P ou 2P ]
       │
       ▼ (2. Cria / Identifica o usuário)
[ NameEntryScene ]
       │
       ▼ (3. Joga a partida: Melhor de 3)
[ MatchScene ]
       │
       ▼ (4. Termina a partida: Alguém atinge 2 vitórias)
[ ResultScene / Modal "Quer jogar novamente?" ]
       ├──► [ SIM ] ────────────► Inicia nova partida direta (mesmos jogadores)
       └──► [ NÃO ] ────────────► Salva resultados e volta ao [ Menu Principal ]
```

---

## 2. Reestruturação do Fluxo e Rotas de Navegação

### 2.1 Especificação Detalhada das 5 Etapas

| Etapa | Cena / Estado | Ações do Usuário | Comportamento do Sistema |
|---|---|---|---|
| **1. Escolhe o Jogo** | `MenuScene` | Clica em **"Par ou Ímpar"** ou **"Jokenpô"** | Define `GameId` selecionado e avança para a seleção de modo/usuário. |
| **2. Cria o Usuário** | `NameEntryScene` | Digita o nome do Jogador 1 (e Jogador 2, se 2P) ou clica em perfil rápido existente | Valida caracteres (limite 12, Latin-1/acentos, nomes distintos no 2P). Cria instâncias de `HumanPlayer` (e `CpuPlayer` no 1P). |
| **3. Joga a Partida** | `MatchScene` | Executa as rodadas (escolha de dedos/jogadas, balanço das mãos, revelação e placar ●●○) | Executa as regras de `IGameRules` até que um jogador conquiste 2 rodadas na melhor de 3. |
| **4. Termina a Partida** | `ResultScene` | Visualiza o vencedor, troféu/efeitos e o painel de decisão pós-jogo | Registra a pontuação **uma única vez** no `ScoreStore` imediatamente ao abrir a tela. |
| **5. Decisão de Revanche** | `ResultScene` (Modal de Ação) | Pergunta central: **"Quer jogar novamente?"**<br>• **[ SIM ]**: Nova partida imediata<br>• **[ NÃO ]**: Volta ao menu | • Se **SIM**: Recarrega a `MatchScene` preservando `p1`, `p2`, `gameId`, zerando apenas o placar da partida.<br>• Se **NÃO**: Limpa o estado da partida e navega para `MenuScene`. |

### 2.2 Estrutura de Dados de Sessão (`GameSessionContext`)

Para que as cenas transitem dados sem acoplamento global nem vazamento de memória, adota-se um contexto de sessão único compartilhado:

```cpp
namespace sl {

enum class GameId { OddEven, Rps, Flappy };
enum class GameMode { SinglePlayer, TwoPlayers };

struct GameSessionContext {
    GameId gameId = GameId::Rps;
    GameMode mode = GameMode::SinglePlayer;
    std::string player1Name = "Jogador 1";
    std::string player2Name = "Jogador 2";
    int skinP1 = 0;
    int skinP2 = 1;

    // Placar da série atual (quantas partidas inteiras cada um venceu na sessão)
    int sessionWinsP1 = 0;
    int sessionWinsP2 = 0;

    void resetSession() {
        sessionWinsP1 = 0;
        sessionWinsP2 = 0;
    }
};

} // namespace sl
```

### 2.3 Lógica da `ResultScene` e Rota "Jogar Novamente"

A classe `ResultScene` deve implementar a guarda contra duplicação de pontuação e os disparadores das rotas:

```cpp
class ResultScene : public Scene {
private:
    GameSessionContext m_context;
    std::string m_winnerName;
    int m_scoreP1 = 0;
    int m_scoreP2 = 0;
    bool m_scoreRecorded = false;
    ScoreStore& m_store;

public:
    ResultScene(GameSessionContext ctx, const std::string& winner, int scoreP1, int scoreP2, ScoreStore& store)
        : m_context(ctx), m_winnerName(winner), m_scoreP1(scoreP1), m_scoreP2(scoreP2), m_store(store) {}

    void onEnter() override {
        // Gravação idempotente: só registra uma única vez
        if (!m_scoreRecorded) {
            bool p1Won = (m_winnerName == m_context.player1Name);
            m_store.recordMatch(m_context.player1Name, m_context.gameId, p1Won, m_scoreP1, m_scoreP2);
            
            if (m_context.mode == GameMode::TwoPlayers) {
                m_store.recordMatch(m_context.player2Name, m_context.gameId, !p1Won, m_scoreP2, m_scoreP1);
            }
            m_store.save();
            m_scoreRecorded = true;
        }
    }

    void onPlayAgain() {
        // [SIM] -> Reinicia partida direta com os mesmos jogadores
        SceneManager::get().replaceScene(std::make_unique<MatchScene>(m_context, m_store));
    }

    void onBackToMenu() {
        // [NÃO] -> Retorna ao Menu Principal
        m_context.resetSession();
        SceneManager::get().switchScene(std::make_unique<MenuScene>());
    }
};
```

---

## 3. Nova Paleta de Cores: "Preto & Roxo Moderno" (Dark Violet)

A antiga estética de "mesa verde" (`#0B3D2E` / `#145A43`) é integralmente substituída por uma paleta contemporânea baseada em tons escuros profundos e roxos em degradê com toques neon, proporcionando visual limpo, moderno e com contraste superior (WCAG AAA para textos).

### 3.1 Tokens de Cor e Referência Hexadecimal

| Token de Design | Cor Hex | RGBA raylib (`Color`) | Uso na Interface |
|---|---|---|---|
| `BgDark` | `#0A0713` | `{ 10, 7, 19, 255 }` | Fundo principal da janela e cantos externos |
| `BgDeepPurple` | `#150D2A` | `{ 21, 13, 42, 255 }` | Centro do degradê de fundo (efeito vinheta radial) |
| `CardSurface` | `#1A1333` | `{ 26, 19, 51, 235 }` | Superfície dos cartões flutuantes, HUD e modais |
| `CardSurfaceHover`| `#251C45` | `{ 37, 28, 69, 245 }` | Cartão selecionado ou focado |
| `BorderSubtle` | `#392666` | `{ 57, 38, 102, 255 }` | Bordas padrão de cartões e divisórias (1 a 2 px) |
| `BorderGlow` | `#8A3FFC` | `{ 138, 63, 252, 255 }` | Borda em foco, seleções e realces de botões |
| `PurplePrimary` | `#6C2BD9` | `{ 108, 43, 217, 255 }` | Cor base de botões primários e CTAs |
| `PurpleHover` | `#8B45FF` | `{ 139, 69, 255, 255 }` | Hover de botões primários (+iluminação) |
| `PurpleActive` | `#531CB8` | `{ 83, 28, 184, 255 }` | Estado pressionado (pressionamento com escala 0.98) |
| `PurpleNeon` | `#B784FF` | `{ 183, 132, 255, 255 }` | Detalhes neon, ícones, textos de ênfase e contadores |
| `TextBright` | `#FFFFFF` | `{ 255, 255, 255, 255 }` | Títulos e textos de alta prioridade |
| `TextNormal` | `#E1DBF4` | `{ 225, 219, 244, 255 }` | Rótulos de botões, nomes de jogadores e instruções |
| `TextMuted` | `#9B8EB8` | `{ 155, 142, 184, 255 }` | Informações secundárias, rodapés e atalhos |
| `SuccessGreen` | `#10B981` | `{ 16, 185, 129, 255 }` | Indicador de vitória, botão "SIM" / "Jogar novamente" |
| `DangerRed` | `#EF4444` | `{ 239, 68, 68, 255 }` | Botão "NÃO" / "Voltar ao Menu", avisos e derrotas |
| `AccentGold` | `#F59E0B` | `{ 245, 158, 11, 255 }` | Estrelas, troféus e marcadores de melhor de 3 |

### 3.2 Atualização do `Theme.h`

```cpp
#pragma once
#include <raylib.h>

namespace sl {

struct Theme {
    // Fundos e Superfícies
    static constexpr Color BgDark          = { 10, 7, 19, 255 };      // #0A0713
    static constexpr Color BgDeepPurple    = { 21, 13, 42, 255 };     // #150D2A
    static constexpr Color CardSurface     = { 26, 19, 51, 235 };     // #1A1333 (com alpha)
    static constexpr Color CardSurfaceHover= { 37, 28, 69, 245 };     // #251C45
    static constexpr Color CardSurfaceDark = { 16, 11, 31, 240 };     // #100B1F

    // Bordas e Efeitos
    static constexpr Color BorderSubtle    = { 57, 38, 102, 255 };    // #392666
    static constexpr Color BorderGlow      = { 138, 63, 252, 255 };   // #8A3FFC
    static constexpr Color ShadowColor     = { 4, 2, 8, 160 };

    // Roxo Principal (Botões e Destaques)
    static constexpr Color Primary         = { 108, 43, 217, 255 };   // #6C2BD9
    static constexpr Color PrimaryHover    = { 139, 69, 255, 255 };   // #8B45FF
    static constexpr Color PrimaryPressed  = { 83, 28, 184, 255 };    // #531CB8
    static constexpr Color NeonPurple      = { 183, 132, 255, 255 };  // #B784FF

    // Tipografia
    static constexpr Color TextBright      = { 255, 255, 255, 255 };  // #FFFFFF
    static constexpr Color TextNormal      = { 225, 219, 244, 255 };  // #E1DBF4
    static constexpr Color TextMuted       = { 155, 142, 184, 255 };  // #9B8EB8

    // Semânticas e Ações
    static constexpr Color ActionPlayAgain = { 16, 185, 129, 255 };   // #10B981 (Verde esmeralda harmônico)
    static constexpr Color ActionMenu      = { 239, 68, 68, 255 };    // #EF4444 (Vermelho rubi)
    static constexpr Color TrophyGold      = { 245, 158, 11, 255 };   // #F59E0B
};

} // namespace sl
```

---

## 4. Layout dos Elementos e Responsividade

### 4.1 Filosofia de Responsividade (1280×720 Canvas Virtual)
O raylib opera com um sistema de coordenadas virtual fixo em **1280×720** escalado proporcionalmente via `RenderTexture2D` com letterboxing (barras pretas automáticas mantendo a proporção de aspecto). Para garantir visual limpo e ergonomia:
1. **Âncoras Percentuais:** Todos os painéis calculam sua posição central baseados em `GetScreenWidth()` e `GetScreenHeight()` (ou `VIRTUAL_WIDTH / 2` e `VIRTUAL_HEIGHT / 2`).
2. **Larguras Clamped:** Nenhum botão possui largura arbitrária estática colada na lateral. Larguras de botões de menu variam de 360 px a 440 px, centralizados no eixo X.
3. **Alturas Ergonômicas:** Botões de ação possuem altura mínima de **56 px** (área de clique confortável) com raio de arredondamento de **12 px**.
4. **Espaçamentos Modulares:** Margens verticais e horizontais em múltiplos de 8 px (16, 24, 32, 48 px).

### 4.2 Blueprint: Menu Principal (`MenuScene`)

```
+-------------------------------------------------------------------------+
|                                                                         |
|                       [ LOGO: STARLIGHT COMPANY ]                       |
|                     "Par ou Ímpar  •  Jokenpô  •  Bird"                 |
|                                                                         |
|                +---------------------------------------+                |
|                |         [ 1. PAR OU ÍMPAR ]           |  (H: 60px)     |
|                +---------------------------------------+                |
|                                   │ (gap: 16px)                         |
|                +---------------------------------------+                |
|                |            [ 2. JOKENPÔ ]             |  (H: 60px)     |
|                +---------------------------------------+                |
|                                   │                                     |
|                +---------------------------------------+                |
|                |         [ 3. STARLIGHT BIRD ]         |  (H: 60px)     |
|                +---------------------------------------+                |
|                                   │                                     |
|                +---------------------------------------+                |
|                |             [ 🏆 PLACAR ]             |  (H: 52px)     |
|                +---------------------------------------+                |
|                                   │                                     |
|                +---------------------------------------+                |
|                |              [ 🚪 SAIR ]              |  (H: 52px)     |
|                +---------------------------------------+                |
|                                                                         |
|   v2.0 • Paleta Preto & Roxo                 Pressione [ENTER] / Clique |
+-------------------------------------------------------------------------+
```

### 4.3 Blueprint: Criação de Usuário (`NameEntryScene`)

```
+-------------------------------------------------------------------------+
|                              [ VOLTAR ]                                 |
|                                                                         |
|                +---------------------------------------+                |
|                |          QUEM VAI JOGAR?              |                |
|                |                                       |                |
|                |  Jogador 1 (Nome):                    |                |
|                |  [ Leonardo____________ ]  [Ícones]   |                |
|                |                                       |                |
|                |  Jogador 2 (se 2P):                   |                |
|                |  [ Beatriz_____________ ]  [Ícones]   |                |
|                |                                       |                |
|                |  Perfis Recentes:                     |                |
|                |  [+ Leonardo] [+ Beatriz] [+ Convidado]                |
|                |                                       |                |
|                |      +-------------------------+      |                |
|                |      |   INICIAR PARTIDA ->    |      | (Destaque)     |
|                |      +-------------------------+      |                |
|                +---------------------------------------+                |
|                                                                         |
+-------------------------------------------------------------------------+
```

### 4.4 Blueprint: HUD e Tela de Partida (`MatchScene`)

```
+-------------------------------------------------------------------------+
| [<- MENU]                       RODADA 2                       [REGRAS] |
|                                                                         |
| +---------------------+                           +-------------------+ |
| | LEONARDO (P1)       |                           | COMPUTADOR (CPU)  | |
| | Vitórias: ● ○ ○     |                           | Vitórias: ● ● ○   | |
| +---------------------+                           +-------------------+ |
|                                                                         |
|                 [ MÃO P1 ]             [ MÃO P2 ]                       |
|               (Pixel Art 5x)         (Pixel Art 5x)                     |
|                                                                         |
|                                                                         |
|                      +-----------------------+                          |
|                      |   FAÇA SUA JOGADA!    |                          |
|                      +-----------------------+                          |
|                                                                         |
|  Par ou Ímpar:  [ 0 ]   [ 1 ]   [ 2 ]   [ 3 ]   [ 4 ]   [ 5 ]           |
|  Jokenpô:          [ ✊ PEDRA ]   [ ✋ PAPEL ]   [ ✌️ TESOURA ]          |
+-------------------------------------------------------------------------+
```

### 4.5 Blueprint: Encerramento e "Quer Jogar Novamente?" (`ResultScene`)

Este é o elemento central da correção solicitada: um **Modal Card Responsivo** focado na pergunta de pós-jogo:

```
+-------------------------------------------------------------------------+
|                                                                         |
|               +---------------------------------------+                 |
|               |              🎉 VITÓRIA!              |                 |
|               |          LEONARDO VENCEU O JOGO       |                 |
|               |          Placar Final: 2 x 1          |                 |
|               | ───────────────────────────────────── |                 |
|               |                                       |                 |
|               |        QUER JOGAR NOVAMENTE?          |                 |
|               |                                       |                 |
|               |  +---------------------------------+  |                 |
|               |  |     [ SIM - JOGAR NOVAMENTE ]   |  | (Destaque Roxo/ |
|               |  +---------------------------------+  |  Verde, Enter)  |
|               |                   │                   |                 |
|               |  +---------------------------------+  |                 |
|               |  |     [ NÃO - VOLTAR AO MENU ]    |  | (Borda Vermelha/|
|               |  +---------------------------------+  |  Cinza, Esc)    |
|               |                                       |                 |
|               |  [ 🏆 Ver Placar ]   [ 👥 Trocar Nomes ] | (Secundários) |
|               +---------------------------------------+                 |
|                                                                         |
+-------------------------------------------------------------------------+
```

---

## 5. Implementação dos Componentes de UI (Código em C++)

### 5.1 Botão Responsivo com Feedback Visual (`src/ui/Button.h`)

O botão deve reagir a três estados (normal, hover, pressed), com animação de escala e borda iluminada por neon:

```cpp
#pragma once
#include <raylib.h>
#include <string>
#include "Theme.h"

namespace sl {

class ModernButton {
public:
    Rectangle bounds;
    std::string text;
    Color baseColor;
    Color hoverColor;
    Color textColor;
    bool isPrimary = false;

    ModernButton(Rectangle b, const std::string& t, bool primary = false)
        : bounds(b), text(t), isPrimary(primary) {
        if (primary) {
            baseColor = Theme::Primary;
            hoverColor = Theme::PrimaryHover;
            textColor = Theme::TextBright;
        } else {
            baseColor = Theme::CardSurface;
            hoverColor = Theme::CardSurfaceHover;
            textColor = Theme::TextNormal;
        }
    }

    bool updateAndDraw(Font font) {
        Vector2 mouse = GetMousePosition();
        bool hovered = CheckCollisionPointRec(mouse, bounds);
        bool clicked = hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        // Feedback de cor e sombra
        Color currentBg = hovered ? hoverColor : baseColor;
        Color borderCol = hovered ? Theme::BorderGlow : Theme::BorderSubtle;

        // Efeito de elevação / sombra no hover
        if (hovered) {
            DrawRectangleRounded({ bounds.x, bounds.y + 4, bounds.width, bounds.height }, 0.25f, 8, Theme::ShadowColor);
        }

        // Desenho do corpo do botão
        DrawRectangleRounded(bounds, 0.25f, 8, currentBg);
        DrawRectangleRoundedLines(bounds, 0.25f, 8, hovered ? 2.5f : 1.5f, borderCol);

        // Alinhamento centralizado do texto
        float fontSize = 22.0f;
        Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 1.0f);
        Vector2 textPos = {
            bounds.x + (bounds.width - textSize.x) * 0.5f,
            bounds.y + (bounds.height - textSize.y) * 0.5f
        };

        DrawTextEx(font, text.c_str(), textPos, fontSize, 1.0f, hovered ? Theme::TextBright : textColor);

        return clicked;
    }
};

} // namespace sl
```

### 5.2 Helper de Layout Centralizado (`src/ui/UiLayout.h`)

Para evitar posições "mágicas" hardcoded em pixels, implementa-se um helper responsivo para centralizar cartões e distribuir pilhas verticais:

```cpp
#pragma once
#include <raylib.h>

namespace sl {

struct UiLayout {
    // Retorna um retângulo centralizado na tela lógica
    static Rectangle centerCard(float width, float height) {
        float sw = (float)GetScreenWidth();
        float sh = (float)GetScreenHeight();
        return Rectangle{ (sw - width) * 0.5f, (sh - height) * 0.5f, width, height };
    }

    // Calcula retângulo de botão em uma lista vertical centralizada
    static Rectangle vButton(Rectangle container, int index, int totalButtons, float btnHeight, float gap) {
        float totalHeight = (totalButtons * btnHeight) + ((totalButtons - 1) * gap);
        float startY = container.y + (container.height - totalHeight) * 0.5f;
        float btnY = startY + index * (btnHeight + gap);
        float btnWidth = container.width * 0.85f;
        float btnX = container.x + (container.width - btnWidth) * 0.5f;

        return Rectangle{ btnX, btnY, btnWidth, btnHeight };
    }
};

} // namespace sl
```

---

## 6. Roteiro de Modificação nos Arquivos do Projeto

| Arquivo | Mudança Necessária |
|---|---|
| `src/ui/Theme.h` | Substituir toda a paleta verde pelos tokens **Preto e Roxo** (`BgDark`, `BgDeepPurple`, `Primary`, `NeonPurple`). |
| `src/scenes/MenuScene.cpp` | Aplicar gradiente roxo no fundo, centralizar botões com `UiLayout::centerCard`, conectar seleção de jogo à `NameEntryScene`. |
| `src/scenes/NameEntryScene.cpp` | Configurar o formulário para aceitar o nome do usuário e avançar diretamente para `MatchScene` passando o `GameSessionContext`. |
| `src/scenes/MatchScene.cpp` | Executar o jogo com placar ●●○ no topo; ao detectar vencedor da série (2 vitórias), chamar `SceneManager::get().replaceScene(std::make_unique<ResultScene>(...))`. |
| `src/scenes/ResultScene.cpp` | Substituir a tela antiga pelo **Card Central "Quer jogar novamente?"** com botões prioritários `[SIM]` e `[NÃO]`. |
| `src/core/SceneManager.h` | Adicionar método `replaceScene()` para substituir a cena atual sem deixar histórico acumulando na pilha. |

---

## 7. Fases de Execução e Checklist de Aceite

### Fase 1 — Rotas e "Jogar Novamente" (Prioridade Alta)
- [x] Criar a struct `GameSessionContext` para carregar nomes dos jogadores e opções de jogo entre as cenas.
- [x] Implementar a `ResultScene` com a pergunta explícita: **"Quer jogar novamente?"**.
- [x] Fazer o botão **"SIM"** instanciar nova `MatchScene` reutilizando os mesmos jogadores e zerando as rodadas.
- [x] Fazer o botão **"NÃO"** salvar dados e retornar com segurança à `MenuScene`.
- [x] Adicionar atalho de teclado: tecla **ENTER** aciona o "SIM"; tecla **ESC** aciona o "NÃO".
- [x] Testar idempotência: garantir que o placar no `scores.txt` seja gravado exatamente uma única vez por partida terminada.

### Fase 2 — Aplicação da Paleta Preto & Roxo (Prioridade Alta)
- [x] Atualizar `Theme.h` com as cores hexadecimais `#0A0713`, `#150D2A`, `#1A1333`, `#6C2BD9`, `#B784FF`.
- [x] Atualizar o shader ou rotina de desenho de fundo para desenhar o gradiente suave preto-roxo.
- [x] Estilizar cartões, caixas de texto e janelas com bordas `BorderSubtle` (`#392666`) e cantos arredondados.
- [x] Atualizar o HUD de pontuação durante a partida para os cartões escuros com texto branco/lilás.

### Fase 3 — Ajustes de Disposição e Responsividade dos Botões (Prioridade Média)
- [x] Integrar `UiLayout` nos menus para que todos os botões fiquem centralizados e alinhados automaticamente.
- [x] Ajustar altura dos botões para 56–60 px e padding interno para textos não colidirem nas bordas.
- [x] Adicionar efeito hover com elevação visual (sombra suave + brilho de borda violeta).
- [x] Verificar a disposição das 6 poses de mão (pixel art) sobre o novo fundo escuro, garantindo contraste perfeito.

### Fase 4 — Testes e Validação Final (Critérios de Aceite)
- [x] **Fluxo Par ou Ímpar:** Menu -> Escolhe Par ou Ímpar -> Cria/Digita Nome -> Joga Partida (Melhor de 3) -> Tela de Resultado pergunta "Quer jogar novamente?" -> [SIM] Inicia nova partida imediata -> [NÃO] Volta ao Menu.
- [x] **Fluxo Jokenpô:** Menu -> Escolhe Jokenpô -> Cria/Digita Nome -> Joga Partida (Melhor de 3) -> Tela de Resultado pergunta "Quer jogar novamente?" -> [SIM] Inicia nova partida imediata -> [NÃO] Volta ao Menu.
- [x] **Persistência do Placar:** Jogar 3 partidas seguidas clicando em [SIM] e conferir se o `scores.txt` acumulou as 3 vitórias/derrotas corretamente sem duplicidade.
- [x] **Estética:** Nenhuma tela exibe resquícios da cor verde antiga. Toda a navegação é imersiva em preto e roxo.
- [x] **Responsividade:** Janela redimensionada ou em tela cheia mantém todos os elementos centrados, sem cortes de texto nem botões desalinhados.

