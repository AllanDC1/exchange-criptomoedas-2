#include <stdio.h>
#include "functions.h"

int main(){
    Usuario user_atual;
    int operacao;
    ResultadoLogin login;

    do {
        exibir_menu();
        operacao = escolha_operacao(3);

        switch (operacao) {
            case 1:
                login = login_usuario();
                if (login.resultado == OK) {
                    user_atual = login.usuario_atual;
                    printf("Login feito com sucesso!\n");                    
                    menu_operacoes(user_atual);
                }
                sleep(1);
                break;
            case 2:
                if (criar_usuario() == OK) {
                    printf("\nUsuario criado com sucesso! Voltando ao menu...\n");
                }
                sleep(1);
                break;
            case 3:
                if (excluir_usuario() == OK) {
                    printf("\nUsuario excluido com sucesso. Voltando ao menu...\n");
                }
                sleep(1);
                break;
            case 0:
                printf("Obrigado pela sua presenca!\n");
                printf("Encerrando o programa...\n");
                sleep(1);
                break;
        }
    
    }while (operacao != 0);

    return 0;
}