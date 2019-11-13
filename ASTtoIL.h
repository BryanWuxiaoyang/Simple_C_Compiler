#pragma once
#include "AbstractSyntaxTree.h"
#include "IntermediateLanguage.h"

int tryGenInstantNode(ASTNode node, OP op) {
	BasicType type1 = BASIC_ERROR, type2 = BASIC_ERROR;
	void* value1 = NULL;
	void* value2 = NULL;
	if (isInstantASTNode(node->lc, &type1, &value1) && isInstantASTNode(node->rc, &type2, &value2)) {
		switch (type1) {
		case BASIC_INTEGER: {
			int v1 = ((Integer)value1)->value;
			int v2 = ((Integer)value2)->value;
			Integer value = NULL;
			switch (op) {
			case OP_PLUS:	value = createInteger(v1 + v2); break;
			case OP_MINUS:	value = createInteger(v1 - v2); break;
			case OP_STAR:	value = createInteger(v1 * v2); break;
			case OP_DIV:	value = createInteger(v1 / v2); break;
			default:		value = createInteger(v1);
			}
			node->nodeType = AST_INTEGER;
			node->nodeValue.integer = value;
			node->name = NULL;
			break;
		}
		case BASIC_FLOAT: {
			float v1 = ((Float)value1)->value;
			float v2 = ((Float)value2)->value;
			Float value = NULL;
			switch (op) {
			case OP_PLUS:	value = createFloat(v1 + v2); break;
			case OP_MINUS:	value = createFloat(v1 - v2); break;
			case OP_STAR:	value = createFloat(v1 * v2); break;
			case OP_DIV:	value = createFloat(v1 / v2); break;
			default:		value = createFloat(v1);
			}
			node->nodeType = AST_FLOAT;
			node->nodeValue.float_ = value;
			node->name = NULL;
			break;
		}
		}
		return 1;
	}
	return 0;
}

void ASTtoIL(ASTNode node) {
	if (node == NULL) exit(-1);
	if (node->accessTag > 0) return;
	node->accessTag++;
	if (node->nodeType == AST_OP) {
		ASTtoIL(node->lc);
		ASTtoIL(node->rc);
		char* arg1 = node->lc ? getASTNodeStr(node->lc) : NULL;
		char* arg2 = node->rc ? getASTNodeStr(node->rc) : NULL;
		char* target = node->name;
		OP op = node->nodeValue.op;
		ILOP ilop = ILOP_NONE;
		switch (op) {
		case OP_ASSIGN: {
			ilop = ILOP_ASSIGN; 
			target = arg1; 
			arg1 = arg2;
			arg2 = NULL;
			BasicType type = BASIC_ERROR;
			void* value = NULL;
			if (node->lc->nodeType == AST_SYM && isInstantASTNode(node->rc, &type, &value)) {
				Sym sym = node->lc->nodeValue.sym;
				switch (type) {
				case BASIC_INTEGER:
					sym->instantValue = createInteger(((Integer)value)->value);
					break;
				case BASIC_FLOAT:
					sym->instantValue = createFloat(((Float)value)->value);
					break;
				}
			}
			tryGenInstantNode(node, op);
			appendInterCode(createInterCode(arg1, arg2, target, ilop));
			break;
		}
		case OP_AND: exit(-1); break;
		case OP_OR: exit(-1); break;
		case OP_G:exit(-1); break;
		case OP_GE:exit(-1); break;
		case OP_E:exit(-1); break;
		case OP_NE:exit(-1); break;
		case OP_L:exit(-1); break;
		case OP_LE:exit(-1); break;
		case OP_PLUS:{
			ilop = ILOP_PLUS;
			if(tryGenInstantNode(node, op) == 0)
				appendInterCode(createInterCode(arg1, arg2, target, ilop));
			break;
		}
		case OP_MINUS: {
			ilop = ILOP_MINUS; 
			if (tryGenInstantNode(node, op) == 0)
				appendInterCode(createInterCode(arg1, arg2, target, ilop));
			break;
		}
		case OP_STAR: {
			ilop = ILOP_MUL; 
			if (tryGenInstantNode(node, op) == 0)
				appendInterCode(createInterCode(arg1, arg2, target, ilop));
			break;
		}
		case OP_DIV: {
			ilop = ILOP_DIV;
			if (tryGenInstantNode(node, op) == 0)
				appendInterCode(createInterCode(arg1, arg2, target, ilop));
			break;
		}
		case OP_NEG: {
			exit(-1); 
			break;
		}
		case OP_NOT: {
			exit(-1); 
			break;
		}
		}
		
	}
}