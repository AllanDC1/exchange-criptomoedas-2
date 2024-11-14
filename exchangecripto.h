#ifndef EXCHANGECRIPTO_H
#define EXCHANGECRIPTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// DEFINICAO TAMANHO DOS DADOS
#define TAM_CPF 12 // 11 dígitos + caracter especial no final
#define MAX_SENHA 16 // Máximo 15 dígitos + caracter especial no final
#define TAM_NOME 31 // Maximo 30 letras + caracter especial no final
#define TAM_DATA 20 // 00/00/0000 00:00:00 + \0 = 20 caracteres
#define TAM_SIGLA 5 // 4 letras + \0, BTC ou DOGE por exemplo

// LIMITACOES DO SISTEMA
#define MAX_TRANSACOES 100
#define MAX_USUARIOS 10
#define MAX_CRIPTOMOEDAS 10 // quantas criptomoedas o sistema tiver
#define MAX_ADMS 5

typedef enum {OK, FALHA = -1} Resposta; // enum para melhor legibilidade


// STRUCTS PARA SALVAMENTO EM ARQUIVOS
typedef struct {
    char sigla[TAM_SIGLA];
    float saldo;
} SaldoCripto;

typedef struct {
    float real;
    SaldoCripto criptomoeda[MAX_CRIPTOMOEDAS];
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
} Criptomoeda;

typedef struct {
    int idx_usuario_atual;
    Resposta resultado;
} ResultadoLogin;

typedef struct {
    char cpf[TAM_CPF];
    char senha[MAX_SENHA];
    char nome[TAM_NOME];
} Admin;

//Declaração de funções

// FUNCOES GERAIS
void delay(int tempo_ms);
void upper(char* str);
void limpar_buffer();
void verificar_buffer(char *entrada);
void print_erro(const char *msg);
void print_titulo(const char *msg);
void print_sucesso(const char *msg, ...);
void print_confirmacao(const char *msg);
void voltar_menu();
char* gerar_data();
Resposta confirmar_acao();

FILE* abrir_arquivo(char* nome_arquivo, char* modo);
Resposta verificar_tamanho_arquivo(FILE *ponteiro);

Resposta ler_usuarios(Usuario array_usuarios[], int *quantidade_lida);
Resposta salvar_usuarios(Usuario array_usuarios[], int quantidade_usuarios);
Resposta ler_moedas(Criptomoeda array_moedas[], int *qnt_moedas);
Resposta salvar_moedas(Criptomoeda array_moedas[], int quantidade_moedas);

Resposta verificar_cpf(char *entrada_cpf);
Resposta verificar_senha(char *entrada_senha);
Resposta verificar_nome(char *entrada_nome);
Resposta validar_senha(Usuario *usuario_logado);

int checar_usuario(char *entrada_cpf, char* entrada_senha, Usuario array_usuarios[], int quantidade_usuarios);
int achar_usuario(Usuario array_usuarios[], int quantidade_usuarios, const char* cpf_usuario);

Resposta salvar_transacao(Usuario *usuario_logado, char* tipo, char* moeda, float valor, float taxa);

// FUNCOES MENUS

int escolha_operacao(int max);
//MENU INICIAL
void exibir_menu();
ResultadoLogin login_usuario();
Resposta registro_usuario();
Resposta excluir_usuario();
// MENU OPERACOES
void exibir_operacoes();
void consultar_saldo(Usuario *usuario_atual);
void consultar_extrato(Usuario *usuario_atual);
void depositar(Usuario *usuario_atual);
void sacar(Usuario *usuario_atual);
void comprar_criptomoeda(Usuario *usuario_atual);
void vender_criptomoeda(Usuario *usuario_atual);
void atualizar_cotacao();
void menu_operacoes(int idx_logado);

// FUNCOES ADM
void menu_adm();
Resposta ler_admins(Admin array_admins[], int *quantidade_lida);
Resposta registro_admin();
Resposta salvar_admins(Admin array_admins[], int qnt_adms);
Resposta excluir_adm();
Resposta login_adm();
int selecionar_investidor();
void operacoes_adm();
void criar_cripto();

#endif