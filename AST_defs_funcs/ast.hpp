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

class ArrayAssignNode : public StatementNode {
public:
    std::vector<AssignNode*> assigns;  
    ArrayAssignNode(
        int _line_index,
        std::vector<AssignNode*> _assigns,  
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

int expr_eval(ASTNode* ptr, std::map<std::string, std::pair<int, int>>& mp);

void traverse_tree(ASTNode* ptr, std::map<std::string, std::pair<int, int>>& mp, int* var_counter, 
                   int* loop_counter, int* if_counter, int* cond_counter, int* main_counter);

std::tuple<std::string, bool, int> var_checker(ASTNode* ptr, std::map<std::string, std::pair<int, int>>& mp);

void print_asm(ASTNode* ptr, std::map<std::string, std::pair<int, int>>& mp);

#endif
