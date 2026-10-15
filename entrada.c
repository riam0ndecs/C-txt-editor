#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "entrada.h"

#define TAM_BUFFER 256

static int ler_linha(char *destino, size_t tam) {
    if (fgets(destino, (int)tam, stdin) == NULL) {
        return 0;
    }
    size_t n = strlen(destino);
    if (n > 0 && destino[n-1] == '\n') {
        destino[n-1] = '\0';
    }else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    return 1;
}

static int so_espacos(const char *s) {
    for (; *s != '\0'; s++) {
        if (!isspace((unsigned char)*s)) {
            return 0;
        }
    }
    return 1;
}

int ler_inteiro(const char *prompt, int minimo, int maximo, int *valor) {
    char buffer[TAM_BUFFER];

    while (1) {
        printf("%s", prompt);
        if (!ler_linha(buffer, sizeof(buffer))) {
            return 0;
        }

        char *fim;
        errno = 0;
        long v = strtol(buffer, &fim, 10);
        int convertido = (fim != buffer);
        while (isspace((unsigned char)*fim)) {
            fim++;
        }
        if (!convertido || *fim != '\0' || errno == ERANGE) {
            printf("Entrada invalida: digite um numero inteiro.\n");
        }else if (v < minimo || v > maximo){
            printf("Valor fora do intervalo(%d a %d).\n", minimo, maximo);
        }else {
            *valor = (int)v;
            return 1;
        }
    }
}

int ler_texto(const char *prompt, char *destino, size_t tam) {
    char buffer[TAM_BUFFER];

    while (1) {
        printf("%s", prompt);
        if (!ler_linha(buffer, sizeof(buffer))) {
            return 0;
        }

        if (so_espacos(buffer)) {
            printf(" O campo nao pode ficar vazio.\n");
        }else if (strchr(buffer, ';') != NULL) {
            printf(" O caractere ';' nao sera permitido ( o separador do arquivo texto).\n");
        }else if (strlen(buffer) >= tam) {
            printf(" Texto muito longo (maximo %d caracteres).\n", (int)(tam-1));
        }else {
            strcpy(destino, buffer);
            return 1;
        }
    }
}

int ler_sim_nao(const char *prompt, int *resposta) {
    char buffer[TAM_BUFFER];

    while (1) {
        printf("%s", prompt);
        if (!ler_linha(buffer,sizeof(buffer))) {
            return 0;
        }

        if (buffer[0] != '\0' && buffer[1] == '\0') {
            char c = (char)tolower((unsigned char)buffer[0]);
            if (c == 's' || c == 'n') {
                *resposta = (c == 's');
                return 1;
            }
        }
        printf("Responda com 's' ou 'n'.\n");
    }
}