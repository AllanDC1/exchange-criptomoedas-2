# Projeto 1-Exchange de Criptomoedas (2° Sem. CC FEI)
## Objetivo do Projeto
Projeto desenvolvido na linguagem C, por alunos no 2° semestre do curso de Ciência da Computação da FEI.
O intuito desse projeto é exercer uma simulação de um exchange de criptomoedas, esse sistema se comporta como um sistema de banco,
onde os usuários podem fazer diversas operações financeiras, como consultar saldo, realizar depósitos e saques e transações de compra e venda de criptomoedas.
Além de um sistema de registro, login e exclusão de usuários.
## Funcionalidades
### Menu inicial
1. Efetuar o login | Os usuários com uma conta registrada podem realizar login inserindo CPF e sua senha criada.
2. Registrar uma nova conta | Um novo usuário pode se cadastrar na plataforma informando CPF, uma senha e seu primeiro nome.
3. Exclusão de usuário | Um usuário pode ser excluído da plataforma, informando o CPF e fazendo uma confirmação.
0. Encerrar programa | Finaliza o programa.
### Menu de operações
1. Consultar saldo | Exibe o saldo de Reais, Bitcoin, Ethereum e Ripple, além dos dados cadastrais do usuário.
2. Consultar extrato | Exibe todas as transações feitas pelo usuário, com valor, data e taxa.
3. Depositar | Usuário faz um depósito em reais na sua conta informando o valor desejado.
4. Sacar | Usuário faz um saque em reais de sua carteira, informando um valor válido, e validando sua senha.
5. Comprar criptomoedas | Usuário informa o valor em reais e a sigla da criptomoeda que deseja comprar. Após validar a senha, é exibido o valor da compra com taxas inclusas para uma confirmação final.
6. Vender criptomoedas | Usuário informa o valor e a sigla da criptomoeda que deseja vender. Após validar a senha, é exibido o valor em reais da venda com taxas inclusas para uma confirmação final.
7. Atualizar cotação | A cotação de todas as criptomoedas são atualizadas com uma variação de no mínimo -5% e no máximo 5% do valor atual.
0. Sair | Volta para o menu inicial.
## Estrutura
'main.c': Arquivo que contém a lógica de execução do programa.
'functions.c': Arquivo que possui todas as funções executadas pelo programa.
'functions.h': Arquivo de cabeçalho onde são declaradas as funções, structs/enums e bibliotecas usadas pelos arquivos '.c'.
'dados-usuarios': Arquivo em binário são salvos os dados, saldos e transações dos usuários.
'moedas': Arquivo em binário com taxas de compra e venda e cotação das criptomoedas.
'exchange-criptomoedas.exe': Arquivo que faz a execução do programa.
## Compilação e Execução
