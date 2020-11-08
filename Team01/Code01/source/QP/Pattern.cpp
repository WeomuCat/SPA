#include <queue>
#include <unordered_map>
#include <vector>
#include <set>
#include <algorithm>
#include "Pattern.h"

Pattern::Pattern(QueryNode pattern_node, ResultList& intermediate_result_list) {
    this->pattern_node_ = pattern_node;
    this->intermediate_result_list = intermediate_result_list;
}

void Pattern::getPatternResult(PKB pkb, bool &clause_bool, ResultList &clause_result_list) {
    // Get ref nodes of the pattern node
    QUERY_NODE_LIST refs = pattern_node_.getChildren();
    SYNONYM_NAME refs0_name = refs[0].getString();

    std::unordered_set<SYNONYM_VALUE> resultlist_values;
    if (intermediate_result_list.containsSynonym(refs0_name)) {
        
        SYNONYM_VALUES_LIST all_values = intermediate_result_list.getValuesOfSynonym(refs0_name);
        std::unordered_set<SYNONYM_VALUE> unique_values(all_values.begin(), all_values.end());
        resultlist_values = unique_values;
        
        //resultlist_values = intermediate_result_list.getValuesOfSynonym(refs0_name);
    }


    /* Initialization */

    // valid_stmt_nums consists of:
    // 1. all assign statement numbers
    // 2. all if statement numbers
    // 3. all while statement numbers

    // var_ast_map consists of:
    // 1. LHS of assign statements
    // 2. CondExpr of if statements
    // 3. CondExpr of while statements

    // expr_ast_map consists of:
    // 1. RHS of assign statements
    std::vector<STMT_NUM> valid_stmt_nums;
    std::unordered_map<STMT_NUM, AST_NODE_PTR> var_ast_map;
    std::unordered_map<STMT_NUM, EXPR_NODE_PTR> expr_ast_map;
    switch (refs[0].getSynonymType()) {
    case SYNONYM_TYPE::assign:
        for (ASSIGN_NODE_PTR a : pkb.getAssigns()) {
            STMT_NUM s = a->getStatementNumber();
            AST_NODE_PTR v = a->getVariableNode();
            EXPR_NODE_PTR e = a->getExpressionNode();

            if (!resultlist_values.empty()) {
                if (resultlist_values.find(std::to_string(s)) != resultlist_values.end()) {
                    valid_stmt_nums.push_back(s);
                    var_ast_map.insert({ s, v });
                    expr_ast_map.insert({ s, e });
                }
            }
            else {
                valid_stmt_nums.push_back(s);
                var_ast_map.insert({ s, v });
                expr_ast_map.insert({ s, e });
            }
            
        }
        break;
    case SYNONYM_TYPE::ifs:
        for (IF_NODE_PTR i : pkb.getIfs()) {
            STMT_NUM s = i->getStatementNumber();
            AST_NODE_PTR v = i->getConditionNode();

            if (!resultlist_values.empty()) {
                if (resultlist_values.find(std::to_string(s)) != resultlist_values.end()) {
                    valid_stmt_nums.push_back(s);
                    var_ast_map.insert({ s, v });
                }
            }
            else {
                valid_stmt_nums.push_back(s);
                var_ast_map.insert({ s, v });
            }
            
        }
        break;
    case SYNONYM_TYPE::whiles:
        for (WHILE_NODE_PTR w : pkb.getWhiles()) {
            STMT_NUM s = w->getStatementNumber();
            AST_NODE_PTR v = w->getConditionNode();

            if (!resultlist_values.empty()) {
                if (resultlist_values.find(std::to_string(s)) != resultlist_values.end()) {
                    valid_stmt_nums.push_back(s);
                    var_ast_map.insert({ s, v });
                }
            }
            else {
                valid_stmt_nums.push_back(s);
                var_ast_map.insert({ s, v });
            }
            
        }
        break;
    }


    /* Variable Matching */
    // We do variable matching only if refs[1] is not a wild_card

    // If refs[1] is a synonym, var_names_map consists of valid var_names of the stmt_num
    std::unordered_map<STMT_NUM, VAR_NAME_LIST> var_names_map;
    if (refs[1].getNodeType() != QUERY_NODE_TYPE::wild_card) {
        // s1 is a set of variable names from either:
        // 1. ident ("x") in entRef
        // 2. synonym (v) in entRef
        VAR_NAME_LIST s1;
        switch (refs[1].getNodeType()) {
        case QUERY_NODE_TYPE::ident:
            s1.push_back(refs[1].getString());
            break;
        case QUERY_NODE_TYPE::synonym:
            for (VAR_NAME v : pkb.getVariableNameList()) {
                s1.push_back(v);
            }
            break;
        }

        for (auto it = valid_stmt_nums.begin(); it != valid_stmt_nums.end(); ) {
            STMT_NUM s = *it;
            AST_NODE_PTR v = var_ast_map.at(s);

            // s2 is a set of variable names from either:
            // 1. LHS of assign statements
            // 2. CondExpr of if statements
            // 3. CondExpr of while statements
            VAR_NAME_LIST s2 = getVarNameListFromAst(v);

            // The intersection of s1 and s2 will give a set s3, which contains the matched var_names for this stmt_num!
            VAR_NAME_LIST s3;
            std::sort(s1.begin(), s1.end());
            std::sort(s2.begin(), s2.end());
            std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), std::back_inserter(s3));

            if (s3.empty()) {
                // Empty means no matches! Discard this stmt_num
                it = valid_stmt_nums.erase(it);
                continue;
            }

            // If refs[1] is a synonym, we want to add variables from s3 to the ResultList
            if (refs[1].getNodeType() == QUERY_NODE_TYPE::synonym) {
                var_names_map.insert({ s, s3 });
            }
            it++;
        }
    }


    /* Tree Matching */
    // We do tree matching only if refs[1] is an assign synonym, and refs[2] is not a wild_card
    if (refs[0].getSynonymType() == SYNONYM_TYPE::assign && refs[2].getNodeType() != QUERY_NODE_TYPE::wild_card) {
        EXPR_NODE_PTR needle_ast = std::static_pointer_cast<ExpressionNode>(refs[2].getAstNode());

        for (auto it = valid_stmt_nums.begin(); it != valid_stmt_nums.end(); ) {
            STMT_NUM s = *it;
            EXPR_NODE_PTR haystack_ast = expr_ast_map.at(s);

            bool match = false;
            switch (refs[2].getNodeType()) {
            case QUERY_NODE_TYPE::expression:
                match = exactExpressionTreeMatch(haystack_ast, needle_ast);
                break;
            case QUERY_NODE_TYPE::partial_expression:
                match = partialExpressionTreeMatch(haystack_ast, needle_ast);
                break;
            }

            if (!match) {
                it = valid_stmt_nums.erase(it);
                continue;
            }
            it++;
        }
    }


    /* ResultList Creation */
    // If refs[1] is ident or wild_card, we only need to add vaild_stmt_nums to ResultList
    // If refs[1] is a synonym, we need to add a variable column too
    SYNONYM_NAME s0 = refs[0].getString();
    switch (refs[1].getNodeType()) {
    case QUERY_NODE_TYPE::ident:
    case QUERY_NODE_TYPE::wild_card:
        clause_result_list.addColumn(s0, valid_stmt_nums);
        break;
    case QUERY_NODE_TYPE::synonym:
        SYNONYM_NAME s1 = refs[1].getString();
        clause_result_list.addColumn(s0);
        clause_result_list.addColumn(s1);
        for (STMT_NUM s : valid_stmt_nums) {
            for (VAR_NAME v : var_names_map.at(s)) {
                ROW row;
                row.insert({ s0, std::to_string(s) });
                row.insert({ s1, v });
                clause_result_list.addRow(row);
            }
        }
        break;
    }

    // If valid_stmt_nums is non-empty, then this clause is true
    clause_bool = valid_stmt_nums.size() > 0;
}

