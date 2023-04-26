#include "ast.hpp"
#include <string>
#include <cstddef>
#include <cstdlib>
#include <map>
#include <tuple>
#include <vector>
#include <utility>

#ifndef _TREE_HPP_
#define _TREE_HPP_

int expr_eval(ASTNode* ptr, std::map<std::string, int>& mp);

Result* traverse_tree(ASTNode* ptr, std::map<std::string, int>& mp, std::map<std::string, std::pair<int, ArrayType>>& arrs,
    int* loop_counter, int* if_counter, int* cond_counter, int* main_counter, int* arrayDecl_loop);

void num_of_scans(ASTNode* ptr, int num);

void print_asm(ASTNode* ptr, std::map<std::string, int>& mp, std::map<std::string, std::pair<int, ArrayType>>& arrs);

std::string get_err_line(int err_index, std::string filename);

#endif
