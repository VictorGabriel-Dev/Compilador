#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "ast.h" // Inclui a definição da AST

// Define uma estrutura simples para a tabela de símbolos (lista encadeada)
typedef struct SymbolNode {
    char name[64];              // Nome da variável
    struct SymbolNode *next;    // Próximo nó na lista
} SymbolNode;

// Extern para acessar a cabeça da tabela de símbolos no main.c
extern SymbolNode *symbol_table_head;

// Adiciona um símbolo (nome de variável) à tabela de símbolos
void add_symbol(const char *name);

// Verifica se um símbolo (nome de variável) existe na tabela de símbolos
int symbol_exists(const char *name);

// Libera toda a memória alocada para a tabela de símbolos
void clear_symbols();

// Função principal de análise semântica que percorre a AST
void semantic_analysis(AST *node);

#endif // SEMANTIC_ANALYZER_H