VAR_NAME_LIST Pattern::getVarNameListFromAst(AST_NODE_PTR ast) {
    // Do BFS
    std::queue<AST_NODE_PTR> queue;
    std::unordered_set<VAR_NAME> var_names;
    queue.push(ast);
    while (!queue.empty()) {
        AST_NODE_PTR n = queue.front();
        queue.pop();

        // Variable node found! Add to var_names
        if (n->getNodeType() == NODE_TYPE::variableNode) {
            VAR_NODE_PTR v = std::static_pointer_cast<VariableNode>(n);
            var_names.insert(v->getVariableName());
        }

        // Push children of current node to queue
        for (AST_NODE_PTR c : n->getChildrenNode()) {
            queue.push(c);
        }
    }
    
    VAR_NAME_LIST result;
    for (VAR_NAME v : var_names) {
        result.push_back(v);
    }
    return result;
}

bool Pattern::exactExpressionTreeMatch(EXPR_NODE_PTR haystack, EXPR_NODE_PTR needle) {
    // Algorithm: BFS
    std::queue<AST_NODE_PTR> queue1;
    std::queue<AST_NODE_PTR> queue2;
    queue1.push(haystack);
    queue2.push(needle);
    while (!queue1.empty() && !queue2.empty()) {
        AST_NODE_PTR n1 = queue1.front();
        AST_NODE_PTR n2 = queue2.front();
        queue1.pop();
        queue2.pop();

        // Check if the current nodes are equal
        if (!areExpressionNodesEqual(n1, n2)) {
            return false;
        }

        // If both nodes are nullptr, skip as they have no children!
        if (n1 == nullptr && n2 == nullptr) {
            continue;
        }

        // Push children of nodes to queues
        queue1.push(getExpressionNodeLeft(n1));
        queue1.push(getExpressionNodeRight(n1));

        queue2.push(getExpressionNodeLeft(n2));
        queue2.push(getExpressionNodeRight(n2));
    }

    // Both queues always have the same length!
    // If both queues are empty, it means that both trees are equal :)
    return true;
}

