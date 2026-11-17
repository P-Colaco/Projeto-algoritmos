// ============================================================================
// Arquivo: partidas.h | Autor: Matias (RGM: 47346787)
// MODULO 4: Partidas, Venda de Fichas, High Scores e Relatorios
// (persistido em partidas.csv; cruza dados de Jogadores, Maquinas e Pacotes)
// ============================================================================
#ifndef PARTIDAS_H
#define PARTIDAS_H

#include "util.h"
#include "jogadores.h"
#include "maquinas.h"
#include "pacotes.h"

#define MAX_PARTIDAS 50
#define TOP_RANKING 10

struct Partida {
    int idPartida;
    int idJogadorPartida;   // liga com struct Jogador (modulo do Pedro)
    int idMaquinaPartida;   // liga com struct Maquina (modulo do Nicolas)
    int pontuacao;          // pontos da partida (base do High Score)
    int fichasUsadas;       // calculado automaticamente pela maquina
    char data[11];          // DD/MM/AAAA + '\0'
};

struct Partida historicoPartidas[MAX_PARTIDAS];
int totalPartidas = 0;

// ----------------------------------------------------------------------------
// PERSISTENCIA (modo "w" zera o arquivo e regrava o vetor atualizado)
// ----------------------------------------------------------------------------
void salvarPartidasCSV() {
    int i;
    FILE *arq = fopen("partidas.csv", "w");
    if (arq == NULL) {
        printf("[ERRO] Nao foi possivel gravar partidas.csv!\n");
        return;
    }
    for (i = 0; i < totalPartidas; i++) {
        fprintf(arq, "%d;%d;%d;%d;%d;%s\n",
                historicoPartidas[i].idPartida,
                historicoPartidas[i].idJogadorPartida,
                historicoPartidas[i].idMaquinaPartida,
                historicoPartidas[i].pontuacao,
                historicoPartidas[i].fichasUsadas,
                historicoPartidas[i].data);
    }
    fclose(arq);
}

// Le partidas.csv na abertura. A data e o ultimo campo, entao a mascara
// %10[^\n] le ate 10 caracteres ou ate o fim da linha.
void carregarPartidasCSV() {
    FILE *arq = fopen("partidas.csv", "r");
    if (arq == NULL) return;
    totalPartidas = 0;
    while (totalPartidas < MAX_PARTIDAS &&
           fscanf(arq, "%d;%d;%d;%d;%d;%10[^\n]\n",
                  &historicoPartidas[totalPartidas].idPartida,
                  &historicoPartidas[totalPartidas].idJogadorPartida,
                  &historicoPartidas[totalPartidas].idMaquinaPartida,
                  &historicoPartidas[totalPartidas].pontuacao,
                  &historicoPartidas[totalPartidas].fichasUsadas,
                  historicoPartidas[totalPartidas].data) == 6) {
        totalPartidas++;
    }
    fclose(arq);
}

int buscarIndicePartida(int idProcurado) {
    int i;
    for (i = 0; i < totalPartidas; i++) {
        if (historicoPartidas[i].idPartida == idProcurado) return i;
    }
    return -1;
}

// Gera o proximo ID automaticamente (maior ID existente + 1): nunca repete
int proximoIdPartida() {
    int i, maior = 0;
    for (i = 0; i < totalPartidas; i++) {
        if (historicoPartidas[i].idPartida > maior) maior = historicoPartidas[i].idPartida;
    }
    return maior + 1;
}

// Valida o formato DD/MM/AAAA: 10 caracteres, barras nas posicoes 2 e 5,
// digitos no resto e dia/mes dentro do intervalo. Retorna 1 se valida.
int dataValida(char d[]) {
    int i, dia, mes;
    if (strlen(d) != 10 || d[2] != '/' || d[5] != '/') return 0;
    for (i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && (d[i] < '0' || d[i] > '9')) return 0;
    }
    dia = (d[0] - '0') * 10 + (d[1] - '0');   // '0' vira 0, '1' vira 1...
    mes = (d[3] - '0') * 10 + (d[4] - '0');
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12) return 0;
    return 1;
}

// Maior pontuacao ja feita numa maquina (0 se ninguem jogou ainda)
int recordeDaMaquina(int idMaquina) {
    int i, recorde = 0;
    for (i = 0; i < totalPartidas; i++) {
        if (historicoPartidas[i].idMaquinaPartida == idMaquina &&
            historicoPartidas[i].pontuacao > recorde) {
            recorde = historicoPartidas[i].pontuacao;
        }
    }
    return recorde;
}

