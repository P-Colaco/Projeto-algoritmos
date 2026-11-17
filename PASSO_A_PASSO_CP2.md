# Checkpoint 2: os 4 commits do Mega Arcade 90

Cada integrante faz **um commit pela própria conta do GitHub**, subindo o próprio `.h`, o próprio `.csv` e a nova versão do `main.c`. O programa compila e funciona depois de **cada** commit: os módulos que ainda não foram migrados continuam no formato do Checkpoint 1 até a vez do responsável.

| Ordem | Integrante | Arquivos que sobe | Versão do main.c |
|---|---|---|---|
| 1º | Pedro | `util.h`, `jogadores.h`, `jogadores.csv`, `README.md` | `etapa1_main.c` |
| 2º | Nicolas | `maquinas.h`, `maquinas.csv` | `etapa2_main.c` |
| 3º | Gabriel | `pacotes.h`, `pacotes.csv` | `etapa3_main.c` |
| 4º | Matias | `partidas.h`, `partidas.csv` | `etapa4_main.c` |

**A ordem importa:** cada um só começa depois que o anterior fez o commit.

> Diferente do Checkpoint 1, agora pode **substituir o `main.c` inteiro**: os nomes e RGMs saíram do `main.c` e foram para o cabeçalho de cada `.h`. Nas versões intermediárias, os módulos ainda não migrados aparecem com `RGM: _______`, mas isso some quando o dono do módulo faz o commit dele.

---

## Passo a passo de cada commit (pelo site do GitHub)

1. No seu computador, **renomeie** o seu `etapaN_main.c` para `main.c`.
2. Abra o **seu `.h`** e troque `RGM: _______` pelo seu RGM na primeira linha (o Pedro também faz isso no `util.h`).
3. **Teste antes de subir:** coloque na mesma pasta o `main.c`, todos os `.h` e `.csv` que já estão no GitHub e os seus. Abra o `main.c` no Dev-C++ e compile (F11).
4. No repositório: **Add file > Upload files**. Arraste o `main.c` e os seus arquivos da tabela acima. O `main.c` substitui o antigo automaticamente.
5. Escreva a mensagem do commit e clique em **Commit changes**.

### Mensagens dos commits

```
Pedro:   CP2 Modulo 1: jogadores.h + util.h com CRUD completo e jogadores.csv
Nicolas: CP2 Modulo 2: maquinas.h com CRUD completo e maquinas.csv
Gabriel: CP2 Modulo 3: pacotes.h com CRUD completo e pacotes.csv
Matias:  CP2 Modulo 4: partidas.h com cruzamento, relatorios, Bubble Sort e Matriz 2D
```

### Pelo Git no terminal (alternativa)

```
git pull
(copie os arquivos para a pasta do repositório)
git add main.c seu_modulo.h seu_modulo.csv
git commit -m "mensagem acima"
git push
```

---

## O que mudou nas structs desde o Checkpoint 1

Mudanças de propósito; vocês devem saber explicar.

- **Removemos `ativo`/`disponivel`**. A remoção agora é **física**, igual ao modelo do professor: o registro sai do vetor e do CSV. Por isso não precisamos mais de um campo para marcar removido.
- **Entrou um campo `char` de 1 letra em três structs**, que é exatamente o que a Etapa 3 pede para justificar:
  - `Jogador.categoria`: `'C'` Comum ou `'V'` VIP.
  - `Maquina.genero`: `L`, `T`, `C`, `E` ou `B`.
  - `PacoteFichas.status`: `'A'` à venda ou `'F'` fora de linha.
- **Os vetores usam `#define MAX_... 50`** em vez do número 50 solto no código.

---

## Na bancada: perguntas que todos devem saber responder

**Por que `#define MAX_JOGADORES 50` e não `int max = 50;`?**
O `#define` é trocado pelo valor *antes* de compilar (pelo pré-processador). Ele não ocupa memória nem pode ser alterado sem querer durante a execução. E o tamanho de um vetor global precisa ser constante, então `int listaJogadores[max]` com uma variável nem compila.

