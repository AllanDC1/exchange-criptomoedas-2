#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define TAM_CPF 12 // 11 dígitos + caracter especial no final
#define MAX_SENHA 16 // Máximo 15 dígitos + caracter especial no final
#define TAM_NOME 21 // Primeiro nome, 20 letras + caracter especial no final
#define TAM_DATA 20 // 00/00/0000 00:00:00 + \0 = 20 caracteres
#define TAM_SIGLA 5 // 4 letras + \0, BTC ou DOGE por exemplo

#define MAX_TRANSACOES 100
#define MAX_USUARIOS 10 

typedef enum {DEPOSITO = 1, SAQUE = 2, COMPRA = 3, VENDA = 4} ETipoTransacao;
typedef enum {OK, FALHA} Resposta;

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

//Declaração de funções

void limpar_buffer();
void print_erro(const char *msg);
Resposta verificar_arquivo(FILE *ponteiro);
Resposta verificar_cpf(char *entrada_cpf);
Resposta verificar_senha(char *entrada_senha);
Resposta verificar_nome(char *entrada_nome);
Resposta ler_usuarios(Usuario array_usuarios[], int *quantidade_lida);
Resposta salvar_usuarios(Usuario array_usuarios[], int quantidade_usuarios);
void gerar_data(char* data);
void criar_usuario();
