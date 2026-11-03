#include <stdio.h>
#include <string.h>

// ============================================================================
// PROJETO: MEGA ARCADE 90 - SISTEMA DE FICHAS E HIGH SCORES
// Tema: Fliperama / Arcade dos Anos 90
// Disciplina: Algoritmos e Pensamento Computacional - Checkpoint 1
// ============================================================================


// ============================================================================
// PARTE 1: MODELAGEM DE DADOS (ESCOPO GLOBAL - 1 STRUCT POR INTEGRANTE)
// ============================================================================

// 1. Perspectiva do Pedro: Jogadores Cadastrados no Fliperama
struct Jogador {
    int idJogador;
    char nome[50];
    char iniciais[4];      // 3 letras do placar (ex: "AAA") + '\0'
    int saldoFichas;       // fichas que o jogador ainda tem para jogar
    float totalGasto;      // quanto o jogador ja gastou em fichas (R$)
    int ativo;             // 1 = Ativo, 0 = Removido
};

struct Jogador listaJogadores[50];
int totalJogadores = 0;

// 2. Perspectiva do Nicolas: Maquinas de Arcade (Gabinetes)
struct Maquina {
    int idMaquina;
    char nomeJogo[40];     // ex: "Street Fighter II", "Metal Slug"
    char fabricante[30];   // ex: "Capcom", "SNK", "Namco"
    int anoLancamento;
    int fichasPorPartida;  // quantas fichas a maquina consome por partida
    int funcionando;       // 1 = Funcionando, 0 = Em manutencao
};

struct Maquina listaMaquinas[50];
int totalMaquinas = 0;

// ============================================================================
// PARTE 2: FUNCOES DE INTERFACE E MODULOS DE CADA INTEGRANTE
// ============================================================================

// Funcao Visual Comum da Equipe
void exibirCabecalho() {
    printf("\n======================================================\n");
    printf("   MEGA ARCADE 90 - FICHAS & HIGH SCORES (1994)       \n");
    printf("            >>> INSERT COIN TO CONTINUE <<<           \n");
    printf("======================================================\n");
}

// ----------------------------------------------------------------------------
// MODULO 1: PERSPECTIVA DE JOGADORES
// Autor: Pedro 
// ----------------------------------------------------------------------------
void moduloJogadores() {
    int opcaoJogador;
    do {
        printf("\n--- [MODULO 1: CADASTRO DE JOGADORES (Pedro)] ---\n");
        printf("1. Cadastrar Novo Jogador\n");
        printf("2. Listar / Buscar Jogadores\n");
        printf("3. Editar Dados ou Saldo de Fichas do Jogador\n");
        printf("4. Remover Jogador\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        scanf("%d", &opcaoJogador);

        switch (opcaoJogador) {
            case 1:
                printf("\n[Em construcao] Rotina de Cadastro de Jogador...\n");
                printf("Campos previstos: ID, Nome, Iniciais do placar e Fichas iniciais.\n");
                break;
            case 2:
                printf("\n[Em construcao] Rotina de Listagem e Busca de Jogadores (por ID ou iniciais)...\n");
                break;
            case 3:
                printf("\n[Em construcao] Rotina de Edicao de Jogador / Recarga de fichas por ID...\n");
                break;
            case 4:
                printf("\n[Em construcao] Rotina de Remocao de Jogador...\n");
                break;
            case 0:
                printf("\nRetornando ao menu principal...\n");
                break;
            default:
                printf("\n[ERRO] Opcao invalida no Modulo de Jogadores!\n");
        }
    } while (opcaoJogador != 0);
}

// ----------------------------------------------------------------------------
// MODULO 2: PERSPECTIVA DE MAQUINAS DE ARCADE
// Autor: Nicolas 
// ----------------------------------------------------------------------------
void moduloMaquinas() {
    int opcaoMaquina;
    do {
        printf("\n--- [MODULO 2: MAQUINAS DE ARCADE (Nicolas)] ---\n");
        printf("1. Cadastrar Nova Maquina / Jogo\n");
        printf("2. Listar Maquinas / Buscar Jogo\n");
        printf("3. Alterar Fichas por Partida ou Status de Manutencao\n");
        printf("4. Remover Maquina do Salao\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        scanf("%d", &opcaoMaquina);

        switch (opcaoMaquina) {
            case 1:
                printf("\n[Em construcao] Cadastrando Maquina (ID, Jogo, Fabricante, Ano, Fichas por partida)...\n");
                break;
            case 2:
                printf("\n[Em construcao] Listando todas as maquinas do salao...\n");
                break;
            case 3:
                printf("\n[Em construcao] Atualizando fichas por partida ou colocando em manutencao...\n");
                break;
            case 4:
                printf("\n[Em construcao] Retirando maquina quebrada do salao...\n");
                break;
            case 0:
                printf("\nRetornando ao menu principal...\n");
                break;
            default:
                printf("\n[ERRO] Opcao invalida no Modulo de Maquinas!\n");
        }
    } while (opcaoMaquina != 0);
}

// ============================================================================
// PARTE 3: FUNCAO PRINCIPAL (MAIN) - DESPACHANTE DE MENUS
// ============================================================================
int main() {
    int opcaoPrincipal;

    do {
        exibirCabecalho();
        printf("SELECIONE A PERSPECTIVA DO SISTEMA:\n");
        printf("1. Modulo de Jogadores (Pedro)\n");
        printf("2. Modulo de Maquinas de Arcade (Nicolas)\n");
        printf("0. Encerrar o Sistema\n");
        printf("------------------------------------------------------\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &opcaoPrincipal);

        switch (opcaoPrincipal) {
            case 1:
                moduloJogadores();
                break;
            case 2:
                moduloMaquinas();
                break;
            case 0:
                printf("\nGAME OVER! Encerrando o Mega Arcade 90... Ate a proxima ficha!\n");
                break;
            default:
                printf("\n[ALERTA] Opcao invalida! Digite um numero de 0 a 2.\n");
        }
    } while (opcaoPrincipal != 0);

    return 0;
}
