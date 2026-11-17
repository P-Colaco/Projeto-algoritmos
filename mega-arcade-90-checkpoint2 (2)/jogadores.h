// ============================================================================
// Arquivo: jogadores.h | Autor: Pedro (RGM: 48163457)
// MODULO 1: Cadastro de Jogadores do fliperama (persistido em jogadores.csv)
// ============================================================================
#ifndef JOGADORES_H
#define JOGADORES_H

#include "util.h"

#define MAX_JOGADORES 50

struct Jogador {
    int idJogador;
    char nome[50];
    char iniciais[4];   // 3 letras do placar (ex: "PVC") + '\0'
    char categoria;     // 'C' = Comum, 'V' = VIP (ganha ficha bonus na compra)
    int saldoFichas;    // fichas que o jogador ainda tem para jogar
    float totalGasto;   // quanto ja gastou comprando fichas (R$)
};

struct Jogador listaJogadores[MAX_JOGADORES];
int totalJogadores = 0;

// ----------------------------------------------------------------------------
// PERSISTENCIA: grava o vetor inteiro no CSV. O modo "w" apaga o arquivo e
// reescreve tudo, entao serve para cadastro, edicao e remocao.
// ----------------------------------------------------------------------------
void salvarJogadoresCSV() {
    int i;
    FILE *arq = fopen("jogadores.csv", "w");
    if (arq == NULL) {
        printf("[ERRO] Nao foi possivel gravar jogadores.csv!\n");
        return;
    }
    for (i = 0; i < totalJogadores; i++) {
        fprintf(arq, "%d;%s;%s;%c;%d;%.2f\n",
                listaJogadores[i].idJogador,
                listaJogadores[i].nome,
                listaJogadores[i].iniciais,
                listaJogadores[i].categoria,
                listaJogadores[i].saldoFichas,
                listaJogadores[i].totalGasto);
    }
    fclose(arq);
}

// Carrega o jogadores.csv para o vetor na abertura do programa.
// %49[^;] = le ate 49 caracteres OU ate encontrar um ';' (aceita espacos no nome).
// Campos int/float/char levam & (o fscanf precisa do endereco da variavel);
// vetores de char (strings) nao levam, pois o nome do vetor ja e um endereco.
void carregarJogadoresCSV() {
    FILE *arq = fopen("jogadores.csv", "r");
    if (arq == NULL) return;   // arquivo ainda nao existe: comeca vazio
    totalJogadores = 0;
    while (totalJogadores < MAX_JOGADORES &&
           fscanf(arq, "%d;%49[^;];%3[^;];%c;%d;%f\n",
                  &listaJogadores[totalJogadores].idJogador,
                  listaJogadores[totalJogadores].nome,
                  listaJogadores[totalJogadores].iniciais,
                  &listaJogadores[totalJogadores].categoria,
                  &listaJogadores[totalJogadores].saldoFichas,
                  &listaJogadores[totalJogadores].totalGasto) == 6) {
        totalJogadores++;
    }
    fclose(arq);
}

// Retorna a posicao (indice) do jogador no vetor, ou -1 se o ID nao existe.
// Tambem e usada pelo modulo de Partidas (Matias) para validar o jogador.
int buscarIndiceJogador(int idProcurado) {
    int i;
    for (i = 0; i < totalJogadores; i++) {
        if (listaJogadores[i].idJogador == idProcurado) return i;
    }
    return -1;
}

// Le as iniciais e valida: exatamente 3 letras, convertidas para maiuscula.
// Retorna 1 se ficou valido, 0 se nao.
int lerIniciais(char destino[]) {
    int i;
    char texto[20];
    scanf(" %19[^\n]", texto);
    limparBuffer();
    if (strlen(texto) != 3) return 0;
    for (i = 0; i < 3; i++) {
        texto[i] = paraMaiuscula(texto[i]);
        if (texto[i] < 'A' || texto[i] > 'Z') return 0;
    }
    strcpy(destino, texto);
    return 1;
}