bool Pattern::partialExpressionTreeMatch(EXPR_NODE_PTR haystack, EXPR_NODE_PTR needle) {
    // Algorithm: BFS
    // Traverse the entire haystack tree
    // For every node, check if a partial top match with the needle exists
    std::queue<AST_NODE_PTR> queue;
    queue.push(haystack);
    while (!queue.empty()) {
        AST_NODE_PTR n = queue.front();
        queue.pop();

        if (partialTopExpressionTreeMatch(n, needle)) {
            return true;
        }

        // Push children of current node to queues
        for (AST_NODE_PTR c : getExpressionNodeChildren(n)) {
            queue.push(c);
        }
    }
    
    // If the queue is empty, there is no partial match in the entire haystack tree :(
    return false;
}

bool Pattern::partialTopExpressionTreeMatch(AST_NODE_PTR haystack, AST_NODE_PTR needle) {
    // Algorithm: BFS
    // Partial top match means that the needle is compared only at the root of the haystack
    std::queue<AST_NODE_PTR> queue1;
    std::queue<AST_NODE_PTR> queue2;
    queue1.push(haystack);
    queue2.push(needle);
    while (!queue1.empty() && !queue2.empty()) {
        AST_NODE_PTR n1 = queue1.front();
        AST_NODE_PTR n2 = queue2.front();
        queue1.pop();
        queue2.pop();

        if (!areExpressionNodesEqual(n1, n2)) {
            return false;
        }

        // We check by traversing based on the needle's tree
        // For the current needle node,
        // 1. If it has a left child, append both n1 & n2's left child
        // 2. If it has a right child, append both n1 & n2's right child
        AST_NODE_PTR needle_left = getExpressionNodeLeft(n2);
        AST_NODE_PTR needle_right = getExpressionNodeRight(n2);
        if (needle_left != nullptr) {
            queue1.push(getExpressionNodeLeft(n1));
            queue2.push(needle_left);
        }
        if (needle_right != nullptr) {
            queue1.push(getExpressionNodeRight(n1));
            queue2.push(needle_right);
        }
    }
    
    // Both queues always have the same length!
    // If both queues are empty, it means that all needle nodes can be found in the haystack :)
    return true;
}

