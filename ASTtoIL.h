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
		
	}
}