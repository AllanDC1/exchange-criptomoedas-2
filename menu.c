#include "exchangecripto.h"

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

// MENU INICIAL

void exibir_menu() {
    print_titulo("\nBem-Vindo ao Exchange de CriptoMoedas!");
    printf("\n1. Login\n");
    printf("2. Registrar-se\n");
    printf("3. Excluir conta\n");
    printf("0. Sair\n");
}

ResultadoLogin login_usuario() {
    Usuario usuarios[MAX_USUARIOS];
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

    print_titulo("Faca seu login:");
    
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

Resposta registro_usuario() {
    Usuario usuarios[MAX_USUARIOS];
    Criptomoeda criptos_sistema[MAX_CRIPTOMOEDAS];
    int qnt_usuarios = 0, qnt_moedas = 0;
    char entrada_cpf[14], entrada_senha[18], entrada_nome[23]; // buffer para conferir tamanho
    
    if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA || ler_moedas(criptos_sistema, &qnt_moedas) == FALHA) {
        print_erro("Erro ao acessar dados dos arquivos. Cancelando operacao...");
        return FALHA;
    }
    
    if (qnt_usuarios >= MAX_USUARIOS) {
        print_erro("Quantidade de usuarios criados atingiu o limite. Cancelando operacao...");
        return FALHA; // voltar pro menu
    }

    // Criar novo usuario e definir dados padroes
    Usuario novo_usuario = { .carteira = { .real = 0.0 }, .qnt_transacoes = 0};

    for (int i = 0; i < qnt_moedas; i++) {
        strcpy(novo_usuario.carteira.criptomoeda[i].sigla, criptos_sistema[i].sigla);
        novo_usuario.carteira.criptomoeda[i].saldo = 0;
    }

    print_titulo("Crie sua conta:");

    // CRIAÇÃO CPF
    do {
        printf("\nInforme seu CPF (apenas numeros): ");
        fgets(entrada_cpf, sizeof(entrada_cpf), stdin);               
    } while (verificar_cpf(entrada_cpf) == FALHA);

    if (achar_usuario(usuarios, qnt_usuarios, entrada_cpf) != FALHA) {
        print_erro("O CPF inserido ja possui uma conta cadastrada. Cancelando operacao...");
        return FALHA;
    }
    
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
        printf("\nInforme seu nome: ");
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
    Usuario usuarios[MAX_USUARIOS];
    int qnt_usuarios = 0, excluir_idx = FALHA;
    char entrada_cpf[14];
    
    if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {
        print_erro("Erro ao acessar dados dos usuarios. Cancelando operacao...");
        return FALHA;
    }

    if (qnt_usuarios <= 0) {
        print_erro("A plataforma ainda nao possui nenhum usuario.");
        return FALHA;
    }

    print_titulo("Excluir uma conta:");

    do{
        printf("\nInsira o CPF da conta que sera excluida: ");
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

    printf("\nDeseja realmente excluir a conta de \033[0;35m%s\033[0m?\n", usuarios[excluir_idx].nome);
    
    if (confirmar_acao() == OK) {
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

// MENU OPERACOES

void exibir_operacoes() {
    print_titulo("\nOperacoes disponiveis:");
    printf("\n1. Consultar saldo\n");
    printf("2. Consultar extrato\n");
    printf("3. Depositar\n");
    printf("4. Sacar\n");
    printf("5. Comprar criptomoedas\n");
    printf("6. Vender criptomoedas\n");
    printf("7. Atualizar cotacao das criptomoedas\n");
    printf("0. Sair\n");
}

void consultar_saldo(Usuario *usuario_atual) {
    print_titulo("Dados da sua conta:");

    printf("\nNome: %s\n", usuario_atual->nome);
    printf("CPF: %s\n", usuario_atual->cpf);
    
    print_titulo("\nSaldo:");
    
    // IDEALMENTE DEVERIA SER DINAMICO
    printf("\nReais: R$ %.2f\n", usuario_atual->carteira.real);
    printf("Bitcoin: %.4f BTC\n", usuario_atual->carteira.criptomoeda[0].saldo);
    printf("Ethereum: %.4f ETH\n", usuario_atual->carteira.criptomoeda[1].saldo);
    printf("Ripple: %.4f XRP\n", usuario_atual->carteira.criptomoeda[2].saldo);

    voltar_menu();
}

void consultar_extrato(Usuario *usuario_atual) {

    if (usuario_atual->qnt_transacoes == 0) {
        print_erro("Sua conta ainda nao possui nenhuma transacao.");
        return;
    }

    print_titulo("Transacoes da sua conta:");
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
    int qnt_moedas = 0;

    if (ler_moedas(criptos_sistema, &qnt_moedas) == FALHA) {
        print_erro("Erro ao acessar dados das moedas. Cancelando operacao...");
        return; // volta pro menu
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
        int numero_variacao = (rand() % 11) - 5;
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

void menu_operacoes(int idx_logado) {
    Usuario usuarios[MAX_USUARIOS], *usuario_logado;
    int qnt_usuarios = 0, operacao;    

    if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {
        print_erro("Erro ao acessar dados dos usuarios. Cancelando operacao...");
        return; // voltar ao menu
    }

    usuario_logado = &usuarios[idx_logado];

    printf("\nBem vindo\033[0;35m %s\033[0m!\n", usuario_logado->nome);

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
            delay(1000);
            break;
        case 4:
            sacar(usuario_logado);
            delay(1000);
            break;
        case 5:
            comprar_criptomoeda(usuario_logado);
            delay(1000);
            break;
        case 6:
            vender_criptomoeda(usuario_logado);
            delay(1000);
            break;
        case 7:
            atualizar_cotacao();
            delay(1000);
            break;
        case 0:
            printf("Voltando ao menu inicial...\n");
            break;
        }

    }while (operacao != 0);
}