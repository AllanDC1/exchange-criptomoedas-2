#include "functions.h"

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void verificar_buffer(char *entrada) {
    if (entrada[strlen(entrada) -1] != '\n') {
        limpar_buffer();
    }
    entrada[strcspn(entrada, "\n")] = 0;    
}

void print_erro(const char *msg) {
    printf("\033[0;31m %s \033[0m\n", msg);
}

void voltar_menu() {
    printf("\n\033[0;32m Pressione ENTER para voltar ao menu. \033[0m");
    getchar();
    printf("\nVoltando ao menu de operacoes...\n");
}

FILE* abrir_arquivo(char* nome_arquivo, char* modo) {    
    FILE *ponteiro = fopen(nome_arquivo, modo);  // Tentar abrir no modo de leitura

    if (ponteiro == NULL) {
        ponteiro = fopen(nome_arquivo, "wb+"); // cria pela primeira vez
        if (ponteiro != NULL) {
            fclose(ponteiro); // se não houver falha, fecha o arquivo aberto
            ponteiro = fopen(nome_arquivo, modo); // reabre o arquivo para retorna-lo
        }else {
            print_erro("Erro ao criar arquivo.");
            return NULL;
        }        
    }

    return ponteiro;
}

Resposta verificar_tamanho_arquivo(FILE *ponteiro) {
    fseek(ponteiro, 0, SEEK_END);
    long tamanho = ftell(ponteiro); 
    rewind(ponteiro);

    if (tamanho == 0) {
        return FALHA;
    }else {
        return OK;
    }
}

