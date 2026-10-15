#include <stdio.h>
#include "livro.h"
#include "arquivos.h"

static Base base;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <arquivo_texto_entrada> <arquivo_binario_saida>\n", argv[0]);
        return 1;
    }

    base_iniciar(&base);

    if (texto_carregar(argv[1], &base)!=0) {
        return 1;
    }
    printf("%d livro(s) lido(s) de '%s'.\n", base.quantidade, argv[1]);

    if (binario_salvar(argv[2], &base)!=0) {
        return 1;
    }
    printf("Arquivo binario '%s' gerado com sucesso.\n", argv[2]);

    return 0;
}