void imprimirJogador(int i) {
    printf("%3d | %-24s | %s |  %c  | %5d | R$ %8.2f\n",
           listaJogadores[i].idJogador, listaJogadores[i].nome,
           listaJogadores[i].iniciais, listaJogadores[i].categoria,
           listaJogadores[i].saldoFichas, listaJogadores[i].totalGasto);
}

void imprimirCabecalhoJogadores() {
    printf(" ID | Nome                     | INI | Cat | Fichas | Total gasto\n");
    printf("----+--------------------------+-----+-----+--------+------------\n");
}

// ----------------------------------------------------------------------------
// CADASTRO (com trava de ID repetido e validacao dos campos)
// ----------------------------------------------------------------------------
void cadastrarJogador() {
    int id;
    struct Jogador novo;

    if (totalJogadores >= MAX_JOGADORES) {
        printf("[ERRO] Limite de %d jogadores atingido!\n", MAX_JOGADORES);
        return;
    }
    printf("\n--- NOVO JOGADOR ---\n");
    printf("ID do jogador: ");
    id = lerInteiro();
    if (id <= 0) {
        printf("[ERRO] O ID precisa ser maior que zero.\n");
        return;
    }
    if (buscarIndiceJogador(id) != -1) {
        printf("[ERRO] Ja existe um jogador com o ID %d!\n", id);
        return;
    }
    novo.idJogador = id;

    printf("Nome completo: ");
    scanf(" %49[^\n]", novo.nome);
    limparBuffer();
    if (temPontoEVirgula(novo.nome)) {
        printf("[ERRO] O nome nao pode conter ';'.\n");
        return;
    }

    printf("Iniciais para o placar (3 letras, ex: PVC): ");
    if (!lerIniciais(novo.iniciais)) {
        printf("[ERRO] As iniciais devem ter exatamente 3 letras.\n");
        return;
    }

    printf("Categoria (C = Comum, V = VIP): ");
    novo.categoria = lerLetra();
    if (novo.categoria != 'C' && novo.categoria != 'V') {
        printf("[ERRO] Categoria invalida! Use C ou V.\n");
        return;
    }

    printf("Fichas de boas-vindas (saldo inicial): ");
    novo.saldoFichas = lerInteiro();
    if (novo.saldoFichas < 0) {
        printf("[ERRO] O saldo nao pode ser negativo.\n");
        return;
    }
    novo.totalGasto = 0.0;

    listaJogadores[totalJogadores] = novo;   // copia a struct inteira para o vetor
    totalJogadores++;
    salvarJogadoresCSV();
    printf("[OK] Jogador %s cadastrado e salvo em jogadores.csv!\n", novo.iniciais);
}

// ----------------------------------------------------------------------------
// LISTAGEM formatada em tabela
// ----------------------------------------------------------------------------
void listarJogadores() {
    int i;
    printf("\n--- JOGADORES CADASTRADOS (%d) ---\n", totalJogadores);
    if (totalJogadores == 0) {
        printf("Nenhum jogador cadastrado.\n");
        return;
    }
    imprimirCabecalhoJogadores();
    for (i = 0; i < totalJogadores; i++) {
        imprimirJogador(i);
    }
}

// ----------------------------------------------------------------------------
// BUSCA TEXTUAL: strstr acha o termo em qualquer parte do nome;
// strcmp compara as iniciais por igualdade exata (retorna 0 se forem iguais).
// ----------------------------------------------------------------------------
void buscarJogador() {
    int i, achou = 0;
    char termo[50];
    char termoIniciais[50];

    printf("\nDigite parte do nome ou as iniciais (ex: Silva ou PVC): ");
    scanf(" %49[^\n]", termo);
    limparBuffer();

    // copia do termo em maiusculas, para comparar com as iniciais
    strcpy(termoIniciais, termo);
    for (i = 0; termoIniciais[i] != '\0'; i++) {
        termoIniciais[i] = paraMaiuscula(termoIniciais[i]);
    }

    for (i = 0; i < totalJogadores; i++) {
        if (strstr(listaJogadores[i].nome, termo) != NULL ||
            strcmp(listaJogadores[i].iniciais, termoIniciais) == 0) {
            if (achou == 0) imprimirCabecalhoJogadores();
            imprimirJogador(i);
            achou++;
        }
    }
    if (achou == 0) {
        printf("Nenhum jogador encontrado com \"%s\".\n", termo);
    } else {
        printf("%d jogador(es) encontrado(s).\n", achou);
    }
}

