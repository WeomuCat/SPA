#include<stdio.h>

#include "ParserPrintTree.h"

std::string ParserPrintTree::parserPrintTree(AST_NODE_PTR parent_node_ptr) {
	std::deque< AST_NODE_PTR> node_queue = std::deque< AST_NODE_PTR>();
	node_queue.push_back(parent_node_ptr);
	std::string result;

	while (!node_queue.empty()) {
		AST_NODE_PTR node_ptr = node_queue.front();
		node_queue.pop_front();
		NODE_TYPE node_type = node_ptr->getNodeType();
		AST_NODE_PTR_LIST children_nodes = node_ptr->getChildrenNode();

		// Get properties of current node, and print:
		std::vector<std::string> node_properties = getNodeProperties(node_ptr, node_type);

		result.append("\n\n Node Type: ");
		;			for (STRING properties : node_properties) {
			result.append(properties + '\n');
		}

		// Enqueue currrent node's children nodes:
		for (AST_NODE_PTR child : children_nodes) {
			node_queue.push_back(child);
		}

		// Print number of children nodes:
		result.append("No. of Children: ");
		result.append(std::to_string(children_nodes.size()) + '\n');
	}

	return result;
}

std::vector<std::string> ParserPrintTree::getNodeProperties(AST_NODE_PTR node_ptr, NODE_TYPE node_type) {
	std::vector<std::string> node_properties;
	ExpressionTypeEnum expr = std::static_pointer_cast<ExpressionNode>(node_ptr)->getExpressionType();
	STRING expr_str = "ERROR NO EXPR";
	switch (node_type) {

	case NODE_TYPE::assignNode:
		node_properties.push_back("assignNode");
		break;

	case NODE_TYPE::conditionNode:
		node_properties.push_back("conditionNode");
		//node_properties.push_back(std::static_pointer_cast<ConditionNode>(node_ptr)->getConditionType());
		break;

	case NODE_TYPE::constantNode:
		node_properties.push_back("constantNode");
		node_properties.push_back(std::static_pointer_cast<ConstantNode>(node_ptr)->getValue());
		break;

	case NODE_TYPE::expressionNode:
		node_properties.push_back("expressionNode");

		if (expr == ExpressionTypeEnum::div) {
			expr_str = "DIVISION";
		}
		else if (expr == ExpressionTypeEnum::times) {
			expr_str = "MULTIPLICATION";
		}
		else if (expr == ExpressionTypeEnum::plus) {
			expr_str = "PLUS";
		}
		else if (expr == ExpressionTypeEnum::min) {
			expr_str = "MINUS";
		}
		else if (expr == ExpressionTypeEnum::mod) {
			expr_str = "MODULO";
		}
		else if (expr == ExpressionTypeEnum::none) {
			expr_str = "VAR OR CONST";
		}
		node_properties.push_back(expr_str);
		break;

	case NODE_TYPE::ifNode:
		node_properties.push_back("ifNode");
		break;

	case NODE_TYPE::printNode:
		node_properties.push_back("printNode");
		break;

	case NODE_TYPE::procedureNode:
		node_properties.push_back("procedureNode");
		node_properties.push_back(std::static_pointer_cast<ProcedureNode>(node_ptr)->getProcedureName());
		break;

	case NODE_TYPE::programNode:
		node_properties.push_back("programNode");
		break;

	case NODE_TYPE::readNode:
		node_properties.push_back("readNode");
		break;

	case NODE_TYPE::relationNode:
		node_properties.push_back("relationNode");
		//node_properties.push_back(std::static_pointer_cast<RelationNode>(node_ptr)->getRelationType());
		break;

	case NODE_TYPE::statementListNode:
		node_properties.push_back("statementListNode");
		break;

	case NODE_TYPE::variableNode:
		node_properties.push_back("variableNode");
		node_properties.push_back(std::static_pointer_cast<VariableNode>(node_ptr)->getVariableName());
		break;

	case NODE_TYPE::whileNode:
		node_properties.push_back("whileNode");
		break;

	default:
		node_properties.push_back("Error: No such node.");
	}

	return node_properties;
}