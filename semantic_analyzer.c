#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic_analyzer.h" // Inclui o cabeçalho do próprio analisador semântico

// Variável global estática para a cabeça da tabela de símbolos
// O 'static' limita o escopo a este arquivo, mas a declaramos 'extern' em .h
// para que o main.c possa acessá-la (para declarar variáveis no código gerado).
SymbolNode *symbol_table_head = NULL;

// Adiciona um novo símbolo à tabela de símbolos se ele ainda não existir.
void add_symbol(const char *name) {
    if (!symbol_exists(name)) { // Adiciona apenas se o símbolo não existir
        SymbolNode *new_node = (SymbolNode*)malloc(sizeof(SymbolNode));
        if (new_node == NULL) {
            fprintf(stderr, "Erro de alocação de memória para tabela de símbolos.\n");
            exit(1); // Sai do programa em caso de erro grave de memória
        }
        strncpy(new_node->name, name, 63); // Copia o nome da variável
        new_node->name[63] = '\0';         // Garante terminação nula
        new_node->next = symbol_table_head; // Adiciona no início da lista (mais rápido)
        symbol_table_head = new_node;
    }
}

// Verifica se um símbolo (nome de variável) já existe na tabela de símbolos.
int symbol_exists(const char *name) {
    SymbolNode *current = symbol_table_head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return 1; // Símbolo encontrado
        }
        current = current->next;
    }
    return 0; // Símbolo não encontrado
}

// Libera toda a memória alocada para a tabela de símbolos.
void clear_symbols() {
    SymbolNode *current = symbol_table_head;
    SymbolNode *next;
    while (current != NULL) {
        next = current->next; // Guarda o próximo para não perder a referência
        free(current);        // Libera o nó atual
        current = next;       // Avança para o próximo nó
    }
    symbol_table_head = NULL; // Reseta a cabeça da lista
}

// Percorre a Árvore Sintática Abstrata (AST) para realizar a análise semântica.
// Nesta versão simplificada, o foco principal é verificar se variáveis são usadas
// antes de serem atribuídas/declaradas.
void semantic_analysis(AST *node) {
    if (node == NULL) {
        return; // Caso base da recursão
    }

    switch (node->type) {
        case NODE_INT:
        case NODE_FLOAT:
            // Literais inteiros e flutuantes não precisam de verificação semântica nesta fase.
            break;
        case NODE_VAR:
            // Se o nó é uma variável sendo USADA (e não o lado esquerdo de uma atribuição),
            // verificamos se ela já foi adicionada à tabela de símbolos.
            if (!symbol_exists(node->name)) {
                fprintf(stderr, "Erro Semântico: Variável '%s' não definida antes de uso.\n", node->name);
                exit(1); // Termina o programa com erro
            }
            break;
        case NODE_ASSIGN:
            // Se o nó é uma atribuição, a variável no lado esquerdo (node->name)
            // é considerada definida. Adicionamos ela à tabela de símbolos.
            add_symbol(node->name);
            // Em seguida, analisamos semanticamente a expressão no lado direito.
            semantic_analysis(node->left); // 'left' armazena a expressão da atribuição
            break;
        case NODE_ADD:
        case NODE_SUB:
        case NODE_MUL:
        case NODE_DIV:
            // Para operações binárias, analisamos recursivamente ambos os operandos.
            semantic_analysis(node->left);
            semantic_analysis(node->right);
            break;
        case NODE_PRINT:
            // Para o comando PRINT, analisamos semanticamente a expressão que será impressa.
            semantic_analysis(node->left); // 'left' armazena a expressão a ser impressa
            break;
        default:
            // Se um tipo de nó AST desconhecido for encontrado, reporta um erro.
            fprintf(stderr, "Erro Semântico: Tipo de nó AST desconhecido (%d).\n", node->type);
            exit(1); // Termina o programa com erro
    }
}