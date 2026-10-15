# Book Database: Text and Binary File I/O in C

A small C application made of **three programs** that move a collection of books between a human-readable text file and a compact binary file, with an interactive editor in between.

It was developed as an assignment for an *Imperative Programming* course (PBL07) and focuses on file handling in C: `fopen`/`fclose`, `fgets`/`sscanf`/`fprintf` for text files, and `fread`/`fwrite` for binary files.

> **Language note:** the user interface (menus, prompts, messages), the code comments, the identifiers (`Livro`, `base_inserir`, `texto_carregar`, ...) and the sample data file (`dados.txt`) are all in **Brazilian Portuguese (pt-BR)**. This README is in English for a wider audience. A glossary is provided [at the end](#glossary-pt-br--english).

## How it works

```
dados.txt ──[gerador]──► livros.bin ──[editor]──► novo.bin ──[exportador]──► saida.txt
  (text)                  (binary)                (binary)                    (text)
```

| Program | Reads | Does | Writes |
|---|---|---|---|
| `gerador` (generator) | text file | loads the records into memory | binary file |
| `editor` (editor) | binary file | interactive menu: insert, remove, search, list | new binary file |
| `exportador` (exporter) | binary file | loads the records into memory | text file, in the same format the generator reads |

## Data model

Each record is a book (`struct Livro`):

| Field | Type | Notes |
|---|---|---|
| `codigo` | `int` | **unique key** (identifier) |
| `titulo` | `char[50]` | title |
| `autor` | `char[40]` | author |
| `ano` | `int` | year of publication |
| `paginas` | `int` | number of pages |

The in-memory database (`struct Base`) is a fixed-size array of up to 1000 books plus a counter of how many are in use.

## Project layout

```
.
├── livro.h / livro.c           Livro and Base types; insert, remove, search, list
├── arquivos.h / arquivos.c     text <-> memory and binary <-> memory conversions
├── entrada.h / entrada.c       validated keyboard input (used by the editor)
├── gerador.c                   main() of the generator
├── editor.c                    main() of the editor
├── exportador.c                main() of the exporter
├── dados.txt                   sample data: 22 books (pt-BR)
└── Makefile                    (or CMakeLists.txt)
```

Each program has its own `main()`, so each one is linked from a different set of files:

| Program | Source files |
|---|---|
| `gerador` | `gerador.c`, `livro.c`, `arquivos.c` |
| `editor` | `editor.c`, `livro.c`, `arquivos.c`, `entrada.c` |
| `exportador` | `exportador.c`, `livro.c`, `arquivos.c` |

## Building

Requires a C99 compiler (GCC or MinGW, for example).

**With Make:**

```
make
```

**With CMake** (for example, in CLion), declare three executables, one per program, and build:

```
cmake -B build
cmake --build build
```

**Directly with GCC:**

```
gcc -Wall -Wextra -std=c99 -o gerador    gerador.c    livro.c arquivos.c
gcc -Wall -Wextra -std=c99 -o editor     editor.c     livro.c arquivos.c entrada.c
gcc -Wall -Wextra -std=c99 -o exportador exportador.c livro.c arquivos.c
```

## Usage

File names are passed as command-line arguments (`argc`/`argv`):

```
./gerador    <text_input>    <binary_output>
./editor     <binary_input>  <binary_output>
./exportador <binary_input>  <text_output>
```

A complete run with the sample data (on Windows, use `gerador.exe` etc.):

```
./gerador dados.txt livros.bin
./editor livros.bin novo.bin
./exportador novo.bin saida.txt
```

> Run the programs from the folder that contains `dados.txt`; the file names are relative to the current working directory.

### The text file format

One book per line, fields separated by semicolons:

```
codigo;titulo;autor;ano;paginas
```

Example (from `dados.txt`):

```
1;Ensaio sobre a cegueira;José Saramago;1995;310
2;Cem sonetos de amor;Pablo Neruda;1959;128
```

- There is **no record count** in the file; the end of file is detected while reading (`fgets` returning `NULL`).
- The file must be saved as **UTF-8** (the sample data contains accented characters).
- The sample file has 22 records (the assignment requires at least 20).
- Blank lines are skipped. Malformed lines and lines with a duplicate `codigo` are ignored with a warning on `stderr`.
- Titles and authors cannot contain `;`, since it is the field separator.

### The editor menu

```
===== EDITOR DE LIVROS =====
1 - Inserir livro                      (insert a book)
2 - Remover livro                      (remove a book)
3 - Buscar livro por código            (search a book by code)
4 - Listar todos                       (list all books)
5 - Salvar arquivo binário agora       (save the binary file now)
0 - Sair                               (exit)
```

- **Insert** asks for the code first and rejects it immediately if it already exists.
- **Remove** shows the book and asks for confirmation (`s` = yes, `n` = no) before deleting.
- **Save** writes the output binary file on demand.
- On **exit**, the editor saves automatically *only if* there are changes that were not saved yet.
- All input is validated: non-numeric text, out-of-range numbers, empty fields, over-long text and `;` are rejected, and the question is asked again.

## Implementation notes

- **Text input:** each line is read with `fgets` and split with `sscanf` using `"%d;%49[^;];%39[^;];%d;%d"`. The field widths (49 and 39) are the array sizes minus one, which prevents buffer overflows.
- **Text output:** `fprintf` with the mirrored format `"%d;%s;%s;%d;%d\n"`, so a text file exported by `exportador` can be read again by `gerador`.
- **Binary output:** the whole array is written at once with `fwrite(base->itens, sizeof(Livro), quantidade, arq)`.
- **Binary input:** read in blocks of 8 records with `fread`; a short read means end of file.
- **Unique key:** enforced in a single place (`base_inserir`), so every path that adds data (text loading, binary loading, editor) respects it.
- **Removal:** the following records are shifted one position to the left, preserving order.
- **Safe keyboard input:** `fgets` + `strtol` instead of `scanf`, which avoids leftover characters in the input buffer.
- **Dirty flag:** the editor tracks whether the in-memory data differs from the last saved file.
- Every `fopen`/`fclose`/`fwrite` result is checked, and errors are reported with `perror` or `stderr`.

## Limitations

- The binary file is a raw dump of the `Livro` struct, so it depends on the platform and compiler (size of `int`, struct padding; typically 104 bytes per record). Use it with programs compiled the same way, not as an interchange format. The text file is the portable one.
- The database holds at most 1000 books (`MAX_LIVROS` in `livro.h`).
- Lookup is a linear search, which is fine for this size.
- Because of UTF-8, the 49-character title limit and the 39-character author limit are counted in bytes, so accented characters use more than one.

## Verifying the round trip

Generating a binary file and exporting it again should reproduce the original text file:

```
./gerador dados.txt livros.bin
./exportador livros.bin saida.txt
diff dados.txt saida.txt
```

On Windows PowerShell, use `Compare-Object (Get-Content dados.txt) (Get-Content saida.txt)`. No output means the files have the same content. The size of `livros.bin` should be `number of books × sizeof(Livro)` (22 × 104 = 2288 bytes with the sample data on a typical platform).

## Glossary (pt-BR → English)

| Portuguese | English |
|---|---|
| `Livro` | book |
| `Base` | database (the in-memory collection) |
| `codigo` | code (unique key) |
| `titulo` | title |
| `autor` | author |
| `ano` | year |
| `paginas` | pages |
| `quantidade` | quantity / count |
| `gerador` | generator |
| `exportador` | exporter |
| `arquivo` / `arq` | file |
| `texto_carregar`, `texto_salvar` | load from / save to text file |
| `binario_carregar`, `binario_salvar` | load from / save to binary file |
| `base_inserir`, `base_remover`, `base_buscar`, `base_listar` | insert, remove, search, list |
| `alterado` | modified (unsaved-changes flag) |
| `ler_inteiro`, `ler_texto`, `ler_sim_nao` | read integer, read text, read yes/no |
| `s` / `n` | `s` = *sim* (yes), `n` = *não* (no) |
