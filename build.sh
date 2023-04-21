#!/bin/bash

lex_res= flex --version > /dev/null 
if [[ $lex_res -ne 0 ]]; then
    sudo apt install flex
    echo "[INFO] Flex installed"
fi

bison_res= bison --version > /dev/null
if [[ $bison_res -ne 0 ]]; then
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
    else 
        echo "[ERROR] Wrong file extension; expected .exp at the end..."
    fi 
    
    if [ -s "${2}.s" ];
    then 
        comp_res= gcc asm/asm_ops.c -m64 -fno-pie -no-pie "${2}.s" -g -o $2 2>/dev/null
        if [[ $comp_res -ne 0 ]]; then
            echo "[ERROR] Compilation error"
        else 
            echo "[INFO] Successfull compilation"
        fi
    fi
    
}

remove () {
    rm -r *.tab.* *.yy.* exp
}
