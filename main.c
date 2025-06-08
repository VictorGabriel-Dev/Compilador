#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
//#include "imc.h"

int main(void)
{
    char codigo[256];
    printf("Dígite o programa (ex: altura = 1.80; peso = 70; imc = peso / (altura * altura); print(imc); ): \n");
    fgets(codigo,sizeof(codigo), stdin);
    //  Inicializar o lexer
    init_lexer(codigo);           // Chamamos init_lexer que está definido em lexer.c
    current_token = next_token(); // Chamamos next_token que está definido em lexer.c

    AST *comandos[100];
    int num_comandos = 0;

    parse_lista_comandos(comandos, &num_comandos);

    printf("Foram lidos %d comandos e montadas %d árvores AST.\n",num_comandos, num_comandos);
    
    return 0;
}