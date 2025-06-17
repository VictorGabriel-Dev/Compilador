#include <stdio.h>

int main() {
    float peso, altura, imc;
    int UAltura;

    printf("Digite seu peso (kg): ");
    scanf("%f", &peso);

    printf("\nSua altura está em:\n");
    printf("1 - Metros \n");
    printf("2 - Centímetros \n");
    printf("Escolha: ");
    scanf("%d", &UAltura);

    printf("Digite sua altura: ");
    scanf("%f", &altura);

    if (UAltura == 2) {
        altura = altura / 100;
    }

    imc = peso / (altura * altura);

    printf("\nSeu IMC é: %.2f\n", imc);
    printf("Classificação: ");

    if (imc <= 18.5) {
        printf("Abaixo do Peso\n");
    } else if (imc > 18.5 && imc <= 24.9) {
        printf("Saudável\n");
    } else if (imc >= 25.0 && imc <= 29.9) {
        printf("Peso em Excesso\n");
    } else if (imc >= 30.0 && imc <= 34.9) {
        printf("Obesidade Grau I\n");
    } else if (imc >= 35.0 && imc <= 39.9) {
        printf("Obesidade Grau II (Severa)\n");
    } else {
        printf("Obesidade Grau III (Mórbida)\n");
    }

    return 0;
}
