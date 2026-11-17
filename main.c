// ============================================================================
// PROJETO: MEGA ARCADE 90 - SISTEMA DE FICHAS E HIGH SCORES
// Tema: Fliperama / Arcade dos Anos 90
// Disciplina: Algoritmos e Pensamento Computacional - Checkpoint 2
// Arquivo: main.c | Autores: Pedro, Nicolas, Gabriel e Matias
// ============================================================================
#include <stdio.h>
#include <string.h>
#include <locale.h>

// Cada integrante tem o seu proprio modulo (.h), importado aqui:
#include "util.h"        // Pedro   - funcoes de tela e leitura
#include "jogadores.h"   // Pedro   - Modulo 1
#include "maquinas.h"    // Nicolas - Modulo 2
#include "pacotes.h"     // Gabriel - Modulo 3
#include "partidas.h"    // Matias  - Modulo 4

// Funcao Visual Comum da Equipe
void exibirCabecalho() {
    printf("======================================================\n");
    printf("   MEGA ARCADE 90 - FICHAS & HIGH SCORES (EST. 1991)  \n");
    printf("            >>> INSERT COIN TO CONTINUE <<<           \n");
    printf("======================================================\n");
}

// ============================================================================
// FUNCAO PRINCIPAL (MAIN) - CARREGA OS CSVs E DESPACHA OS MENUS
// ============================================================================
int main() {
    int opcaoPrincipal;

    setlocale(LC_ALL, "Portuguese");
    // Mantem o PONTO como separador decimal: sem isso, o locale portugues
    // gravaria "4,50" no CSV e o fscanf nao leria os precos de volta.
    setlocale(LC_NUMERIC, "C");

    // Carrega automaticamente os dados salvos de cada modulo
    carregarJogadoresCSV();
    carregarMaquinasCSV();
    carregarPacotesCSV();
    carregarPartidasCSV();

    do {
        limparTela();
        exibirCabecalho();
        printf("Dados carregados: %d jogadores | %d maquinas | %d pacotes | %d partidas\n",
               totalJogadores, totalMaquinas, totalPacotes, totalPartidas);
        printf("------------------------------------------------------\n");
        printf("SELECIONE A PERSPECTIVA DO SISTEMA:\n");
        printf("1. Modulo de Jogadores (Pedro)\n");
        printf("2. Modulo de Maquinas de Arcade (Nicolas)\n");
        printf("3. Modulo de Pacotes de Fichas (Gabriel)\n");
        printf("4. Modulo de Partidas e High Scores (Matias)\n");
        printf("0. Encerrar o Sistema\n");
        printf("------------------------------------------------------\n");
        printf("Digite a opcao desejada: ");
        opcaoPrincipal = lerInteiro();

        switch (opcaoPrincipal) {
            case 1:
                moduloJogadores();
                break;
            case 2:
                moduloMaquinas();
                break;
            case 3:
                moduloPacotesFichas();
                break;
            case 4:
                moduloPartidasRelatorios();
                break;
            case 0:
                printf("\nGAME OVER! Dados salvos nos arquivos .csv. Ate a proxima ficha!\n");
                break;
            default:
                printf("\n[ALERTA] Opcao invalida! Digite um numero de 0 a 4.\n");
                pausar();
        }
    } while (opcaoPrincipal != 0);

    return 0;
}
