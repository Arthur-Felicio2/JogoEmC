# PLAN.md — Modernização do jogo Starlight Company

> Projeto original: console Windows (C/C++), jogos **Par ou Ímpar** e **Jokenpo** (1P e 2P).
> Meta: nova versão para apresentação, com botões, mãos realistas, animações, layout moderno e um terceiro jogo (Flappy).

---

## 1. Objetivos

| # | Objetivo | Resultado esperado |
|---|---|---|
| 1 | Botões | Toda a navegação e as jogadas por mouse/teclado, com estados visuais |
| 2 | Mãos realistas | Sprites com formato e tom de pele próximos da realidade |
| 3 | Animações no gameplay | Contagem, balanço das mãos, revelação, vitória, transições |
| 4 | Layout moderno | Design system único (cores, fontes, cartões, placar) |
| 5 | Terceiro jogo | Flappy próprio ("Starlight Bird") integrado ao menu |

**Fora de escopo:** multiplayer online, loja/progressão, 3D.

---

## 2. Diagnóstico do código atual (itens a corrigir na migração)

| Problema | Onde | Tratamento |
|---|---|---|
| Só compila como C++ (inicializadores na `struct`) | `typedef struct player` | Migrar para C++17 de forma assumida |
| Leitura além do fim das strings (`largura` > tamanho da linha) | `logo`, `trofeu`, `hand*` | Substituir por sprites; sem matrizes em string |
| 6 funções de mão e 4 funções de jogo duplicadas | `hand*`, `singleplayer`, `multplayer`, `jokenpo`, `singjokenpo` | Classes `IGameRules` + `Match` |
| Validação de `qtdplayers` grava em `game` (loop infinito) | `main` | Menu por botões; sem validação textual |
| "Melhor de 3" são 3 rodadas fixas | todos os modos | `Match` com primeiro a 2 vitórias |
| `gets`, `fflush(stdin)`, `main()` sem tipo, `scanf` sem checagem | geral | Removidos (entrada por botões) |
| `srand` a cada rodada | single | `std::mt19937` criado uma vez |
| Renderização por `printf` + cursor (lenta, pisca) | geral | Renderização gráfica com game loop |

**Comportamento a preservar:** no 2P, a jogada do primeiro jogador não pode ficar visível (antes: verde sobre verde). Passa a ser a **tela "Passe ao Jogador 2"**.

---

## 3. Decisões técnicas

| Tema | Decisão (padrão proposto) | Alternativa |
|---|---|---|
| Linguagem | C++17 | — |
| Biblioteca gráfica | **raylib** (janela, input, texturas, fontes, áudio) | SFML se a equipe preferir API 100% OO |
| Build | CMake (raylib via `FetchContent` ou pasta `third_party/`) | Makefile para MinGW/w64devkit |
| Plataforma | Windows (MinGW ou MSVC); código sem chamadas Win32 | Linux/macOS sem mudanças |
| Resolução | Lógica 1280×720, escalada para a janela | — |
| Física/animação | Delta time; física do Flappy em passo fixo de 60 Hz | — |
| Testes | Catch2 ou asserts simples apenas na camada de regras | — |
| Controle de versão | Git, uma branch por fase | — |

> Se o compilador da apresentação for o Dev-C++ antigo, validar o suporte a C++17 na Fase 0. Se não houver, usar MinGW atualizado ou w64devkit.

---

## 4. Arquitetura

```
StarlightGames/
├─ CMakeLists.txt
├─ PLAN.md
├─ assets/
│  ├─ hands/        pose_0..pose_5 (base, sombra, contorno)
│  ├─ flappy/       bird_0..2, pipe, bg_far, bg_near, ground
│  ├─ fonts/        Titulo.ttf, Corpo.ttf
│  ├─ ui/           logo, icones, trofeu
│  └─ sfx/          (opcional)
├─ src/
│  ├─ main.cpp
│  ├─ core/    Application, Scene, SceneManager, AssetManager, Input, Config
│  ├─ ui/      Theme, Widget, Button, Label, Panel, Layout
│  ├─ fx/      Tween, Easing, Timeline, ParticleSystem
│  ├─ games/   IGameRules, OddEvenRules, RpsRules, IPlayer, HumanPlayer, CpuPlayer, Match
│  ├─ render/  HandRenderer, ScoreBoard, Trophy
│  ├─ scenes/  SplashScene, MenuScene, ModeScene, PassDeviceScene,
│  │           MatchScene, ResultScene, FlappyScene, CreditsScene
│  └─ save/    SaveData (recorde do Flappy)
└─ tests/      rules_tests.cpp
```

### Interfaces principais