Resposta verificar_cpf(char *entrada_cpf) {
    int len_cpf = strlen(entrada_cpf);
    verificar_buffer(entrada_cpf);
    
    if (len_cpf != TAM_CPF) { // strlen() retorna a quantidade sem o \0, porém com o \n        
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
    verificar_buffer(entrada_senha);
    
    if (len_senha < 7 || len_senha > 16) {        
        print_erro("Senha invalida, insira novamente.");
        return FALHA;
    }
    
    return OK;
}

Resposta verificar_nome(char *entrada_nome) {
    verificar_buffer(entrada_nome);
    int len_nome = strlen(entrada_nome);
    
    if (len_nome < 4 || len_nome > TAM_NOME) {         
        print_erro("Tamanho do nome deve ser entre 3 a 20 letras, insira um nome valido.");
        return FALHA;
    }
    
    return OK;
}

int checar_usuario(char *entrada_cpf, char* entrada_senha, Usuario array_usuarios[], int quantidade_usuarios) {    

    for (int i = 0; i < quantidade_usuarios; i++) {
        if (strcmp(entrada_cpf, array_usuarios[i].cpf) == 0) {
            if (strcmp(entrada_senha, array_usuarios[i].senha) == 0) {
                return i;
            }
        }
    }
    print_erro("Credenciais incorretas. Insira novamente");
    return FALHA;
}

void exibir_menu() {
    printf("\nBem-Vindo ao Exchange de CriptoMoedas!\n");
    printf("\n1. Login\n");
    printf("2. Registrar-se\n");
    printf("3. Excluir conta\n");
    printf("0. Sair\n");
}

void exibir_operacoes() {
    printf("\nOperacoes disponiveis:\n");
    printf("\n1. Consultar saldo\n");
    printf("2. Consultar extrato\n");
    printf("3. Depositar\n");
    printf("4. Sacar\n");
    printf("5. Comprar criptomoedas\n");
    printf("6. Vender criptomoedas\n");
    printf("7. Atualizar cotacao das criptomoedas\n");
    printf("0. Sair\n");
}

int escolha_operacao(int max) {
    int op;
    do {
        printf("\nEscolha a operacao: ");
        if (scanf("%d", &op) != 1 || (op < 0 || op > max) ) {
            print_erro("Operacao invalida, insira novamente.");
            op = -1; // força o loop
        }
        limpar_buffer();
    }while (op < 0 || op > max);
    printf("\n");
    
    return op;
}

Resposta ler_usuarios(Usuario array_usuarios[], int *quantidade_lida) {
    Usuario usuario_temp;
    *quantidade_lida = 0;
    FILE *fP = abrir_arquivo("dados-usuarios", "rb");

    if (fP == NULL) {
        print_erro("Erro ao acessar arquivo.");
        return FALHA;
    }

    if (verificar_tamanho_arquivo(fP) == OK) {
            while(fread(&usuario_temp, sizeof(Usuario), 1, fP) != 0) {
                array_usuarios[*quantidade_lida] = usuario_temp; // pega o indice disponivel no array, e armazena o usuario lido nele
                (*quantidade_lida)++;
        }
    }

    fclose(fP);
    return OK;
}

Resposta salvar_usuarios(Usuario array_usuarios[], int quantidade_usuarios) {
    FILE *fP = abrir_arquivo("dados-usuarios", "wb");

    if (fP == NULL) {
        print_erro("Erro ao acessar arquivo.");             
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
    Usuario usuarios[10], nulo;
    ResultadoLogin retorno = { .usuario_atual = nulo, .resultado = FALHA};
    int qnt_usuarios = 0, idx;
    char entrada_cpf[14], entrada_senha[18];

    if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {
        print_erro("Erro ao acessar dados dos usuarios. Cancelando operacao...");
        return retorno;
    }

    if (qnt_usuarios <= 0) {
        print_erro("A plataforma ainda nao possui nenhum usuario.");
        return retorno;
    }

    printf("Faca seu login:\n");
    
    do{
        printf("\nCPF: ");
        fgets(entrada_cpf, sizeof(entrada_cpf), stdin);
        verificar_buffer(entrada_cpf);
        printf("Senha: ");
        fgets(entrada_senha, sizeof(entrada_senha), stdin);
        verificar_buffer(entrada_senha);
        printf("\n");
        
        idx = checar_usuario(entrada_cpf, entrada_senha, usuarios, qnt_usuarios);
    }while(idx == FALHA);
  
    retorno.resultado = OK;
    retorno.usuario_atual = usuarios[idx];
    
    return retorno;
}

Resposta criar_usuario() {
    Usuario usuarios[10];
    int qnt_usuarios = 0;
    char entrada_cpf[14], entrada_senha[18], entrada_nome[23]; // buffer para conferir tamanho
    
    if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {
        print_erro("Erro ao acessar dados dos usuarios. Cancelando operacao...");
        return FALHA;
    }
    
    if (qnt_usuarios >= MAX_USUARIOS) {
        print_erro("Quantidade de usuarios criados atingiu o limite. Cancelando operacao...");
        return FALHA; // voltar pro menu
    }

    Usuario novo_usuario = { .carteira = { .real = 0.0, .btc = 0.0, .eth = 0.0, .xrp = 0.0 }, .qnt_transacoes = 0}; // cria novo usuario e define dados padroes
    // CONFERIR SE PRECISARA DECLARAR UMA TRANSACAO PADRAO

    printf("Crie sua conta:\n");

    // CRIAÇÃO CPF
    do {
        printf("\nInforme seu CPF (apenas numeros): ");
        fgets(entrada_cpf, sizeof(entrada_cpf), stdin);               
    } while (verificar_cpf(entrada_cpf) == FALHA);
    
    strncpy(novo_usuario.cpf, entrada_cpf, 11); // adiciona o CPF no novo Usuario, garantindo apenas os 11 digitos
    novo_usuario.cpf[11] = '\0'; // garante o nulo no fim do array

    // CRIAÇÃO SENHA
    do {
        printf("\nCrie uma senha (de 6 a 15 caracteres): ");
        fgets(entrada_senha, sizeof(entrada_senha), stdin);        
    } while (verificar_senha(entrada_senha) == FALHA);

    int len_senha = strlen(entrada_senha);
    strncpy(novo_usuario.senha, entrada_senha, len_senha); // adiciona a senha no novo Usuario, garantindo o tamanho da senha inserida
    novo_usuario.senha[len_senha] = '\0'; // garante o nulo no fim do array

    // CRIAÇÃO NOME
    do {
        printf("\nInforme seu primeiro nome: ");
        fgets(entrada_nome, sizeof(entrada_nome), stdin);         
    } while (verificar_nome(entrada_nome) == FALHA);

    int len_nome = strlen(entrada_nome);
    strncpy(novo_usuario.nome, entrada_nome, len_nome); // adiciona a entrada do nome no novo Usuario, garantindo o tamanho do nome inserido
    novo_usuario.nome[len_nome] = '\0'; // garante o nulo no fim do array

    usuarios[qnt_usuarios] = novo_usuario;
    qnt_usuarios++;
     
    if (salvar_usuarios(usuarios, qnt_usuarios) == FALHA) {
        print_erro("Erro ao salvar os dados do usuario. Cancelando operacao...");
        return FALHA; // voltar pro menu
    }

    return OK;
}

Resposta excluir_usuario() {
    Usuario usuarios[10];
    int confirmacao, qnt_usuarios = 0, excluir_idx = FALHA;
    char entrada_cpf[14];
    
    if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {
        print_erro("Erro ao acessar dados dos usuarios. Cancelando operacao...");
        return FALHA;
    }

    if (qnt_usuarios <= 0) {
        print_erro("A plataforma ainda nao possui nenhum usuario.");
        return FALHA;
    }

    do{
        printf("Insira o CPF da conta que sera excluida: ");
        fgets(entrada_cpf, sizeof(entrada_cpf), stdin);
        verificar_buffer(entrada_cpf);        

        for (int i = 0; i < qnt_usuarios; i++) {
            if (strcmp(entrada_cpf, usuarios[i].cpf) == 0) {
                excluir_idx = i;
                break;
            }
        }

        if (excluir_idx == FALHA) {
            print_erro("Usuario nao encontrado. Insira novamente.");
            printf("\n");
        }        
    }while (excluir_idx == FALHA);

    do{
        printf("\nDeseja realmente excluir a conta de %s?\n", usuarios[excluir_idx].nome);
        printf("1(Sim) ou 2(Nao): ");
        if (scanf("%d", &confirmacao) != 1 || confirmacao != 1 && confirmacao != 2) {
            print_erro("Escolha invalida.");
            confirmacao = FALHA;
        }
        limpar_buffer();
    }while (confirmacao == FALHA);

    if (confirmacao == 1) {
        for (int i = excluir_idx; i < qnt_usuarios - 1; i++) {
            usuarios[i] = usuarios[i + 1];
        }
        qnt_usuarios--;

        if (salvar_usuarios(usuarios, qnt_usuarios) == FALHA) {
            print_erro("Erro ao salvar os dados do usuario. Cancelando operacao...");
            return FALHA; // voltar pro menu
        }
        return OK;
    }else {
        print_erro("Exclusao de conta cancelada. Voltando ao menu...");
        return FALHA;
    }
}

void menu_operacoes(Usuario usuario_logado) {
    int operacao;

    printf("Bem vindo %s!\n", usuario_logado.nome);

    do {
        sleep(1);
        exibir_operacoes();
        operacao = escolha_operacao(7);

        switch (operacao) {
        case 1:
            consultar_saldo(usuario_logado);
            sleep(1);
            break;
        case 2:
            // consultar_extrato();
            break;
        case 3:
            // depositar();
            break;
        case 4:
            // sacar();
            break;
        case 5:
            // comprar_criptos();
            break;
        case 6:
            // vender_criptos();
            break;
        case 7:
            // atualizar_cotacao();
            break;
        case 0:
            printf("Voltando ao menu inicial...\n");
            break;
        }

    }while (operacao != 0);
}

void consultar_saldo(Usuario usuario_atual) {
    printf("Dados da sua conta:\n");

    printf("\nNome: %s\n", usuario_atual.nome);
    printf("CPF: %s\n", usuario_atual.cpf);
    
    printf("\nSaldo:\n");

    printf("\nReais: R$ %.2f\n", usuario_atual.carteira.real);
    printf("Bitcoin: %.4f BTC\n", usuario_atual.carteira.btc);
    printf("Ethereum: %.4f ETH\n", usuario_atual.carteira.eth);
    printf("Ripple: %.4f XRP\n", usuario_atual.carteira.xrp);

    voltar_menu();
}