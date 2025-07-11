# Jogo de Truco em C

## 📋 Descrição

Este projeto consiste em uma implementação simples do jogo de **Truco (versão paulista)** em linguagem **C**, desenvolvida com o objetivo de aprendizado e como um desafio pessoal. O jogo é executado totalmente em terminal e permite que dois jogadores participem da partida no mesmo computador.

## 🎯 Funcionalidades

- 🔀 Embaralhamento aleatório das cartas.
- 🃏 Distribuição automática das cartas aos jogadores.
- ⚔️ Comparação de cartas por força hierárquica (incluindo manilhas).
- 🧮 Contabilização de pontos e controle do placar.
- 💬 Mecânica de “Truco” com possibilidade de aceitar, recusar ou aumentar (6/9/12).
- 🏆 Determinação automática do vencedor de cada rodada e da partida.

## ⚙️ Compilação e Execução

Para compilar o jogo, instale o compilador gcc e utilize o seguinte comando no terminal:

```bash
gcc -o truco truco.c
````

E para executá-lo:

```bash
./truco
```

## 👀 Observações

> ⚠️ Por limitações da interface, todos os jogadores devem jogar no mesmo terminal, o que significa que todos podem ver as cartas uns dos outros.

## 💡 Considerações

Apesar de ser uma versão simplificada, o projeto cobre as principais regras e dinâmicas do Truco. A experiência foi importante para aplicar conceitos de lógica de programação, uso de estruturas condicionais, loops, structs e gerenciamento de estado de jogo.

## 🧑‍💻 Créditos

Este projeto foi desenvolvido por **Filipe S. de Medeiros**, aluno do curso de **Bacharelado em Ciências da Computação** da **FCT-UNESP**.
