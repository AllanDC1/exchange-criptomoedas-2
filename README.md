# Projeto 1 e 3-Exchange de Criptomoedas (2° Sem. CC FEI)
## Objetivo do Projeto
- Projeto desenvolvido na linguagem C, por alunos no 2° semestre do curso de Ciência da Computação da FEI. <br>
- O intuito desse projeto é exercer uma simulação de um exchange de criptomoedas, esse sistema se comporta como um sistema de banco,
onde os investidores podem fazer diversas operações financeiras, como consultar saldo, realizar depósitos, saques e transações de compra e venda de criptomoedas. <br>
Além de um sistema de registro, login e exclusão de usuários. <br>
- O sistema também conta com um módulo para administradores, que poderão executar operações que gerenciam o sistema, como criar e excluir investidores e criptomoedas.

## Funcionalidades para Investidores
### Menu inicial
1. **Efetuar o login** | Os usuários com uma conta registrada podem realizar login inserindo seu CPF e senha.
2. **Registrar uma nova conta** | Um novo usuário pode se cadastrar na plataforma informando CPF, senha e nome.
3. **Exclusão de usuário** | Um usuário pode ser excluído da plataforma, informando o CPF e fazendo uma confirmação.
0. **Encerrar programa** | Finaliza o programa.

### Menu de operações
1. **Consultar saldo** | Exibe o saldo de Reais e das Criptomoedas, além dos dados cadastrais do usuário.
2. **Consultar extrato** | Exibe todas as transações feitas pelo usuário, com tipo, valor, data e taxa.
3. **Depositar** | Usuário faz um depósito em reais na sua conta informando o valor desejado.
4. **Sacar** | Usuário faz um saque em reais de sua carteira, informando um valor válido, e validando sua senha.
5. **Comprar criptomoedas** | Usuário informa a sigla da criptomoeda e o valor em reais que deseja comprar. <br> Após validar a senha, é exibido o valor da compra com taxas inclusas para uma confirmação final.
6. **Vender criptomoedas** | Usuário informa a sigla e o valor da criptomoeda que deseja vender. <br> Após validar a senha, é exibido o valor em reais da venda com taxas inclusas para uma confirmação final.
7. **Atualizar cotação** | A cotação das criptomoedas são atualizadas com uma variação de no mínimo -5% e no máximo 5% do valor atual.
0. **Sair** | Volta para o menu inicial.

## Funcionalidades para Administradores
### Menu inicial
1. **Login** | Os admins com uma conta registrada podem realizar login inserindo seu CPF e senha.
2. **Registrar um novo ADM** | Um novo admin pode se cadastrar na plataforma informando CPF, senha e nome.
3. **Exclusão de ADM** | Um admin pode ser excluído da plataforma, informando o CPF e fazendo uma confirmação.
0. **Encerrar programa** | Finaliza o programa.

### Menu de operações de ADMS
1. **Cadastrar investidor** | O admin pode inserir dados de uma nova conta de investidor (CPF, senha e nome).
2. **Excluir investidor** | Admin pode excluir a conta de um investidor inserindo o CPF da conta e confirmando.
3. **Cadastrar criptomoeda** | Para cadastrar uma criptomoeda no sistema um admin deve informar sigla, cotação inicial e as taxas de compra e venda da nova criptomoeda.
4. **Excluir criptomoeda** | O admin informa a sigla da criptomoeda que deseja excluir e faz uma confirmação.
5. **Consultar saldo de um investidor** | Admin informa CPF do investidor que deseja consultar, é exibido os dados do investidor junto com o saldo do mesmo.
6. **Consultar extrato de um investidor** | Admin informa CPF do investidor que deseja consultar o extrato, é exibido todas as transações já feitas pelo investidor escolhido.
7. **Atualizar cotação** | A cotação das criptomoedas são atualizadas com uma variação de no mínimo -5% e no máximo 5% do valor atual (Mesma funcionalidade dos investidores).
0. **Sair** | Volta para o menu inicial.

## Estrutura
- `main.c:` Arquivo que contém a lógica de execução do programa para investidores. <br>
- `adm.c:` Arquivo que contém a lógica de execução do programa para administradores. <br>
- `menu.c:` Arquivo que contém as lógicas e funções dos menus. <br>
- `arquivo.c:` Arquivo que contém as funções de manipulações dos arquivos. <br>
- `functions.c:` Arquivo que possui as funções gerais executadas pelos arquivos do programa. <br>
- `exchangecripto.h:` Arquivo de cabeçalho onde são declaradas as funções, structs/enums e bibliotecas usadas pelos arquivos `.c`. <br>
- `dados-usuarios:` Arquivo em binário onde são salvos os dados, saldos e transações dos usuários. <br>
- `dados-moedas:` Arquivo em binário com taxas de compra e venda e a cotação das criptomoedas. <br>
- `admins.bin:` Arquivo em binário com dados dos admins registrados. <br>
- `exchange-investidor(.exe):` Arquivo que faz a execução do programa de investidor.
- `exchange-admin(.exe):` Arquivo que faz a execução do programa de administrador.
## Compilação e Execução
*É necessário a instalação do [GCC](https://gcc.gnu.org/) e [CMake](https://cmake.org) no seu dispositivo para fazer a compilação.* <br>

### Com os programas instalados, siga as etapas no seu terminal: <br>
- Ir para o diretório build:
`cd build`
- Compilar os programas:
`make`
- Limpar os executáveis (caso necessário):
`make clean`
### Com os executáveis criados: <br>
- Executar programa de investidor:
`./exchange-investidor`
- Executar programa de administrador:
`./exchange-admin`

## Guia de uso
- Com o sistema executando no seu terminal, o menu incial já será exibido com as operações primárias. <br>
- No primeiro acesso, o passo inicial é o registro, seguido pelo login, assim você terá acesso ao menu principal de operações (dependendo se for investidor ou administrador) onde todas serão listadas para serem executadas de acordo com o número indicado. <br>
- Os arquivos binários de armazenamento de dados serão criados automaticamente conforme a necessidade do sistema.

## Integrantes
- Allan Donetti Calen R.A.: 22.124.002-1
- Luca Munhoz Rossi R.A.: 22.124.003-9
