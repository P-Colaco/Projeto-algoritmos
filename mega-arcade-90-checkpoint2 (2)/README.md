# Mega Arcade 90 — Fichas & High Scores

Sistema de gestão de um fliperama dos anos 90, em C (modo console), para a disciplina de Algoritmos e Pensamento Computacional — Projeto P2.

## Checkpoint 2

O sistema funciona de verdade: cada módulo tem cadastro com validação, listagem, busca textual (`strstr`/`strcmp`), edição e remoção. Os dados são salvos e carregados de arquivos `.csv` com separador `;`, e qualquer edição ou remoção regrava o arquivo. O módulo de Partidas cruza os dados dos outros módulos, descontando ou creditando fichas no saldo do jogador. Ele também gera relatórios com Bubble Sort e uma Matriz 2D.

## Divisão da equipe

| Integrante | Arquivo | Dados | Responsabilidade |
|---|---|---|---|
| Pedro | `jogadores.h` + `util.h` | `jogadores.csv` | Cadastro de jogadores; funções de tela e leitura |
| Nicolas | `maquinas.h` | `maquinas.csv` | Máquinas de arcade do salão |
| Gabriel | `pacotes.h` | `pacotes.csv` | Pacotes de fichas vendidos no caixa |
| Matias | `partidas.h` | `partidas.csv` | Partidas, venda de fichas, high scores e relatórios |

O `main.c` só importa os módulos, carrega os CSVs e exibe o menu principal.

## Como compilar

Todos os arquivos (`main.c`, `.h` e `.csv`) devem ficar **na mesma pasta**. O programa procura os `.csv` na pasta de onde é executado.

Dev-C++: abrir `main.c` e usar **Executar > Compilar e Executar** (F11).

GCC:

```
gcc main.c -o fliperama
./fliperama
```

Valores com casas decimais devem ser digitados com **ponto** (ex: `4.50`).
