#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "ast.h" // Inclui a definição da AST
#include <stdio.h> // Para o tipo FILE*

// Função principal de geração de código.
// Recebe um nó da AST e um ponteiro para o arquivo de saída.
void generate_code(AST *node, FILE *output_file);

#endif // CODE_GENERATOR_H