```cpp
enum class Outcome { WinA, WinB, Draw };

class IGameRules {
public:
    virtual ~IGameRules() = default;
    virtual int  moveCount() const = 0;             // opções válidas
    virtual Outcome resolve(int a, int b) const = 0;
};

class IPlayer {
public:
    virtual ~IPlayer() = default;
    virtual const std::string& name() const = 0;
    virtual bool isHuman() const = 0;
    virtual int  chooseMove(const IGameRules&) = 0; // CPU decide; humano usa UI
};

class Scene {
public:
    virtual ~Scene() = default;
    virtual void onEnter() {}
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
};
```

- **Strategy:** `OddEvenRules` e `RpsRules` implementam `IGameRules`.
- **State/Scene:** `SceneManager` controla pilha e transições.
- **Polimorfismo:** `HumanPlayer` e `CpuPlayer` usam a mesma interface.

---

## 5. Especificações

### 5.1 Regras e partida (`Match`)

- **Par ou Ímpar:** cada jogador escolhe 0–5 dedos; soma par vence quem escolheu par. Não há empate.
- **Jokenpo:** pedra (0), papel (1), tesoura (2). **Empate repete a rodada** e não conta no placar.
- **Partida:** melhor de 3 (primeiro a 2 vitórias). Encerra assim que alguém chega a 2.
- **Fim da partida:** tela de resultado com "Revanche" (mesmos jogadores) e "Menu".
- **Mapeamento das mãos (igual ao original):** pedra = pose 0, papel = pose 5, tesoura = pose 2. Basta criar **6 poses**.

### 5.2 Design system

| Item | Valor inicial |
|---|---|
| Fundo | Gradiente `#0B3D2E` → `#145A43` (mantém a "mesa verde") |
| Cartão | Branco 8% de opacidade, borda `#FFFFFF22`, raio 16 px, sombra suave |
| Destaque | Dourado `#F4B942` |
| Texto | `#F5F7F2` (principal), `#B8C9BF` (secundário) |
| Alerta/derrota | `#E4572E` |
| Fontes | 1 de título (display) e 1 de corpo (sans); licença livre verificada |
| Botão | 56 px de altura mínima, hover (+6% brilho), pressionado (escala 0,97), desabilitado (50% opacidade) |
| Margens | Grade de 8 px |

**Telas:** Splash → Menu (Par/Ímpar, Jokenpo, Flappy, Créditos, Sair) → Modo (1P/2P) → Partida → Resultado.
**HUD da partida:** cartões laterais com nome e pontos, indicador de rodadas (●●○) e botão "Menu".

### 5.3 Mãos

- Arte em PNG 512×512 transparente, **três camadas** por pose: base (cor de pele), sombra/luz (multiply) e contorno.
- Cor final = base tingida pelo tom escolhido + camada de sombra. Isso permite 3–5 tons de pele sem redesenhar.
- Jogador 2 usa a pose espelhada horizontalmente, entrando pelo lado direito.
- Requisitos visuais: proporção natural de dedos e polegar, unhas, dobras nas articulações, sombra sob a mão.
- Licença: arte própria ou de banco com licença verificada, documentada em `assets/LICENSES.md`.

### 5.4 Animações

**Sequência de revelação (tempos iniciais, ajustáveis):**

| Tempo | Evento |
|---|---|
| 0,0–0,4 s | Mãos entram pelos lados (`easeOutBack`) |
| 0,4–1,6 s | Três balanços, com a contagem "1… 2… 3…" |
| 1,6 s | Revelação da pose com "punch" de escala (1,0 → 1,25 → 1,0 em 0,25 s) |
| 1,9 s | No Par/Ímpar, a soma aparece entre as mãos e conta até o valor |
| 2,5 s | Texto do resultado com fade e slide |
| 2,5–4,5 s | Confetes e troféu com bounce para o vencedor |

**Demais animações:**
- Transição entre cenas: fade/slide de 0,3 s.
- Botões: hover, pressed e foco.
- Placar: número subindo a cada ponto.
- Menu: mão acenando e logo pulsando suavemente.

### 5.5 Flappy ("Starlight Bird")

| Parâmetro | Valor inicial |
|---|---|
| Gravidade | 1800 px/s² |
| Impulso | −520 px/s |
| Velocidade dos canos | 220 px/s (+3% a cada 10 pontos, até um teto) |
| Abertura | 190 px (reduz levemente com o nível) |
| Espaçamento | 320 px |
| Largura do cano | 90 px |

- Estados: **Pronto → Jogando → Morto → Game Over**.
- Entrada: espaço, clique ou botão na tela.
- Colisão círculo × retângulo, mais chão e teto.
- Pássaro com 3 frames de asa e rotação conforme a velocidade.
- Fundo com parallax em 2 camadas.
- Recorde persistido em arquivo (`save/scores.dat`).
- Arte e nome próprios, sem reutilizar assets do jogo original.

---

## 6. Fases e tarefas

> Esforço relativo: **P** pequeno, **M** médio, **G** grande.

### Fase 0 — Fundação (P)
- [x] Criar repositório e estrutura de pastas
- [x] Configurar CMake e compilar uma janela raylib
- [x] Game loop com delta time e escala 1280×720
- [x] Definir padrão de código (nomes, namespaces, formatação)

