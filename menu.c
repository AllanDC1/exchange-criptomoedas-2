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

    print_titulo("Crie uma conta:");

    // CRIAÇÃO CPF
    do {
        printf("\nInforme o CPF (apenas numeros): ");
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
        printf("\nInforme nome: ");
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

    print_titulo("\nDados da conta:");

    printf("\nNome: %s\n", usuarios[excluir_idx].nome);
    printf("CPF: %s\n", usuarios[excluir_idx].cpf);

    printf("\nDeseja realmente excluir a conta de \033[0;35m%s\033[0m?\n", usuarios[excluir_idx].nome);
    
    if (confirmar_acao() == OK) {
        for (int i = excluir_idx; i < qnt_usuarios - 1; i++) {
            usuarios[i] = usuarios[i + 1];
        }
        qnt_usuarios--;

        if (salvar_usuarios(usuarios, qnt_usuarios) == FALHA) {
            print_erro("\nErro ao salvar os dados do usuario. Cancelando operacao...");
            return FALHA; // voltar pro menu
        }
        return OK;
    }else {
        print_erro("\nExclusao de conta cancelada. Voltando ao menu...");
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

void menu_adm() {
    print_titulo("\nPrograma de Administrador do Exchange de CriptoMoedas.");
    printf("\n1. Login\n");
    printf("2. Registrar admin\n");
    printf("3. Excluir admin\n");
    printf("0. Sair\n");
}

Resposta registro_admin() {
    Admin admins[MAX_ADMS], novo_admin;
    int qnt_adms = 0;
    char entrada_cpf[14], entrada_senha[18], entrada_nome[23]; // buffer para conferir tamanho
    
    if (ler_admins(admins, &qnt_adms) == FALHA) {
        print_erro("Erro ao acessar dados dos admins. Cancelando operacao...");
        return FALHA;
    }
    
    if (qnt_adms >= MAX_ADMS) {
        print_erro("Quantidade de administradores atingiu o limite. Cancelando operacao...");
        return FALHA; // voltar pro menu
    }

    print_titulo("Crie sua conta de ADM:");

    // CRIAÇÃO CPF
    do {
        printf("\nInforme seu CPF (apenas numeros): ");
        fgets(entrada_cpf, sizeof(entrada_cpf), stdin);               
    } while (verificar_cpf(entrada_cpf) == FALHA);

    for (int i = 0; i < qnt_adms; i++) {
        if (strcmp(entrada_cpf, admins[i].cpf) == 0) {
            print_erro("O CPF inserido ja possui uma conta cadastrada. Cancelando operacao...");
            return FALHA;
        }
    }
    
    strncpy(novo_admin.cpf, entrada_cpf, 11);
    novo_admin.cpf[11] = '\0';

    // CRIAÇÃO SENHA
    do {
        printf("\nCrie uma senha (de 6 a 15 caracteres): ");
        fgets(entrada_senha, sizeof(entrada_senha), stdin);        
    } while (verificar_senha(entrada_senha) == FALHA);

    int len_senha = strlen(entrada_senha);
    strncpy(novo_admin.senha, entrada_senha, len_senha);
    novo_admin.senha[len_senha] = '\0';

    // CRIAÇÃO NOME
    do {
        printf("\nInforme seu nome: ");
        fgets(entrada_nome, sizeof(entrada_nome), stdin);         
    } while (verificar_nome(entrada_nome) == FALHA);

    int len_nome = strlen(entrada_nome);
    strncpy(novo_admin.nome, entrada_nome, len_nome);
    novo_admin.nome[len_nome] = '\0';

    admins[qnt_adms] = novo_admin;
    qnt_adms++;
     
    if (salvar_admins(admins, qnt_adms) == FALHA) {
        print_erro("Erro ao salvar o novo admin. Cancelando operacao...");
        return FALHA;
    }

    return OK;
}

Resposta excluir_adm() {
    Admin admins[MAX_ADMS];
    int qnt_adms = 0, excluir_idx = FALHA;
    char entrada_cpf[14];
    
    if (ler_admins(admins, &qnt_adms) == FALHA) {
        print_erro("Erro ao acessar dados dos adms. Cancelando operacao...");
        return FALHA;
    }

    if (qnt_adms <= 0) {
        print_erro("A plataforma ainda nao possui nenhum ADM.");
        return FALHA;
    }

    print_titulo("Excluir conta de ADM:");

    do{
        printf("\nInsira o CPF da conta que sera excluida: ");
        fgets(entrada_cpf, sizeof(entrada_cpf), stdin);
        verificar_buffer(entrada_cpf);        

        for (int i = 0; i < qnt_adms; i++) {
            if (strcmp(entrada_cpf, admins[i].cpf) == 0) {
                excluir_idx = i;
                break;
            }
        }

        if (excluir_idx == FALHA) {
            print_erro("Admin nao encontrado. Insira novamente.");
            printf("\n");
        }        
    }while (excluir_idx == FALHA);

    printf("\nDeseja realmente excluir a conta de \033[0;35m%s\033[0m?\n", admins[excluir_idx].nome);
    
    if (confirmar_acao() == OK) {
        for (int i = excluir_idx; i < qnt_adms - 1; i++) {
            admins[i] = admins[i + 1];
        }
        qnt_adms--;

        if (salvar_admins(admins, qnt_adms) == FALHA) {
            print_erro("Erro ao salvar os dados dos ADMS. Cancelando operacao...");
            return FALHA; // voltar pro menu
        }
        return OK;
    }else {
        print_erro("Exclusao de conta cancelada. Voltando ao menu...");
        return FALHA;
    }
}

Resposta login_adm() {
    Admin admins[MAX_ADMS];    
    int qnt_adms = 0, auth = FALHA;
    char entrada_cpf[14], entrada_senha[18];

    if (ler_admins(admins, &qnt_adms) == FALHA) {
        print_erro("Erro ao acessar dados dos adms. Cancelando operacao...");
        return FALHA;
    }

    if (qnt_adms <= 0) {
        print_erro("A plataforma ainda nao possui nenhum ADM.");
        return FALHA;
    }

    print_titulo("Faca login na conta de ADM:");
    
    do{
        printf("\nCPF: ");
        fgets(entrada_cpf, sizeof(entrada_cpf), stdin);
        verificar_buffer(entrada_cpf);
        printf("Senha: ");
        fgets(entrada_senha, sizeof(entrada_senha), stdin);
        verificar_buffer(entrada_senha);
        printf("\n");
        
        for (int i = 0; i < qnt_adms; i++) {
            if (strcmp(entrada_cpf, admins[i].cpf) == 0) {
                if (strcmp(entrada_senha, admins[i].senha) == 0) {
                    auth = i;
                    break;
                }
            }
        }
        if (auth == FALHA) {
            print_erro("Credenciais incorretas. Insira novamente.");
        }

    }while(auth == FALHA);

    return OK;
}

void exibir_operacoes_adm() {
    print_titulo("\nOperacoes para ADMS:");
    printf("\n1. Cadastrar Investidor\n");
    printf("2. Excluir Investidor\n");
    printf("3. Cadastrar Criptomoeda\n");
    printf("4. Excluir Criptomoeda\n");
    printf("5. Consultar Saldo de Investidor\n");
    printf("6. Consultar Extrato de Investidor\n");
    printf("7. Atualizar cotacao das criptomoedas\n");
    printf("0. Sair\n");
}

void operacoes_adm() {
    Usuario usuarios[MAX_USUARIOS], *usuario_selecionado;
    int qnt_usuarios = 0, operacao, idx_selecionado;

    do {
        delay(1000);
        exibir_operacoes_adm();
        operacao = escolha_operacao(7);

        switch (operacao) {
        case 1:
            if (registro_usuario() == OK) {
                print_sucesso("\nInvestidor criado. Voltando ao menu...");
            }
            delay(1000);
            break;
        case 2:
            if (excluir_usuario() == OK) {
                print_sucesso("\nInvestidor excluido. Voltando ao menu...");
            } 
            delay(1000);
            break;
        case 3:
            criar_cripto();
            delay(1000);
            break;
        case 4:
            excluir_cripto();
            delay(1000);
            break;
        case 5:
            idx_selecionado = selecionar_investidor();
            if (idx_selecionado != FALHA) {
                if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {
                    print_erro("Erro ao acessar dados dos usuarios. Cancelando operacao...");
                    return; // voltar ao menu
                }  
                usuario_selecionado = &usuarios[idx_selecionado];
                consultar_saldo(usuario_selecionado);
            }
            delay(1000);
            break;
        case 6:
            idx_selecionado = selecionar_investidor();
            if (idx_selecionado != FALHA) {
                if (ler_usuarios(usuarios, &qnt_usuarios) == FALHA) {
                    print_erro("Erro ao acessar dados dos usuarios. Cancelando operacao...");
                    return; // voltar ao menu
                }  
                usuario_selecionado = &usuarios[idx_selecionado];
                consultar_extrato(usuario_selecionado);
            }
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