// Escreve as iniciais do jogador da partida (ou "???" se ele foi removido)
void imprimirIniciaisDoJogador(int idJogador) {
    int j = buscarIndiceJogador(idJogador);
    if (j == -1) {
        printf("???");
    } else {
        printf("%s", listaJogadores[j].iniciais);
    }
}

void imprimirCabecalhoPartidas() {
    printf("  ID | Data       | INI | Jogo                       |  Pontuacao | Fichas\n");
    printf("-----+------------+-----+----------------------------+------------+-------\n");
}

void imprimirPartida(int i) {
    int m = buscarIndiceMaquina(historicoPartidas[i].idMaquinaPartida);
    printf("%4d | %s | ", historicoPartidas[i].idPartida, historicoPartidas[i].data);
    imprimirIniciaisDoJogador(historicoPartidas[i].idJogadorPartida);
    if (m == -1) {
        printf(" | %-26s", "(maquina removida)");
    } else {
        printf(" | %-26s", listaMaquinas[m].nomeJogo);
    }
    printf(" | %10d | %5d\n", historicoPartidas[i].pontuacao, historicoPartidas[i].fichasUsadas);
}

// ----------------------------------------------------------------------------
// TRANSACAO 1: REGISTRAR PARTIDA (cruza Jogador + Maquina)
// Valida os dois IDs nos modulos dos colegas, calcula as fichas pela maquina,
// confere o saldo e da BAIXA nas fichas do jogador.
// ----------------------------------------------------------------------------
void registrarPartida() {
    int idJog, idMaq, posJog, posMaq, custo, recordeAnterior;
    struct Partida nova;

    if (totalPartidas >= MAX_PARTIDAS) {
        printf("[ERRO] Historico cheio (%d partidas)!\n", MAX_PARTIDAS);
        return;
    }
    printf("\n--- REGISTRAR PARTIDA ---\n");
    printf("ID do jogador: ");
    idJog = lerInteiro();
    posJog = buscarIndiceJogador(idJog);
    if (posJog == -1) {
        printf("[ERRO] Jogador %d nao esta cadastrado (Modulo 1)!\n", idJog);
        return;
    }
    printf("Jogador: %s (%s) | Saldo: %d fichas\n", listaJogadores[posJog].nome,
           listaJogadores[posJog].iniciais, listaJogadores[posJog].saldoFichas);

    printf("ID da maquina: ");
    idMaq = lerInteiro();
    posMaq = buscarIndiceMaquina(idMaq);
    if (posMaq == -1) {
        printf("[ERRO] Maquina %d nao esta cadastrada (Modulo 2)!\n", idMaq);
        return;
    }
    if (listaMaquinas[posMaq].funcionando == 0) {
        printf("[ERRO] %s esta em manutencao!\n", listaMaquinas[posMaq].nomeJogo);
        return;
    }

    custo = listaMaquinas[posMaq].fichasPorPartida;   // calculo automatico
    if (listaJogadores[posJog].saldoFichas < custo) {
        printf("[ERRO] Saldo insuficiente: %s tem %d ficha(s) e %s cobra %d.\n",
               listaJogadores[posJog].iniciais, listaJogadores[posJog].saldoFichas,
               listaMaquinas[posMaq].nomeJogo, custo);
        printf("       Venda um pacote de fichas na opcao 2 deste menu.\n");
        return;
    }

    printf("Pontuacao feita: ");
    nova.pontuacao = lerInteiro();
    if (nova.pontuacao < 0) {
        printf("[ERRO] Pontuacao nao pode ser negativa.\n");
        return;
    }
    printf("Data (DD/MM/AAAA): ");
    scanf(" %10[^\n]", nova.data);
    limparBuffer();
    if (!dataValida(nova.data)) {
        printf("[ERRO] Data invalida! Use o formato DD/MM/AAAA.\n");
        return;
    }

    recordeAnterior = recordeDaMaquina(idMaq);

    nova.idPartida = proximoIdPartida();
    nova.idJogadorPartida = idJog;
    nova.idMaquinaPartida = idMaq;
    nova.fichasUsadas = custo;
    historicoPartidas[totalPartidas] = nova;
    totalPartidas++;

    listaJogadores[posJog].saldoFichas = listaJogadores[posJog].saldoFichas - custo;   // baixa

    salvarPartidasCSV();
    salvarJogadoresCSV();   // o saldo do jogador mudou: regrava o CSV do Pedro

    printf("\n[OK] Partida #%d registrada! %d ficha(s) descontada(s). Novo saldo de %s: %d\n",
           nova.idPartida, custo, listaJogadores[posJog].iniciais,
           listaJogadores[posJog].saldoFichas);
    if (nova.pontuacao > recordeAnterior) {
        printf("*** NEW HIGH SCORE! %s bateu o recorde de %s (antes: %d) ***\n",
               listaJogadores[posJog].iniciais, listaMaquinas[posMaq].nomeJogo, recordeAnterior);
    }
}

