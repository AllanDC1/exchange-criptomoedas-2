#include "exchangecripto.h"

void delay(int tempo_ms) {
    #ifdef _WIN32
    Sleep(tempo_ms);
    #else
    usleep(tempo_ms * 1000); // mili * 1000 -> micro
    #endif
}

void upper(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void verificar_buffer(char *entrada) {
    if (entrada[strlen(entrada) -1] != '\n') {
        limpar_buffer();
    } else {
        entrada[strcspn(entrada, "\n")] = 0;    
    }
}

void print_erro(const char *msg) {
    printf("\033[0;31m %s \033[0m\n", msg);
}

void print_titulo(const char *msg) {
    printf("\033[1;36m %s \033[0m\n", msg);
}

void print_confirmacao(const char *msg) {
    printf("\033[1;33m %s \033[0m", msg);
}

void print_sucesso(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    printf("\033[1;32m ");
    vprintf(msg, args);
    printf("\033[0m\n");
    va_end(args);
}

void voltar_menu() {
    printf("\n\033[0;32m Pressione ENTER para voltar ao menu. \033[0m");
    getchar();
    printf("\nVoltando ao menu de operacoes...\n");
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

Resposta confirmar_acao() {
    int confirmacao;

    do{
        printf("1-Confirmar ou 2-Cancelar: ");
        if (scanf("%d", &confirmacao) != 1 || confirmacao != 1 && confirmacao != 2) {
            print_erro("Escolha invalida.\n");
            confirmacao = FALHA;
        }            
        limpar_buffer();
    }while (confirmacao == FALHA);

    if (confirmacao == 1) {
        return OK;
    } else {
        return FALHA;
    }
}

FILE* abrir_arquivo(char* nome_arquivo, char* modo) {    
    FILE *ponteiro = fopen(nome_arquivo, modo);  // tentar abrir no modo

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

Resposta ler_moedas(Criptomoeda array_moedas[], int *qnt_moedas) {
    Criptomoeda moeda_temp;
    FILE *fP = abrir_arquivo("dados-moedas", "rb+");

    if (fP == NULL) {
        print_erro("Erro ao acessar arquivo.");
        return FALHA;
    }

    if (verificar_tamanho_arquivo(fP) == FALHA) { // se estiver vazio/recem criado, salva as informacoes das moedas
        Criptomoeda criptos_padrao[] = {
            {"BTC", 0.03, 0.02, 350.00},
            {"ETH", 0.02, 0.01, 14.50},
            {"XRP", 0.01, 0.01, 3.30}
        };        

        for (int i = 0; i < MAX_CRIPTOMOEDAS; i++) {
            fwrite(&criptos_padrao[i], sizeof(Criptomoeda), 1, fP);
        }

        rewind(fP);
    }

    while(fread(&moeda_temp, sizeof(Criptomoeda), 1, fP) != 0) {
        array_moedas[*qnt_moedas] = moeda_temp; // pega o indice disponivel no array, e armazena nele
        (*qnt_moedas)++;
    }

    fclose(fP);
    return OK;
}

Resposta salvar_moedas(Criptomoeda array_moedas[], int quantidade_moedas) {
    FILE *fP = abrir_arquivo("dados-moedas", "wb");

    if (fP == NULL) {
        print_erro("Erro ao acessar arquivo.");             
        return FALHA;
    }

    fwrite(array_moedas, sizeof(Criptomoeda), quantidade_moedas, fP);
    fclose(fP);
    return OK;
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

Resposta validar_senha(Usuario *usuario_logado) {
    char entr_senha[25];
    int auth = FALHA;
    int tentativas = 0;

    print_titulo("\nValide sua senha:");

    do {
        printf("\nInsira sua senha: ");
        fgets(entr_senha, sizeof(entr_senha), stdin);
        verificar_buffer(entr_senha);
        if (strcmp(entr_senha, usuario_logado->senha) != 0) {
            print_erro("Senha inserida incorreta.");
            auth = FALHA;
            tentativas++;
        } else {
            auth = OK;
        }
    }while (auth == FALHA && tentativas < 3);

    if (tentativas == 3) {
        print_erro("\n Maximo de tentativas de senha atingido.");
        delay(1000);
        return FALHA;
    }else {
        print_sucesso("\nSenha validada.\n");
        delay(1500);
        return OK;
    }
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

int achar_usuario(Usuario array_usuarios[], int quantidade_usuarios, const char* cpf_usuario) {
    for (int i = 0; i < quantidade_usuarios; i++) {
        if (strcmp(cpf_usuario, array_usuarios[i].cpf) == 0) {           
            return i;            
        }    
    }
    return FALHA;
}

Resposta salvar_transacao(Usuario *usuario_logado, char* tipo, char* moeda, float valor, float taxa) {
    Usuario usuarios[MAX_USUARIOS];
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

    idx_usuario = achar_usuario(usuarios, qnt_usuarios, usuario_logado->cpf);
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