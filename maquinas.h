// ============================================================================
// Arquivo: maquinas.h | Autor: Nicolas (RGM: 48192295)
// MODULO 2: Maquinas de Arcade do salao (persistido em maquinas.csv)
// ============================================================================
#ifndef MAQUINAS_H
#define MAQUINAS_H

#include "util.h"

#define MAX_MAQUINAS 50

struct Maquina {
    int idMaquina;
    char nomeJogo[40];      // ex: "Street Fighter II"
    char fabricante[30];    // ex: "Capcom"
    int anoLancamento;
    char genero;            // L=Luta, T=Tiro, C=Corrida, E=Esporte, B=Briga de rua
    int fichasPorPartida;   // quantas fichas a maquina consome por partida
    int funcionando;        // 1 = Funcionando, 0 = Em manutencao
};

struct Maquina listaMaquinas[MAX_MAQUINAS];
int totalMaquinas = 0;

// ----------------------------------------------------------------------------
// PERSISTENCIA (modo "w" zera o arquivo e regrava o vetor atualizado)
// ----------------------------------------------------------------------------
void salvarMaquinasCSV() {
    int i;
    FILE *arq = fopen("maquinas.csv", "w");
    if (arq == NULL) {
        printf("[ERRO] Nao foi possivel gravar maquinas.csv!\n");
        return;
    }
    for (i = 0; i < totalMaquinas; i++) {
        fprintf(arq, "%d;%s;%s;%d;%c;%d;%d\n",
                listaMaquinas[i].idMaquina,
                listaMaquinas[i].nomeJogo,
                listaMaquinas[i].fabricante,
                listaMaquinas[i].anoLancamento,
                listaMaquinas[i].genero,
                listaMaquinas[i].fichasPorPartida,
                listaMaquinas[i].funcionando);
    }
    fclose(arq);
}

// Le maquinas.csv na abertura. fscanf devolve quantos campos leu:
// so aceita a linha se leu os 7 campos.
void carregarMaquinasCSV() {
    FILE *arq = fopen("maquinas.csv", "r");
    if (arq == NULL) return;
    totalMaquinas = 0;
    while (totalMaquinas < MAX_MAQUINAS &&
           fscanf(arq, "%d;%39[^;];%29[^;];%d;%c;%d;%d\n",
                  &listaMaquinas[totalMaquinas].idMaquina,
                  listaMaquinas[totalMaquinas].nomeJogo,
                  listaMaquinas[totalMaquinas].fabricante,
                  &listaMaquinas[totalMaquinas].anoLancamento,
                  &listaMaquinas[totalMaquinas].genero,
                  &listaMaquinas[totalMaquinas].fichasPorPartida,
                  &listaMaquinas[totalMaquinas].funcionando) == 7) {
        totalMaquinas++;
    }
    fclose(arq);
}

// Retorna o indice da maquina no vetor, ou -1 se nao existir.
// Usada tambem pelo modulo de Partidas (Matias).
int buscarIndiceMaquina(int idProcurado) {
    int i;
    for (i = 0; i < totalMaquinas; i++) {
        if (listaMaquinas[i].idMaquina == idProcurado) return i;
    }
    return -1;
}

// Retorna 1 se a letra e um genero conhecido, 0 se nao
int generoValido(char g) {
    switch (g) {
        case 'L': case 'T': case 'C': case 'E': case 'B':
            return 1;
        default:
            return 0;
    }
}

// Escreve o nome do genero por extenso, com largura fixa para alinhar a tabela
void imprimirGenero(char g) {
    switch (g) {
        case 'L': printf("Luta        "); break;
        case 'T': printf("Tiro        "); break;
        case 'C': printf("Corrida     "); break;
        case 'E': printf("Esporte     "); break;
        case 'B': printf("Briga de rua"); break;
        default:  printf("Outro       ");
    }
}

void imprimirCabecalhoMaquinas() {
    printf(" ID | Jogo                       | Fabricante | Ano  | Genero       | Fichas | Status\n");
    printf("----+----------------------------+------------+------+--------------+--------+-----------\n");
}

