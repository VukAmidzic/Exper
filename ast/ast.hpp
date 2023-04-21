#include <string>
#include <cstddef>
#include <cstdlib>
#include <map>
#include <tuple>
#include <vector>
#include <utility>

#ifndef AST_HPP
#define AST_HPP


enum Tag {
    _ADD_, _SUB_, _MUL_, _DIV_,
    _MOD_, _SHL_, _SHR_, _LESS_,
    _GREAT_, _EQ_, _NEQ_, _LEQ_,
    _GEQ_, _AND_, _OR_, _NOT_,
    _NEG_
};

enum ArrayType { _STAT_, _DYN_ };
   
enum ErrType { _OK_, _ERR_VAR_, _ERR_ARR_ };

class Result {
public:
    ErrType err;
    int err_line;
    std::string msg;
    Result(ErrType _err, int _err_line, std::string _msg) {
        err = _err;
        err_line = _err_line;
        msg = _msg;
    };
};

class ASTNode {
public:
    int line_index;
    virtual ~ASTNode() = default;
};

class NumNode : public ASTNode {
public:
    int num;
    NumNode(int _line_index, int _num);
};

class VarNode : public ASTNode {
public:
    std::string var_name;
    VarNode(int _line_index, std::string _var_name);
};

class ArrayElemNode : public ASTNode {
public:
    std::string arr_name;
    ASTNode* elem_index;
    ArrayElemNode(int _line_index, std::string _arr_name, ASTNode* _elem_index);
};

class BinaryNode : public ASTNode {
public: 
    Tag tag;
    ASTNode* left;
    ASTNode* right;
    BinaryNode(
        int _line_index,
        Tag _tag,
        ASTNode* _left,
        ASTNode* _right
    );
};

class StatementNode : public ASTNode {
public: 
    virtual ~StatementNode() = default;
    ASTNode* next;
};

class MainNode : public StatementNode {
public:
    MainNode(int _line_index, ASTNode* _next);
};

class AssignNode : public StatementNode {
public:
    std::string var_name;
    ASTNode* assign_val;  
    AssignNode(
        int _line_index,
        std::string _var_name,
        ASTNode* _assign_val,
        ASTNode* _next
    );
};

class StatArrayDeclNode : public StatementNode {
public:
    std::string arr_name;
    int arr_size;
    std::vector<ASTNode*> arr_vals;
    
    StatArrayDeclNode(
        int _line_index,
        std::string _arr_name,
        int _arr_size,
        std::vector<ASTNode*> _arr_vals,
        ASTNode* _next
    );
};

class DynArrayDeclNode : public StatementNode {
public:
    int arrayDecl_loop;
    std::string arr_name;
    ASTNode* arr_size;
    ASTNode* arr_val;
    
    DynArrayDeclNode(
        int _line_index,
        int _arrayDecl_loop,
        std::string _arr_name,
        ASTNode* _arr_size,
        ASTNode* _arr_val,
        ASTNode* _next
    );
};

class ArrayElemAssignNode : public StatementNode {
public:
    std::string arr_name;
    ASTNode* elem_index;
    ASTNode* assign_val;
    ArrayElemAssignNode(
        int _line_index,
        std::string _arr_name,
        ASTNode* _elem_index,
        ASTNode* _assign_val,
        ASTNode* _next
    );
};

class PrintNode : public StatementNode {
public:
    ASTNode* print_val; 
    PrintNode(
        int _line_index,
        ASTNode* _print_val,
        ASTNode* _next
    );
};

class ScanNode : public StatementNode {
public:
    std::string var_name; 
    ScanNode(
        int _line_index,
        std::string _var_name,
        ASTNode* _next
    );
};

class IfElseNode : public StatementNode {
public:
    int if_num;
    int main_num;
    std::vector<std::pair<ASTNode*, ASTNode*>> conds; 
    std::vector<int> cond_num;
    IfElseNode(
        int _line_index,
        int _if_num,
        int _main_num,
        std::vector<std::pair<ASTNode*, ASTNode*>> _conds,
        std::vector<int> _cond_num,
        ASTNode* _next
    );
};

class WhileNode : public StatementNode {
public:
    int while_num;
    int main_num;
    ASTNode* cond;
    ASTNode* stmts;
    WhileNode(
        int _line_index,
        int _while_num,
        int _main_num,
        ASTNode* _cond, 
        ASTNode* _stmts, 
        ASTNode* _next
    );
};

int expr_eval(ASTNode* ptr, std::map<std::string, int>& mp);

Result* traverse_tree(ASTNode* ptr, std::map<std::string, int>& mp, std::map<std::string, std::pair<int, ArrayType>>& arrs,
    int* loop_counter, int* if_counter, int* cond_counter, int* main_counter, int* arrayDecl_loop);

void num_of_scans(ASTNode* ptr, int num);

void print_asm(ASTNode* ptr, std::map<std::string, int>& mp, std::map<std::string, std::pair<int, ArrayType>>& arrs);

std::string get_err_line(int err_index, std::string filename);

#endif