**O que é `FILE *arq`?**
É um ponteiro para o arquivo aberto. O `fopen` devolve esse ponteiro, ou `NULL` se não conseguir abrir. Por isso sempre testamos `if (arq == NULL)` antes de usar.

**Qual a diferença entre `"r"` e `"w"` no `fopen`?**
- `"r"` só lê, e o arquivo precisa existir.
- `"w"` **apaga tudo** e escreve do zero.

Usamos `"w"` no `salvar...CSV()` para regravar o vetor inteiro, já atualizado, depois de cadastrar, editar ou remover.

**O que faz `%49[^;]`?**
Lê caracteres até encontrar um `;`, aceitando espaços (`Street Fighter II`), com no máximo 49 caracteres. É 49 porque o vetor tem 50 posições e uma fica para o `'\0'`.

**Por que `&` no `int` e não na string?**
O `fscanf`/`scanf` precisa do **endereço** onde guardar o valor, e `&` pega o endereço de uma variável comum. O nome de um vetor (`nome`) já é o endereço da primeira posição, então não leva `&`.

**Por que `== 6` no `while` do `carregar`?**
O `fscanf` devolve quantos campos conseguiu ler. Se a linha veio completa (6 campos), aceitamos; senão, o laço para.

**Por que `buscarIndice...()` devolve `-1`?**
Nenhuma posição válida do vetor é negativa, então `-1` significa "não achei". A função é usada para bloquear ID repetido no cadastro e para encontrar o registro na edição e na remoção.

**Como funciona a remoção?**
Um `for` puxa cada elemento uma posição para trás, a partir do removido (`lista[j] = lista[j + 1]`). Depois diminuímos o total e regravamos o CSV.

**Por que tem `setlocale(LC_NUMERIC, "C")` depois do `setlocale(LC_ALL, "Portuguese")`?**
O locale português troca o ponto decimal por vírgula. Aí o preço seria gravado como `4,50` e o `fscanf` não conseguiria ler de volta. Testamos: sem essa linha, ao reabrir o programa só 1 dos 9 pacotes era carregado.

---

## Perguntas específicas de cada um

### Pedro: `jogadores.h` e `util.h`
- **`limparBuffer()`:** o `scanf` deixa o ENTER (`'\n'`) no teclado. Esse laço com `getchar()` joga fora tudo até o `'\n'`, para não atrapalhar a próxima leitura.
- **`lerInteiro()`:** o `scanf` devolve 1 se leu o número. Se o usuário digitar letras, devolve 0, e aí limpamos e pedimos de novo (antes, digitar uma letra travava o menu em loop infinito).
- **`#ifdef _WIN32` no `limparTela()`:** `_WIN32` já vem definido pelo compilador no Windows. Lá usamos `cls`, e no Linux/Mac usamos `clear`.
- **`paraMaiuscula()`:** na tabela ASCII, minúscula e maiúscula ficam a 32 de distância (`'a'` = 97 e `'A'` = 65).
- **`strstr` x `strcmp` no `buscarJogador()`:** o `strstr` acha um pedaço do nome ("Rocha"). O `strcmp` compara as iniciais por igualdade exata e devolve 0 quando são iguais.
- **Por que `totalGasto` é `float`:** é dinheiro, tem centavos. `saldoFichas` é `int` porque não existe meia ficha.

### Nicolas: `maquinas.h`
- **`generoValido()`:** um `switch` com vários `case` empilhados (`case 'L': case 'T': ...`) que caem no mesmo `return 1`.
- **Por que `genero` é `char` e não `char[]`:** é uma letra só. Um `char` ocupa 1 byte, enquanto uma string precisaria de pelo menos 2 (letra + `'\0'`).
- **Validação do cadastro:** ano entre 1970 e 2005 e de 1 a 5 fichas por partida. A máquina nova entra com `funcionando = 1`.
- **Por que a busca usa `||`:** o termo pode estar no nome do jogo **ou** no fabricante ("Capcom").
- **Quem usa `buscarIndiceMaquina()` fora do seu módulo:** o `partidas.h` do Matias, para validar a máquina antes de registrar uma partida.

