#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"

Token current_token;

//Metodo eat para "comer e avançar" se for o token esperado
static void eat(TokenType esperado) {
    if (current_token.type == esperado){
        current_token.type = next_token();
    } else {
        printf("Erro sintático: esperado token %d, mas veio %d (lexeme='%s')\n", esperado, current_token.type, current_token.lexeme);
        exit(1);
    }
}

//Declarações antecipadas das funções de parsing (para ordem)
static AST* parse_fator(void);
static AST* parse_termo(void);
static AST* parse_expr(void);
static AST* parse_comando(void);

//Parse lista de comandos
void parse_lista_comandos(AST *comandos[], int *num_comandos){
    *num_comandos = 0;

    //primeiro comando (pode ser print ou expr/atribuição)
    AST *c = parse_comando();
    comandos[(*num_comandos)++] = c;

    //Enquanto tiver ';' e não for EOF, continuar lendo
    while (current_token.type == TOKEN_SEMICOLON){
        eat(TOKEN_SEMICOLON);
        if (current_token.type == TOKEN_EOF) break;
        c = parse_comando();
        comandos[(*num_comandos)++] = c;
    }

    //Se não acamos em EOF, há erro
    if (current_token.type != TOKEN_EOF){
        printf("Erro: esperava ';' ou EOF, mas veio %s\n", current_token.lexeme);
        exit(1);
    }
}

//Parse de um comando: (pode ser print(expr) ou uma expressão/atribuição)
static AST* parse_comando(void) {
    if (current_token.type == TOKEN_PRINT){
        eat(TOKEN_PRINT);
        eat(TOKEN_LPAREN);
        AST *e = parse_expr();
        eat(TOKEN_RPAREN);
        return new_print_node(e);
    } else {
        //se não é print então deve ser expr
        return parse_expr();
    }
}

//Perse de expr = termo (( '+' | '-' )termo)*
static AST* parse_expr(void) {
    AST *node = parse_termo();

    while(current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS){
        TokenType op = current_token.type;
        if(op == TOKEN_PLUS){
            eat(TOKEN_PLUS);
            AST *right = parse_termo();
            node = new_binary_node(NODE_ADD, node, right);
        } else {
            eat(TOKEN_MINUS);
            AST *right = parse_termo();
            node = new_binary_node(NODE_SUB, node, right);
        }
    }
    return node;
}

static AST* parse_fator(void) {
    AST *node = NULL;

    if (current_token.type == TOKEN_NUM) {
        // É nó número
        node = new_num_node(current_token.value);
        eat(TOKEN_NUM);
        return node;
    }
    else if (current_token.type == TOKEN_IDEN) {
        // Pode ser variável (uso) ou atribuição
        char nome[64];
        strcpy(nome, current_token.lexeme);
        eat(TOKEN_IDEN);

        if (current_token.type == TOKEN_ASSIGN) {
            // É atribuição: x = expr
            eat(TOKEN_ASSIGN);
            AST *expr_node = parse_expr();
            return new_assign_node(nome, expr_node);
        } else {
            // É só uso de variável: retorna nó VAR
            return new_var_node(nome);
        }
    }
    else if (current_token.type == TOKEN_LPAREN) {
        // Expressão entre parênteses: '(' expr ')'
        eat(TOKEN_LPAREN);
        AST *e = parse_expr();
        eat(TOKEN_RPAREN);
        return e;
    }
    else {
        printf("Erro sintático: fator inesperado '%s'\n", current_token.lexeme);
        exit(1);
    }
}