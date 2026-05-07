# MiniOver Compiler
A compiler for the MiniOver language.

## Features
- Lexical Analysis (Flex)
- Syntax Parsing (Bison)
- AST Generation
- Type Checking with Implicit Promotion (int -> float)
- Direct Interpretation (Execution)

## Building
1. bison -d miniover.y
2. flex miniover.lex
3. gcc miniover.tab.c lex.yy.c ast.c symbol_table.c type_checker.c interpreter.c -o compiler.exe
