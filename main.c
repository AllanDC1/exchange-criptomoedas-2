#include <stdio.h>
#include "functions.h"

int main(){
    int op;

    printf("\nBem-Vindo ao Exchange de CriptoMoedas!\n");
    printf("\n 1. Login\n");
    printf(" 2. Registrar-se\n");
    printf(" 3. Excluir conta\n");

    do {
        printf("\nEscolha a operação:");
        scanf("%d", &op);
        printf("\n");

        if (op != 1 && op != 2 && op != 3) {
            printf("Operação inválida, insira novamente.\n");
        }
    }while (op != 1 && op != 2 && op != 3);

    switch op {
        case 1:
            // login();
            break;
        case 2:
            criar_usuario();
            break;
        case 3:
            // excluir_usuario();
            break;
    }

    return 0;
}