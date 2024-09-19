#include <stdio.h>
#include "functions.h"

int main(){
    int operacao;

    do {
        exibir_menu();
        operacao = escolha_menu();

        switch (operacao) {
            case 1:
                // login();
                break;
            case 2:
                criar_usuario();
                break;
            case 3:
                // excluir_usuario();
                break;
            case 0:
                printf("Obrigado pela sua presenca!\n");
                printf("Encerrando o programa...\n");
                break;
        }
    
    }while (operacao != 0);

    return 0;
}