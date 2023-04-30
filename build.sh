#!/bin/bash

flex --version 2>/dev/null 
if [[ $? != 0 ]]; then
    sudo apt install flex
    echo "[INFO] Flex installed"
fi

bison --version 2>/dev/null
if [[ $? != 0 ]]; then
    sudo apt install bison
    echo "[INFO] Bison installed"
fi

lex frontend/lexer.l
echo "[INFO] Lexer successfully built"
bison -d frontend/parser.ypp 2>/dev/null
g++ -Wall -Wextra ast/ast.cpp lex.yy.c parser.tab.cpp -g -o exp
echo "[INFO] Parser successfully built"

exp () {
    if [[ "$1" == *.exp ]]; then
        ./exp $1 > "${2}.s"
        if [[ $? -eq 0 ]]; then
            echo "[INFO] AST tree successfully created"
        else 
            echo "[ERROR] Error while creating AST tree"
        fi
    else 
        echo "[ERROR] Wrong file extension; expected .exp at the end..."
    fi 
    
    if [[ -s "${2}.s" ]];
    then 
        gcc asm/asm_ops.c -m64 -fno-pie -no-pie "${2}.s" -g -o $2 2>/dev/null
        if [[ $? -ne 0 ]]; then
            echo "[ERROR] Compilation error"
        else 
            echo "[INFO] Successfull compilation"
        fi
    fi
    
}

remove () {
    rm -r *.tab.* *.yy.* exp
}
