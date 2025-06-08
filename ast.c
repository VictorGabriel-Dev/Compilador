#include <stdlib.h>
#include <string.h>
#include "ast.h"

AST* new_int_node(int value) {
    AST *node = (AST*)malloc(sizeof(AST));
    node->type = NODE_INT;
    node->int_val = value;
    node->float_val = (float)value;
    node->left = node->right = NULL;
    return node;
}

AST* new_float_node(float value) {
    AST *node = malloc(sizeof(AST));
    node->type = NODE_FLOAT;
    node->float_val = value;
    node->int_val = (int)value;
    node->left = node->right = NULL;
    return node;
}

AST* new_var_node(const char *name) {
    AST *node = (AST*)malloc(sizeof(AST));
    node->type = NODE_VAR;
    strncpy(node->name, name, 63);
    node->name[63] = '\0';
    node->left = node->right = NULL;
    return node;
}

AST* new_binary_node(NodeType type, AST *left, AST* right) {
    AST *node = (AST*)malloc(sizeof(AST));
    node->type = type;
    node->left = left;
    node->right = right;
    return node;
}

AST* new_assign_node(const char *name, AST* expr) {
    AST *node = (AST*)malloc(sizeof(AST));
    node->type = NODE_ASSIGN;
    strncpy(node->name, name, 63);
    node->name[63] = '\0';
    node->left = expr;
    node->right = NULL;
    return node;
}

AST* new_print_node(AST *expr) {
    AST *node = (AST*)malloc(sizeof(AST));
    node->type = NODE_PRINT;
    node->left = expr; //guarda a sub-árvore de expr
    node->right = NULL;
    return node;
}