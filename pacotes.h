// ============================================================================
// Arquivo: pacotes.h | Autor: Gabriel (RGM: 48144509)
// MODULO 3: Pacotes de Fichas vendidos no Caixa (persistido em pacotes.csv)
// ============================================================================
#ifndef PACOTES_H
#define PACOTES_H

#include "util.h"

#define MAX_PACOTES 50

struct PacoteFichas {
    int idPacote;
    char descricao[40];     // ex: "Pacote Turbo 10+2"
    int quantidadeFichas;   // fichas que o jogador recebe
    float preco;            // valor do pacote em R$
    char status;            // 'A' = A venda, 'F' = Fora de linha
};

struct PacoteFichas listaPacotes[MAX_PACOTES];
int totalPacotes = 0;

// ----------------------------------------------------------------------------
// PERSISTENCIA (modo "w" zera o arquivo e regrava o vetor atualizado)
// ----------------------------------------------------------------------------
void salvarPacotesCSV() {
    int i;
    FILE *arq = fopen("pacotes.csv", "w");
    if (arq == NULL) {
        printf("[ERRO] Nao foi possivel gravar pacotes.csv!\n");
        return;
    }
    for (i = 0; i < totalPacotes; i++) {
        fprintf(arq, "%d;%s;%d;%.2f;%c\n",
                listaPacotes[i].idPacote,
                listaPacotes[i].descricao,
                listaPacotes[i].quantidadeFichas,
                listaPacotes[i].preco,
                listaPacotes[i].status);
    }
    fclose(arq);
}

// Le pacotes.csv na abertura do programa (so aceita linhas com os 5 campos)
void carregarPacotesCSV() {
    FILE *arq = fopen("pacotes.csv", "r");
    if (arq == NULL) return;
    totalPacotes = 0;
    while (totalPacotes < MAX_PACOTES &&
           fscanf(arq, "%d;%39[^;];%d;%f;%c\n",
                  &listaPacotes[totalPacotes].idPacote,
                  listaPacotes[totalPacotes].descricao,
                  &listaPacotes[totalPacotes].quantidadeFichas,
                  &listaPacotes[totalPacotes].preco,
                  &listaPacotes[totalPacotes].status) == 5) {
        totalPacotes++;
    }
    fclose(arq);
}

// Retorna o indice do pacote no vetor, ou -1 se nao existir.
// Usada tambem pelo modulo de Partidas (Matias) na venda de fichas.
int buscarIndicePacote(int idProcurado) {
    int i;
    for (i = 0; i < totalPacotes; i++) {
        if (listaPacotes[i].idPacote == idProcurado) return i;
    }
    return -1;
}

// Regra de negocio: quanto custa CADA ficha dentro do pacote
float precoPorFicha(int i) {
    return listaPacotes[i].preco / listaPacotes[i].quantidadeFichas;
}

void imprimirCabecalhoPacotes() {
    printf(" ID | Descricao                    | Fichas | Preco      | R$/ficha | Status\n");
    printf("----+------------------------------+--------+------------+----------+-----------\n");
}

void imprimirPacote(int i) {
    printf("%3d | %-28s | %6d | R$ %7.2f | %8.2f | ",
           listaPacotes[i].idPacote, listaPacotes[i].descricao,
           listaPacotes[i].quantidadeFichas, listaPacotes[i].preco, precoPorFicha(i));
    if (listaPacotes[i].status == 'A') {
        printf("A venda\n");
    } else {
        printf("Fora de linha\n");
    }
}

// ----------------------------------------------------------------------------
// CADASTRO (trava de ID repetido + validacao de fichas e preco)
// ----------------------------------------------------------------------------
void cadastrarPacote() {
    int id;
    struct PacoteFichas novo;

    if (totalPacotes >= MAX_PACOTES) {
        printf("[ERRO] Limite de %d pacotes atingido!\n", MAX_PACOTES);
        return;
    }
    printf("\n--- NOVO PACOTE DE FICHAS ---\n");
    printf("ID do pacote: ");
    id = lerInteiro();
    if (id <= 0) {
        printf("[ERRO] O ID precisa ser maior que zero.\n");
        return;
    }
    if (buscarIndicePacote(id) != -1) {
        printf("[ERRO] Ja existe um pacote com o ID %d!\n", id);
        return;
    }
    novo.idPacote = id;

    printf("Descricao (ex: Pacote Turbo 10+2): ");
    scanf(" %39[^\n]", novo.descricao);
    limparBuffer();
    if (temPontoEVirgula(novo.descricao)) {
        printf("[ERRO] A descricao nao pode conter ';'.\n");
        return;
    }

    printf("Quantidade de fichas: ");
    novo.quantidadeFichas = lerInteiro();
    if (novo.quantidadeFichas <= 0) {
        printf("[ERRO] O pacote precisa ter pelo menos 1 ficha.\n");
        return;
    }

    printf("Preco do pacote (R$, use ponto): ");
    novo.preco = lerFloat();
    if (novo.preco <= 0) {
        printf("[ERRO] O preco precisa ser maior que zero.\n");
        return;
    }
    novo.status = 'A';   // pacote novo ja entra a venda

    listaPacotes[totalPacotes] = novo;
    totalPacotes++;
    salvarPacotesCSV();
    printf("[OK] Pacote cadastrado e salvo em pacotes.csv! (R$ %.2f por ficha)\n",
           novo.preco / novo.quantidadeFichas);
}

