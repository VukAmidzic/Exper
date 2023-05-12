#!/bin/bash

flex --version 2>/dev/null 
if [[ $? != 0 ]]; then
    sudo apt install flex
fi

bison --version 2>/dev/null
if [[ $? != 0 ]]; then
    sudo apt install bison
fi

lex frontend/lexer.l
echo "[INFO] Lexer successfully built"
bison -d frontend/parser.ypp 2>/dev/null
g++ -Wall -Wextra ast/ast.cpp lex.yy.c parser.tab.cpp -g -o exp
echo "[INFO] Parser successfully built"

sudo cp exp /bin
sudo cp exp.sh /bin
rm -r *.tab.* *.yy.* exp