// ----------------------------------------------------------------------------
// TRANSACAO 2: VENDER PACOTE DE FICHAS (cruza Jogador + Pacote)
// Calcula o valor total, credita as fichas e soma no total gasto do jogador.
// Regra extra: jogador VIP ganha 1 ficha bonus por pacote comprado.
// ----------------------------------------------------------------------------
void venderFichas() {
    int idJog, idPac, posJog, posPac, quantidade, fichas, bonus;
    float valorTotal;

    printf("\n--- VENDA DE FICHAS NO CAIXA ---\n");
    printf("ID do jogador: ");
    idJog = lerInteiro();
    posJog = buscarIndiceJogador(idJog);
    if (posJog == -1) {
        printf("[ERRO] Jogador %d nao esta cadastrado (Modulo 1)!\n", idJog);
        return;
    }
    printf("ID do pacote: ");
    idPac = lerInteiro();
    posPac = buscarIndicePacote(idPac);
    if (posPac == -1) {
        printf("[ERRO] Pacote %d nao esta cadastrado (Modulo 3)!\n", idPac);
        return;
    }
    if (listaPacotes[posPac].status != 'A') {
        printf("[ERRO] O pacote %s esta fora de linha!\n", listaPacotes[posPac].descricao);
        return;
    }
    printf("Quantos pacotes (1 a 10): ");
    quantidade = lerInteiro();
    if (quantidade < 1 || quantidade > 10) {
        printf("[ERRO] Quantidade invalida.\n");
        return;
    }

    valorTotal = listaPacotes[posPac].preco * quantidade;
    fichas = listaPacotes[posPac].quantidadeFichas * quantidade;
    bonus = 0;
    if (listaJogadores[posJog].categoria == 'V') {
        bonus = quantidade;   // 1 ficha extra por pacote para VIP
    }

    listaJogadores[posJog].saldoFichas = listaJogadores[posJog].saldoFichas + fichas + bonus;
    listaJogadores[posJog].totalGasto = listaJogadores[posJog].totalGasto + valorTotal;
    salvarJogadoresCSV();

    printf("\n========== CUPOM DO CAIXA ==========\n");
    printf("Jogador : %s (%s)\n", listaJogadores[posJog].nome, listaJogadores[posJog].iniciais);
    printf("Pacote  : %d x %s\n", quantidade, listaPacotes[posPac].descricao);
    printf("Fichas  : %d", fichas);
    if (bonus > 0) printf(" + %d bonus VIP", bonus);
    printf("\nTOTAL   : R$ %.2f\n", valorTotal);
    printf("Novo saldo: %d fichas\n", listaJogadores[posJog].saldoFichas);
    printf("====================================\n");
}

// ----------------------------------------------------------------------------
// LISTAGEM do historico
// ----------------------------------------------------------------------------
void listarPartidas() {
    int i;
    printf("\n--- HISTORICO DE PARTIDAS (%d) ---\n", totalPartidas);
    if (totalPartidas == 0) {
        printf("Nenhuma partida registrada.\n");
        return;
    }
    imprimirCabecalhoPartidas();
    for (i = 0; i < totalPartidas; i++) {
        imprimirPartida(i);
    }
}

// ----------------------------------------------------------------------------
// BUSCA TEXTUAL: partidas pelo nome do jogo (strstr no modulo do Nicolas)
// ----------------------------------------------------------------------------
void buscarPartidasPorJogo() {
    int i, m, achou = 0;
    char termo[40];

    printf("\nDigite parte do nome do jogo (ex: Street, Kombat): ");
    scanf(" %39[^\n]", termo);
    limparBuffer();

    for (i = 0; i < totalPartidas; i++) {
        m = buscarIndiceMaquina(historicoPartidas[i].idMaquinaPartida);
        if (m != -1 && strstr(listaMaquinas[m].nomeJogo, termo) != NULL) {
            if (achou == 0) imprimirCabecalhoPartidas();
            imprimirPartida(i);
            achou++;
        }
    }
    if (achou == 0) {
        printf("Nenhuma partida encontrada para \"%s\".\n", termo);
    } else {
        printf("%d partida(s) encontrada(s).\n", achou);
    }
}