void imprimirMaquina(int i) {
    printf("%3d | %-26s | %-10s | %d | ",
           listaMaquinas[i].idMaquina, listaMaquinas[i].nomeJogo,
           listaMaquinas[i].fabricante, listaMaquinas[i].anoLancamento);
    imprimirGenero(listaMaquinas[i].genero);
    printf(" | %6d | ", listaMaquinas[i].fichasPorPartida);
    if (listaMaquinas[i].funcionando == 1) {
        printf("OK\n");
    } else {
        printf("MANUTENCAO\n");
    }
}

// ----------------------------------------------------------------------------
// CADASTRO (trava de ID repetido + validacao de ano, genero e fichas)
// ----------------------------------------------------------------------------
void cadastrarMaquina() {
    int id;
    struct Maquina nova;

    if (totalMaquinas >= MAX_MAQUINAS) {
        printf("[ERRO] O salao ja tem %d maquinas (limite)!\n", MAX_MAQUINAS);
        return;
    }
    printf("\n--- NOVA MAQUINA ---\n");
    printf("ID da maquina: ");
    id = lerInteiro();
    if (id <= 0) {
        printf("[ERRO] O ID precisa ser maior que zero.\n");
        return;
    }
    if (buscarIndiceMaquina(id) != -1) {
        printf("[ERRO] Ja existe uma maquina com o ID %d!\n", id);
        return;
    }
    nova.idMaquina = id;

    printf("Nome do jogo: ");
    scanf(" %39[^\n]", nova.nomeJogo);
    limparBuffer();
    printf("Fabricante: ");
    scanf(" %29[^\n]", nova.fabricante);
    limparBuffer();
    if (temPontoEVirgula(nova.nomeJogo) || temPontoEVirgula(nova.fabricante)) {
        printf("[ERRO] Os textos nao podem conter ';'.\n");
        return;
    }

    printf("Ano de lancamento (1970 a 2005): ");
    nova.anoLancamento = lerInteiro();
    if (nova.anoLancamento < 1970 || nova.anoLancamento > 2005) {
        printf("[ERRO] Ano fora da era dos fliperamas!\n");
        return;
    }

    printf("Genero (L=Luta, T=Tiro, C=Corrida, E=Esporte, B=Briga de rua): ");
    nova.genero = lerLetra();
    if (!generoValido(nova.genero)) {
        printf("[ERRO] Genero invalido!\n");
        return;
    }

    printf("Fichas por partida (1 a 5): ");
    nova.fichasPorPartida = lerInteiro();
    if (nova.fichasPorPartida < 1 || nova.fichasPorPartida > 5) {
        printf("[ERRO] Use de 1 a 5 fichas por partida.\n");
        return;
    }
    nova.funcionando = 1;   // maquina nova entra funcionando

    listaMaquinas[totalMaquinas] = nova;
    totalMaquinas++;
    salvarMaquinasCSV();
    printf("[OK] %s instalada no salao e salva em maquinas.csv!\n", nova.nomeJogo);
}

// ----------------------------------------------------------------------------
// LISTAGEM
// ----------------------------------------------------------------------------
void listarMaquinas() {
    int i, emManutencao = 0;
    printf("\n--- MAQUINAS DO SALAO (%d) ---\n", totalMaquinas);
    if (totalMaquinas == 0) {
        printf("Nenhuma maquina cadastrada.\n");
        return;
    }
    imprimirCabecalhoMaquinas();
    for (i = 0; i < totalMaquinas; i++) {
        imprimirMaquina(i);
        if (listaMaquinas[i].funcionando == 0) emManutencao++;
    }
    printf("\nFuncionando: %d | Em manutencao: %d\n", totalMaquinas - emManutencao, emManutencao);
}

