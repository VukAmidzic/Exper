#include "ast.hpp"
#include <iostream>
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <type_traits>
#include <string>
#include <map>
#include <tuple>
#include <vector>
#include <utility>

const int VAR_STEP = 4;
bool first_scan = true;

NumNode::NumNode(int _line_index, int _num) { line_index = _line_index; num = _num; };

VarNode::VarNode(int _line_index, std::string _var_name) { line_index = _line_index; var_name = _var_name; };

BinaryNode::BinaryNode(int _line_index, Tag _tag, ASTNode* _left, ASTNode* _right) {
    line_index = _line_index;
    tag = _tag;
    left = _left;
    right = _right;
};

MainNode::MainNode(int _line_index, ASTNode* _next) {
    line_index = _line_index;
    next = _next;
};

AssignNode::AssignNode(int _line_index, std::string _var_name, ASTNode* _assign_val, ASTNode* _next) {
    line_index = _line_index;
    var_name = _var_name;
    assign_val = _assign_val;
    next = _next;
};

ArrayAssignNode::ArrayAssignNode(int _line_index, std::vector<AssignNode*> _assigns, ASTNode* _next) {
    line_index = _line_index;
    assigns = _assigns;
    next = _next;
};

PrintNode::PrintNode(int _line_index, ASTNode* _print_val, ASTNode* _next) {
    line_index = _line_index;
    print_val = _print_val;
    next = _next;
};

ScanNode::ScanNode(int _line_index, std::string _var_name, ASTNode* _next) {
    line_index = _line_index;
    var_name = _var_name;
    next = _next;
};

IfElseNode::IfElseNode(int _line_index, int _if_num, int _main_num, std::vector<std::pair<ASTNode*, ASTNode*>> _conds, std::vector<int> _cond_num, ASTNode* _next) {
    line_index = _line_index;
    if_num = _if_num;
    main_num = _main_num;
    conds = _conds;
    cond_num = _cond_num;
    next = _next;
};

WhileNode::WhileNode(int _line_index, int _while_num, int _main_num, ASTNode* _cond, ASTNode* _stmts, ASTNode* _next) {
    line_index = _line_index;
    while_num = _while_num;
    main_num = _main_num;
    cond = _cond; 
    stmts = _stmts; 
    next = _next;
};

