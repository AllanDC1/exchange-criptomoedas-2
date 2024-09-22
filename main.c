#include <stdio.h>
#include <windows.h>
#include "functions.h"

int main(){
    Usuario user_atual;
    int operacao;
    ResultadoLogin login;

    do {
        exibir_menu();
        operacao = escolha_menu();

        switch (operacao) {
            case 1:
                login = login_usuario();
                if (login.resultado == OK) {
                    user_atual = login.usuario_atual;
                    printf("Login feito com sucesso!\n");
                    printf("Bem vindo %s!\n", user_atual.nome);
                    Sleep(1500);
                    //escolha_operacoes(user_atual);
                }
                break;
            case 2:
                criar_usuario();
                Sleep(1500);
                break;
            case 3:
                if (excluir_usuario() == OK) {
                    printf("\nUsuario excluido com sucesso. Voltando ao menu...\n");
                }
                Sleep(1500);
                break;
            case 0:
                printf("Obrigado pela sua presenca!\n");
                printf("Encerrando o programa...\n");
                break;
        }
    
    }while (operacao != 0);

    return 0;
}