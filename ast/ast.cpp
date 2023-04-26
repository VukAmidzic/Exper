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
#include <fstream>

NumNode::NumNode(int _line_index, int _num) { line_index = _line_index; num = _num; };

VarNode::VarNode(int _line_index, std::string _var_name) { line_index = _line_index; var_name = _var_name; };

ArrayElemNode::ArrayElemNode(int _line_index, std::string _arr_name, ASTNode* _elem_index) {
    line_index = _line_index;
    arr_name = _arr_name;
    elem_index = _elem_index;
};

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

StatArrayDeclNode::StatArrayDeclNode(int _line_index, std::string _arr_name, int _arr_size, 
    std::vector<ASTNode*> _arr_vals, ASTNode* _next) {
    line_index = _line_index;
    arr_name = _arr_name;
    arr_size = _arr_size;
    arr_vals = _arr_vals;
    next = _next;
};

DynArrayDeclNode::DynArrayDeclNode(int _line_index, int _arrayDecl_loop, std::string _arr_name, ASTNode* _arr_size, 
    ASTNode* _arr_val, ASTNode* _next) {
    line_index = _line_index;
    arrayDecl_loop = _arrayDecl_loop;
    arr_name = _arr_name;
    arr_size = _arr_size;
    arr_val = _arr_val;
    next = _next;
}

ArrayElemAssignNode::ArrayElemAssignNode(int _line_index, std::string _arr_name, ASTNode* _elem_index, 
    ASTNode* _assign_val, ASTNode* _next) {
    line_index = _line_index;
    arr_name = _arr_name;
    elem_index = _elem_index;
    assign_val = _assign_val;
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

