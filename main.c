#include <stdio.h>
#include "functions.h"

int main(){
    Usuario user_atual;
    int operacao;

    do {
        exibir_menu();
        operacao = escolha_menu();

        switch (operacao) {
            case 1:
                ResultadoLogin login = login_usuario();
                if (login.resultado == OK) {
                    user_atual = login.usuario_atual;
                    //escolha_operacoes(user_atual);
                }
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