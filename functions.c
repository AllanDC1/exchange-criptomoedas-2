#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <functions.h>

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

Resposta verificar_arquivo(FILE *ponteiro) {    
    if (ponteiro == NULL) {    
        printf("Erro ao acessar arquivo.\n");    
        return FALHA;
    }
    else
        return OK;
}

Resposta verificar_cpf(char *entrada_cpf) {
    if (strlen(entrada_cpf != TAM_CPF - 1)) {
        printf("CPF possui apenas 11 digitos, insira um CPF valido.\n");
        return FALHA;
    }
    
    for (int i = 0; i < TAM_CPF - 1; i++) {
        if (!isdigit(entrada_cpf[i])) {
            printf("CPF possui apenas numeros, insira um CPF valido.\n");
            return FALHA; 
        }
    }
    
    return OK;
}

// QUANDO FOR CHAMADA ESSA FUNCAO, FAZER O TRATAMENTO DE ERRO (if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {return; // voltar pro menu})
Resposta ler_usuarios(Usuario usuarios[], int *quantidade_lida) {
    FILE *fP = fopen("dados-usuarios", "rb");

    if (verificar_arquivo(fP) == FALHA) {
        printf("Erro ao ler dados dos usuarios. Cancelando operacao...\n");       
        return FALHA;
    }

    Usuario temp;
    *quantidade_lida = 0;

    while(fread(&temp, sizeof(Usuario), 1, fP)) {
        usuarios[*quantidade_lida] = temp; // pega o indice disponivel no array, e armazena o usuario lido nele
        (*quantidade_lida)++;
    }

    fclose(fP);
    return OK;
}

// TALVEZ TROCAR PARA RESPOSTA (TRATAR ERROS MELHOR)
void criar_usuario() {

    Usuario usuarios[10];
    int qnt_usuarios = 0;
    
    if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {
        return; // voltar pro menu
    }

    if (qnt_usuarios == MAX_USUARIOS) { // TALVEZ TRATAR ERROS MELHOR
        printf("Quantidade de usuarios criados atingiu o limite, nao sera possivel criar uma conta.\n");
        printf("Cancelando operacao...\n");
        return; // voltar pro menu
    }

    Usuario novo_usuario;
    char cpf_temp[13]; // 1 de buffer para conferir tamanho do cpf

    do {
        printf("Informe seu CPF (apenas numeros): ");
        fgets(cpf_temp, sizeof(cpf_temp), stdin);
        printf("\n");
        limpar_buffer();
    } while (verificar_cpf(cpf_temp) == FALHA);

    strcpy(novo_usuario.cpf, cpf_temp);

    // PEDIR SENHA, NOME
    // TERMINAR A CRIACAO DOS DADOS BASICO DO USUARIO    
    
    // criar/salvar o usuario no arquivo
}