// ----------------------------------------------------------------------------
// EDICAO: corrigir pontuacao/data de uma partida (regrava o CSV)
// ----------------------------------------------------------------------------
void corrigirPartida() {
    int id, pos, pontos;
    char data[11];

    printf("\nID da partida que deseja CORRIGIR: ");
    id = lerInteiro();
    pos = buscarIndicePartida(id);
    if (pos == -1) {
        printf("[ERRO] Partida nao encontrada!\n");
        return;
    }
    imprimirCabecalhoPartidas();
    imprimirPartida(pos);

    printf("\nPontuacao correta: ");
    pontos = lerInteiro();
    if (pontos < 0) {
        printf("[ERRO] Pontuacao invalida. Nada foi alterado.\n");
        return;
    }
    printf("Data correta (DD/MM/AAAA): ");
    scanf(" %10[^\n]", data);
    limparBuffer();
    if (!dataValida(data)) {
        printf("[ERRO] Data invalida. Nada foi alterado.\n");
        return;
    }

    historicoPartidas[pos].pontuacao = pontos;
    strcpy(historicoPartidas[pos].data, data);
    salvarPartidasCSV();
    printf("[OK] Partida corrigida na memoria e no partidas.csv!\n");
}

// ----------------------------------------------------------------------------
// REMOCAO (anular partida): devolve as fichas ao jogador e regrava os CSVs
// ----------------------------------------------------------------------------
void anularPartida() {
    int id, pos, j, posJog;

    printf("\nID da partida que deseja ANULAR: ");
    id = lerInteiro();
    pos = buscarIndicePartida(id);
    if (pos == -1) {
        printf("[ERRO] Partida nao encontrada!\n");
        return;
    }

    posJog = buscarIndiceJogador(historicoPartidas[pos].idJogadorPartida);
    if (posJog != -1) {
        listaJogadores[posJog].saldoFichas = listaJogadores[posJog].saldoFichas
                                             + historicoPartidas[pos].fichasUsadas;
        salvarJogadoresCSV();
        printf("%d ficha(s) devolvida(s) para %s.\n",
               historicoPartidas[pos].fichasUsadas, listaJogadores[posJog].iniciais);
    }

    for (j = pos; j < totalPartidas - 1; j++) {
        historicoPartidas[j] = historicoPartidas[j + 1];
    }
    totalPartidas--;
    salvarPartidasCSV();
    printf("[OK] Partida #%d anulada e removida do partidas.csv!\n", id);
}

// ----------------------------------------------------------------------------
// RELATORIO 1: RANKING HIGH SCORE de uma maquina (Top 10) com BUBBLE SORT
// Copia as partidas daquela maquina para um vetor auxiliar e ordena do maior
// para o menor: a cada passada, o menor "afunda" para o fim do vetor.
// ----------------------------------------------------------------------------
void relatorioRankingMaquina() {
    int idMaq, posMaq, i, j, n = 0;
    struct Partida ranking[MAX_PARTIDAS];
    struct Partida aux;

    printf("\nID da maquina para ver o placar: ");
    idMaq = lerInteiro();
    posMaq = buscarIndiceMaquina(idMaq);
    if (posMaq == -1) {
        printf("[ERRO] Maquina nao encontrada!\n");
        return;
    }

    for (i = 0; i < totalPartidas; i++) {
        if (historicoPartidas[i].idMaquinaPartida == idMaq) {
            ranking[n] = historicoPartidas[i];
            n++;
        }
    }
    if (n == 0) {
        printf("Ninguem jogou %s ainda.\n", listaMaquinas[posMaq].nomeJogo);
        return;
    }

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (ranking[j].pontuacao < ranking[j + 1].pontuacao) {
                aux = ranking[j];
                ranking[j] = ranking[j + 1];
                ranking[j + 1] = aux;
            }
        }
    }

    printf("\n======== HIGH SCORES: %s ========\n", listaMaquinas[posMaq].nomeJogo);
    printf(" POS | INI |  PONTUACAO | DATA\n");
    for (i = 0; i < n && i < TOP_RANKING; i++) {
        printf(" %2do | ", i + 1);
        imprimirIniciaisDoJogador(ranking[i].idJogadorPartida);
        printf(" | %10d | %s\n", ranking[i].pontuacao, ranking[i].data);
    }
}

