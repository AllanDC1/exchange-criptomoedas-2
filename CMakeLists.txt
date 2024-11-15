cmake_minimum_required(VERSION 3.10)

project(Projeto)

set(SRC_INVESTIDOR main.c menu.c arquivo.c functions.c)
set(SRC_ADMIN adm.c menu.c arquivo.c functions.c)

add_executable(exchange-investidor ${SRC_INVESTIDOR})
add_executable(exchange-admin ${SRC_ADMIN})
