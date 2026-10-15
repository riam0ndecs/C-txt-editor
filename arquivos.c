#include <stdio.h>
#include <ctype.h>
#include "arquivos.h"

#define TAM_LINHA 256
#define BUFFER_SIZE 8

#define FORMATO_LEITURA "%d;%49[^;];%39[^;];%d;%d"
#define FORMATO_ESCRITA "%d;%s;%s;%d;%d\n"

static int linha_vazia(const char *linha) {
    for (; *linha != '\0'; linha++) {
        if (!isspace((unsigned char)*linha)) {
            return 0;
        }
    }
    return 1;
}

static int linha_para_livro(const char *linha, Livro *livro) {
    int lidos = sscanf(linha, FORMATO_LEITURA,
        &livro->codigo, livro->titulo, livro->autor,
        &livro->ano, &livro->paginas);
    return lidos == 5;
}

int texto_carregar(const char *nome, Base *base) {
    FILE *arq = fopen(nome, "r");
    if (arq == NULL) {
        perror(nome);
        return -1;
    }

    char linha[TAM_LINHA];
    int numero_linha = 0;

    while (fgets(linha, sizeof(linha), arq) != NULL) {
        numero_linha++;
        if (linha_vazia(linha)) {
            continue;
        }

        Livro livro;
        if (!linha_para_livro(linha, &livro)) {
            fprintf(stderr, "Aviso: linha %d mal formatada, ignorada.\n", numero_linha);
            continue;
        }

        int r = base_inserir(base, &livro);
        if (r == BASE_DUPLICADA) {
            fprintf(stderr,"Aviso: linha %d ignorada (codigo %d repetido).\n", numero_linha, livro.codigo);
        }else if (r == BASE_CHEIA) {
            fprintf(stderr,"Aviso: base cheia(%d), leitura interrompida.\n", MAX_LIVROS);
            break;
        }
    }

    fclose(arq);
    return 0;
}

int binario_salvar(const char *nome, const Base *base) {
    FILE *arq = fopen(nome, "wb");
    if (arq == NULL) {
        perror(nome);
        return -1;
    }

    size_t escritos = fwrite(base->itens, sizeof(Livro),(size_t)base->quantidade, arq);
    int ok = (escritos == (size_t)base->quantidade);

    if (fclose(arq) != 0) {
        ok = 0;
    }
    if (!ok) {
        fprintf(stderr,"Erro ao gravar '%s'.\n", nome);
        return -1;
    }
    return 0;
}

int binario_carregar(const char *nome, Base *base) {
    FILE *arq = fopen(nome, "rb");
    if (arq == NULL) {
        perror(nome);
        return -1;
    }

    Livro buffer[BUFFER_SIZE];
    size_t lido;

    do {
        lido = fread(buffer, sizeof(Livro), BUFFER_SIZE, arq);
        for (size_t i = 0; i < lido; i++) {
            if (base_inserir(base, &buffer[i]) != BASE_OK) {
                fprintf(stderr,"Aviso: registro ignorado (codigo %d).\n", buffer[i].codigo);
            }
        }
    }while (lido == BUFFER_SIZE);

    if (ferror(arq)) {
        fprintf(stderr,"Erro ao ler '%s'.\n", nome);
        fclose(arq);
        return -1;
    }

    fclose(arq);
    return 0;
}

int texto_salvar(const char *nome, const Base *base) {
    FILE *arq = fopen(nome, "w");
    if (arq == NULL) {
        perror(nome);
        return -1;
    }

    for (int i = 0; i < base->quantidade; i++){
        const Livro *l = &base->itens[i];
        fprintf(arq, FORMATO_ESCRITA, l->codigo, l->titulo, l->autor, l->ano, l->paginas);
    }

    fclose(arq);
    return 0;
}