// ----------------------------------------------------------------------------
// RELATORIO 2: RECORDISTA GERAL (maior pontuacao de todo o fliperama)
// ----------------------------------------------------------------------------
void relatorioRecordistaGeral() {
    int i, melhor = 0, j, m;
    if (totalPartidas == 0) {
        printf("\nNenhuma partida registrada.\n");
        return;
    }
    for (i = 1; i < totalPartidas; i++) {
        if (historicoPartidas[i].pontuacao > historicoPartidas[melhor].pontuacao) {
            melhor = i;
        }
    }
    j = buscarIndiceJogador(historicoPartidas[melhor].idJogadorPartida);
    m = buscarIndiceMaquina(historicoPartidas[melhor].idMaquinaPartida);
    printf("\n******** RECORDISTA GERAL DO MEGA ARCADE 90 ********\n");
    if (j != -1) {
        printf("Jogador  : %s (%s)\n", listaJogadores[j].nome, listaJogadores[j].iniciais);
    } else {
        printf("Jogador  : (removido)\n");
    }
    if (m != -1) printf("Maquina  : %s\n", listaMaquinas[m].nomeJogo);
    printf("Pontuacao: %d pontos em %s\n",
           historicoPartidas[melhor].pontuacao, historicoPartidas[melhor].data);
}

// ----------------------------------------------------------------------------
// RELATORIO 3: MAQUINA MAIS JOGADA (contagem de partidas e fichas por maquina)
// Usa vetores paralelos: contagem[k] e fichas[k] pertencem a listaMaquinas[k].
// ----------------------------------------------------------------------------
void relatorioMaquinaMaisJogada() {
    int contagem[MAX_MAQUINAS];
    int fichas[MAX_MAQUINAS];
    int i, m, maisJogada = -1, totalFichas = 0;

    for (i = 0; i < totalMaquinas; i++) {
        contagem[i] = 0;
        fichas[i] = 0;
    }
    for (i = 0; i < totalPartidas; i++) {
        m = buscarIndiceMaquina(historicoPartidas[i].idMaquinaPartida);
        if (m != -1) {
            contagem[m]++;
            fichas[m] = fichas[m] + historicoPartidas[i].fichasUsadas;
            totalFichas = totalFichas + historicoPartidas[i].fichasUsadas;
        }
    }

    printf("\n--- USO DAS MAQUINAS ---\n");
    printf(" ID | Jogo                       | Partidas | Fichas | %% das fichas\n");
    for (i = 0; i < totalMaquinas; i++) {
        float percentual = 0;
        if (totalFichas > 0) percentual = fichas[i] * 100.0 / totalFichas;
        printf("%3d | %-26s | %8d | %6d | %5.1f%%\n", listaMaquinas[i].idMaquina,
               listaMaquinas[i].nomeJogo, contagem[i], fichas[i], percentual);
        if (maisJogada == -1 || contagem[i] > contagem[maisJogada]) maisJogada = i;
    }
    if (maisJogada != -1 && contagem[maisJogada] > 0) {
        printf("\n>> MAQUINA MAIS JOGADA: %s com %d partida(s)\n",
               listaMaquinas[maisJogada].nomeJogo, contagem[maisJogada]);
    }
}

// ----------------------------------------------------------------------------
// RELATORIO 4: PARTIDAS DE UM JOGADOR (total de fichas, melhor e media)
// ----------------------------------------------------------------------------
void relatorioPartidasDoJogador() {
    int idJog, posJog, i, qtd = 0, fichas = 0, melhor = 0;
    float soma = 0;

    printf("\nID do jogador: ");
    idJog = lerInteiro();
    posJog = buscarIndiceJogador(idJog);
    if (posJog == -1) {
        printf("[ERRO] Jogador nao encontrado!\n");
        return;
    }
    printf("\n--- PARTIDAS DE %s (%s) ---\n", listaJogadores[posJog].nome,
           listaJogadores[posJog].iniciais);
    for (i = 0; i < totalPartidas; i++) {
        if (historicoPartidas[i].idJogadorPartida == idJog) {
            if (qtd == 0) imprimirCabecalhoPartidas();
            imprimirPartida(i);
            qtd++;
            fichas = fichas + historicoPartidas[i].fichasUsadas;
            soma = soma + historicoPartidas[i].pontuacao;
            if (historicoPartidas[i].pontuacao > melhor) melhor = historicoPartidas[i].pontuacao;
        }
    }
    if (qtd == 0) {
        printf("Esse jogador ainda nao jogou.\n");
        return;
    }
    printf("\nPartidas: %d | Fichas usadas: %d | Melhor: %d | Media: %.1f pontos\n",
           qtd, fichas, melhor, soma / qtd);
    printf("Saldo atual: %d fichas | Total gasto no caixa: R$ %.2f\n",
           listaJogadores[posJog].saldoFichas, listaJogadores[posJog].totalGasto);
}