// ----------------------------------------------------------------------------
// LISTAGEM com destaque para o pacote mais vantajoso (menor R$ por ficha)
// ----------------------------------------------------------------------------
void listarPacotes() {
    int i, melhor = -1;
    printf("\n--- TABELA DE PRECOS DAS FICHAS (%d pacotes) ---\n", totalPacotes);
    if (totalPacotes == 0) {
        printf("Nenhum pacote cadastrado.\n");
        return;
    }
    imprimirCabecalhoPacotes();
    for (i = 0; i < totalPacotes; i++) {
        imprimirPacote(i);
        if (listaPacotes[i].status == 'A') {
            if (melhor == -1 || precoPorFicha(i) < precoPorFicha(melhor)) {
                melhor = i;
            }
        }
    }
    if (melhor != -1) {
        printf("\n>> Melhor custo-beneficio a venda: %s (R$ %.2f por ficha)\n",
               listaPacotes[melhor].descricao, precoPorFicha(melhor));
    }
}

// ----------------------------------------------------------------------------
// BUSCA TEXTUAL com strstr na descricao
// ----------------------------------------------------------------------------
void buscarPacote() {
    int i, achou = 0;
    char termo[40];

    printf("\nDigite parte da descricao (ex: Turbo, Promo): ");
    scanf(" %39[^\n]", termo);
    limparBuffer();

    for (i = 0; i < totalPacotes; i++) {
        if (strstr(listaPacotes[i].descricao, termo) != NULL) {
            if (achou == 0) imprimirCabecalhoPacotes();
            imprimirPacote(i);
            achou++;
        }
    }
    if (achou == 0) {
        printf("Nenhum pacote encontrado com \"%s\".\n", termo);
    } else {
        printf("%d pacote(s) encontrado(s).\n", achou);
    }
}

// ----------------------------------------------------------------------------
// EDICAO: fichas, preco e status (regrava o CSV)
// ----------------------------------------------------------------------------
void editarPacote() {
    int id, pos, fichas;
    float preco;
    char status;

    printf("\nID do pacote que deseja EDITAR: ");
    id = lerInteiro();
    pos = buscarIndicePacote(id);
    if (pos == -1) {
        printf("[ERRO] Pacote nao encontrado!\n");
        return;
    }
    imprimirCabecalhoPacotes();
    imprimirPacote(pos);

    printf("\nNova quantidade de fichas: ");
    fichas = lerInteiro();
    if (fichas <= 0) {
        printf("[ERRO] Quantidade invalida. Nada foi alterado.\n");
        return;
    }
    printf("Novo preco (R$, use ponto): ");
    preco = lerFloat();
    if (preco <= 0) {
        printf("[ERRO] Preco invalido. Nada foi alterado.\n");
        return;
    }
    printf("Status (A = A venda, F = Fora de linha): ");
    status = lerLetra();
    if (status != 'A' && status != 'F') {
        printf("[ERRO] Status invalido. Nada foi alterado.\n");
        return;
    }

    listaPacotes[pos].quantidadeFichas = fichas;
    listaPacotes[pos].preco = preco;
    listaPacotes[pos].status = status;
    salvarPacotesCSV();
    printf("[OK] Pacote atualizado na memoria e no pacotes.csv!\n");
}

// ----------------------------------------------------------------------------
// REMOCAO fisica do vetor + regravacao do CSV
// ----------------------------------------------------------------------------
void removerPacote() {
    int id, pos, j;

    printf("\nID do pacote que deseja REMOVER: ");
    id = lerInteiro();
    pos = buscarIndicePacote(id);
    if (pos == -1) {
        printf("[ERRO] Pacote nao encontrado!\n");
        return;
    }
    printf("Removendo %s...\n", listaPacotes[pos].descricao);
    for (j = pos; j < totalPacotes - 1; j++) {
        listaPacotes[j] = listaPacotes[j + 1];
    }
    totalPacotes--;
    salvarPacotesCSV();
    printf("[OK] Pacote removido do sistema e do pacotes.csv!\n");
}

// ----------------------------------------------------------------------------
// SUBMENU DO MODULO 3
// ----------------------------------------------------------------------------
void moduloPacotesFichas() {
    int opcao;
    do {
        limparTela();
        printf("--- [MODULO 3: PACOTES DE FICHAS NO CAIXA (Gabriel)] ---\n");
        printf("1. Cadastrar Novo Pacote de Fichas\n");
        printf("2. Listar Tabela de Precos\n");
        printf("3. Buscar Pacote (descricao)\n");
        printf("4. Editar Fichas / Preco / Status do Pacote\n");
        printf("5. Remover Pacote\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        opcao = lerInteiro();

        switch (opcao) {
            case 1: cadastrarPacote(); break;
            case 2: listarPacotes(); break;
            case 3: buscarPacote(); break;
            case 4: editarPacote(); break;
            case 5: removerPacote(); break;
            case 0: break;
            default: printf("\n[ERRO] Opcao invalida no Modulo de Pacotes!\n");
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

#endif