// ----------------------------------------------------------------------------
// EDICAO na memoria + regravacao completa do CSV
// ----------------------------------------------------------------------------
void editarJogador() {
    int id, pos, saldo;
    char nome[50];
    char categoria;

    printf("\nID do jogador que deseja EDITAR: ");
    id = lerInteiro();
    pos = buscarIndiceJogador(id);
    if (pos == -1) {
        printf("[ERRO] Jogador nao encontrado!\n");
        return;
    }
    imprimirCabecalhoJogadores();
    imprimirJogador(pos);

    printf("\nNovo nome: ");
    scanf(" %49[^\n]", nome);
    limparBuffer();
    if (temPontoEVirgula(nome)) {
        printf("[ERRO] O nome nao pode conter ';'. Nada foi alterado.\n");
        return;
    }
    printf("Nova categoria (C/V): ");
    categoria = lerLetra();
    if (categoria != 'C' && categoria != 'V') {
        printf("[ERRO] Categoria invalida! Nada foi alterado.\n");
        return;
    }
    printf("Novo saldo de fichas: ");
    saldo = lerInteiro();
    if (saldo < 0) {
        printf("[ERRO] O saldo nao pode ser negativo. Nada foi alterado.\n");
        return;
    }

    // so altera o vetor depois de tudo validado
    strcpy(listaJogadores[pos].nome, nome);
    listaJogadores[pos].categoria = categoria;
    listaJogadores[pos].saldoFichas = saldo;
    salvarJogadoresCSV();
    printf("[OK] Jogador atualizado na memoria e no jogadores.csv!\n");
}

// ----------------------------------------------------------------------------
// REMOCAO fisica: puxa a fila do vetor uma posicao para tras e regrava o CSV
// ----------------------------------------------------------------------------
void removerJogador() {
    int id, pos, j;

    printf("\nID do jogador que deseja REMOVER: ");
    id = lerInteiro();
    pos = buscarIndiceJogador(id);
    if (pos == -1) {
        printf("[ERRO] Jogador nao encontrado!\n");
        return;
    }
    printf("Removendo %s (%s)...\n", listaJogadores[pos].nome, listaJogadores[pos].iniciais);
    for (j = pos; j < totalJogadores - 1; j++) {
        listaJogadores[j] = listaJogadores[j + 1];
    }
    totalJogadores--;
    salvarJogadoresCSV();
    printf("[OK] Jogador removido do sistema e do jogadores.csv!\n");
}

// ----------------------------------------------------------------------------
// SUBMENU DO MODULO 1 (cada case chama a sua propria funcao)
// ----------------------------------------------------------------------------
void moduloJogadores() {
    int opcao;
    do {
        limparTela();
        printf("--- [MODULO 1: CADASTRO DE JOGADORES (Pedro)] ---\n");
        printf("1. Cadastrar Novo Jogador\n");
        printf("2. Listar Jogadores\n");
        printf("3. Buscar Jogador (nome ou iniciais)\n");
        printf("4. Editar Jogador\n");
        printf("5. Remover Jogador\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        opcao = lerInteiro();

        switch (opcao) {
            case 1: cadastrarJogador(); break;
            case 2: listarJogadores(); break;
            case 3: buscarJogador(); break;
            case 4: editarJogador(); break;
            case 5: removerJogador(); break;
            case 0: break;
            default: printf("\n[ERRO] Opcao invalida no Modulo de Jogadores!\n");
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

#endif
