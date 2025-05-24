#ifndef AST_H
#define AST_H

#include <stddef.h>

typedef enum{
    NODE_NUM,
    NODE_ADD,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV,
    NODE_ASSIGN,
    NODE_VAR,
    NODE_PRINT,
} NodeType;

typedef struct AST{
    NodeType type;
    int value;         //Para type == NODE_NUM
    char nome[64];     //NODE_VAR ou NODE_ASSIGN
    struct AST *left;  //Ponteiro para sub-árvore esquerda
    struct AST *right; //Ponteiro para sub-árvore direta
} AST;

//Função auxiliar para criar cada tipo de nó
AST* new_num_node(int value);
AST* new_var_node(const char *name);
AST* new_binary_node(NodeType type, AST *left, AST *right);
AST* new_assign_node(const char *name, AST *expr);
AST* new_print_node(AST *expr);

#endif