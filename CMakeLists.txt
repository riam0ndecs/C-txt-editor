cmake_minimum_required(VERSION 3.10)
project(projeto_livros C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)

include_directories(include)

add_executable(gerador    src/gerador.c    src/livro.c src/arquivos.c)
add_executable(editor     src/editor.c     src/livro.c src/arquivos.c src/entrada.c)
add_executable(exportador src/exportador.c src/livro.c src/arquivos.c)