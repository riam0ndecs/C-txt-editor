#ifndef PRACTICE_ENTRADA_H
#define PRACTICE_ENTRADA_H

#include <stddef.h>

int ler_inteiro(const char *prompt, int minimo, int maximo, int *valor);
int ler_texto(const char *prompt, char *destino, size_t tam);
int ler_sim_nao(const char *prompt, int *resposta);

#endif //PRACTICE_ENTRADA_H