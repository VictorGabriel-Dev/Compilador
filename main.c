#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"

int main(void) {
    const char *codigo = "x = 3 + 4 * 2; print(x);";
    init_lexer(codigo);              // Chamamos init_lexer que está definido em lexer.c
    current_token = next_token();    // Chamamos next_token que está definido em lexer.c

    AST *comandos[100];
    int num_comandos = 0;

    parse_lista_comandos(comandos, &num_comandos);

    printf("Foram lidos %d comandos e montadas %d árvores AST.\n",
           num_comandos, num_comandos);

    return 0;
}
