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

lex FRONTEND/lexer.l
echo "[INFO] Lexer successfully built"
bison -d FRONTEND/parser.ypp
g++ -Wall -Wextra AST_defs_funcs/ast.cpp lex.yy.c parser.tab.cpp -g -o exp
echo "[INFO] Parser successfully built"

enable () {
    if [ $1 = "--var-check" ];
    then 
        sed -i "s/#define VARCHECK_ENABLE 0/#define VARCHECK_ENABLE 1/" FRONTEND/parser.ypp
        lex FRONTEND/lexer.l
        echo "[INFO] Lexer successfully built"
        bison -d FRONTEND/parser.ypp
        g++ -Wall -Wextra AST_defs_funcs/ast.cpp lex.yy.c parser.tab.cpp -g -o exp
        echo "[INFO] Parser successfully built"
    fi
}

disable () {
    if [ $1 = "--var-check" ];
    then 
        sed -i "s/#define VARCHECK_ENABLE 1/#define VARCHECK_ENABLE 0/" FRONTEND/parser.ypp
        lex FRONTEND/lexer.l
        echo "[INFO] Lexer successfully built"
        bison -d FRONTEND/parser.ypp
        g++ -Wall -Wextra AST_defs_funcs/ast.cpp lex.yy.c parser.tab.cpp -g -o exp
        echo "[INFO] Parser successfully built"
    fi
}

exp () {
    if [[ "$1" == *.exp ]]; then
        ./exp $1 > "${2}.s"
    else 
        echo "Wrong file extension; expected .exp at the end..."
    fi 
    
    if [ -s "${2}.s" ];
    then 
        gcc ASM_OPS/asm_ops.c -m64 -fno-pie -no-pie "${2}.s" -g -o $2
    fi
    
}

remove () {
    rm -r *.tab.* *.yy.* exp
}
