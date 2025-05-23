#include "lexer.h"
#include "lexer.c"
#include <stdio.h>

int main(void) {
    const char *codigo = "x = 3 + 4 * 2; print(x);";
    init_lexer(codigo);

    Token t;
    do {
        t = next_token();
        printf("Token: tipo=%d, lexeme='%s'", t.type, t.lexeme);
        if (t.type == TOKEN_NUM) {
            printf(", value=%d", t.value);
        }
        printf("\n");
    } while (t.type != TOKEN_EOF && t.type != TOKEN_INVALID);

    return 0;
}