### Gabriel: `pacotes.h`
- **`precoPorFicha()`:** `float` dividido por `int` dá resultado `float` (ex.: 9.00 / 12 = 0.75).
- **Melhor custo-benefício no `listarPacotes()`:** guardamos em `melhor` a posição do pacote com o menor R$/ficha, comparando só os que estão à venda (`'A'`). Ele começa em `-1` porque ainda não achamos nenhum.
- **Por que `status` é `char`:** só tem dois estados, `'A'` e `'F'`, e uma letra deixa o CSV legível.
- **Máscara `%f` com `&`:** o preço é `float`, então leva `&` no `fscanf`. A descrição é string e não leva.
- **Quem usa o seu módulo:** a venda de fichas do Matias, para validar o pacote, conferir se está à venda e pegar preço e quantidade.

### Matias: `partidas.h`
- **Cruzamento (`registrarPartida`):**
  1. Valida o jogador com `buscarIndiceJogador()` (Pedro) e a máquina com `buscarIndiceMaquina()` (Nicolas).
  2. Bloqueia máquina em manutenção.
  3. Calcula as fichas pela máquina e confere o saldo.
  4. **Dá baixa** no saldo e regrava os **dois** CSVs (o seu e o `jogadores.csv` do Pedro).
- **Venda de fichas (`venderFichas`):** valor total = preço × quantidade. O jogador VIP ganha 1 ficha bônus por pacote. As fichas entram no saldo e o valor entra no total gasto.
- **Anular partida:** devolve as fichas ao jogador antes de remover a partida.
- **Bubble Sort no ranking:** copiamos as partidas da máquina para um vetor auxiliar e comparamos vizinhos, trocando quando o da esquerda é menor. A cada passada, o menor afunda para o fim; por isso o laço interno vai até `n - 1 - i`. A variável `aux` guarda um valor durante a troca.
- **Matriz 2D `recordes[j][m]`:**
  - A linha é a posição do jogador e a coluna é a posição da máquina.
  - Primeiro zeramos tudo com dois `for`. Depois, para cada partida, guardamos a maior pontuação daquele par.
  - O `-` na tabela significa que aquele jogador nunca jogou naquela máquina.
- **Vetores paralelos em `relatorioMaquinaMaisJogada`:** `contagem[k]` e `fichas[k]` correspondem à máquina `listaMaquinas[k]`.
- **`proximoIdPartida()`:** o ID é gerado automaticamente (maior ID + 1), então nunca repete.
- **`dataValida()`:** `d[0] - '0'` converte o caractere `'7'` no número 7, porque os dígitos são seguidos na tabela ASCII.
- **`#include "jogadores.h"` dentro do `partidas.h`:** o seu módulo depende dos outros três. O `#ifndef ... #define ... #endif` (include guard) impede que o mesmo arquivo seja incluído duas vezes.

---

## Roteiro de demonstração na bancada (2 minutos)

1. Abrir o programa e mostrar a linha "Dados carregados" com os registros que vieram dos CSVs.
2. **Pedro:** cadastrar um jogador e mostrar o bloqueio de ID repetido (tente o ID 3).
3. **Nicolas:** buscar "Capcom" e depois listar as máquinas, mostrando a 8 (Time Crisis) em manutenção.
4. **Gabriel:** listar pacotes e mostrar o melhor custo-benefício e o pacote 8 fora de linha.
5. **Matias:**
   1. Tentar uma partida do jogador 7 (saldo 0), que deve ser bloqueada.
   2. Vender o pacote 3 para ele.
   3. Registrar a partida e mostrar o saldo caindo.
   4. Mostrar o ranking da máquina 1 e a matriz.
6. Sair (0), abrir o `jogadores.csv` no Bloco de Notas, mostrar a linha nova, reabrir o programa e mostrar que os dados continuam lá.

## Checklist final

- [ ] 4 commits do CP2 no GitHub, um de cada integrante
- [ ] Os 5 `.h` com `Autor:` e RGM preenchidos
- [ ] `main.c`, `.h` e `.csv` na mesma pasta, compilando no Dev-C++
- [ ] Código aberto na IDE **e** repositório aberto no navegador ao chamar o professor
- [ ] Cada um treinou as perguntas do seu módulo