bool Pattern::areExpressionNodesEqual(AST_NODE_PTR n1, AST_NODE_PTR n2) {
    // Check if nodes are nullptr...
    if (n1 == nullptr && n2 == nullptr) {
        return true;
    }
    if (n1 == nullptr || n2 == nullptr) {
        return false;
    }

    // Handles weird expression node with none type...
    if (n1->getNodeType() == NODE_TYPE::expressionNode) {
        EXPR_NODE_PTR e1 = std::static_pointer_cast<ExpressionNode>(n1);
        if (e1->getExpressionType() == EXPR_TYPE::none) {
            n1 = e1->getLeftAstNode();
        }
    }
    if (n2->getNodeType() == NODE_TYPE::expressionNode) {
        EXPR_NODE_PTR e2 = std::static_pointer_cast<ExpressionNode>(n2);
        if (e2->getExpressionType() == EXPR_TYPE::none) {
            n2 = e2->getLeftAstNode();
        }
    }

    NODE_TYPE t1 = n1->getNodeType();
    NODE_TYPE t2 = n2->getNodeType();
    if (t1 != t2) {
        return false;
    }
    switch (t1) {
    case NODE_TYPE::expressionNode:
    {
        EXPR_NODE_PTR e1 = std::static_pointer_cast<ExpressionNode>(n1);
        EXPR_NODE_PTR e2 = std::static_pointer_cast<ExpressionNode>(n2);
        return e1->getExpressionType() == e2->getExpressionType();
    }
    case NODE_TYPE::variableNode:
    {
        VAR_NODE_PTR v1 = std::static_pointer_cast<VariableNode>(n1);
        VAR_NODE_PTR v2 = std::static_pointer_cast<VariableNode>(n2);
        return v1->getVariableName() == v2->getVariableName();
    }
    case NODE_TYPE::constantNode:
    {
        CONSTANT_NODE_PTR c1 = std::static_pointer_cast<ConstantNode>(n1);
        CONSTANT_NODE_PTR c2 = std::static_pointer_cast<ConstantNode>(n2);
        return c1->getValue() == c2->getValue();
    }
    }
    return false;
}

AST_NODE_PTR_LIST Pattern::getExpressionNodeChildren(AST_NODE_PTR n) {
    if (n->getNodeType() != NODE_TYPE::expressionNode) {
        return {};
    }

    EXPR_NODE_PTR e = std::static_pointer_cast<ExpressionNode>(n);
    // Handles weird expression node with none type...
    if (e->getExpressionType() == EXPR_TYPE::none) {
        return {};
    }

    AST_NODE_PTR_LIST result;
    result.push_back(e->getLeftAstNode());
    result.push_back(e->getRightAstNode());
    return result;
}

AST_NODE_PTR Pattern::getExpressionNodeLeft(AST_NODE_PTR n) {
    if (n->getNodeType() != NODE_TYPE::expressionNode) {
        return nullptr;
    }

    EXPR_NODE_PTR e = std::static_pointer_cast<ExpressionNode>(n);
    // Handles weird expression node with none type...
    if (e->getExpressionType() == EXPR_TYPE::none) {
        return nullptr;
    }

    return e->getLeftAstNode();
}

AST_NODE_PTR Pattern::getExpressionNodeRight(AST_NODE_PTR n) {
    if (n->getNodeType() != NODE_TYPE::expressionNode) {
        return nullptr;
    }

    EXPR_NODE_PTR e = std::static_pointer_cast<ExpressionNode>(n);
    // Handles weird expression node with none type...
    if (e->getExpressionType() == EXPR_TYPE::none) {
        return nullptr;
    }

    return e->getRightAstNode();
}
