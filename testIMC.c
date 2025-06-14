#include <stdio.h>

typedef struct {
    float peso;
    float altura;
    float imc;
} Pessoa;

void lerDados(Pessoa *p) {
    printf("Digite seu peso (kg): ");
    scanf("%f", &p->peso);
    printf("Digite sua altura (m): ");
    scanf("%f", &p->altura);
}

void calcularIMC(Pessoa *p) {
    if (p->altura > 0) {
        p->imc = p->peso / (p->altura * p->altura);
    } else {
        p->imc = 0;
    }
}

void mostrarCategoria(Pessoa *p) {
    printf("Seu IMC é: %.2f\n", p->imc);

    if (p->imc <= 0) {
        printf("Altura inválida para cálculo de IMC.\n");
        return;
    }

    if (p->imc <= 18.5) {
        printf("Abaixo do Peso\n");
    } else if (p->imc <= 24.9) {
        printf("Saudável\n");
    } else if (p->imc <= 29.9) {
        printf("Peso em excesso\n");
    } else if (p->imc <= 34.9) {
        printf("Obesidade Grau I\n");
    } else if (p->imc <= 39.9) {
        printf("Obesidade Grau II (severa)\n");
    } else {
        printf("Obesidade Grau III\n");
    }
}

int main() {
    Pessoa pessoa;

    lerDados(&pessoa);
    calcularIMC(&pessoa);
    mostrarCategoria(&pessoa);

    return 0;
}
