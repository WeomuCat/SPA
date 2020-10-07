#include "Pattern.h"

Pattern::Pattern(QueryNode clause_node) {
    this->clause_node_ = clause_node;
}

void Pattern::getPatternResult(PKB pkb, bool &clause_bool, ResultList &clause_result_list) {
    // Get children nodes of the clause node
    QUERY_NODE_LIST c = clause_node_.getChildren();


    /* Result1 contains stmt_num and ast_node_ptr of each statement */

    // ast_node_ptr is from either:
    // 1. LHS of assign statements
    // 2. CondExpr of if statements
    // 3. CondExpr of while statements
    std::vector<std::tuple<STMT_NUM, AST_NODE_PTR>> result1;
    switch (c[0].getSynonymType()) {
    case QuerySynonymType::assign:
        for (ASSIGN_NODE_PTR a : pkb.getAssigns()) {
            STMT_NUM s = a->getStatementNumber();
            AST_NODE_PTR n = a->getVariableNode();
            result1.push_back({ s, n });
        }
        break;
    case QuerySynonymType::ifs:
        for (IF_NODE_PTR i : pkb.getIfs()) {
            STMT_NUM s = i->getStatementNumber();
            AST_NODE_PTR n = i->getConditionNode();
            result1.push_back({ s, n });
        }
        break;
    case QuerySynonymType::whiles:
        for (WHILE_NODE_PTR w : pkb.getWhiles()) {
            STMT_NUM s = w->getStatementNumber();
            AST_NODE_PTR n = w->getConditionNode();
            result1.push_back({ s, n });
        }
        break;
    }


    /* Result2 contains stmt_num and var_names of matched statements, after variable matching */

    // s1 is a set of variable names from either:
    // 1. ident ("x") in entRef
    // 2. synonym (v) in entRef

    // s2 is a set of variable names from either:
    // 1. LHS of assign statements
    // 2. CondExpr of if statements
    // 3. CondExpr of while statements

    // The intersection of s1 and s2 will give a set s3, which contains the matched var_names for this stmt_num!
    std::unordered_set<VAR_NAME> s1;
    switch (c[1].getNodeType()) {
    case QueryNodeType::ident:
        s1.insert(c[1].getString());
        break;
    case QueryNodeType::synonym:
        for (VAR_NAME v : pkb.getVariableNameList()) {
            s1.insert(v);
        }
        break;
    }

    std::vector<std::tuple<STMT_NUM, VAR_NAME_LIST>> result2;

    switch (c[1].getNodeType()) {
    case QueryNodeType::ident:
    case QueryNodeType::synonym:
        for (auto t : result1) {
            STMT_NUM s = std::get<0>(t);
            // Extract all variables from ast_node_ptr
            std::unordered_set<VAR_NAME> s2 = getVarNameListFromAst(std::get<1>(t));

            VAR_NAME_LIST s3;
            std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), std::back_inserter(s3));
            // Empty means no matches! Discard this stmt_num
            if (s3.empty()) {
                continue;
            }
            result2.push_back({ s, s3 });
        }
        break;
    case QueryNodeType::wild_card:
        // If c[1] is a wildcard, don't do anything
        for (auto t : result1) {
            STMT_NUM s = std::get<0>(t);
            result2.push_back({ s, {} });
        }
        break;
    }


    /* Result3 contains stmt_num and var_names of matched statements, after tree matching */

    // AST tree matching only for assign statements
    switch (c[0].getSynonymType()) {
    case QuerySynonymType::assign:
        // std::vector<EXPR_NODE_PTR> expr_asts;
        // EXPR_NODE_PTR pattern;
        // TODO
        break;
    case QuerySynonymType::ifs:
    case QuerySynonymType::whiles:
        break;
    }


    /* ResultList creation from Result3 */
    // TODO
}

std::unordered_set<VAR_NAME> Pattern::getVarNameListFromAst(AST_NODE_PTR ast) {
    // TODO: Do DFS
}