#ifndef LEXER_H
#define LEXER_H

//Lista de TOKENS
typedef enum{
    TOKEN_INT, // número inteiro (ex.: "123")
    TOKEN_FLOAT, // número float (ex.: "1.80")
    TOKEN_IDEN, // identificador (ex.: "x", "foo")
    TOKEN_PRINT, // palavra‐reservada "print"
    TOKEN_PLUS, // '+'
    TOKEN_MINUS, // '-'
    TOKEN_STAR, // '*'
    TOKEN_SLASH, // '/'
    TOKEN_LPAREN, // '('
    TOKEN_RPAREN,  // ')'
    TOKEN_SEMICOLON, // ';'
    TOKEN_ASSIGN, // '='
    TOKEN_EOF, // fim do arquivo/input
    TOKEN_INVALID // qualquer caractere que não pertença a nada acima
} TokenType;

//Estrutura do TOKEN
typedef struct {
    TokenType type;
    char lexeme[64]; //copiar o texto
    int int_val;
    float float_val;
} Token;

//Função para iniciar o LEXER com uma estrutura string
void init_lexer(const char *input);

//Retorna o TOKEN seguinte
Token next_token(void);

#endif 