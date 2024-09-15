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
    if (strlen(entrada_cpf != 11)) { // strlen() retorna a quantidade sem o \0
        printf("O CPF deve possuir apenas 11 digitos, insira um CPF valido.\n");
        return FALHA;
    }
    
    for (int i = 0; i < 11; i++) {
        if (!isdigit(entrada_cpf[i])) {
            printf("CPF possui apenas numeros, insira um CPF valido.\n");
            return FALHA; 
        }
    }
    
    return OK;
}

Resposta verificar_senha(char *len_senha) {
    
    if (len_senha < 6 || len_senha > 15) {
        printf("A senha deve possuir no minimo 6 e no maximo 15 caracteres, insira uma senha valida.\n");
        return FALHA;
    }
    
    return OK;
}

Resposta verificar_nome(char *len_nome) {
    
    if (len_nome < 3 || len_nome > 20) {
        printf("Tamanho do nome deve ser entre 3 a 20 letras, insira um nome valido.\n");
        return FALHA;
    }
    
    return OK;
}

// QUANDO FOR CHAMADA ESSA FUNCAO, FAZER O TRATAMENTO DE ERRO (if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {return; // voltar pro menu})
Resposta ler_usuarios(Usuario array_usuarios[], int *quantidade_lida) {
    FILE *fP = fopen("dados-usuarios", "rb");

    if (verificar_arquivo(fP) == FALHA) {               
        return FALHA;
    }

    Usuario temp;
    *quantidade_lida = 0;

    while(fread(&temp, sizeof(Usuario), 1, fP) != 0) {
        array_usuarios[*quantidade_lida] = temp; // pega o indice disponivel no array, e armazena o usuario lido nele
        (*quantidade_lida)++;
    }

    fclose(fP);
    return OK;
}

Resposta salvar_usuarios(Usuario array_usuarios[], int quantidade_usuarios) {
    FILE *fP = fopen("dados-usuarios", "wb");

    if (verificar_arquivo(fP) == FALHA) {               
        return FALHA;
    }

    fwrite(array_usuarios, sizeof(Usuario), quantidade_usuarios, fP);
    return OK;
}

// TALVEZ TROCAR PARA RESPOSTA (TRATAR ERROS MELHOR)
void criar_usuario() {

    Usuario usuarios[10];
    int qnt_usuarios = 0;
    
    if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {
        printf("Erro ao ler dados dos usuarios. Cancelando operacao...\n");
        return; // voltar pro menu
    }

    if (qnt_usuarios == MAX_USUARIOS) { // TALVEZ TRATAR ERROS MELHOR
        printf("Quantidade de usuarios criados atingiu o limite, nao sera possivel criar uma conta.\n");
        printf("Cancelando operacao...\n");
        return; // voltar pro menu
    }

    Usuario novo_usuario;

    // CRIAÇÃO CPF
    char entrada_cpf[13]; // buffer para conferir tamanho

    printf("Crie sua conta.\n");
    printf("\n");

    do {
        printf("Informe seu CPF (apenas numeros): ");
        fgets(entrada_cpf, sizeof(entrada_cpf), stdin);
        limpar_buffer();
        printf("\n");
    } while (verificar_cpf(entrada_cpf) == FALHA);

    strncpy(novo_usuario.cpf, entrada_cpf, 11); // adiciona a entrada do CPF no novo Usuario, garantindo apenas os 11 digitos
    novo_usuario.cpf[11] = '\0'; // garante o nulo no fim do array

    // CRIAÇÃO SENHA
    char entrada_senha[17]; // buffer para conferir tamanho
    int len_senha;

    do {
        printf("Insira a senha (de 6 a 15 caracteres): ");
        fgets(entrada_senha, sizeof(entrada_senha), stdin);
        len_senha = strlen(entrada_senha);
        limpar_buffer();
        printf("\n");
    } while (verificar_senha(len_senha) == FALHA);

    strncpy(novo_usuario.senha, entrada_senha, len_senha); // adiciona a entrada da senha no novo Usuario, garantindo o tamanho da senha inserida
    novo_usuario.senha[len_senha] = '\0'; // garante o nulo no fim do array

    // CRIAÇÃO NOME
    char entrada_nome[21];
    int len_nome;

    do {
        printf("Informe seu primeiro nome: ");
        fgets(entrada_nome, sizeof(entrada_nome), stdin);
        len_nome = strlen(entrada_nome);
        limpar_buffer();
        printf("\n");
    } while (verificar_nome(len_nome) == FALHA);

    strncpy(novo_usuario.nome, entrada_nome, len_nome); // adiciona a entrada do nome no novo Usuario, garantindo o tamanho do nome inserido
    novo_usuario.nome[len_nome] = '\0'; // garante o nulo no fim do array

    // DEFININDO DADOS PADRÕES    
    novo_usuario.carteira.real = 0.0;
    novo_usuario.carteira.btc = 0.0;
    novo_usuario.carteira.eth = 0.0;
    novo_usuario.carteira.xrp = 0.0;
    // CONFERIR SE PRECISARA DECLARAR UMA TRANSACAO PADRAO
    novo_usuario.qnt_transacoes = 0;
     
    if (salvar_usuarios(usuarios, qnt_usuarios) == FALHA) {
        printf("Erro ao salvar os dados do usuario. Cancelando operacao...");
        return; // voltar pro menu
    }

    printf("Usuario criado com sucesso!\n");
}