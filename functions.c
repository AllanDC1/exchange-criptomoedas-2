#include "functions.h"

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void print_erro(const char *msg) {
    printf("\033[0;31m %s \033[0m\n", msg);
}

Resposta verificar_arquivo(FILE *ponteiro) {    
    if (ponteiro == NULL) {    
        print_erro("Erro ao acessar arquivo, possivelmente ele nao existe.");    
        return FALHA;
    }
    else
        return OK;
}

Resposta verificar_cpf(char *entrada_cpf) {
    int len_cpf = strlen(entrada_cpf);
    
    if (len_cpf != TAM_CPF) { // strlen() retorna a quantidade sem o \0, porém com o \n
        if (entrada_cpf[len_cpf -1] != '\n'){
            limpar_buffer();
        }
        print_erro("O CPF deve possuir 11 digitos, insira um CPF valido.");
        return FALHA;
    }
    
    for (int i = 0; i < 11; i++) {
        if (!isdigit(entrada_cpf[i])) {
            print_erro("CPF possui apenas numeros, insira um CPF valido.");
            return FALHA; 
        }
    }
    
    return OK;
}

Resposta verificar_senha(char* entrada_senha) {
    int len_senha = strlen(entrada_senha);    
    
    if (len_senha < 7 || len_senha > 16) {
        if (entrada_senha[len_senha -1] != '\n')
            limpar_buffer();
        print_erro("Senha invalida, insira novamente.");
        return FALHA;
    }
    
    return OK;
}

Resposta verificar_nome(char *entrada_nome) {
    int len_nome = strlen(entrada_nome);
    
    if (len_nome < 4 || len_nome > TAM_NOME) {
        if (entrada_nome[len_nome -1] != '\n')
            limpar_buffer();  
        print_erro("Tamanho do nome deve ser entre 3 a 20 letras, insira um nome valido.");
        return FALHA;
    }
    
    return OK;
}

int checar_usuario(char *entrada_cpf, char* entrada_senha, Usuario array_usuarios[], int quantidade_usuarios) {
    
    if (verificar_cpf(entrada_cpf) || verificar_senha(entrada_senha)) {
        return FALHA;
    }
    
    for (int i = 0; i < quantidade_usuarios; i++) {
        if (strcmp(entrada_cpf, array_usuarios[i].cpf) == 0) {
            if (strcmp(entrada_senha, array_usuarios[i].senha) == 0) {
                return i;
            }
        }
        print_erro("Credenciais incorretas. Insira novamente");
        return FALHA;
    }
}

void exibir_menu() {
    printf("\nBem-Vindo ao Exchange de CriptoMoedas!\n");
    printf("\n 1. Login\n");
    printf(" 2. Registrar-se\n");
    printf(" 3. Excluir conta\n");
    printf(" 0. Sair\n");
}

int escolha_menu() {
    int op;
    do {
        printf("\nEscolha a operacao: ");
        if (scanf("%d", &op) != 1 || (op < 0 || op > 3) ) {
            print_erro("Operacao invalida, insira novamente.");          
            limpar_buffer();
            op = -1; // força o loop
        }
    }while (op < 0 || op > 3);
    printf("\n");
    
    return op;
}