**Pronto quando:** janela abre, fecha e roda a 60 FPS no computador da apresentação.

### Fase 1 — Núcleo de UI e cenas (M)
- [x] `Scene` e `SceneManager` (troca com fade)
- [x] `Theme` com paleta e fontes
- [x] `Button`, `Label`, `Panel`, layout vertical/horizontal
- [x] `MenuScene` com navegação por mouse e teclado

**Pronto quando:** o menu navega entre cenas vazias com botões completos (hover, clique, foco).

### Fase 2 — Regras e partida (M)
- [x] `IGameRules`, `OddEvenRules`, `RpsRules`
- [x] `IPlayer`, `HumanPlayer`, `CpuPlayer`
- [x] `Match` (melhor de 3, empate repete no Jokenpo)
- [x] Testes: todas as combinações de cada jogo e o fim da partida

**Pronto quando:** os testes passam sem usar tela e o resultado bate com o jogo original.

### Fase 3 — Telas de partida (G)
- [x] `ModeScene` (1P/2P) e entrada de nomes por campo de texto
- [x] `MatchScene`: botões 0–5, par/ímpar e pedra/papel/tesoura
- [x] `PassDeviceScene` para ocultar a jogada no 2P
- [x] `ScoreBoard` e `ResultScene` com revanche

**Pronto quando:** as 4 combinações (2 jogos × 1P/2P) jogam do início ao fim, sem tela de texto.

### Fase 4 — Mãos (G)
- [x] Definir estilo da arte e produzir as 6 poses (3 camadas)
- [x] `HandRenderer` com tingimento por tom de pele e espelhamento
- [x] Seletor de tom de pele (opcional)
- [x] `assets/LICENSES.md`

**Pronto quando:** as 6 poses aparecem corretamente para os dois jogadores, em todos os tons.

### Fase 5 — Animações e efeitos (M)
- [x] `Tween`, `Easing`, `Timeline`
- [x] Sequência de revelação (seção 5.4)
- [x] `ParticleSystem` (confetes) e troféu animado
- [x] Transições e microanimações de botões

**Pronto quando:** a revelação roda sem engasgos e o tempo total de uma rodada fica abaixo de 5 s.

### Fase 6 — Flappy (M)
- [x] `FlappyScene` com estados, física e geração de canos
- [x] Colisão, pontuação e recorde salvo
- [x] Arte do pássaro, canos e fundo
- [x] Integração ao menu

**Pronto quando:** dá para jogar vários minutos sem travar, o recorde persiste ao reabrir e a dificuldade sobe de forma justa.

### Fase 7 — Polimento e entrega (P)
- [x] `CreditsScene` com os nomes da dupla e a turma (INI-1A)
- [x] Sons (opcional) e controle de volume
- [x] Tela cheia e redimensionamento
- [x] Empacotar `exe` + pasta `assets` e testar em máquina limpa
- [x] Roteiro de demonstração para a apresentação

**Pronto quando:** o pacote roda em outro computador sem instalar nada.

---

## 7. Rastreabilidade (objetivo → fase)

| Objetivo | Fases |
|---|---|
| 1. Botões | 1, 3 |
| 2. Mãos realistas | 4 |
| 3. Animações | 5 (e 3 para os estados) |
| 4. Layout moderno | 1, 3, 7 |
| 5. Flappy | 6 |

---

## 8. Riscos e mitigação

| Risco | Impacto | Mitigação |
|---|---|---|
| Arte das mãos demora ou foge do estilo | Alto | Fixar estilo e uma pose-modelo antes da Fase 4; plano B com vetor desenhado por código |
| Licença de assets | Médio | Registrar fonte e licença de cada arquivo |
| Incompatibilidade de compilador | Médio | Validar na Fase 0, no computador da apresentação |
| Desempenho em PC fraco | Baixo | Texturas ≤ 512 px, partículas com limite |
| Escopo crescer | Médio | Congelar o escopo após a Fase 3; extras só no backlog |

---

## 9. Backlog opcional

- Efeitos sonoros e música
- Seletor de tom de pele persistido
- Flappy para 2 jogadores alternados
- Exportar para web (WebAssembly) para apresentar no navegador
- Estatísticas de partidas

---

## 10. Definição de pronto (apresentação)

- [x] Os 3 jogos abrem pelo menu, sem usar o teclado para digitar números
- [x] Par/Ímpar e Jokenpo funcionam em 1P e 2P, com melhor de 3 correto
- [x] Mãos com a aparência definida e animações de revelação
- [x] Layout consistente em todas as telas
- [x] Nenhum crash em 30 minutos de uso contínuo
- [x] Pacote testado em uma máquina limpa
- [x] Script de build (`build.bat`), execução (`run.bat`) e testes (`test.bat`) prontos para demonstração

