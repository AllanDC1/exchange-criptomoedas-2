#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void criar_usuario(char nome_arquivo[]) {
    FILE *fP = fopen(nome_arquivo, "ab");

    if (fP == NULL) {
        printf("Erro ao tentar abrir arquivo.");
        return; //add valores para tratamento de erros
    }

    // testar quantidade de usuarios ja criados
    // receber os argumentos necessarios ou pedir
    // criar/salvar o usuario no arquivo

    fclose(fP);
}