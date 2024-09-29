#include "exchangecripto.h"

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
                    print_sucesso("Login feito com sucesso!");
                    delay(1000);                   
                    menu_operacoes(idx_user_atual);
                }
                delay(1000);
                break;
            case 2:
                if (registro_usuario() == OK) {
                    print_sucesso("\nUsuario criado com sucesso! Voltando ao menu...");
                }
                delay(1000);
                break;
            case 3:
                if (excluir_usuario() == OK) {
                    print_sucesso("\nUsuario excluido com sucesso. Voltando ao menu...");
                }
                delay(1000);
                break;
            case 0:
                print_sucesso("Obrigado pela sua presenca!");
                printf("Encerrando o programa...\n");
                delay(1500);
                break;
        }
    
    }while (operacao != 0);

    return 0;
}