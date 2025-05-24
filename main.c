#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "lexer.c"


int main(void) {
    const char *codigo = "x = 3 + 4 * 2; print(x);";
    init_lexer(codigo);

    current_token = next_token();
    
    //Vetor para armazenar os ponteiros para ASR de cada comando
    AST *comandos[100];
    int num_comandos = 0;

    //Chama o parser
    parse_lista_comandos(comandos, &num_comandos);

    printf("Foram lidos %d comandos e montadas %d árvores AST.\n", num_comandos, num_comandos);
    return 0;
    
/*    Token t;
    do {
        t = next_token();
        printf("Token: tipo=%d, lexeme='%s'", t.type, t.lexeme);
        if (t.type == TOKEN_NUM) {
            printf(", value=%d", t.value);
        }
        printf("\n");
    } while (t.type != TOKEN_EOF && t.type != TOKEN_INVALID);

    return 0;*/
}
