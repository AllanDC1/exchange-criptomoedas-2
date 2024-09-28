#include "functions.h"

void delay(int tempo_ms) {
    #ifdef _WIN32
    Sleep(tempo_ms);
    #else
    usleep(tempo_ms * 1000); // mili * 1000 -> micro
    #endif
}

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

int achar_usuario(Usuario array_usuarios[], int quantidade_usuarios, Usuario *usuario_logado) {
    for (int i = 0; i < quantidade_usuarios; i++) {
        if (strcmp(usuario_logado->cpf, array_usuarios[i].cpf) == 0) {           
            return i;            
        }    
    }
    return FALHA;
}

Resposta salvar_transacao(Usuario *usuario_logado, char* tipo, char* moeda, float valor, float taxa) {
    Usuario usuarios[10];
    int qnt_usuarios = 0, idx_usuario;

    Transacao nova_linha;
    char data_atual[TAM_DATA];
    strcpy(data_atual, gerar_data());

    
    if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {
        print_erro("Erro ao acessar dados dos usuarios. Cancelando operacao...");
        return FALHA;
    }

    strcpy(nova_linha.tipo, tipo);
    strcpy(nova_linha.data, data_atual);
    strcpy(nova_linha.sigla_moeda, moeda);
    nova_linha.valor = valor;
    nova_linha.taxa = taxa;

    if (usuario_logado->qnt_transacoes >= MAX_TRANSACOES) {
        for (int i = 0; i < MAX_TRANSACOES - 1; i++) { // excluir a linha mais antiga, e mover todas as linhas 1 pra frente
            usuario_logado->extrato[i] = usuario_logado->extrato[i + 1];
        }
        usuario_logado->extrato[MAX_TRANSACOES - 1] = nova_linha;
    }else {
        usuario_logado->extrato[usuario_logado->qnt_transacoes] = nova_linha;
        usuario_logado->qnt_transacoes++;        
    }

    idx_usuario = achar_usuario(usuarios, qnt_usuarios, usuario_logado);
    if (idx_usuario == FALHA) {
        print_erro("Erro ao encontrar o usuario.");
        return FALHA;
    }

    usuarios[idx_usuario] = *usuario_logado;

    if (salvar_usuarios(usuarios, qnt_usuarios) == FALHA) {
        print_erro("Erro ao salvar os dados do usuario. Cancelando operacao...");
        return FALHA;
    }

    return OK;
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

char* gerar_data() {  
    static char data_string[TAM_DATA];
    time_t tempo_data;
    time(&tempo_data); // gera tempo em segundos
    
    struct tm *data_hora = gmtime(&tempo_data);

    data_hora->tm_hour -= 3;

    if (data_hora->tm_hour < 0) {
        data_hora->tm_hour += 24;
        data_hora->tm_mday -= 1; // Decrementa o dia
    }

    strftime(data_string, TAM_DATA, "%d/%m/%Y %H:%M:%S", data_hora); // formatar a data e hora

    return data_string;
}

ResultadoLogin login_usuario() {
    Usuario usuarios[10];
    ResultadoLogin retorno = { .idx_usuario_atual = FALHA, .resultado = FALHA};
    int qnt_usuarios = 0;
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
        
        retorno.idx_usuario_atual = checar_usuario(entrada_cpf, entrada_senha, usuarios, qnt_usuarios);
    }while(retorno.idx_usuario_atual == FALHA);
  
    retorno.resultado = OK;    
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

void menu_operacoes(int idx_logado) {
    Usuario usuarios[10], *usuario_logado;
    int qnt_usuarios = 0, operacao;    

    if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {
        print_erro("Erro ao acessar dados dos usuarios. Cancelando operacao...");
        return; // voltar ao menu
    }

    usuario_logado = &usuarios[idx_logado];

    printf("Bem vindo %s!\n", usuario_logado->nome);

    do {
        delay(1000);
        exibir_operacoes();
        operacao = escolha_operacao(7);

        switch (operacao) {
        case 1:
            consultar_saldo(usuario_logado);
            delay(1000);
            break;
        case 2:            
            consultar_extrato(usuario_logado);
            delay(1000);
            break;
        case 3:
            depositar(usuario_logado);
            break;
        case 4:
            sacar(usuario_logado);
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

void consultar_saldo(Usuario *usuario_atual) {
    printf("Dados da sua conta:\n");

    printf("\nNome: %s\n", usuario_atual->nome);
    printf("CPF: %s\n", usuario_atual->cpf);
    
    printf("\nSaldo:\n");

    printf("\nReais: R$ %.2f\n", usuario_atual->carteira.real);
    printf("Bitcoin: %.4f BTC\n", usuario_atual->carteira.btc);
    printf("Ethereum: %.4f ETH\n", usuario_atual->carteira.eth);
    printf("Ripple: %.4f XRP\n", usuario_atual->carteira.xrp);

    voltar_menu();
}

void consultar_extrato(Usuario *usuario_atual) {

    if (usuario_atual->qnt_transacoes == 0) {
        print_erro("Sua conta ainda nao possui nenhuma transacao.");
        return;
    }

    printf("Transacoes da sua conta:\n");
    printf("\n%-12s %-15s %-10s %-10s\n", "Tipo", "Data", "Valor", "Taxa");
    printf("-----------------------------------------------\n");

    for (int i = 0; i < usuario_atual->qnt_transacoes; i++) {
        printf("%-12s %-15s %s %.2f R$ %.2f\n",
        usuario_atual->extrato[i].tipo,
        usuario_atual->extrato[i].data,
        usuario_atual->extrato[i].sigla_moeda,
        usuario_atual->extrato[i].valor,
        usuario_atual->extrato[i].taxa
        );
    }

    voltar_menu();
}

void depositar(Usuario *usuario_atual) {
    float entr_valor;
    
    printf("Faca seu deposito:\n");

    do {
        printf("\nInforme o valor a ser depositado: R$ ");
        if (scanf("%f", &entr_valor) != 1 || entr_valor <= 0) {
            print_erro("Valor para deposito invalido. Insira novamente.");
            entr_valor = -1.0; // forca o loop
        }
        limpar_buffer();
    }while (entr_valor <= 0);

    usuario_atual->carteira.real += entr_valor;

    if (salvar_transacao(usuario_atual, "Deposito", "R$", entr_valor, 0.0) == FALHA) {
        print_erro("Erro ao salvar a transacao efetuada. Cancelando operacao...");
        return; // volta pro menu
    }
    printf("Deposito de R$ %.2f realizado com sucesso!\n", entr_valor);

    voltar_menu();
}

void sacar(Usuario *usuario_atual) {
    float entr_valor;

    printf("Realize seu saque:\n");

    do {
        printf("\nInforme o valor que deseja sacar: R$ ");
        if (scanf("%f", &entr_valor) != 1 || entr_valor <= 0 || entr_valor > usuario_atual->carteira.real) {
            print_erro("Valor desejado do saque invalido. Insira novamente.");
            entr_valor = -1.0; // forca o loop
        }
        limpar_buffer();
    }while (entr_valor <= 0);

    usuario_atual->carteira.real -= entr_valor;

    if (salvar_transacao(usuario_atual, "Saque", "R$", entr_valor, 0.0) == FALHA) {
        print_erro("Erro ao salvar a transacao efetuada. Cancelando operacao...");
        return; // volta pro menu
    }
    printf("Saque de R$ %.2f realizado com sucesso!\n", entr_valor);

    voltar_menu();    
}