#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define TAM_CPF 12 // 11 dígitos + caracter especial no final
#define MAX_SENHA 16 // Máximo 15 dígitos + caracter especial no final
#define TAM_NOME 21 // Primeiro nome, 20 letras + caracter especial no final
#define TAM_DATA 20 // 00/00/0000 00:00:00 + \0 = 20 caracteres
#define TAM_SIGLA 5 // 4 letras + \0, BTC ou DOGE por exemplo

#define MAX_TRANSACOES 100
#define MAX_USUARIOS 10 

typedef enum {DEPOSITO = 1, SAQUE = 2, COMPRA = 3, VENDA = 4} ETipoTransacao;
typedef enum {OK, FALHA = -1} Resposta;

typedef struct {
    float real;
    float btc;
    float eth;
    float xrp;
} Saldo;

typedef struct {
    char tipo[20]; // deposito, saque, compra, venda
    char data[TAM_DATA];
    char sigla_moeda[TAM_SIGLA];
    float valor; // valor da transação
    float taxa; // valor da taxa
} Transacao;

typedef struct {
    char cpf[TAM_CPF];
    char senha[MAX_SENHA];
    char nome[TAM_NOME];
    Saldo carteira;
    Transacao extrato[MAX_TRANSACOES];
    int qnt_transacoes;
} Usuario;

typedef struct {
    char sigla[TAM_SIGLA];
    float tx_venda;
    float tx_compra;
    float cotacao;
} Moeda;

typedef struct {
    int idx_usuario_atual;
    Resposta resultado;
} ResultadoLogin;

//Declaração de funções

void delay(int tempo_ms);
void limpar_buffer();
void verificar_buffer();
void voltar_menu();
void print_erro(const char *msg);

FILE* abrir_arquivo(char* nome_arquivo, char* modo);
Resposta verificar_tamanho_arquivo(FILE *ponteiro);
Resposta verificar_cpf(char *entrada_cpf);
Resposta verificar_senha(char *entrada_senha);
Resposta verificar_nome(char *entrada_nome);

int checar_usuario(char *entrada_cpf, char* entrada_senha, Usuario array_usuarios[], int quantidade_usuarios);
int achar_usuario(Usuario array_usuarios[], int quantidade_usuarios, Usuario *usuario_logado);

Resposta salvar_transacao(Usuario *usuario_logado, char* tipo, char* moeda, float valor, float taxa);

void exibir_menu();
int escolha_operacao(int max);
Resposta ler_usuarios(Usuario array_usuarios[], int *quantidade_lida);
Resposta salvar_usuarios(Usuario array_usuarios[], int quantidade_usuarios);
char* gerar_data();

ResultadoLogin login_usuario();
Resposta criar_usuario();
Resposta excluir_usuario();

void menu_operacoes(int idx_logado);
void consultar_saldo(Usuario *usuario_atual);
void consultar_extrato(Usuario *usuario_atual);
void depositar(Usuario *usuario_atual);
void sacar(Usuario *usuario_atual);

#endif