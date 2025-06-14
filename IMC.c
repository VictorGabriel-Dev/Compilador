#include <stdio.h>


int main () {

float massa, altura, imc;


printf("Digite sua massa em kg: ");
scanf("%f", &massa);
printf("Digite sua altura em metros: ");
scanf("%f", &altura);

imc = massa / (altura * altura);

if(imc < 18.5) {
    printf("Abaixo do peso\n");
} else if(imc >= 18.5 && imc < 24.9) {
    printf("Peso normal\n");
} else if(imc >= 25 && imc < 29.9) {
    printf("Sobrepeso\n");
} else {
    printf("Obesidade\n");
}
printf("Seu IMC é: %.2f", imc);

    return 0;