// ----------------------------------------------------------------------------
// BUSCA TEXTUAL com strstr (procura no nome do jogo E no fabricante)
// ----------------------------------------------------------------------------
void buscarMaquina() {
    int i, achou = 0;
    char termo[40];

    printf("\nDigite parte do nome do jogo ou do fabricante (ex: Fighter, Capcom): ");
    scanf(" %39[^\n]", termo);
    limparBuffer();

    for (i = 0; i < totalMaquinas; i++) {
        if (strstr(listaMaquinas[i].nomeJogo, termo) != NULL ||
            strstr(listaMaquinas[i].fabricante, termo) != NULL) {
            if (achou == 0) imprimirCabecalhoMaquinas();
            imprimirMaquina(i);
            achou++;
        }
    }
    if (achou == 0) {
        printf("Nenhuma maquina encontrada com \"%s\".\n", termo);
    } else {
        printf("%d maquina(s) encontrada(s).\n", achou);
    }
}

// ----------------------------------------------------------------------------
// EDICAO: fichas por partida e status de manutencao (regrava o CSV)
// ----------------------------------------------------------------------------
void editarMaquina() {
    int id, pos, fichas, status;

    printf("\nID da maquina que deseja EDITAR: ");
    id = lerInteiro();
    pos = buscarIndiceMaquina(id);
    if (pos == -1) {
        printf("[ERRO] Maquina nao encontrada!\n");
        return;
    }
    imprimirCabecalhoMaquinas();
    imprimirMaquina(pos);

    printf("\nNovas fichas por partida (1 a 5): ");
    fichas = lerInteiro();
    if (fichas < 1 || fichas > 5) {
        printf("[ERRO] Valor invalido. Nada foi alterado.\n");
        return;
    }
    printf("Status (1 = Funcionando, 0 = Em manutencao): ");
    status = lerInteiro();
    if (status != 0 && status != 1) {
        printf("[ERRO] Status invalido. Nada foi alterado.\n");
        return;
    }

    listaMaquinas[pos].fichasPorPartida = fichas;
    listaMaquinas[pos].funcionando = status;
    salvarMaquinasCSV();
    printf("[OK] Maquina atualizada na memoria e no maquinas.csv!\n");
}

// ----------------------------------------------------------------------------
// REMOCAO fisica do vetor + regravacao do CSV
// ----------------------------------------------------------------------------
void removerMaquina() {
    int id, pos, j;

    printf("\nID da maquina que deseja REMOVER do salao: ");
    id = lerInteiro();
    pos = buscarIndiceMaquina(id);
    if (pos == -1) {
        printf("[ERRO] Maquina nao encontrada!\n");
        return;
    }
    printf("Retirando %s do salao...\n", listaMaquinas[pos].nomeJogo);
    for (j = pos; j < totalMaquinas - 1; j++) {
        listaMaquinas[j] = listaMaquinas[j + 1];
    }
    totalMaquinas--;
    salvarMaquinasCSV();
    printf("[OK] Maquina removida do sistema e do maquinas.csv!\n");
}

// ----------------------------------------------------------------------------
// SUBMENU DO MODULO 2
// ----------------------------------------------------------------------------
void moduloMaquinas() {
    int opcao;
    do {
        limparTela();
        printf("--- [MODULO 2: MAQUINAS DE ARCADE (Nicolas)] ---\n");
        printf("1. Cadastrar Nova Maquina / Jogo\n");
        printf("2. Listar Maquinas\n");
        printf("3. Buscar Maquina (jogo ou fabricante)\n");
        printf("4. Editar Fichas por Partida / Manutencao\n");
        printf("5. Remover Maquina do Salao\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        opcao = lerInteiro();

        switch (opcao) {
            case 1: cadastrarMaquina(); break;
            case 2: listarMaquinas(); break;
            case 3: buscarMaquina(); break;
            case 4: editarMaquina(); break;
            case 5: removerMaquina(); break;
            case 0: break;
            default: printf("\n[ERRO] Opcao invalida no Modulo de Maquinas!\n");
        }
        if (opcao != 0) pausar();
    } while (opcao != 0);
}

#endif
