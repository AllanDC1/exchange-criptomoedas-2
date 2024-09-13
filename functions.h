#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAM_CPF 12 // 11 dígitos + \0 ou \n no final
#define MAX_SENHA 16 // Senha do usuário, no máximo 15 dígitos + \0 ou \n no final
#define TAM_NOME 13 // Primeiro nome, no máximo 12 letras + \0 ou \n no final
#define TAM_DATA 9 // 00/00/00 + \0 = 9 caracteres
#define TAM_SIGLA 5 // 4 letras + \0, BTC ou DOGE por exemplo

#define MAX_TRANSACOES 100
#define MAX_USUARIOS 10 

typedef struct {
    float real;
    float btc;
    float eth;
    float xrp;
} Saldo;

typedef struct {
    ETipoTransacao tipo; // deposito = 1, saque = 2, compra = 3, venda = 4
    char data[TAM_DATA];
    float valor; // valor da transação
    float taxa; // valor da taxa
} Transacao;

typedef struct {
    char cpf[TAM_CPF];
    char senha[MAX_SENHA];
    char nome[TAM_NOME];
    Saldo carteira;
    Transacao historico[MAX_TRANSACOES];
    int qnt_transacoes;
} Usuario;

typedef struct {
    char sigla[TAM_SIGLA];
    float tx_venda;
    float tx_compra;
    float cotacao;
} Moeda;

typedef enum {DEPOSITO = 1, SAQUE = 2, COMPRA = 3, VENDA = 4} ETipoTransacao;
typedef enum {OK, FALHA} Resposta;

//Declaração de funções

void limpar_buffer(void);
Resposta verificar_arquivo(FILE *ponteiro);
Resposta verificar_cpf(char *entrada_cpf);
Resposta ler_usuarios(Usuario usuarios[], int *quantidade_lida);