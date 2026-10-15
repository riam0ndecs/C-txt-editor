#include <stdio.h>
#include "livro.h"
#include "arquivos.h"
#include "entrada.h"

enum{ OP_SAIR = 0, OP_INSERIR, OP_REMOVER, OP_BUSCAR, OP_LISTAR, OP_SALVAR };

static Base base;

static void exibir_menu(void) {
    printf("\n===== EDITOR DE LIVROS =====\n");
    printf("1 - Inserir livro\n");
    printf("2 - Remover livro\n");
    printf("3 - Buscar livro por codigo\n");
    printf("4 - Listar todos\n");
    printf("5 - Salvar arquivo binario agora\n");
    printf("0 - Sair\n");
}

static void op_buscar(const Base *base)
{
    int codigo;

    if (!ler_inteiro("Codigo do livro: ", 1, 999999, &codigo))
        return;

    int pos = base_buscar(base, codigo);
    if (pos == -1)
        printf("Livro %d nao encontrado.\n", codigo);
    else
        livro_exibir(&base->itens[pos]);
}

static int ler_dados_livro(Livro *livro)
{
    return ler_texto("Titulo: ", livro->titulo, TAM_TITULO)
        && ler_texto("Autor: ", livro->autor, TAM_AUTOR)
        && ler_inteiro("Ano de publicacaoo: ", 1, 2100, &livro->ano)
        && ler_inteiro("Numero de paginas: ", 1, 10000, &livro->paginas);
}

static void op_inserir(Base *base, int *alterado)
{
    Livro novo;

    if (!ler_inteiro("Codigo (chave unica): ", 1, 999999, &novo.codigo))
        return;
    if (base_buscar(base, novo.codigo) != -1) {
        printf("Ja existe um livro com o codigo %d.\n", novo.codigo);
        return;
    }
    if (!ler_dados_livro(&novo))
        return;

    if (base_inserir(base, &novo) == BASE_OK) {
        *alterado = 1;
        printf("Livro inserido.\n");
    } else {
        printf("Nao foi possivel inserir (base cheia).\n");
    }
}

static void op_remover(Base *base, int *alterado)
{
    int codigo, confirma;

    if (!ler_inteiro("Codigo do livro a remover: ", 1, 999999, &codigo))
        return;

    int pos = base_buscar(base, codigo);
    if (pos == -1) {
        printf("Livro %d não encontrado.\n", codigo);
        return;
    }

    livro_exibir(&base->itens[pos]);
    if (!ler_sim_nao("Confirma a remocao? (s/n): ", &confirma) || !confirma) {
        printf("Remocaoo cancelada.\n");
        return;
    }

    base_remover(base, codigo);
    *alterado = 1;
    printf("Livro removido.\n");
}

static void op_salvar(const Base *base, const char *arquivo, int *alterado)
{
    if (binario_salvar(arquivo, base) == 0) {
        *alterado = 0;
        printf("Dados salvos em '%s'.\n", arquivo);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr,"Uso: %s <arquivo_binario_entrada> <arquivo_binario_saida>\n", argv[0]);
        return 1;
    }

    base_iniciar(&base);
    if (binario_carregar(argv[1], &base) != 0) {
        return 1;
    }
    printf("%d livro(s) carregado(s) de '%s'.\n", base.quantidade, argv[1]);

    int opcao;
    int alterado = 0;
    do {
        exibir_menu();
        if (!ler_inteiro("Opcao: ", 0,5, &opcao)) {
            opcao = OP_SAIR;
        }
        switch (opcao) {
            case OP_LISTAR: base_listar(&base); break;
            case OP_BUSCAR: op_buscar(&base); break;
            case OP_INSERIR: op_inserir(&base, &alterado); break;
            case OP_REMOVER: op_remover(&base, &alterado); break;
            case OP_SALVAR:  op_salvar(&base, argv[2], &alterado);  break;
            default: break;
        }
    }while (opcao != OP_SAIR);
    if (alterado) {
        printf("Ha alteracoes nao salvas. Gravando...\n");
        op_salvar(&base, argv[2], &alterado);
        if (alterado)
            return 1;
    }

    printf("Encerrado.\n");
    return 0;
}