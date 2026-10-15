#ifndef PRACTICE_ARQUIVOS_H
#define PRACTICE_ARQUIVOS_H

#include "livro.h"

int texto_carregar(const char *nome, Base *base);
int texto_salvar(const char *nome, const Base *base);
int binario_carregar(const char *nome, Base *base);
int binario_salvar(const char *nome, const Base *base);

#endif //PRACTICE_ARQUIVOS_H
