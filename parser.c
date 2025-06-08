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
        current_token = next_token();
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

// Parse de “termo” = fator (( '*' | '/' ) fator)
static AST* parse_termo(void) {
    AST *node = parse_fator();

    while (current_token.type == TOKEN_STAR || current_token.type == TOKEN_SLASH) {
        TokenType op = current_token.type;
        if (op == TOKEN_STAR) {
            eat(TOKEN_STAR);
            AST *right = parse_fator();
            node = new_binary_node(NODE_MUL, node, right);
        } else { // op == TOKEN_SLASH
            eat(TOKEN_SLASH);
            AST *right = parse_fator();
            node = new_binary_node(NODE_DIV, node, right);
        }
    }
    return node;
}


// Parse de “expr” = termo (( '+' | '-' ) termo)*
static AST* parse_expr(void) {
    AST *node = parse_termo();

    while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS) {
        TokenType op = current_token.type;
        if (op == TOKEN_PLUS) {
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

    if (current_token.type == TOKEN_INT) {
        // É nó número
        node = new_int_node(current_token.int_val);
        eat(TOKEN_INT);
        return node;
    }
    else if (current_token.type == TOKEN_FLOAT) {
        // Literal ponto flutuante
        node = new_float_node(current_token.float_val);
        eat(TOKEN_FLOAT);
        return node;
    }
    else if (current_token.type == TOKEN_IDEN) {
        // Pode ser variável (uso) ou atribuição
        char name[64];
        strcpy(name, current_token.lexeme);
        eat(TOKEN_IDEN);

        if (current_token.type == TOKEN_ASSIGN) {
            // É atribuição: x = expr
            eat(TOKEN_ASSIGN);
            AST *expr_node = parse_expr();
            return new_assign_node(name, expr_node);
        } else {
            // É só uso de variável: retorna nó VAR
            return new_var_node(name);
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