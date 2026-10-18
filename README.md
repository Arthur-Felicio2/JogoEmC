# Starlight Games

> Modernização completa do jogo original em C da **Starlight Company**, agora desenvolvido em **C++17** com **Raylib**, com interface gráfica moderna, mãos realistas em múltiplas camadas, animações fluidas, sistema melhor de 3 e o jogo extra **Starlight Bird (Flappy)**.

---

## 🎮 Como Executar

### Opção 1: Execução Imediata
Basta dar um duplo clique no script ou rodar no terminal:
```bat
.\run.bat
```
*(ou abra diretamente o executável em `build\Release\StarlightGames.exe`)*

### Opção 2: Recompilar o Projeto
Para compilar novamente pelo terminal ou em outra máquina:
```bat
.\build.bat
```

### Opção 3: Executar os Testes Unitários de Regras
Para verificar todas as combinações de regras de Par ou Ímpar e Jokenpô:
```bat
.\test.bat
```

---

## ✨ Recursos Implementados

1. **Par ou Ímpar**:
   - Modos 1 Jogador (vs Computador inteligente com `std::mt19937`) e 2 Jogadores (Local).
   - Botões interativos de 0 a 5 dedos.
   - Partida em formato **Melhor de 3** (primeiro a 2 vitórias).
   - Escolha de Par ou Ímpar para o Jogador 1.
   - Tela de **"Passe o Dispositivo"** no modo 2 jogadores para garantir o sigilo da jogada.

2. **Jokenpô**:
   - Modos 1P e 2P com botões para Pedra, Papel e Tesoura.
   - Regra original respeitada: **empate repete a rodada** sem contar no placar.
   - Melhor de 3 com tela de comemoração e revanche imediata.

3. **Starlight Bird (Flappy)**:
   - Jogo completo integrado ao menu.
   - Física em passo fixo de 60 Hz com gravidade de 1800 px/s² e impulso de -520 px/s.
   - Canos com variação de velocidade e espaçamento progressivo.
   - Parallax em duas camadas e chão com esteira rolante.
   - Pássaro com animação de 3 frames de asa e inclinação suave.
   - **Recorde salvo automaticamente** em arquivo persistente (`save/scores.dat`).

4. **Mãos Realistas em 3 Camadas**:
   - 6 poses anatômicas (0 = Pedra/Fechada, 1 = 1 dedo, 2 = Tesoura/V, 3 = 3 dedos, 4 = 4 dedos, 5 = Papel/Aberta).
   - Camadas individuais: Base (tingida pelo tom de pele), Sombra/Luz (detalhes e articulações) e Contorno.
   - Suporte a 5 tons de pele selecionáveis.
   - Espelhamento horizontal para o Jogador 2.

5. **Animações e Efeitos (Seção 5.4)**:
   - Entrada das mãos pelas laterais com interpolação `easeOutBack`.
   - 3 balanços das mãos com contagem sincronizada ("1... 2... 3..." / "JÔ... KÊN... PÔ!").
   - Efeito de impacto/escala ("punch") na revelação.
   - Contagem e soma dinâmica no Par ou Ímpar.
   - Troféu dourado animado com brilho e efeito de balanço.
   - Sistema de partículas com chuva e explosão de confetes coloridos.

6. **Design System & Usabilidade**:
   - Mesa verde moderna (`#0B3D2E` → `#145A43`).
   - Cartões com visual de vidro fosco (glassmorphism), bordas suaves e sombras.
   - Navegação completa por **Mouse** e **Teclado** (Setas, Tab, Espaço, Enter).
   - Resolução lógica de 1280×720 com redimensionamento e letterbox preservando proporção.
   - Suporte a tela cheia com a tecla **F11**.

---

## 👥 Autoria e Créditos

- **Autores:** Arthur Moreira Felicio (Nº 5) & Gabriel Sulzbacher Coimbra de Souza (Nº 10)
- **Turma:** INI-1A
- **Empresa / Equipe:** Starlight Company
- **Licenças de Assets:** Documentadas em `assets/LICENSES.md`
