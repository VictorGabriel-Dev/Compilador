#include "lexer.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

//Ponteiro para string de entrada e posição inicial
static const char *src;
static int pos;
static char current_char;

//Avança um caractere na string e atualiza o current_char
static void advance(void){
    pos++;
    current_char = src[pos];
}

//Pula espaços, tabs e quebras de linha até achar algo importante
static void skip_whitespace(void) {
    while (current_char == ' ' || current_char == '\t' || current_char == '\n' || current_char == '\r'){
        advance();
    }
}

//Construir um TOKEN com o ripo indicado e cópia do texto
static Token make_token(TokenType type, const char *text){
    Token t;
    t.type = type;
    //Copia o lexema (trunca se for maior que 63 caracteres)
    strncpy(t.lexeme, text, 63);
    t.lexeme[63] = '\0';
    if (type == TOKEN_NUM){
        t.value = atoi(text); //converter string para inteiro, já guardando em .value
    } else{
        t.value = 0;
    }
    return t;
}

//Retorna o próximo token da entrada
Token next_token(void) {
    skip_whitespace();
    
    // 1 Fim da string
    if (current_char == '\0'){
        return make_token(TOKEN_EOF, "EOF");
    }

    // 2 Número, sequência de dígitos
    if (isdigit(current_char)){
        char buffer[64];
        int len = 0;
        //enquanto for dígito, adiciona ao buffer
        while (isdigit(current_char)){
            if (len < 63){
                buffer[len++] = current_char;
            }
            advance();
        }
        buffer[len] = '\0';
        return make_token(TOKEN_NUM, buffer);//pode estar escrito errado!!
    }

    // 3 Identificador ou palavra-reservada
    if (isalpha(current_char)){
        char buffer[64];
        int len = 0;
        while (isalnum(current_char)){
            if (len <63){
                buffer[len++] = current_char;
            }
            advance();
        }
        buffer[len] = '\0';
        if(strcmp(buffer, "print") == 0){
            return make_token(TOKEN_PRINT,buffer);
        }
        return make_token(TOKEN_IDEN, buffer);
    }

    // 4 Símpbolos únicos
    char c = current_char;
    advance();
    switch (c){
        case '+': return make_token(TOKEN_PLUS, "+");
        case '-': return make_token(TOKEN_MINUS, "-");
        case '*': return make_token(TOKEN_STAR, "*");
        case '/': return make_token(TOKEN_SLASH, "/");
        case '(': return make_token(TOKEN_LPAREN, "(");
        case ')': return make_token(TOKEN_RPAREN, ")");
        case ';': return make_token(TOKEN_SEMICOLON, ";");
        case '=': return make_token(TOKEN_ASSIGN, "=");
        default:
            {
                char str[2] = {c, '\0'};
                return make_token(TOKEN_INVALID,str);
            }
    }
}

//Chamar no inicio do programa, passando todo o texto-fonte
void init_lexer(const char *input){
    src = input;
    pos = 0;
    current_char = src[0];
}