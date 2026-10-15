//
// Created by Rafael on 14/10/2026.
//

#ifndef PRACTICE_LIVRO_H
#define PRACTICE_LIVRO_H

#define TAM_TITULO 50
#define TAM_AUTOR 40
#define MAX_LIVROS 1000

typedef struct {
    int codigo;
    char titulo[TAM_TITULO];
    char autor[TAM_AUTOR];
    int ano;
    int paginas;
}Livro;

typedef struct {
    Livro itens[MAX_LIVROS];
    int quantidade;
}Base;

enum {
    BASE_OK = 0,
    BASE_CHEIA,
    BASE_DUPLICADA,
    BASE_NAO_ENCONTRADA
};

void base_iniciar(Base *base);
int  base_buscar(const Base *base, int codigo);
int  base_inserir(Base *base, const Livro *livro);
int  base_remover(Base *base, int codigo);
void livro_exibir(const Livro *livro);
void base_listar(const Base *base);


#endif //PRACTICE_LIVRO_H