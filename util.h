// ============================================================================
// Arquivo: util.h | Autor: Pedro (RGM: 48163457)
// Funcoes de apoio usadas por TODOS os modulos (tela e leitura do teclado).
// ============================================================================
#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>   // system() e exit()
#include <string.h>

// Limpa o console. _WIN32 e definido automaticamente pelo compilador no Windows
// (Dev-C++), entao la usamos "cls"; no Linux/Mac o comando e "clear".
void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Joga fora tudo o que sobrou no teclado ate o ENTER ('\n').
// Sem isso, o '\n' deixado pelo scanf atrapalha a proxima leitura.
void limparBuffer() {
    int c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

// Segura a tela ate o usuario apertar ENTER (para dar tempo de ler o resultado)
void pausar() {
    printf("\nPressione ENTER para continuar...");
    limparBuffer();
}

// Le um numero inteiro. Se o usuario digitar letras, pede de novo
// (scanf devolve quantos campos conseguiu ler: 1 = deu certo).
int lerInteiro() {
    int valor;
    int lidos = scanf("%d", &valor);
    while (lidos != 1) {
        if (lidos == EOF) exit(0);   // a entrada acabou: encerra o programa
        limparBuffer();
        printf("[ERRO] Digite um numero inteiro: ");
        lidos = scanf("%d", &valor);
    }
    limparBuffer();
    return valor;
}

// Mesma ideia do lerInteiro, mas para numeros com casas decimais (use ponto: 4.50)
float lerFloat() {
    float valor;
    int lidos = scanf("%f", &valor);
    while (lidos != 1) {
        if (lidos == EOF) exit(0);
        limparBuffer();
        printf("[ERRO] Digite um numero (use ponto, ex: 4.50): ");
        lidos = scanf("%f", &valor);
    }
    limparBuffer();
    return valor;
}

// Converte uma letra minuscula em maiuscula usando a tabela ASCII:
// 'a' (97) - 32 = 'A' (65)
char paraMaiuscula(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 32;
    }
    return c;
}

// Le UMA letra (ja convertida para maiuscula). O espaco antes do %c
// pula ENTERs e espacos que tenham ficado no teclado.
char lerLetra() {
    char c;
    if (scanf(" %c", &c) != 1) exit(0);
    limparBuffer();
    return paraMaiuscula(c);
}

// Retorna 1 se o texto tem ';' (proibido: quebraria as colunas do CSV), 0 se nao
int temPontoEVirgula(char texto[]) {
    if (strchr(texto, ';') != NULL) {
        return 1;
    }
    return 0;
}

#endif