// ----------------------------------------------------------------------------
// RELATORIO 5: MATRIZ 2D de RECORDES (linha = jogador, coluna = maquina)
// recordes[j][m] guarda a MELHOR pontuacao do jogador j na maquina m.
// ----------------------------------------------------------------------------
void relatorioMatrizRecordes() {
    int recordes[MAX_JOGADORES][MAX_MAQUINAS];
    int j, m, i;

    if (totalJogadores == 0 || totalMaquinas == 0) {
        printf("\nCadastre jogadores e maquinas primeiro.\n");
        return;
    }

    // 1) zera a matriz
    for (j = 0; j < totalJogadores; j++) {
        for (m = 0; m < totalMaquinas; m++) {
            recordes[j][m] = 0;
        }
    }
    // 2) percorre as partidas guardando a maior pontuacao de cada par
    for (i = 0; i < totalPartidas; i++) {
        j = buscarIndiceJogador(historicoPartidas[i].idJogadorPartida);
        m = buscarIndiceMaquina(historicoPartidas[i].idMaquinaPartida);
        if (j != -1 && m != -1 && historicoPartidas[i].pontuacao > recordes[j][m]) {
            recordes[j][m] = historicoPartidas[i].pontuacao;
        }
    }

    // 3) imprime: cabecalho com o ID de cada maquina
    printf("\n--- MATRIZ DE RECORDES (Jogador x Maquina) ---\n");
    printf("INI |");
    for (m = 0; m < totalMaquinas; m++) {
        printf(" Maq%-4d|", listaMaquinas[m].idMaquina);
    }
    printf("\n");
    for (j = 0; j < totalJogadores; j++) {
        printf("%s |", listaJogadores[j].iniciais);
        for (m = 0; m < totalMaquinas; m++) {
            if (recordes[j][m] == 0) {
                printf("    -   |");
            } else {
                printf("%8d|", recordes[j][m]);
            }
        }
        printf("\n");
    }
    printf("\nLegenda das maquinas:\n");
    for (m = 0; m < totalMaquinas; m++) {
        printf("  Maq%d = %s\n", listaMaquinas[m].idMaquina, listaMaquinas[m].nomeJogo);
    }
}

// ----------------------------------------------------------------------------
// SUBMENU DO MODULO 4
// ----------------------------------------------------------------------------
void moduloPartidasRelatorios() {
    int opcao;
    do {
        limparTela();
        printf("--- [MODULO 4: PARTIDAS E HIGH SCORES (Matias)] ---\n");
        printf(" 1. Registrar Nova Partida (Jogador + Maquina)\n");
        printf(" 2. Vender Pacote de Fichas (Jogador + Pacote)\n");
        printf(" 3. Listar Historico de Partidas\n");
        printf(" 4. Buscar Partidas por Nome do Jogo\n");
        printf(" 5. Corrigir Pontuacao / Data de Partida\n");
        printf(" 6. Anular Partida (devolve fichas)\n");
        printf(" 7. [RELATORIO] Ranking High Score de uma Maquina (Top 10)\n");
        printf(" 8. [RELATORIO] Recordista Geral do Fliperama\n");
        printf(" 9. [RELATORIO] Maquina Mais Jogada\n");
        printf("10. [RELATORIO] Partidas de um Jogador\n");
        printf("11. [RELATORIO] Matriz de Recordes (Jogador x Maquina)\n");
        printf(" 0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        opcao = lerInteiro();

        switch (opcao) {
            case 1:  registrarPartida(); break;
            case 2:  venderFichas(); break;
            case 3:  listarPartidas(); break;
            case 4:  buscarPartidasPorJogo(); break;
            case 5:  corrigirPartida(); break;
            case 6:  anularPartida(); break;
            case 7:  relatorioRankingMaquina(); break;
            case 8:  relatorioRecordistaGeral(); break;
            case 9:  relatorioMaquinaMaisJogada(); break;
            case 10: relatorioPartidasDoJogador(); break;
            case 11: relatorioMatrizRecordes(); break;
            case 0:  break;
            default: printf("\n[ERRO] Opcao invalida no Modulo de Partidas!\n");
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

#endif
