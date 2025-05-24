# A3 – Teoria da Computação e Compiladores

## Objetivo

Este projeto tem como foco a construção de um compilador simples, seguindo as etapas clássicas do desenvolvimento de compiladores. A atividade visa aplicar os conceitos estudados na disciplina de Teoria da Computação e Compiladores, envolvendo desde a definição da linguagem até a geração de código.

## Requisitos do Projeto

Para que o compilador atenda aos objetivos propostos, ele deve contemplar as seguintes etapas:

### 1. Definir a Linguagem

* Especificar a gramática da linguagem que será compilada.
* Descrever a sintaxe e as regras semânticas utilizando notações formais como BNF (Backus-Naur Form).

### 2. Análise Léxica (Lexer)

* Converter o código-fonte em uma sequência de tokens.
* É recomendável o uso de ferramentas como Flex para facilitar essa etapa.

### 3. Análise Sintática (Parser)

* A partir dos tokens, construir uma Árvore Sintática Abstrata (AST).
* Ferramentas como Bison podem ser utilizadas para gerar os parsers automaticamente.

### 4. Análise Semântica

* Verificar a coerência semântica do código, como tipos de dados, escopo de variáveis, entre outros.

### 5. Geração de Código

* Traduzir a AST para um código intermediário ou diretamente para código de máquina.
* O uso de frameworks como LLVM é recomendado.

### 6. Otimizações (Opcional)

* Implementar otimizações para melhorar o desempenho do código gerado, como eliminação de código morto e propagação de constantes.
