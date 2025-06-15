#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_generator.h" // Inclui o cabeçalho do próprio gerador de código
#include "ast.h"            // Inclui a definição da AST para NodeType

// Função auxiliar para gerar código para expressões.
// Esta função é chamada recursivamente para os sub-nós de uma expressão.
static void generate_expr_code(AST *node, FILE *output_file) {
    if (node == NULL) {
        return;
    }

    switch (node->type) {
        case NODE_INT:
            // Emite o valor inteiro como um literal C.
            fprintf(output_file, "%d", node->int_val);
            break;
        case NODE_FLOAT:
            // Emite o valor float como um literal C.
            // O "F" no final garante que seja tratado como float em C.
            // Usamos "%.2f" para consistência na saída, embora o valor possa ter mais precisão.
            fprintf(output_file, "%.2fF", node->float_val); 
            break;
        case NODE_VAR:
            // Emite o nome da variável.
            fprintf(output_file, "%s", node->name);
            break;
        case NODE_ADD:
            // Emite código para adição: (left + right)
            fprintf(output_file, "(");
            generate_expr_code(node->left, output_file);
            fprintf(output_file, " + ");
            generate_expr_code(node->right, output_file);
            fprintf(output_file, ")");
            break;
        case NODE_SUB:
            // Emite código para subtração: (left - right)
            fprintf(output_file, "(");
            generate_expr_code(node->left, output_file);
            fprintf(output_file, " - ");
            generate_expr_code(node->right, output_file);
            fprintf(output_file, ")");
            break;
        case NODE_MUL:
            // Emite código para multiplicação: (left * right)
            fprintf(output_file, "(");
            generate_expr_code(node->left, output_file);
            fprintf(output_file, " * ");
            generate_expr_code(node->right, output_file);
            fprintf(output_file, ")");
            break;
        case NODE_DIV:
            // Emite código para divisão: (left / right)
            fprintf(output_file, "(");
            generate_expr_code(node->left, output_file);
            fprintf(output_file, " / ");
            generate_expr_code(node->right, output_file);
            fprintf(output_file, ")");
            break;
        default:
            // Reporta um erro se um tipo de nó de expressão inesperado for encontrado.
            fprintf(stderr, "Erro de Geração de Código: Tipo de nó AST inesperado em expressão (%d).\n", node->type);
            exit(1);
    }
}

// Função principal para gerar código C a partir de um único comando da AST.
// Assume que a declaração das variáveis será feita no main.c, antes de chamar esta função
// para cada comando.
void generate_code(AST *node, FILE *output_file) {
    if (node == NULL) {
        return;
    }

    switch (node->type) {
        case NODE_ASSIGN:
            // Emite código para atribuição: nome_variavel = expressao;
            fprintf(output_file, "%s = ", node->name);
            generate_expr_code(node->left, output_file); // 'left' contém a expressão a ser atribuída
            fprintf(output_file, ";\n");
            break;
        case NODE_PRINT:
            // Emite código para o comando print, usando printf em C.
            // Sempre imprime como float (%.2f) para simplificar a tipagem no C gerado.
            fprintf(output_file, "printf(\"%%.2f\\n\", (float)("); 
            generate_expr_code(node->left, output_file); // 'left' contém a expressão a ser impressa
            fprintf(output_file, "));\n");
            break;
        default:
            // Se for um nó que não representa um comando (ex: um operador isolado sem atribuição/print),
            // ele é ignorado para fins de geração de código C neste compilador simples.
            // Isto pode ocorrer se o parser permitir expressões isoladas que não são atribuições ou prints.
            fprintf(stderr, "Aviso: Comando ignorado na geração de código (tipo %d). Apenas atribuições e prints são gerados.\n", node->type);
            break;
    }
}