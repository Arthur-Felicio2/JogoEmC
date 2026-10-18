# PLAN_AJUSTES.md — Ajustes pós-implementação (Starlight Company)

> Complemento do `PLAN.md`, que já foi implementado por completo. Este plano cobre **apenas** os ajustes pedidos depois do primeiro build.

---

## 1. Escopo

| # | Ajuste | Situação atual |
|---|---|---|
| A1 | **Mãos em pixel art/cartoon** | Mãos feitas de círculos e retângulos, com contornos de construção aparecendo |
| A2 | **Escolha de nome do jogador** | Removida; o placar não consegue identificar ninguém |
| A3 | **"Jogar novamente"** em Par ou Ímpar e Jokenpô | Não existe no fim da partida |
| A4 | **Placar** | Não existe (nem na partida, nem acumulado) |
| A5 | **Pontuação dos 3 jogos** por jogador | Inclui o Flappy, que hoje só guarda um recorde sem dono |

**Fora de escopo:** qualquer mudança em regras, animações, layout geral ou Flappy além de identificar o jogador e registrar o recorde.

---

## 2. Insumos prontos (pasta `starlight-update/`)

| Arquivo | Para que serve |
|---|---|
| `src/render/HandSprites.h` | 6 poses de mão (0–5 dedos) em 48×64 px, paleta indexada, 4 tons de pele, 2 mangas (azul e vermelha). Gerado por script |
| `src/render/HandAtlasRaylib.h` | Classe `sl::HandAtlas`: cria texturas com filtro ponto e desenha a mão com pivô no pulso |
| `src/save/ScoreStore.h` | Perfis e placar dos 3 jogos, com gravação em arquivo. Só biblioteca padrão |
| `tools/gen_hands.py` | Gerador das mãos. Ajustes de arte: editar, rodar e conferir `hands_preview.png` |
| `tests/*_test.cpp` | Testes dos dois cabeçalhos, compilam sem raylib |

**API que será usada**

```cpp
// Mãos
sl::HandAtlas atlas;   // depois de InitWindow(); chamar atlas.unloadAll() antes de CloseWindow()
atlas.draw(pose, skin, sleeve, mirror, wristPos, scale, rotationDeg, tint);
// pose: pedra = 0, papel = 5, tesoura = 2 | mirror = true para o jogador da direita

// Placar
sl::ScoreStore store("scores.txt");
store.load();
store.recordMatch(nome, sl::GameId::Rps, venceu, rodadasGanhas, rodadasPerdidas);
bool novoRecorde = store.recordFlappy(nome, pontos);
store.ranking(sl::GameId::Flappy);   // por jogo
store.overallRanking();              // geral (pontos dos 3 jogos)
store.save();
```

> Os nomes das classes citadas abaixo (`MatchScene`, `ResultScene`, `ModeScene`, `HandRenderer`, `SaveData`) seguem o `PLAN.md`. Se no código estiverem com outro nome, adapte.

---

## 3. Fluxo alvo

```
Menu ─┬─ Par ou Ímpar ─┐
      ├─ Jokenpô ──────┼─► Modo (1P/2P) ─► NameEntryScene ─► MatchScene ─► ResultScene
      ├─ Flappy ───────┴─► NameEntryScene (1 nome) ─► FlappyScene ─► Game Over
      └─ Placar ───────────────────────────────► ScoreScene

ResultScene:  [Jogar novamente] ─► MatchScene (mesmos jogadores, placar da partida zerado)
              [Trocar jogadores] ─► NameEntryScene
              [Placar] ─► ScoreScene   |   [Menu]
```

---

## 4. Fases

> Esforço relativo: **P** pequeno, **M** médio.

#### Fase A — Integração dos módulos (P)

- [x] Copiar `src/render/` e `src/save/ScoreStore.h` para o projeto (são só cabeçalhos)
- [x] Incluir os caminhos no CMake; garantir C++17 (`<filesystem>`)
- [x] Copiar os testes e adicioná-los ao `test.bat`
- [x] Instanciar um único `ScoreStore` e um único `HandAtlas` na aplicação, disponíveis às cenas
- [x] Definir o caminho do arquivo: `scores.txt` ao lado do `.exe`

**Pronto quando:** o projeto compila com os novos cabeçalhos e os testes passam.

### Fase B — Mãos em pixel art (M) · A1

- [x] Substituir o desenho do `HandRenderer` por chamadas a `HandAtlas::draw`
- [x] Mapear as jogadas: Par ou Ímpar usa a pose = número escolhido (0–5); Jokenpô usa pedra = 0, papel = 5, tesoura = 2
- [x] Jogador da direita usa `mirror = true`; cada jogador recebe uma manga (azul e vermelha)
- [x] Usar escala inteira (4, 5 ou 6) no estado normal
- [x] Balanço da contagem: `rotationDeg` oscilando em torno do pulso
- [x] "Punch" da revelação: escala animada de 1,0 a 1,25 e de volta. Durante os 0,25 s os pixels ficam levemente irregulares, o que é aceitável
- [x] Reposicionar a sombra no chão e o painel de resultado para a nova altura da mão (48×64 px × escala)
- [x] Manter o seletor de tom de pele (índices 0–3 de `kSkins`)
- [x] Remover o código antigo das formas vetoriais (círculos e retângulos)

**Pronto quando:** as 6 poses aparecem nos dois lados da tela, com pixels nítidos, sem contornos de construção e sem borrar.

### Fase C — Identificação do jogador (M) · A2