void traverse_tree(ASTNode* ptr, std::map<std::string, std::pair<int, int>>& mp, int* var_counter, 
    int* loop_counter, int* if_counter, int* cond_counter, int* main_counter) {
    auto* num_node = dynamic_cast<NumNode*>(ptr);
    auto* var_node = dynamic_cast<VarNode*>(ptr);
    auto* bin_op_node = dynamic_cast<BinaryNode*>(ptr);
    auto* main_node = dynamic_cast<MainNode*>(ptr);
    auto* assign_node = dynamic_cast<AssignNode*>(ptr);
    auto* arr_assign_node = dynamic_cast<ArrayAssignNode*>(ptr);
    auto* print_node = dynamic_cast<PrintNode*>(ptr);
    auto* scan_node = dynamic_cast<ScanNode*>(ptr);
    auto* if_else_node = dynamic_cast<IfElseNode*>(ptr);
    auto* while_node = dynamic_cast<WhileNode*>(ptr);
    
    if (num_node) { return; }
    else if (var_node) { return; }
    else if (bin_op_node) {
        traverse_tree(bin_op_node->left, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
        traverse_tree(bin_op_node->right, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
    }
    else if (main_node) {
        traverse_tree(main_node->next, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
    }
    else if (assign_node) {
        int tmp = expr_eval(assign_node->assign_val, mp);
        auto it = mp.find(assign_node->var_name);
        if (it != mp.end()) {
            mp[assign_node->var_name].first = tmp;
        }
        else {
            mp[assign_node->var_name] = {tmp, *var_counter};
            *var_counter += VAR_STEP;
        }
        
        traverse_tree(assign_node->next, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
    }
    else if (arr_assign_node) {
        for (auto it : arr_assign_node->assigns) {
            traverse_tree(it, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
        }
        traverse_tree(arr_assign_node->next, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
    }
    else if (print_node) {
        traverse_tree(print_node->next, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
    }
    else if (scan_node) {
        traverse_tree(scan_node->next, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
    }
    else if (if_else_node) {
        int n = if_else_node->conds.size();
        
        if (n == 1) {
            if_else_node->if_num = *if_counter;
            if_else_node->main_num = *main_counter;
            if_else_node->cond_num.push_back(*cond_counter);
            
            *if_counter += 1;
            *main_counter += 1;
            *cond_counter += 1;
            
            /*std::cout << "#IF-ELSE:" << if_else_node->if_num << std::endl;
            std::cout << "#CONDS: ";
            for (int i : if_else_node->cond_num) {
                std::cout << "<cond," << i << ">, ";
            }
            std::cout << std::endl;
            */
            
            traverse_tree(if_else_node->conds[0].first, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
            traverse_tree(if_else_node->conds[0].second, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
            traverse_tree(if_else_node->next, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
        }
        else {
            if_else_node->if_num = *if_counter;
            if_else_node->main_num = *main_counter;
            for (int i = 1; i < n; ++i) {
                if_else_node->cond_num.push_back(*cond_counter + i - 1);
                traverse_tree(if_else_node->conds[i].first, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
            }
            if_else_node->cond_num.push_back(*cond_counter + n - 1);
            *cond_counter += n;
            *main_counter += n;
            
            for (int i = 1; i < n; ++i) {
                *if_counter += 1;
                traverse_tree(if_else_node->conds[i].second, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
            }
            traverse_tree(if_else_node->conds[0].second, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
            
            /*std::cout << "#IF-ELSE:" << if_else_node->if_num << std::endl;
            std::cout << "#CONDS: ";
            for (int i : if_else_node->cond_num) {
                std::cout << "<cond," << i << ">, ";
            }
            std::cout << std::endl;
            */
            
            traverse_tree(if_else_node->next, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
        }
    }
    else if (while_node) {
        while_node->while_num = *loop_counter;
        while_node->main_num = *main_counter;
        *loop_counter += 1;    
        *main_counter += 1;
            
        //std::cout << "#LOOP: " << while_node->while_num << std::endl;
        
        traverse_tree(while_node->cond, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
        traverse_tree(while_node->stmts, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
        traverse_tree(while_node->next, mp, var_counter, loop_counter, if_counter, cond_counter, main_counter);
    }
    else { return; }
};

int expr_eval(ASTNode* ptr, std::map<std::string, std::pair<int, int>>& mp) {
    int result = 0;
    
    auto* num_node = dynamic_cast<NumNode*>(ptr);
    auto* var_node = dynamic_cast<VarNode*>(ptr);
    auto* bin_op_node = dynamic_cast<BinaryNode*>(ptr);
    
    if (num_node) { result = num_node->num; }
    else if (var_node) {
        auto found = mp.find(var_node->var_name);
        if (found == mp.end()) { result = 0; }
        else { result = (found->second).first; }
    }
    else if (bin_op_node) {
        switch (bin_op_node->tag) {
            case _ADD_ : {
                result = expr_eval(bin_op_node->left, mp) + expr_eval(bin_op_node->right, mp);
                break;
            }
            case _SUB_ : {
                result = expr_eval(bin_op_node->left, mp) - expr_eval(bin_op_node->right, mp);
                break;
            }
            case _MUL_ : {
                result = expr_eval(bin_op_node->left, mp) * expr_eval(bin_op_node->right, mp);
                break;
            }
            case _DIV_ : {
                result = expr_eval(bin_op_node->left, mp) / expr_eval(bin_op_node->right, mp);
                break;
            }
            case _SHL_ : {
                result = expr_eval(bin_op_node->left, mp) << expr_eval(bin_op_node->right, mp);
                break;
            }
            case _SHR_ : {
                result = expr_eval(bin_op_node->left, mp) >> expr_eval(bin_op_node->right, mp);
                break;
            }
            case _AND_ : {
                result = expr_eval(bin_op_node->left, mp) && expr_eval(bin_op_node->right, mp);
                break;
            }
            case _OR_ : {
                result = expr_eval(bin_op_node->left, mp) || expr_eval(bin_op_node->right, mp);
                break;
            }
            case _NOT_ : {
                result = !expr_eval(bin_op_node->right, mp);
                break;
            }
            case _LESS_ : {
                result = expr_eval(bin_op_node->left, mp) < expr_eval(bin_op_node->right, mp);
                break;
            }
            case _GREAT_ : {
                result = expr_eval(bin_op_node->left, mp) > expr_eval(bin_op_node->right, mp);
                break;
            }
            case _EQ_ : {
                result = expr_eval(bin_op_node->left, mp) == expr_eval(bin_op_node->right, mp);
                break;
            }
            case _NEQ_ : {
                result = expr_eval(bin_op_node->left, mp) != expr_eval(bin_op_node->right, mp);
                break;
            }
            case _LEQ_ : {
                result = expr_eval(bin_op_node->left, mp) <= expr_eval(bin_op_node->right, mp);
                break;
            }
            case _GEQ_ : {
                result = expr_eval(bin_op_node->left, mp) >= expr_eval(bin_op_node->right, mp);
                break;
            }
            case _NEG_ : {
                result = -expr_eval(bin_op_node->right, mp);
                break;
            }
            default: break;
        }
    }
    
    return result;
};

std::tuple<std::string, bool, int> var_checker(ASTNode* ptr, std::map<std::string, std::pair<int, int>>& mp) {
    auto* num_node = dynamic_cast<NumNode*>(ptr);
    auto* var_node = dynamic_cast<VarNode*>(ptr);
    auto* bin_op_node = dynamic_cast<BinaryNode*>(ptr);
    auto* main_node = dynamic_cast<MainNode*>(ptr);
    auto* assign_node = dynamic_cast<AssignNode*>(ptr);
    auto* arr_assign_node = dynamic_cast<ArrayAssignNode*>(ptr);
    auto* print_node = dynamic_cast<PrintNode*>(ptr);
    auto* scan_node = dynamic_cast<ScanNode*>(ptr);
    auto* if_else_node = dynamic_cast<IfElseNode*>(ptr);
    auto* while_node = dynamic_cast<WhileNode*>(ptr);
    
    if (num_node) { return {"", true, -1}; }
    else if (var_node) {
        if (mp.find(var_node->var_name) == mp.end()) return {var_node->var_name, false, var_node->line_index};
        else return {"", true, -1};
    }
    else if (bin_op_node) {
        std::tuple<std::string, bool, int> lhs = var_checker(bin_op_node->left, mp);
        std::tuple<std::string, bool, int> rhs = var_checker(bin_op_node->right, mp);
                
        if (!std::get<1>(lhs)) return {std::get<0>(lhs), false, std::get<2>(lhs)};
        else if (!std::get<1>(rhs)) return {std::get<0>(rhs), false, std::get<2>(rhs)};
        else return {"", true, -1};
    }
    else if (main_node) { return var_checker(main_node->next, mp); }
    else if (assign_node) {
        std::tuple<std::string, bool, int> val = var_checker(assign_node->assign_val, mp);
        std::tuple<std::string, bool, int> next = var_checker(assign_node->next, mp);
        
        if (!std::get<1>(val)) return {std::get<0>(val), false, std::get<2>(val)};
        else if (!std::get<1>(next)) return {std::get<0>(next), false, std::get<2>(next)};
        else return {"", true, -1};
    }
    else if (arr_assign_node) {
        std::tuple<std::string, bool, int> next = var_checker(arr_assign_node->next, mp);
        if (!std::get<1>(next)) return {std::get<0>(next), false, std::get<2>(next)};
        
        std::tuple<std::string, bool, int> tmp;
        for (auto it : arr_assign_node->assigns) {
            tmp = var_checker(it, mp);
            if (!std::get<1>(tmp)) return {std::get<0>(tmp), false, std::get<2>(tmp)};
        }
        
        return {"", true, -1};
    }
    else if (print_node) {
        std::tuple<std::string, bool, int> val = var_checker(print_node->print_val, mp);
        std::tuple<std::string, bool, int> next = var_checker(print_node->next, mp);
        
        if (!std::get<1>(val)) return {std::get<0>(val), false, std::get<2>(val)};
        else if (!std::get<1>(next)) return {std::get<0>(next), false, std::get<2>(next)};
        else return {"", true, -1};
    }
    else if (scan_node) {
        std::tuple<std::string, bool, int> next = var_checker(scan_node->next, mp);
        if (!std::get<1>(next)) return {std::get<0>(next), false, std::get<2>(next)};
        else if (mp.find(scan_node->var_name) == mp.end()) return {scan_node->var_name, false, scan_node->line_index};
        else return {"", true, -1};
    }
    else if (if_else_node) {
        std::tuple<std::string, bool, int> next = var_checker(if_else_node->next, mp);
        if (!std::get<1>(next)) return {std::get<0>(next), false, std::get<2>(next)};
        
        int n = if_else_node->conds.size();
        
        if (n == 1) { return var_checker(if_else_node->conds[0].second, mp); }
        else {
            std::tuple<std::string, bool, int> tmp;
            for (int i = 1; i < n; ++i) {
                tmp = var_checker(if_else_node->conds[i].first, mp);
                if (!std::get<1>(tmp)) return {std::get<0>(tmp), false, std::get<2>(tmp)};
                tmp = var_checker(if_else_node->conds[i].second, mp);
                if (!std::get<1>(tmp)) return {std::get<0>(tmp), false, std::get<2>(tmp)};
            }
            
            return var_checker(if_else_node->conds[0].second, mp);
        }
    }
    else if (while_node) {
        std::tuple<std::string, bool, int> next = var_checker(while_node->next, mp);
        if (!std::get<1>(next)) return {std::get<0>(next), false, std::get<2>(next)};
        
        std::tuple<std::string, bool, int> cond = var_checker(while_node->cond, mp);
        if (!std::get<1>(cond)) return {std::get<0>(cond), false, std::get<2>(cond)};
        
        return var_checker(while_node->stmts, mp);
    }
    
    return {"", true, -1};
};

int get_first_divby_16(int n) {
    int factor = 1;
    
    while (16 * factor <= n) factor++;
    
    return 16*factor;
}

void print_asm(ASTNode* ptr, std::map<std::string, std::pair<int, int>>& mp) {
    auto* num_node = dynamic_cast<NumNode*>(ptr);
    auto* var_node = dynamic_cast<VarNode*>(ptr);
    auto* bin_op_node = dynamic_cast<BinaryNode*>(ptr);
    auto* main_node = dynamic_cast<MainNode*>(ptr);
    auto* assign_node = dynamic_cast<AssignNode*>(ptr);
    auto* arr_assign_node = dynamic_cast<ArrayAssignNode*>(ptr);
    auto* print_node = dynamic_cast<PrintNode*>(ptr);
    auto* scan_node = dynamic_cast<ScanNode*>(ptr);
    auto* if_else_node = dynamic_cast<IfElseNode*>(ptr);
    auto* while_node = dynamic_cast<WhileNode*>(ptr);
    
    if (num_node) {
        std::cout << "  mov rax, " << num_node->num << std::endl;
    }
    else if (var_node) {
        std::cout << "  mov rax, QWORD PTR [rbp-" << 2 * mp[var_node->var_name].second << "]" << std::endl;
    }
    else if (bin_op_node) {
        switch (bin_op_node->tag) {
            case _ADD_ : {
                print_asm(bin_op_node->left, mp);
                std::cout << "  push rax" << std::endl;
                print_asm(bin_op_node->right, mp);
                std::cout << "  pop rbx" << std::endl;
                std::cout << "  add rax, rbx" << std::endl;
                break;
            }
            case _SUB_ : {
                print_asm(bin_op_node->left, mp);
                std::cout << "  push rax" << std::endl;
                print_asm(bin_op_node->right, mp);
                std::cout << "  push rax" << std::endl;
                std::cout << "  pop rbx" << std::endl;
                std::cout << "  pop rax" << std::endl;
                std::cout << "  sub rax, rbx" << std::endl;
                break;
            }
            case _MUL_ : {
                print_asm(bin_op_node->left, mp);
                std::cout << "  push rax" << std::endl;
                print_asm(bin_op_node->right, mp);
                std::cout << "  pop rbx" << std::endl;
                std::cout << "  imul rax, rbx" << std::endl;
                break;
            }
            case _DIV_ : {
                print_asm(bin_op_node->left, mp);
                std::cout << "  push rax" << std::endl;
                print_asm(bin_op_node->right, mp);
                std::cout << "  pop rbx" << std::endl;
                std::cout << "  push rax" << std::endl;
                std::cout << "  mov rax, rbx" << std::endl;
                std::cout << "  pop rbx" << std::endl;
                std::cout << "  cqo" << std::endl;
                std::cout << "  xor rdx, rdx" << std::endl;
                std::cout << "  idiv rbx" << std::endl;
                break;
            }
            case _MOD_ : {
                print_asm(bin_op_node->left, mp);
                std::cout << "  push rax" << std::endl;
                print_asm(bin_op_node->right, mp);
                std::cout << "  pop rbx" << std::endl;
                std::cout << "  push rax" << std::endl;
                std::cout << "  mov rax, rbx" << std::endl;
                std::cout << "  pop rbx" << std::endl;
                std::cout << "  cqo" << std::endl;
                std::cout << "  xor rdx, rdx" << std::endl;
                std::cout << "  idiv rbx" << std::endl;
                std::cout << "  mov rax, rdx" << std::endl;
                break;
            }
            case _SHL_ : {
                print_asm(bin_op_node->left, mp);
                std::cout << "  shl rax, " << expr_eval(bin_op_node->right, mp) << std::endl;
                break;
            }
            case _SHR_ : {
                print_asm(bin_op_node->left, mp);
                std::cout << "  shr rax, " << expr_eval(bin_op_node->right, mp) << std::endl;
                break;
            }
            case _AND_ : {
                print_asm(bin_op_node->left, mp);
                std::cout << "  push rax" << std::endl;
                print_asm(bin_op_node->right, mp);
                std::cout << "  pop rbx" << std::endl;
                std::cout << "  and rax, rbx" << std::endl;
                break;
            }
            case _OR_ : {
                print_asm(bin_op_node->left, mp);
                std::cout << "  push rax" << std::endl;
                print_asm(bin_op_node->right, mp);
                std::cout << "  pop rbx" << std::endl;
                std::cout << "  or rax, rbx" << std::endl;
                break;
            }
            case _NOT_ : {
                print_asm(bin_op_node->right, mp);
                std::cout << "  not rax" << std::endl;
                break;
            }
            case _LESS_ : {
                print_asm(bin_op_node->left, mp);
                std::cout << "  push rax" << std::endl;
                print_asm(bin_op_node->right, mp);
                std::cout << "  push rax" << std::endl;
                std::cout << "  pop rbx" << std::endl;
                std::cout << "  pop rax" << std::endl;
                std::cout << "  mov rdi, rax" << std::endl;
                std::cout << "  mov rsi, rbx" << std::endl;
                std::cout << "  call cmp_less" << std::endl;  
                break;
            }
            case _GREAT_ : {
                print_asm(bin_op_node->left, mp);
                std::cout << "  push rax" << std::endl;
                print_asm(bin_op_node->right, mp);
                std::cout << "  push rax" << std::endl;
                std::cout << "  pop rbx" << std::endl;
                std::cout << "  pop rax" << std::endl;
                std::cout << "  mov rdi, rax" << std::endl;
                std::cout << "  mov rsi, rbx" << std::endl;
                std::cout << "  call cmp_great" << std::endl; 
                break;
            }
            case _EQ_ : {
                print_asm(bin_op_node->left, mp);
                std::cout << "  push rax" << std::endl;
                print_asm(bin_op_node->right, mp);
                std::cout << "  push rax" << std::endl;
                std::cout << "  pop rbx" << std::endl;
                std::cout << "  pop rax" << std::endl;
                std::cout << "  mov rdi, rax" << std::endl;
                std::cout << "  mov rsi, rbx" << std::endl;
                std::cout << "  call cmp_eq" << std::endl; 
                break;
            }
            case _NEQ_ : {
                print_asm(bin_op_node->left, mp);
                std::cout << "  push rax" << std::endl;
                print_asm(bin_op_node->right, mp);
                std::cout << "  push rax" << std::endl;
                std::cout << "  pop rbx" << std::endl;
                std::cout << "  pop rax" << std::endl;
                std::cout << "  mov rdi, rax" << std::endl;
                std::cout << "  mov rsi, rbx" << std::endl;
                std::cout << "  call cmp_neq" << std::endl; 
                break;
            }
            case _LEQ_ : {
                print_asm(bin_op_node->left, mp);
                std::cout << "  push rax" << std::endl;
                print_asm(bin_op_node->right, mp);
                std::cout << "  push rax" << std::endl;
                std::cout << "  pop rbx" << std::endl;
                std::cout << "  pop rax" << std::endl;
                std::cout << "  mov rdi, rax" << std::endl;
                std::cout << "  mov rsi, rbx" << std::endl;
                std::cout << "  call cmp_leq" << std::endl; 
                break;
            }
            case _GEQ_ : {
                print_asm(bin_op_node->left, mp);
                std::cout << "  push rax" << std::endl;
                print_asm(bin_op_node->right, mp);
                std::cout << "  push rax" << std::endl;
                std::cout << "  pop rbx" << std::endl;
                std::cout << "  pop rax" << std::endl;
                std::cout << "  mov rdi, rax" << std::endl;
                std::cout << "  mov rsi, rbx" << std::endl;
                std::cout << "  call cmp_geq" << std::endl; 
                break;
            }
            case _NEG_ : {
                print_asm(bin_op_node->right, mp);
                std::cout << "  mov r8, -1" << std::endl;
                std::cout << "  mul r8" << std::endl;
                break;
            }
            default: break;
        }
    }
    else if (main_node) {
        std::cout << ".intel_syntax noprefix" << std::endl;
        std::cout << ".text\n" << std::endl;
        std::cout << ".global main" << std::endl;
        std::cout << "main:" << std::endl;
        std::cout << "  enter " << get_first_divby_16((int)8*mp.size()) << ", 0" << std::endl;
        print_asm(main_node->next, mp);
        std::cout << "  leave" << std::endl;
        std::cout << "  ret\n\n" << std::endl;
        
        std::cout << ".data" << std::endl;
        std::cout << "  print_format: .asciz \"\%ld\\n\"" << std::endl;
        std::cout << "  scan_format: .asciz \"\%ld\"" << std::endl;
    }
    else if (assign_node) {
        print_asm(assign_node->assign_val, mp);
        std::cout << "  mov QWORD PTR [rbp-" << 2 * mp[assign_node->var_name].second << "], rax" << std::endl;
        print_asm(assign_node->next, mp);
    }
    else if (arr_assign_node) {
        for (auto it : arr_assign_node->assigns) {
            print_asm(it, mp);
        }
        print_asm(arr_assign_node->next, mp);
    }
    else if (print_node) {
        print_asm(print_node->print_val, mp);
        std::cout << "  lea rdi, print_format" << std::endl;
        std::cout << "  mov rsi, rax" << std::endl;
        std::cout << "  xor rax, rax" << std::endl;
        std::cout << "  call printf" << std::endl;
        print_asm(print_node->next, mp);
    }
    else if (scan_node) {
        std::cout << "  lea rdi, scan_format" << std::endl;
        std::cout << "  lea rsi, [rbp-" << 2 * mp[scan_node->var_name].second << "]" << std::endl;
        std::cout << "  xor rax, rax" << std::endl;
        std::cout << "  call scanf" << std::endl;
        print_asm(scan_node->next, mp);
    }
    else if (if_else_node) {
        auto* next_if_else = dynamic_cast<IfElseNode*>(if_else_node->next);
        auto* next_while = dynamic_cast<WhileNode*>(if_else_node->next);
        int n = if_else_node->conds.size();
        
        if (n == 1) {
            std::cout << "if" << if_else_node->if_num << ":" << std::endl;
            print_asm(if_else_node->conds[0].first, mp);
            std::cout << "  cmp rax, 1" << std::endl;
            std::cout << "  je cond" << if_else_node->cond_num[0] << std::endl;
            
            if (next_if_else) {
                std::cout << "  jmp if" << next_if_else->if_num << std::endl;
            }
            else if (next_while) {
                std::cout << "  jmp loop" << next_while->while_num << std::endl;
            }
            else {
                std::cout << "  jmp main" << if_else_node->main_num << std::endl;
            }
            
            std::cout << "cond" << if_else_node->cond_num[0] << ":" << std::endl;
            print_asm(if_else_node->conds[0].second, mp);
                
            if (next_if_else) {
                std::cout << "  jmp if" << next_if_else->if_num << std::endl; 
            }
            else if (next_while) {
                std::cout << "  jmp loop" << next_while->while_num << std::endl;
            }
            else {
                std::cout << "  jmp main" << if_else_node->main_num << std::endl;
            }
            
            if (!(next_if_else || next_while)) {
                std::cout << "main" << if_else_node->main_num << ":" << std::endl;
            }
            
            print_asm(if_else_node->next, mp);
        }
        else {
            std::cout << "if" << if_else_node->if_num << ":" << std::endl;
            int i;
            for (i = 1; i < n; ++i) {
                print_asm(if_else_node->conds[i].first, mp);
                std::cout << "  cmp rax, 1" << std::endl;
                std::cout << "  je cond" << if_else_node->cond_num[i-1] << std::endl;
            }
            std::cout << "  jmp cond" << if_else_node->cond_num[i-1] << std::endl;
            
            for (i = 1; i < n; ++i) {
                std::cout << "cond" << if_else_node->cond_num[i-1] << ":" << std::endl;
                print_asm(if_else_node->conds[i].second, mp);
                
                if (next_if_else) {
                    std::cout << "  jmp if" << next_if_else->if_num << std::endl; 
                }
                else if (next_while) {
                    std::cout << "  jmp loop" << next_while->while_num << std::endl;
                }
                else {
                    std::cout << "  jmp main" << if_else_node->main_num << std::endl;
                }
            }
            std::cout << "cond" << if_else_node->cond_num[i-1] << ":" << std::endl;
            print_asm(if_else_node->conds[0].second, mp);
                
            if (next_if_else) {
                std::cout << "  jmp if" << next_if_else->if_num << std::endl; 
            }
            else if (next_while) {
                std::cout << "  jmp loop" << next_while->while_num << std::endl;
            }
            else {
                std::cout << "  jmp main" << if_else_node->main_num << std::endl;
                std::cout << "main" << if_else_node->main_num << ":" << std::endl;
            }
            print_asm(if_else_node->next, mp);
        }
        
    }
    else if (while_node) {
        auto* next_while = dynamic_cast<WhileNode*>(while_node->next);
        auto* next_if_else = dynamic_cast<IfElseNode*>(while_node->next);
        
        std::cout << "loop" << while_node->while_num << ":" << std::endl;
        print_asm(while_node->cond, mp);
        std::cout << "  cmp rax, 0" << std::endl;
        if (next_while) {
            std::cout << "  je loop" << next_while->while_num << std::endl;
        }
        else if (next_if_else) {
            std::cout << "  je loop" << next_if_else->if_num << std::endl;
        }
        else {
            std::cout << "  je main" << while_node->main_num << std::endl;
        }
        print_asm(while_node->stmts, mp);
        std::cout << "  jmp loop" << while_node->while_num << std::endl;
        
        if (!next_while) {
            std::cout << "main" << while_node->main_num << ":" << std::endl;
        }
        print_asm(while_node->next, mp);
    }
};
