#include "exchangecripto.h"

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

    while(fread(&moeda_temp, sizeof(Criptomoeda), 1, fP) != 0) {
        array_moedas[*qnt_moedas] = moeda_temp; // pega o indice disponivel no array, e armazena nele
        (*qnt_moedas)++;
    }

    fclose(fP);
    return OK;
}

// ADMINS

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

Resposta ler_admins(Admin array_admins[], int *quantidade_lida) {
    Admin temp;
    *quantidade_lida = 0;
    FILE *fP = abrir_arquivo("admins.bin", "rb");

    if (fP == NULL) {
        print_erro("Erro ao acessar arquivo de admins.");
        return FALHA;
    }

    if (verificar_tamanho_arquivo(fP) == OK) {
        while(fread(&temp, sizeof(Admin), 1, fP) != 0) {
            array_admins[*quantidade_lida] = temp;
            (*quantidade_lida)++;
        }
    }

    fclose(fP);
    return OK;
}

Resposta salvar_admins(Admin array_admins[], int qnt_adms) {
    FILE *fP = abrir_arquivo("admins.bin", "wb");

    if (fP == NULL) {
        print_erro("Erro ao acessar arquivo.");             
        return FALHA;
    }

    fwrite(array_admins, sizeof(Admin), qnt_adms, fP);
    fclose(fP);
    return OK;
}