- [x] Criar a `NameEntryScene` (ou recolocar o campo na `ModeScene`, se já existir a estrutura)
- [x] 1P: um campo (a CPU não tem perfil). 2P: dois campos
- [x] Flappy: um campo
- [x] Limite de 12 caracteres; Enter confirma, Esc volta, Backspace remove **um caractere** (não um byte)
- [x] Aceitar acentos: a fonte carregada precisa conter os glifos de Latin-1 (`0x20`–`0xFF`), e a entrada deve converter o código lido para UTF-8
- [x] Atalhos com os perfis já existentes (`store.all()`), em botões, para não redigitar
- [x] Campo vazio vira "Jogador 1" / "Jogador 2"
- [x] No 2P, nomes iguais (sem diferenciar maiúsculas) são bloqueados com mensagem
- [x] Passar os nomes para a partida, para o HUD e para a tela "Passe ao Jogador 2"

**Pronto quando:** toda partida e toda tentativa no Flappy tem um jogador identificado, e o nome aparece no HUD.

### Fase D — Fim de partida e "Jogar novamente" (M) · A3

- [x] `ResultScene` com quatro botões: **Jogar novamente**, **Trocar jogadores**, **Placar**, **Menu**
- [x] "Jogar novamente": reinicia a `Match` com os mesmos jogadores e o mesmo jogo, zerando só o placar da partida
- [x] "Trocar jogadores": volta à `NameEntryScene`
- [x] Registrar o resultado **uma única vez**, ao entrar na `ResultScene` (guardar uma flag para não duplicar ao voltar da tela de placar)
- [x] Sair da partida pelo "Menu" no meio dela **não** registra nada
- [x] Registro: `recordMatch` para cada jogador humano, depois `save()`

**Pronto quando:** dá para jogar várias partidas seguidas sem voltar ao menu, e cada uma soma exatamente uma vez no placar.

### Fase E — Placar na partida e `ScoreScene` (M) · A4, A5

- [x] HUD da partida: cartões laterais com nome e rodadas ganhas, mais o indicador ●●○
- [x] `ScoreScene` com 4 abas: **Geral**, **Par ou Ímpar**, **Jokenpô**, **Flappy**
- [x] Geral: pontos = 10 por partida vencida + recorde do Flappy (constantes em `ScoreStore.h`)
- [x] Top 10 por aba, com o jogador atual destacado
- [x] Botão "Placar" no menu principal e no fim de partida (abre a aba do jogo que acabou)
- [x] Mostrar "sem registros ainda" quando a aba estiver vazia

**Pronto quando:** o placar mostra os resultados dos 3 jogos por jogador e sobrevive a fechar e reabrir o programa.

### Fase F — Flappy com dono (P) · A5

- [x] Antes de jogar, passar pela `NameEntryScene`
- [x] Ao morrer: `recordFlappy(nome, pontos)`, `save()` e aviso de "novo recorde" quando retornar `true`
- [x] Game Over com "Jogar novamente" (mesmo jogador), "Trocar jogador", "Placar" e "Menu"
- [x] Substituir o uso do `SaveData` antigo (`save/scores.dat`) pelo `ScoreStore`
- [x] Recorde antigo do arquivo legado: **não migrar** (não tem dono). O arquivo antigo fica intocado e deixa de ser lido

**Pronto quando:** cada jogador tem seu recorde, e ele aparece na aba Flappy do placar.

### Fase G — Testes e aceite (P)

- [x] Rodar os testes automáticos (`HandSprites_test`, `ScoreStore_test`)
- [x] Teste manual das 4 combinações (2 jogos × 1P/2P): nome → partida → "Jogar novamente" ×2 → placar
- [x] Teste do Flappy: dois jogadores diferentes, recordes separados
- [x] Fechar e reabrir o programa: dados mantidos
- [x] Apagar o `scores.txt`: o jogo abre com placar vazio, sem erro
- [x] Nome com acento ("João", "Zé"), nome com 12+ caracteres e campo vazio
- [x] Conferir o visual das mãos em 1280×720 e em tela cheia

---

## 5. Decisões adotadas

| Tema | Decisão |
|---|---|
| Perfil | Identificado pelo nome, sem diferenciar maiúsculas; guarda a grafia da primeira vez |
| CPU | Não tem perfil nem aparece no placar |
| Partida abandonada | Não conta |
| Empate no Jokenpô | Repete a rodada, como já está implementado |
| Pontuação geral | 10 pontos por partida vencida + recorde do Flappy |
| Arquivo | `scores.txt`, texto simples, gravado com arquivo temporário e renomeação |
| Arquivo corrompido | Linhas inválidas são ignoradas; o jogo nunca trava ao carregar |

---

## 6. Riscos

| Risco | Impacto | Mitigação |
|---|---|---|
| Fonte sem glifos acentuados (nome sai como `?`) | Médio | Carregar a fonte com os códigos `0x20`–`0xFF` e testar "João", "Zé" |
| Resultado registrado em duplicidade | Alto | Flag de "já registrado" na `ResultScene` |
| Escala não inteira deixa os pixels irregulares | Baixo | Escala inteira no estado normal; irregularidade só durante o "punch" |
| `<filesystem>` ausente em MinGW antigo | Médio | Validar na Fase A; se faltar, atualizar o MinGW/w64devkit |
| Sombra e painel desalinhados com a nova mão | Baixo | Reposicionar na Fase B, usando altura = 64 × escala |

---

## 7. Definição de pronto

- [x] As mãos estão em pixel art nos dois lados, em todas as poses
- [x] Todo jogo pede o nome do jogador
- [x] Par ou Ímpar e Jokenpô terminam com "Jogar novamente", "Trocar jogadores", "Placar" e "Menu"
- [x] O placar mostra os 3 jogos por jogador, com aba Geral
- [x] Os dados persistem entre execuções
- [x] Testes automáticos passando
- [x] Nenhum crash em 30 minutos de uso contínuo