#include "lexer.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Ponteiro para string de entrada e posição inicial
static const char *src;
static int pos;
static char current_char;

// Avança um caractere na string e atualiza o current_char
static void advance(void)
{
    pos++;
    current_char = src[pos];
}

// Pula espaços, tabs e quebras de linha até achar algo importante
static void skip_whitespace(void)
{
    while (current_char == ' ' || current_char == '\t' || current_char == '\n' || current_char == '\r')
    {
        advance();
    }
}

// Construir um TOKEN com o ripo indicado e cópia do texto
static Token make_token(TokenType type, const char *text) {
    Token t;
    t.type = type;
    // Copia o lexema (trunca se for maior que 63 caracteres)
    strncpy(t.lexeme, text, 63);
    t.lexeme[63] = '\0';
    if (type == TOKEN_INT)
    {
        t.int_val = atoi(text); // converter string para inteiro, já guardando em .value
        t.float_val = (float)t.int_val;
    }
    else if (type == TOKEN_FLOAT){
        t.float_val = atof(text);
        t.int_val = (int)t.float_val;
    } else {
        t.int_val = 0;
        t.float_val =  0.0f;
    }
    return t;
}

// Retorna o próximo token da entrada
Token next_token(void)
{
    skip_whitespace();

    // 1 Fim da string
    if (current_char == '\0')
    {
        return make_token(TOKEN_EOF, "EOF");
    }

    // 2 Número, sequência de dígitos
    if (isdigit(current_char))
    {
        char buffer[64];
        int len = 0;
        int has_dot = 0;

        // enquanto for dígito ou ponto, adiciona ao buffer
        while (isdigit(current_char) || current_char == '.')
        {
            if (current_char == '.')
            {
                if (has_dot)
                    break; // dois pontos não são validos
                has_dot = 1;
                buffer[len++] = current_char;
                advance();
            }
            else
                // dígito
                if (len < 63)
                {
                    buffer[len++] = current_char;
                    advance();
                }
        }
        buffer[len] = '\0';
        // return make_token(TOKEN_NUM, buffer);
        if (has_dot)
        {
            // token_float
            Token t = make_token(TOKEN_FLOAT, buffer);
            t.float_val = atof(buffer);
            return t;
        }
        else
        {
            // TOKEN_INT
            Token t = make_token(TOKEN_INT, buffer);
            t.int_val = atoi(buffer);
            return t;
        }
    }

    // 3 Identificador ou palavra-reservada
    if (isalpha(current_char))
    {
        char buffer[64];
        int len = 0;
        while (isalnum(current_char))
        {
            if (len < 63)
            {
                buffer[len++] = current_char;
            }
            advance();
        }
        buffer[len] = '\0';
        if (strcmp(buffer, "print") == 0)
        {
            return make_token(TOKEN_PRINT, buffer);
        }
        return make_token(TOKEN_IDEN, buffer);
    }

    // 4 Símpbolos únicos
    char c = current_char;
    advance();
    switch (c)
    {
    case '+':
        return make_token(TOKEN_PLUS, "+");
    case '-':
        return make_token(TOKEN_MINUS, "-");
    case '*':
        return make_token(TOKEN_STAR, "*");
    case '/':
        return make_token(TOKEN_SLASH, "/");
    case '(':
        return make_token(TOKEN_LPAREN, "(");
    case ')':
        return make_token(TOKEN_RPAREN, ")");
    case ';':
        return make_token(TOKEN_SEMICOLON, ";");
    case '=':
        return make_token(TOKEN_ASSIGN, "=");
    default:
    {
        char str[2] = {c, '\0'};
        return make_token(TOKEN_INVALID, str);
    }
    }
}

// Chamar no inicio do programa, passando todo o texto-fonte
void init_lexer(const char *input)
{
    src = input;
    pos = 0;
    current_char = src[0];
}