#pragma once
#include "AbstractSyntaxTree.h"
#include "IntermediateLanguage.h"


void ASTtoIL(ASTNode node) {
	if (node == NULL) {
		exit(-1);
	}
	else if (node->nodeType == AST_OP) {
		ASTtoIL(node->lc);
		ASTtoIL(node->rc);
		char* arg1 = node->lc ? getASTNodeStr(node->lc) : NULL;
		char* arg2 = node->rc ? getASTNodeStr(node->rc) : NULL;
		char* target = node->name;
		OP op = node->nodeValue.op;
		ILOP ilop = ILOP_NONE;
		switch (op) {
		case OP_ASSIGN: ilop = ILOP_ASSIGN; break;
		case OP_AND: exit(-1); break;
		case OP_OR: exit(-1); break;
		case OP_G:exit(-1); break;
		case OP_GE:exit(-1); break;
		case OP_E:exit(-1); break;
		case OP_NE:exit(-1); break;
		case OP_L:exit(-1); break;
		case OP_LE:exit(-1); break;
		case OP_PLUS:ilop = ILOP_PLUS; break;
		case OP_MINUS:ilop = ILOP_MINUS; break;
		case OP_STAR:ilop = ILOP_MUL; break;
		case OP_DIV:ilop = ILOP_DIV; break;
		case OP_NEG:exit(-1); break;
		case OP_NOT:exit(-1); break;
		}
		appendInterCode(createInterCode(arg1, arg2, target, ilop));
	}
}