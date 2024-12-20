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

void consultar_saldo(Usuario *usuario_atual) {
    Criptomoeda criptos_sistema[MAX_CRIPTOMOEDAS];
    int qnt_moedas = 0;

    if (ler_moedas(criptos_sistema, &qnt_moedas) == FALHA) {
        print_erro("Erro ao acessar dados das moedas. Cancelando operacao...");
        return; // volta pro menu
    }

    print_titulo("Dados da conta:");

    printf("\nNome: %s\n", usuario_atual->nome);
    printf("CPF: %s\n", usuario_atual->cpf);
    
    print_titulo("\nSaldo:");
    
    printf("\nBRL: R$ %.2f\n", usuario_atual->carteira.real);

    for (int i = 0; i < qnt_moedas; i++) {
        printf("%s: %4.4f\n", usuario_atual->carteira.criptomoeda[i].sigla, usuario_atual->carteira.criptomoeda[i].saldo);
    }

    voltar_menu();
}

void consultar_extrato(Usuario *usuario_atual) {

    if (usuario_atual->qnt_transacoes == 0) {
        print_erro("Essa conta ainda nao possui nenhuma transacao.");
        return;
    }

    print_titulo("Transacoes da conta:");
    printf("\n%-10s %-20s %-12s %-10s\n", "Tipo", "Data", "Valor", "Taxa");
    printf("------------------------------------------------------\n");

    for (int i = 0; i < usuario_atual->qnt_transacoes; i++) {
        printf("%-10s %-20s %-3s %-8.2f R$ %-10.2f\n",
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
    
    print_titulo("Faca seu deposito:");

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
    print_sucesso("Deposito de R$ %.2f realizado com sucesso!", entr_valor);

    voltar_menu();
}

void sacar(Usuario *usuario_atual) {
    float entr_valor;

    if (usuario_atual->carteira.real <= 0) {
        print_erro("Voce nao possui saldo na sua carteira.");
        return; // volta pro menu
    }

    print_titulo("Realize seu saque:");
    printf("Seu saldo: R$ %.2f\n", usuario_atual->carteira.real);

    do {
        printf("\nInforme o valor que deseja sacar: R$ ");
        if (scanf("%f", &entr_valor) != 1 || entr_valor <= 0 || entr_valor > usuario_atual->carteira.real) {
            print_erro("Valor desejado do saque invalido. Insira novamente.");
            entr_valor = -1.0; // forca o loop
        }
        limpar_buffer();
    }while (entr_valor <= 0);

    if (validar_senha(usuario_atual) == FALHA) {
        print_erro("Validacao de senha falhou. Cancelando operacao...");
        return; // volta pro menu
    }

    usuario_atual->carteira.real -= entr_valor;

    if (salvar_transacao(usuario_atual, "Saque", "R$", entr_valor, 0.0) == FALHA) {
        print_erro("Erro ao salvar a transacao efetuada. Cancelando operacao...");
        return; // volta pro menu
    }
    print_sucesso("Saque de R$ %.2f realizado com sucesso!", entr_valor);

    voltar_menu();    
}

void comprar_criptomoeda(Usuario *usuario_atual) {
    Criptomoeda criptos_sistema[MAX_CRIPTOMOEDAS];
    int qnt_moedas = 0, idx_moeda;
    float entr_valor, taxa, valor_taxado, valor_moeda;
    char entr_sigla[TAM_SIGLA];
    
    if (ler_moedas(criptos_sistema, &qnt_moedas) == FALHA) {
        print_erro("Erro ao acessar dados das moedas. Cancelando operacao...");
        return; // volta pro menu
    }

    if (qnt_moedas == 0) {
        print_erro("Nenhuma Criptomoeda foi registrada no sistema.");
        return;
    }

    if (usuario_atual->carteira.real <= 0) {
        print_erro("Voce nao possui saldo na sua carteira.");
        return; // volta pro menu
    }

    print_titulo("Comprar criptomoedas:");
    printf("Seu saldo: R$ %.2f\n", usuario_atual->carteira.real);

    while (true) {
        printf("\nInforme a sigla da criptomoeda que deseja comprar: ");
        fgets(entr_sigla, sizeof(entr_sigla), stdin);
        verificar_buffer(entr_sigla);
        upper(entr_sigla);

        for (int i = 0; i < qnt_moedas; i++) {
            if (strcmp(entr_sigla, criptos_sistema[i].sigla) == 0) {
                idx_moeda = i;
                goto MOEDA_ENCONTRADA;
            }
        }
        print_erro("Criptmoeda/Sigla invalida. Insira novamente.");
        delay(500);     
    };

    MOEDA_ENCONTRADA:

    do {
        printf("\nInforme o valor, em reais, que deseja comprar de %s: R$ ", criptos_sistema[idx_moeda].sigla);
        if (scanf("%f", &entr_valor) != 1 || entr_valor <= 0 || entr_valor > usuario_atual->carteira.real) {
            print_erro("Valor para compra invalido. Insira novamente.");
            entr_valor = -1.0; // forca o loop
        }
        limpar_buffer();
    }while (entr_valor <= 0);

    if (validar_senha(usuario_atual) == FALHA) {
        print_erro("Validacao de senha falhou. Cancelando operacao...");
        return; // volta pro menu
    }
    
    taxa = entr_valor * criptos_sistema[idx_moeda].tx_compra;
    valor_taxado = entr_valor - taxa;
    valor_moeda = valor_taxado / criptos_sistema[idx_moeda].cotacao;

    print_titulo("Informacoes da sua transacao:");
    printf("\nCompra de %.4f %s, por R$ %.2f\n", valor_moeda, criptos_sistema[idx_moeda].sigla, entr_valor);
    printf("Taxa de transacao: R$ %.2f\n", taxa);
    printf("Cotacao %s: R$ %.2f\n", criptos_sistema[idx_moeda].sigla, criptos_sistema[idx_moeda].cotacao);

    print_confirmacao("\nConfirme sua compra.\n");
    if (confirmar_acao() == FALHA) {
        print_erro("Transacao cancelada. Voltando para o menu...");
        return; // volta pro menu
    }
    
    usuario_atual->carteira.criptomoeda[idx_moeda].saldo += valor_moeda; // os indices das moedas sao iguais, pois o usuario eh criado com base nas criptos do sistema
    usuario_atual->carteira.real -= entr_valor;

    if (salvar_transacao(usuario_atual, "Compra", criptos_sistema[idx_moeda].sigla, valor_moeda, taxa) == FALHA) {
        print_erro("Erro ao salvar a transacao efetuada. Cancelando operacao...");
        return; // volta pro menu
    }

    print_sucesso("\nCompra de %.4f %s realizada com sucesso!", valor_moeda, criptos_sistema[idx_moeda].sigla);

    voltar_menu();
}

void vender_criptomoeda(Usuario *usuario_atual) {
    Criptomoeda criptos_sistema[MAX_CRIPTOMOEDAS];
    int qnt_moedas = 0, idx_moeda;
    float entr_valor, taxa, valor_taxado, valor_reais;
    char entr_sigla[TAM_SIGLA];

    if (ler_moedas(criptos_sistema, &qnt_moedas) == FALHA) {
        print_erro("Erro ao acessar dados das moedas. Cancelando operacao...");
        return; // volta pro menu
    }

    if (qnt_moedas == 0) {
        print_erro("Nenhuma Criptomoeda foi registrada no sistema.");
        return;
    }

    print_titulo("Vender criptomoedas:");

    while (true) {
        printf("\nInforme a sigla da criptomoeda que deseja vender: ");
        fgets(entr_sigla, sizeof(entr_sigla), stdin);
        verificar_buffer(entr_sigla);
        upper(entr_sigla);

        for (int i = 0; i < qnt_moedas; i++) {
            if (strcmp(entr_sigla, criptos_sistema[i].sigla) == 0) {
                idx_moeda = i;
                goto MOEDA_ENCONTRADA;
            }
        }
        print_erro("Criptmoeda/Sigla invalida. Insira novamente.");
        delay(500);     
    };

    MOEDA_ENCONTRADA:

    if (usuario_atual->carteira.criptomoeda[idx_moeda].saldo <= 0) {
        print_erro("\nVoce nao possui saldo dessa moeda. Cancelando operacao...");
        return; // volta pro menu
    }

    printf("Seu saldo de %s: %.4f\n", criptos_sistema[idx_moeda].sigla, usuario_atual->carteira.criptomoeda[idx_moeda].saldo);

    do {
        printf("\nInforme a quantidade que deseja vender de %s:  ", criptos_sistema[idx_moeda].sigla);
        if (scanf("%f", &entr_valor) != 1 || entr_valor <= 0 || entr_valor > usuario_atual->carteira.criptomoeda[idx_moeda].saldo) {
            print_erro("Valor de venda invalido. Insira novamente.");
            entr_valor = -1.0; // forca o loop
        }
        limpar_buffer();
    }while (entr_valor <= 0);

    if (validar_senha(usuario_atual) == FALHA) {
        print_erro("Validacao de senha falhou. Cancelando operacao...");
        return; // volta pro menu
    }
    
    valor_reais = entr_valor * criptos_sistema[idx_moeda].cotacao;
    taxa = valor_reais * criptos_sistema[idx_moeda].tx_venda;
    valor_taxado = valor_reais - taxa;

    print_titulo("Informacoes da sua transacao:");
    printf("\nVenda de %.4f %s, por R$ %.2f\n", entr_valor, criptos_sistema[idx_moeda].sigla, valor_taxado);
    printf("Taxa de transacao: R$ %.2f\n", taxa);
    printf("Cotacao %s: R$ %.2f\n", criptos_sistema[idx_moeda].sigla, criptos_sistema[idx_moeda].cotacao);

    print_confirmacao("\nConfirme a venda.\n");
    if (confirmar_acao() == FALHA) {
        print_erro("Transacao cancelada. Voltando para o menu...");
        return; // volta pro menu
    }
    
    usuario_atual->carteira.criptomoeda[idx_moeda].saldo -= entr_valor; // os indices das moedas sao iguais, pois o usuario eh criado com base nas criptos do sistema
    usuario_atual->carteira.real += valor_taxado;

    if (salvar_transacao(usuario_atual, "Venda", criptos_sistema[idx_moeda].sigla, entr_valor, taxa) == FALHA) {
        print_erro("Erro ao salvar a transacao efetuada. Cancelando operacao...");
        return; // volta pro menu
    }

    print_sucesso("\nVenda de %.4f %s realizada com sucesso!", entr_valor, criptos_sistema[idx_moeda].sigla);

    voltar_menu();
}

void atualizar_cotacao() {
    Criptomoeda criptos_sistema[MAX_CRIPTOMOEDAS];
    int qnt_moedas = 0, numero_variacao;

    if (ler_moedas(criptos_sistema, &qnt_moedas) == FALHA) {
        print_erro("Erro ao acessar dados das moedas. Cancelando operacao...");
        return; // volta pro menu
    }

    if (qnt_moedas == 0) {
        print_erro("Nenhuma Criptomoeda foi registrada no sistema.");
        return;
    }

    print_titulo("Atualizar a cotacao das criptomoedas:");
    printf("\nCotacao atual:\n");
    for (int i = 0; i < qnt_moedas; i++) {
        printf("%s: R$%.2f\n", criptos_sistema[i].sigla, criptos_sistema[i].cotacao);
    }

    print_confirmacao("\nDeseja realmente atualizar as cotacoes das criptomoedas?\n");
    if (confirmar_acao() == FALHA) {
        print_erro("Operacao cancelada. Voltando para o menu...");
        return; // volta pro menu
    }
    print_sucesso("\nCotacao das moedas atualizadas:");

    srand(time(NULL));

    for (int i = 0; i < qnt_moedas; i++) {
        do {
            numero_variacao = (rand() % 11) - 5;
        } while (numero_variacao == 0);
        float porcentagem_variacao = (float)numero_variacao / 100.0;
        
        criptos_sistema[i].cotacao *= (1 + porcentagem_variacao);
        
        printf("Nova cotacao %s: %.2f (Variacao de %d%%)\n", criptos_sistema[i].sigla, criptos_sistema[i].cotacao, numero_variacao);
    }

    if (salvar_moedas(criptos_sistema, qnt_moedas) == FALHA) {
        print_erro("Erro ao salvar os dados da moeda. Cancelando operacao...");
        return; // voltar pro menu
    }

    voltar_menu();
}

int selecionar_investidor() {
    Usuario usuarios[MAX_USUARIOS];
    int qnt_usuarios = 0, idx_selecionado = FALHA;
    char entrada_cpf[14];

    if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {
        print_erro("Erro ao acessar dados dos usuarios. Cancelando operacao...");
        return FALHA;
    }

    if (qnt_usuarios <= 0) {
        print_erro("A plataforma ainda nao possui nenhum investidor.");
        return FALHA;
    }

    do {
        printf("Insira o CPF do investidor que deseja consultar: ");        
        fgets(entrada_cpf, sizeof(entrada_cpf), stdin);
        verificar_buffer(entrada_cpf);        

        for (int i = 0; i < qnt_usuarios; i++) {
            if (strcmp(entrada_cpf, usuarios[i].cpf) == 0) {
                idx_selecionado = i;
                break;
            }
        }

        if (idx_selecionado == FALHA) {
            print_erro("Investidor nao encontrado. Insira novamente.");
            printf("\n");
        }

    }while (idx_selecionado == FALHA);
    
    return idx_selecionado;
}

void criar_cripto() {
    Criptomoeda criptos_sistema[MAX_CRIPTOMOEDAS], nova_moeda;
    Usuario usuarios[MAX_USUARIOS];
    int qnt_usuarios = 0, qnt_moedas = 0, continuar = FALHA;
    char entrada_sigla[TAM_SIGLA + 2];
    float entrada_cotacao, entrada_taxa[2];

    if (ler_moedas(criptos_sistema, &qnt_moedas) == FALHA) {
        print_erro("Erro ao acessar dados das moedas. Cancelando operacao...");
        return;
    }

    if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {
        print_erro("Erro ao acessar dados dos usuarios. Cancelando operacao...");
        return;
    }

    if (qnt_moedas >= MAX_CRIPTOMOEDAS) {
        print_erro("Limite de moedas do sistema atingido. Cancelando operacao...");
        return;
    }
    
    print_titulo("Criacao de Criptomoeda:");

    do {
        printf("\nInsira a sigla da criptomoeda: ");
        fgets(entrada_sigla, sizeof(entrada_sigla), stdin);
        verificar_buffer(entrada_sigla);
        upper(entrada_sigla);
        continuar = OK;

        if (strlen(entrada_sigla) < 2 || strlen(entrada_sigla) > TAM_SIGLA - 1) {
            print_erro("Sigla invalida, de 2 a 4 caracteres. Insira novamente.");
            continuar = FALHA;
        }

    } while(continuar == FALHA);

    do {
        printf("\nInsira a cotacao inicial da moeda: ");
        continuar = OK;
        if (scanf("%f", &entrada_cotacao) != 1 || entrada_cotacao <= 0) {
            print_erro("Cotacao invalida. Insira novamente.");
            continuar = FALHA;
        }
        limpar_buffer();
    } while(continuar == FALHA);

    do {
        printf("\nInsira a taxa de compra da moeda (em porcentagem): ");
        continuar = OK;
        if (scanf("%f", &entrada_taxa[0]) != 1 || entrada_taxa[0] < 0|| entrada_taxa[0] >= 100) {
            print_erro("Taxa de compra invalida. Insira novamente.");
            continuar = FALHA;
        }
        limpar_buffer();
    } while(continuar == FALHA);

    do {
        printf("\nInsira a taxa de venda da moeda (em porcentagem): ");
        continuar = OK;
        if (scanf("%f", &entrada_taxa[1]) != 1 || entrada_taxa[1] < 0|| entrada_taxa[0] >= 100) {
            print_erro("Taxa de venda invalida. Insira novamente.");
            continuar = FALHA;
        }
        limpar_buffer();
    } while(continuar == FALHA);

    strcpy(nova_moeda.sigla, entrada_sigla);
    nova_moeda.cotacao = entrada_cotacao;
    nova_moeda.tx_compra = entrada_taxa[0] / 100;
    nova_moeda.tx_venda = entrada_taxa[1] / 100;

    criptos_sistema[qnt_moedas] = nova_moeda;
    qnt_moedas++;   

    if (salvar_moedas(criptos_sistema, qnt_moedas) == FALHA) {
        print_erro("Erro ao salvar dados da moeda. Cancelando operacao...");
        return;
    }

    // Adicionar a moeda na carteira de todos usarios
    for (int i = 0; i < qnt_usuarios; i++) {
        strcpy(usuarios[i].carteira.criptomoeda[qnt_moedas - 1].sigla, nova_moeda.sigla);  
        usuarios[i].carteira.criptomoeda[qnt_moedas - 1].saldo = 0.0;
    }

    if (salvar_usuarios(usuarios, qnt_usuarios) == FALHA) {
        print_erro("Erro ao salvar moeda na carteira dos usuarios. Cancelando operacao...");
        return;
    }

    print_sucesso("\nCriptomoeda criada com sucesso!");
    voltar_menu();
}

void excluir_cripto() {
    Criptomoeda criptos_sistema[MAX_CRIPTOMOEDAS];
    Usuario usuarios[MAX_USUARIOS];
    int qnt_usuarios = 0, qnt_moedas = 0, idx_escolhido = FALHA;
    char entrada_sigla[TAM_SIGLA + 2];    

    if (ler_moedas(criptos_sistema, &qnt_moedas) == FALHA) {
        print_erro("Erro ao acessar dados das moedas. Cancelando operacao...");
        return;
    }

    if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {
        print_erro("Erro ao acessar dados dos usuarios. Cancelando operacao...");
        return;
    }

    if (qnt_moedas == 0) {
        print_erro("Nenhuma Criptomoeda foi registrada no sistema.");
        return;
    }
    
    print_titulo("Excluir Criptomoeda:");

    do {
        printf("\nInsira a sigla da criptomoeda que deseja excluir: ");
        fgets(entrada_sigla, sizeof(entrada_sigla), stdin);
        verificar_buffer(entrada_sigla);
        upper(entrada_sigla);

        for (int i = 0; i < qnt_moedas; i++) {
            if (strcmp(criptos_sistema[i].sigla, entrada_sigla) == 0) {
                idx_escolhido = i;
                break;
            }
        }

        if (idx_escolhido == FALHA) {
            print_erro("Sigla invalida ou nao encontrada. Insira novamente.");
        }

    } while(idx_escolhido == FALHA);    

    print_titulo("\nDados da moeda:");

    printf("\nSigla: %s\n", criptos_sistema[idx_escolhido].sigla);
    printf("Cotacao: %.2f\n", criptos_sistema[idx_escolhido].cotacao);
    printf("Taxa de compra: %.2f\n", criptos_sistema[idx_escolhido].tx_compra);
    printf("Taxa de venda: %.2f\n", criptos_sistema[idx_escolhido].tx_venda);

    printf("\nDeseja realmente excluir a moeda \033[0;35m%s\033[0m?\n", criptos_sistema[idx_escolhido].sigla);

    if (confirmar_acao() == OK) {
        qnt_moedas--;

        // Excluir moeda do sistema
        for (int i = idx_escolhido; i < qnt_moedas; i++) {
            criptos_sistema[i] = criptos_sistema[i + 1];
        }

        // Excluir moeda da carteira de todos os usuarios
        for (int i = 0; i < qnt_usuarios; i++) {
            for (int j = idx_escolhido; j < qnt_moedas; j++) {
                usuarios[i].carteira.criptomoeda[j] = usuarios[i].carteira.criptomoeda[j + 1];
            }
        }

        if (salvar_moedas(criptos_sistema, qnt_moedas) == FALHA) {
            print_erro("\nErro ao salvar dados das moedas. Cancelando operacao...");
            return;
        }

        if (salvar_usuarios(usuarios, qnt_usuarios) == FALHA) {
            print_erro("\nErro ao salvar dados dos usuarios. Cancelando operacao...");
            return;
        }

        print_sucesso("\nExclusao concluida.");
        voltar_menu();
        return;
    }else {
        print_erro("\nExclusao da moeda cancelada. Voltando ao menu...");
        return;
    }
}