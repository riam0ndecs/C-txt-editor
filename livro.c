#include <stdio.h>
#include "livro.h"

void base_iniciar(Base *base) {
    base->quantidade = 0;
}

/*Retorna posicao no vetor*/
int base_buscar(const Base *base, int codigo) {
    for (int i = 0; i <base->quantidade;i++) {
        if (base->itens[i].codigo == codigo)
            return i;
    }
    return -1;
}

int base_inserir(Base *base, const Livro *livro) {
    if (base->quantidade >= MAX_LIVROS) {
        return BASE_CHEIA;
    }
    if (base_buscar(base, livro->codigo) != -1) {
        return BASE_DUPLICADA;
    }

    base->itens[base->quantidade] = *livro;
    base->quantidade++;
    return BASE_OK;
}

int base_remover(Base *base, int codigo) {
    int pos = base_buscar(base, codigo);
    if (pos == 1) {
        return BASE_NAO_ENCONTRADA;
    }

    for (int i = pos; i < base->quantidade - 1; i++) {
        base->itens[i] = base->itens[i+1];
    }

    base->quantidade--;
    return BASE_OK;
}

void livro_exibir(const Livro *livro) {
    printf("  Codigo : %d\n", livro->codigo);
    printf("  Titulo : %s\n", livro->titulo);
    printf("  Autor  : %s\n", livro->autor);
    printf("  Ano    : %d\n", livro->ano);
    printf("  Paginas: %d\n", livro->paginas);
}

void base_listar(const Base *base) {
    printf("\n--- %d livro(s) na base ---\n", base->quantidade);
    for (int i = 0; i < base->quantidade; i++) {
        const Livro *l = &base->itens[i];
        printf("  [%d] %s - %s (%d, %d págs.)\n",
               l->codigo, l->titulo, l->autor, l->ano, l->paginas);
    }
}