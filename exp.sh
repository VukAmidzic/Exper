exp_run () {
    if [[ "$1" == *.exp ]]; then
        /bin/exp $1 > "${2}.s"
        if [[ $? == 0 ]]; then
            echo "[INFO] AST tree successfully created"
        else 
            echo "[ERROR] Error while creating AST tree"
        fi
    else 
        echo "[ERROR] Wrong file extension; expected .exp at the end..."
    fi 
    
    if [[ -s "${2}.s" ]];
    then 
        ASM_OPS= find /home -type f -name asm_ops.c 1>/dev/null
        gcc $ASM_OPS -m64 -fno-pie -no-pie "${2}.s" -g -o $2
        if [[ $? != 0 ]]; then
            echo "[ERROR] Compilation error"
        else 
            echo "[INFO] Successfull compilation"
        fi
    fi
}