Resposta ler_usuarios(Usuario array_usuarios[], int *quantidade_lida) {
    FILE *fP = fopen("dados-usuarios", "rb");

    if (verificar_arquivo(fP) == FALHA) {
        // CRIAR O ARQUIVO PELA PRIMEIRA VEZ
        int op;
        do {
            printf("\nDigite 1 se deseja criar o arquivo. (Se for a primeira vez executando o programa)\n");
            printf("Digite 2 para relatar a falha e cancelar a execucao. (Possivel falha no arquivo)\n");        
            scanf("%d", &op);
            limpar_buffer();
        }while (op != 1 && op != 2);

        if (op == 1){
            FILE *arquivo = fopen("dados-usuarios", "wb");
            fclose(arquivo);
            printf("Arquivo criado.\n");
            printf("\n");
        }
        else if (op == 2) {
            return FALHA;
        }
    }

    // verificação se o arquivo está vazio
    fseek(fP, 0, SEEK_END);
    long tamanho = ftell(fP); 
    rewind(fP);

    if (tamanho != 0) {
        Usuario usuario_temp;
        *quantidade_lida = 0;

        while(fread(&usuario_temp, sizeof(Usuario), 1, fP) != 0) {
            array_usuarios[*quantidade_lida] = usuario_temp; // pega o indice disponivel no array, e armazena o usuario lido nele
            (*quantidade_lida)++;
        }
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
    fclose(fP);
    return OK;
}

void gerar_data(char* var_data) {    
    time_t tempo_data;
    time(&tempo_data); // gera tempo em segundos
    
    struct tm *local = localtime(&tempo_data); // converte para fuso/formato local

    strftime(var_data, 20, "%d/%m/%Y %H:%M:%S", local); // formatar a data e hora
}

ResultadoLogin login_usuario() {
    Usuario nulo;
    ResultadoLogin retorno = { .usuario_atual = nulo, .resultado = FALHA};
    Usuario usuarios[10];    
    int qnt_usuarios = 0, idx;
    char entrada_cpf[14], entrada_senha[18];

    if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {
        print_erro("Erro ao ler dados dos usuarios. Cancelando operacao...");
        return retorno; // voltar pro menu
    }

    printf("Faca seu login:\n");
    
    do{
        printf("\nCPF: ");
        printf("\nSenha: ");
        fgets(entrada_cpf, sizeof(entrada_cpf), stdin);
        fgets(entrada_senha, sizeof(entrada_senha), stdin);
        
        idx = checar_usuario(entrada_cpf, entrada_senha, usuarios, qnt_usuarios);
    }while(idx == FALHA);
  
    retorno.resultado = OK;
    retorno.usuario_atual = usuarios[idx];
    printf("Login feito com sucesso!\n");
    return retorno;
}

void criar_usuario() {

    // LER USUÁRIOS E VERIFICAR QUANTIDADE JÁ CRIADOS
    Usuario usuarios[10];
    int qnt_usuarios = 0;
    
    if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {
        print_erro("Erro ao ler dados dos usuarios. Cancelando operacao...");
        return; // voltar pro menu
    }
    
    if (qnt_usuarios >= MAX_USUARIOS) {
        print_erro("Quantidade de usuarios criados atingiu o limite. Cancelando operacao...");
        return; // voltar pro menu
    }

    // DEFININDO DADOS PADRÕES 
    Usuario novo_usuario = { .carteira = { .real = 0.0, .btc = 0.0, .eth = 0.0, .xrp = 0.0 }, .qnt_transacoes = 0};
    // CONFERIR SE PRECISARA DECLARAR UMA TRANSACAO PADRAO

    // CRIAÇÃO CPF
    char entrada_cpf[14]; // buffer para conferir tamanho

    printf("Crie sua conta:\n");

    do {
        printf("\nInforme seu CPF (apenas numeros): ");
        fgets(entrada_cpf, sizeof(entrada_cpf), stdin);               
    } while (verificar_cpf(entrada_cpf) == FALHA);
    
    strncpy(novo_usuario.cpf, entrada_cpf, 11); // adiciona o CPF no novo Usuario, garantindo apenas os 11 digitos
    novo_usuario.cpf[11] = '\0'; // garante o nulo no fim do array

    // CRIAÇÃO SENHA
    char entrada_senha[18]; // buffer para conferir tamanho

    do {
        printf("\nCrie uma senha (de 6 a 15 caracteres): ");
        fgets(entrada_senha, sizeof(entrada_senha), stdin);        
    } while (verificar_senha(entrada_senha) == FALHA);

    int len_senha = strlen(entrada_senha) -1; // faz a contagem sem o \n
    strncpy(novo_usuario.senha, entrada_senha, len_senha); // adiciona a senha no novo Usuario, garantindo o tamanho da senha inserida
    novo_usuario.senha[len_senha] = '\0'; // garante o nulo no fim do array

    // CRIAÇÃO NOME
    char entrada_nome[23]; // buffer para conferir tamanho

    do {
        printf("\nInforme seu primeiro nome: ");
        fgets(entrada_nome, sizeof(entrada_nome), stdin);         
    } while (verificar_nome(entrada_nome) == FALHA);

    int len_nome = strlen(entrada_nome) -1; // faz a contagem sem o \n
    strncpy(novo_usuario.nome, entrada_nome, len_nome); // adiciona a entrada do nome no novo Usuario, garantindo o tamanho do nome inserido
    novo_usuario.nome[len_nome] = '\0'; // garante o nulo no fim do array

    usuarios[qnt_usuarios] = novo_usuario;
    qnt_usuarios++;
     
    if (salvar_usuarios(usuarios, qnt_usuarios) == FALHA) {
        print_erro("Erro ao salvar os dados do usuario. Cancelando operacao...");
        return; // voltar pro menu
    }

    printf("\nUsuario criado com sucesso!\n");
    printf("Voltando ao menu...\n");
}