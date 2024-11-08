#include "exchangecripto.h"

int main() {
    int operacao;
    ResultadoLogin login;

    do {
        menu_adm();
        operacao = escolha_operacao(3);

        switch (operacao) {
            case 1:
                if (login_adm()== OK) {                    
                    print_sucesso("Login realizado com sucesso.");
                    delay(1000);                   
                    operacoes_adm();
                }
                delay(1000);
                break;
            case 2:
                if (registro_admin() == OK) {
                    print_sucesso("\nAdmin registrado! Voltando ao menu...");
                }
                delay(1000);
                break;
            case 3:
                if (excluir_adm() == OK) {
                    print_sucesso("\nAdmin excluido. Voltando ao menu...");
                }
                delay(1000);
                break;
            case 0:
                printf("Encerrando o programa...\n");
                delay(1500);
                break;
        }
    
    }while (operacao != 0);

    return 0;
}