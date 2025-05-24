#ifndef PARSE_H
#define PARSE_H

#include "lexer.h"
#include "ast.h"

extern Token current_token;

//Função principal para começar a parsear
void parse_lista_comandos(AST *comandos[], int *num_comandos);

#endif