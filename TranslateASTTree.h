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
		if (node->accessTag <= 1 && isConstASTNode(node) == 0) {
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
			if (node->lc->type == AST_OP && node->lc->value.op == OP_DEREF) {
				node->name = getASTNodeStr_l(node->lc->lc);
			}
			else if (node->lc->type == AST_OP && node->lc->value.op == OP_REF && node->lc->name[0] == '&') {
				char* temp = createName_temp();
				char* newName = createName_temp();
				appendInterCode(createInterCode(getASTNodeStr_r(node->lc), NULL, temp, ILOP_ASSIGN));
				appendInterCode(createInterCode(temp, NULL, newName, ILOP_ADDR));
				node->name = newName;
			}
			else {

			}
		}
		break;
	}
	case OP_DEREF: {
		if (node->accessTag <= 1) {
			translateASTTree(node->lc);
			if (node->name[0] == '*') {
				if (node->lc->type == AST_OP && node->lc->value.op == OP_REF) {
					node->name = getASTNodeStr_l(node->lc->lc);
				}
				else if (node->lc->type == AST_OP && node->lc->value.op == OP_DEREF && node->lc->name[0] == '*') {
					char* temp = createName_temp();
					char* newName = createName_temp();
					appendInterCode(createInterCode(getASTNodeStr_r(node->lc), NULL, temp, ILOP_ASSIGN));
					appendInterCode(createInterCode(temp, NULL, newName, ILOP_DEREF));
					node->name = newName;
				}
				else {

				}
			}
		}
		break;
	}
	default:		assert(0);
	}
}