#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Necessário para strcat, strcpy (se for usar), etc.

// Inclui os cabeçalhos das etapas do compilador
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "semantic_analyzer.h" // Novo: para análise semântica
#include "code_generator.h"    // Novo: para geração de código

// Função auxiliar para liberar a memória da AST.
// É importante para evitar vazamentos de memória em programas maiores.
void free_ast(AST *node) {
    if (node == NULL) {
        return;
    }
    // Libera recursivamente os filhos
    free_ast(node->left);
    free_ast(node->right);
    // Libera o próprio nó
    free(node);
}

int main(void)
{
    char codigo[256]; // Buffer para armazenar o código de entrada do usuário
    printf("Dígite o programa (ex: altura = 1.80; peso = 70; imc = peso / (altura * altura); print(imc); ): \n");
    fgets(codigo, sizeof(codigo), stdin); // Lê a linha de código do usuário

    // --- 2. Análise Léxica (Lexer) ---
    // Inicializa o lexer com o código de entrada
    init_lexer(codigo);           
    current_token = next_token(); // Obtém o primeiro token

    // --- 3. Análise Sintática (Parser) ---
    AST *comandos[100];      // Array para armazenar os nós raiz de cada comando
    int num_comandos = 0;    // Contador de comandos
    parse_lista_comandos(comandos, &num_comandos); // Inicia o parsing

    printf("\n--- Análise de Fases Concluída ---\n");
    printf("Foram lidos %d comandos e montadas %d árvores AST.\n", num_comandos, num_comandos);
    
    // --- 4. Análise Semântica ---
    printf("\n--- Iniciando Análise Semântica ---\n");
    // Percorre cada AST gerada pelo parser para realizar a análise semântica.
    // Nesta etapa, a tabela de símbolos (em semantic_analyzer.c) será populada
    // com todas as variáveis atribuídas e verificará o uso de variáveis não definidas.
    for (int i = 0; i < num_comandos; i++) {
        semantic_analysis(comandos[i]); 
    }
    printf("Análise Semântica concluída sem erros detectados.\n");

    // --- 5. Geração de Código ---
    printf("\n--- Iniciando Geração de Código C ---\n");
    FILE *output_file = fopen("output.c", "w"); // Abre o arquivo "output.c" para escrita
    if (output_file == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir 'output.c' para escrita.\n");
        exit(1); // Sai do programa em caso de erro ao abrir o arquivo
    }

    // Escreve o cabeçalho padrão de um arquivo C
    fprintf(output_file, "#include <stdio.h>\n");
    fprintf(output_file, "#include <math.h>\n\n"); // Inclui math.h caso haja operações como pow, sqrt, etc.
                                                  // Embora para IMC, só multiplicação seja usada para altura*altura.
    fprintf(output_file, "int main() {\n");
    
    // Declarar todas as variáveis que foram coletadas na tabela de símbolos como 'float'.
    // A tabela de símbolos é uma lista encadeada (symbol_table_head definida em semantic_analyzer.c).
    SymbolNode *current_symbol = symbol_table_head; 
    if (current_symbol != NULL) { // Verifica se há variáveis para declarar
        fprintf(output_file, "    float "); // Inicia a declaração com 'float'
        while (current_symbol != NULL) {
            fprintf(output_file, "%s", current_symbol->name); // Escreve o nome da variável
            if (current_symbol->next != NULL) {
                fprintf(output_file, ", "); // Adiciona vírgula se não for a última variável
            }
            current_symbol = current_symbol->next; // Avança para o próximo símbolo
        }
        fprintf(output_file, ";\n\n"); // Fecha a declaração com ponto e vírgula e nova linha
    }

    // Gera o código C para cada comando da AST.
    for (int i = 0; i < num_comandos; i++) {
        fprintf(output_file, "    "); // Adiciona indentação para o corpo da função main
        generate_code(comandos[i], output_file); // Chama a função que gera o código para o comando
    }

    fprintf(output_file, "\n    return 0;\n"); // Finaliza a função main
    fprintf(output_file, "}\n");               // Fecha o arquivo C
    fclose(output_file);                       // Fecha o arquivo de saída
    printf("Código C gerado com sucesso em 'output.c'.\n");

    clear_symbols(); // Libera a memória da tabela de símbolos após a geração de código
    
    // Libera a memória de todas as Árvores Sintáticas Abstratas
    for (int i = 0; i < num_comandos; i++) {
        free_ast(comandos[i]); 
    }

    // --- Compilação e Execução do Código C Gerado (Opcional, para testar) ---
    printf("\n--- Tentando compilar e executar 'output.c' ---\n");
    // Chama o compilador GCC para compilar o arquivo C gerado.
    // '-o output_program' define o nome do executável.
    // '-lm' é necessário se você usar funções matemáticas como `sqrt`, `pow`, etc., embora não seja estritamente necessário para `altura * altura`.
    int compile_status = system("gcc output.c -o output_program -lm"); 
    
    if (compile_status == 0) { // Se a compilação foi bem-sucedida
        printf("Compilação de 'output.c' bem-sucedida! Executando o programa gerado:\n");
        system("./output_program"); // Executa o programa compilado
    } else {
        fprintf(stderr, "Erro na compilação do 'output.c'. Verifique o arquivo para detalhes.\n");
    }

    return 0; // Retorna 0 indicando sucesso do compilador
}