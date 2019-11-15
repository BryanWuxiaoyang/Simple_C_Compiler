#pragma once
#include "AbstractSyntaxTree.h"
#include "IntermediateLanguage.h"


void translateASTTree(ASTNode node) {
	if (node == NULL) return;
	node->accessTag++;
	if (node->type != AST_OP) return;
	

	switch (node->value.op) {
	case OP_ASSIGN:
	case OP_AND:
	case OP_OR:
	case OP_G:
	case OP_GE:
	case OP_E:
	case OP_NE:
	case OP_L:
	case OP_LE:		assert(0);//unimplemented yet
	case OP_PLUS: 
	case OP_MINUS:
	case OP_STAR:
	case OP_DIV: {
		if (node->accessTag <= 1) {
			translateASTTree(node->lc);
			translateASTTree(node->rc);
			appendInterCode(createInterCode(getASTNodeStr_l(node->lc), getASTNodeStr_r(node->rc), getASTNodeStr_r(node), getAlgorithmOP(node->value.op)));
		}
		break;
	}
	case OP_NEG:
	case OP_NOT:	assert(0);// unimplemented yet
	case OP_REF: {
		if (node->accessTag <= 1) {
			translateASTTree(node->lc);
			appendInterCode(createInterCode(getASTNodeStr_r(node->lc), NULL, getASTNodeStr_r(node), ILOP_ADDR));
		}
		break;
	}
	case OP_DEREF: {
		if (node->accessTag <= 1) {
			translateASTTree(node->lc);
			// do not need to print deref result, since the node itself can be directly used as the form '*v'
		}
		break;
	}
	default:		assert(0);
	}
}