# Compiler for Pascal-Like Language

This project provides a build system for compiling a Pascal-like language using the Flex and Bison tools. It generates a `compilador` executable that can parse and compile programs written in this language.

## TO-DO
- Atribuir tipo da função na tabela de simb para comparar com operações
- Juntar lógica de procedimento e função em Subrotina, eh praticamente igual
- Implementar verificação de N parametros_reais = N parametros_formais, verificando se é variavel ou cópia corretamente
- Como permitir chamadas de subrotinas aninhadas sem resetar idx_param? f(f(3));
- Fazer stress test semana 13

## Prerequisites

Before using this Makefile, ensure that you have the following prerequisites installed on your system:

- g++: The C++ compiler.                (>= c++14)
- Flex: A lexical analyzer generator.   (>= 2.6.4)
- Bison: A parser generator.            (>= 3.8.2)

Flex and Bison can be installed using the command
```sh
sudo apt-get install flex bison libfl-dev libbison-dev
```

## Building the Compiler

To build the compiler, run the following command:

```bash
make
```

This command will compile the lexer and parser using Flex and Bison and then compile the C++ source code.

## Running the Compiler

To run the compiled compiler, use the following command:

```bash
make run
```

This will execute the `compilador` with the Pascal-like program specified inside the `Makefile` (usually a `tests/input/` file). You can replace this input file with your own to test the compiler.

## Running Tests

To run a set of predefined tests, use the following command:

```bash
make test
```

This will execute a shell script (`run_tests.sh`) located in the `tests` directory. You can add your own test cases by adding *.pas file inside `tests/input/` with the respective expected *.mepa output on `tests/expected_output` directory. 

## Cleaning Up

To clean up object files and generated files, use the following command:

```bash
make clean
```

This command will remove the generated object files, lexer, and parser files.
To remove all generated files, including the compiled `compilador` executable, use:

```bash
make clear
```

To execute the MEPA interpreter you can use 
```
./executaMepa -r -i output/output.mepa
```

or download the full interpreter here : https://www.inf.ufpr.br/bmuller/assets/ci1211/Recursos/InterprMEPA-2.0.tar.bz2

# Project Directory Structure


```
compiladores/
│
├── src/
│   ├── compilador.cpp          # Main entry point
│   ├── code_generation/
│   │   ├── Simbolo.cpp         # Source files for code generation
│   │   ├── TabelaSimbolos.cpp  
│   │   └── MepaInterface.cpp   
│   ├── lexer.l                 # Flex lexer specification
│   └── grammar.y               # Bison grammar specification
│
├── include/
│   └── code_generation/
│   |   ├── Simbolo.hpp         # Header files for code generation
│   |   ├── TabelaSimbolos.hpp  
│   |   └── MepaInterface.hpp   
│   └── compilador.hpp          # Header for main entry point
|
├── tests/
│   ├── input/                  # Test input files
│   │   └── *.pas               
│   ├── expected_output/        # Correct matching output files
|   |   └── *.mepa              
│   └── run_tests.sh            # Test script
│
├── output/
│   └── output.mepa             # Output MEPA file
│
├── Makefile                    # Your Makefile
├── README.md                   # This project documentation
└── .gitignore 
```
