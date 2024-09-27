#include <stdio.h>
#include "functions.h"

int main(){
    int idx_user_atual, operacao;
    ResultadoLogin login;

    do {
        exibir_menu();
        operacao = escolha_operacao(3);

        switch (operacao) {
            case 1:
                login = login_usuario();
                if (login.resultado == OK) {
                    idx_user_atual = login.idx_usuario_atual;
                    printf("Login feito com sucesso!\n");                    
                    menu_operacoes(idx_user_atual);
                }
                delay(1000);
                break;
            case 2:
                if (criar_usuario() == OK) {
                    printf("\nUsuario criado com sucesso! Voltando ao menu...\n");
                }
                delay(1000);
                break;
            case 3:
                if (excluir_usuario() == OK) {
                    printf("\nUsuario excluido com sucesso. Voltando ao menu...\n");
                }
                delay(1000);
                break;
            case 0:
                printf("Obrigado pela sua presenca!\n");
                printf("Encerrando o programa...\n");
                delay(1000);
                break;
        }
    
    }while (operacao != 0);

    return 0;
}