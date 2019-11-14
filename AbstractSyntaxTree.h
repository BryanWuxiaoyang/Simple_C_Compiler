#pragma once
// used for Abstract Syntax Tree
#include <stdio.h>
#include <stdlib.h>
#include "SymTable.h"
#include "FuncTable.h"
#include "TypeTable.h"
#include "Integer.h"
#include "Float.h"
#include "OP.h"
#include "NameCreator.h"

#define AST_MAX_HEIGHT 10

enum ASTNodeType {
	AST_INTEGER,
	AST_FLOAT,
	AST_SYM,
	AST_FUNC,
	AST_OP
};
typedef enum ASTNodeType ASTNodeType;

union ASTNodeValue {
	Sym sym;
	Func func;
	int intValue;
	float floatValue;
	enum OP op;
};
typedef union ASTNodeValue ASTNodeValue;

struct _ASTNode_ {
	int height;

	enum ASTNodeType type;
	union ASTNodeValue value;
	char* name;

	struct _ASTNode_* lc;
	struct _ASTNode_* rc;
	ListHead parents;

	ConstValue constValue;

	int accessTag;
};
typedef struct _ASTNode_* ASTNode;

ListHead astNodeLists[AST_MAX_HEIGHT + 1];

int maxHeight(ASTNode h1, ASTNode h2) {
	if (h1 && h2) return h1->height > h2->height ? h1->height : h2->height;
	else if (h1) return h1->height;
	else if (h2) return h2->height;
	else return 0;
}

ListHead getASTNodeList(int height) {
	return height >= AST_MAX_HEIGHT ? astNodeLists[AST_MAX_HEIGHT] : astNodeLists[height];
}

void initAST() {
	for (int i = 0; i <= AST_MAX_HEIGHT; i++) {
		astNodeLists[i] = MyList_createList();
	}
}

ASTNode findASTNode(int height, ASTNodeType type, ASTNodeValue value, ASTNode lc, ASTNode rc) {
	ListHead list = getASTNodeList(height);
	ListIterator it = MyList_createIterator(list);
	ASTNode res = NULL;
	while (MyList_hasNext(it)) {
		ASTNode node = (ASTNode)MyList_getNext(it);
		int suc = 0;
		switch (type) {
		case AST_INTEGER:
			suc = (node->type == AST_INTEGER && node->value.intValue == value.intValue && node->lc == lc && node->rc == rc) ? 1 : 0;
			break;
		case AST_FLOAT:
			suc = (node->type == AST_FLOAT && node->value.floatValue == value.floatValue && node->lc == lc && node->rc == rc) ? 1 : 0;
			break;
		case AST_SYM:
			suc = (node->type == AST_SYM && node->value.sym == value.sym && node->lc == lc && node->rc == rc) ? 1 : 0;
			break;
		case AST_FUNC:
			suc = (node->type == AST_FUNC && node->value.func == value.func && node->lc == lc && node->rc == rc) ? 1 : 0;
			break;
		case AST_OP:
			suc = (node->type == AST_OP && node->value.op == value.op && node->lc == lc && node->rc == rc) ? 1 : 0;
			break;
		default:exit(-1);
		}
		if (suc) {
			res = node;
			break;
		}
	}
	MyList_destroyIterator(it);
	return res;
}

void insertASTNode(ASTNode node) {
	int height = node->height;
	ListHead list = getASTNodeList(height);
	MyList_pushElem(list, node);
}

ASTNode createASTNode_raw(int height, ASTNodeType type, ASTNodeValue value, char* name, ASTNode lc, ASTNode rc, ListHead parents) {
	ASTNode node = (ASTNode)malloc(sizeof(struct _ASTNode_));
	if (node) {
		node->height = height;
		node->type = type;
		node->value = value;
		node->name = name;
		node->lc = lc;
		node->rc = rc;
		node->parents = parents;
		node->accessTag = 0;
		node->constValue.type = CONST_NONE;
	}
	return node;
}

ASTNode createASTNode_sym(Sym sym) {
	ASTNode node = NULL;
	ASTNodeValue value;
	value.sym = sym;
	node = findASTNode(0, AST_SYM, value, NULL, NULL);
	if(node == NULL){
		node = createASTNode_raw(
			0,
			AST_SYM,
			value,
			createName_sym(),
			NULL,
			NULL,
			MyList_createList()
		);
		insertASTNode(node);
	}
	return node;
}

ASTNode createASTNode_func(Func func) {
	ASTNode node = NULL;
	ASTNodeValue value;
	value.func = func;
	//node = findASTNode(0, AST_FUNC, value, NULL, NULL);
	if (node == NULL){
		node = createASTNode_raw(
			0,
			AST_FUNC,
			value,
			createName_func(),
			NULL,
			NULL,
			MyList_createList()
		);
		insertASTNode(node);
	}
	return node;
}

ASTNode createASTNode_integer(int v) {
	ASTNode node = NULL;
	ASTNodeValue value;
	value.intValue = v;
	node = findASTNode(0, AST_INTEGER, value, NULL, NULL);
	if (node == NULL){
		node = createASTNode_raw(
			0,
			AST_INTEGER,
			value,
			NULL,
			NULL,
			NULL,
			MyList_createList()
		);
		assignConstInteger(v, &node->constValue);
		insertASTNode(node);
	}
	return node;
}

ASTNode createASTNode_float(float v) {
	ASTNode node = NULL;
	ASTNodeValue value;
	value.floatValue = v;
	node = findASTNode(0, AST_FLOAT, value, NULL, NULL);
	if (node == NULL){
		node = createASTNode_raw(
			0,
			AST_FLOAT,
			value,
			NULL,
			NULL,
			NULL,
			MyList_createList()
		);
		assignConstFloat(v, &node->constValue);
		insertASTNode(node);
	}
	return node;
}

void clearParents(ASTNode lc) {
	ListIterator parents_it = MyList_createIterator(lc->parents);
	while (MyList_hasNext(parents_it)) {
		ASTNode node = (ASTNode)MyList_getNext(parents_it);
		ListHead list = getASTNodeList(node->height);

		ListIterator list_it = MyList_createIterator(list);
		while (MyList_hasNext(list_it)) {
			ASTNode list_node = (ASTNode)MyList_getNext(list_it);

			if (node == list_node) {
				clearParents(list_node);
				MyList_removePrev(list_it);
				break;
			}
		}
		MyList_destroyIterator(list_it);

	}
	MyList_destroyIterator(parents_it);
	MyList_clear(lc->parents);
}

int isConstASTNode(ASTNode node) {
	return node->constValue.type != CONST_NONE;
}

ConstValue getConstValue(ASTNode node) {
	return node->constValue;
}

void calConstValue(OP op, ConstValue* lv, ConstValue* rv, ConstValue* ret_value) {
	ConstType type = lv->type;
	ret_value->type = type;
	if (lv && rv) {
		if (type == CONST_INTEGER) {
			assignConstInteger(procOP2_int(op, getConstInt(lv), getConstInt(rv)), ret_value);
		}
		else if (type == CONST_FLOAT) {
			assignConstFloat(procOP2_float(op, getConstFloat(lv), getConstFloat(rv)), ret_value);
		}
	}
	else if (lv) {
		if (type == CONST_INTEGER) {
			assignConstInteger(procOP1_int(op, getConstInt(lv)), ret_value);
		}
		else if (type == CONST_FLOAT) {
			assignConstFloat(procOP1_float(op, getConstFloat(rv)), ret_value);
		}
	}
	else {
		exit(-1);
	}
}

ASTNode createASTNode_op(OP op, ASTNode lc, ASTNode rc) {
	ASTNode node = NULL;
	ASTNodeValue value;
	value.op = op;
	if (op == OP_ASSIGN) {
		clearParents(lc);
	}

	node = findASTNode(maxHeight(lc, rc) + 1, AST_OP, value, lc, rc);

	if (node == NULL) {
		node = createASTNode_raw(
			maxHeight(lc, rc) + 1,
			AST_OP,
			value,
			op == OP_ASSIGN ? rc->name : createName_temp(),
			lc,
			rc,
			MyList_createList()
		);

		if (op == OP_ASSIGN && isConstASTNode(rc)) {
			assignConstValue(&rc->constValue, &lc->constValue);
		}
		else if (lc && rc && isConstASTNode(lc) && isConstASTNode(rc)) {
			calConstValue(op, &lc->constValue, &rc->constValue, &node->constValue);
		}
		else if (lc && rc == 0 && isConstASTNode(rc)) {
			calConstValue(op, &lc->constValue, NULL, &node->constValue);
		}

		if (lc) MyList_pushElem(lc->parents, node);
		if (rc && rc != lc) MyList_pushElem(rc->parents, node);
		insertASTNode(node);
	}
	return node;
}


char* itostr(int val) {
	char* intStr = (char*)malloc(sizeof(char) * 40);
	if (intStr) sprintf(intStr, "#%d", val);
	return intStr;
}
char* ftostr(float val) {
	char* floatStr = (char*)malloc(sizeof(char) * 40);
	if (floatStr) sprintf(floatStr, "#%f", val);
	return floatStr;
}



char* getASTNodeStr_r(ASTNode node) {
	char* str = NULL;
	if (isConstASTNode(node)) {
		switch (node->constValue.type) {
		case CONST_INTEGER: str = itostr(getConstInt(&node->constValue)); break;
		case CONST_FLOAT:	str = ftostr(getConstFloat(&node->constValue)); break;
		}
	}
	else str = node->name;
	return str;
}

char* getASTNodeStr_l(ASTNode node) {
	char* str = NULL;
	if (node->type == AST_INTEGER || node->type == AST_FLOAT || node->type == AST_OP || node->type == AST_FUNC) assert(0);
	else if (node->type == AST_SYM) {
		str = node->name;
	}
	else assert(0);
	return str;
}

int astIndent = -1;
void printASTTree(ASTNode node) {
	if (node == NULL) return;
	astIndent++;
	for (int i = 0; i < astIndent; i++)
		printf("\t");
	switch (node->type) {
	case AST_INTEGER:	printf("integer: %s", itostr(node->value.intValue)); break;
	case AST_FLOAT:		printf("float: %s", ftostr(node->value.floatValue)); break;
	case AST_SYM:		printf("symbal: %s --> var: %s", node->value.sym->name, node->name); break;
	case AST_FUNC:		printf("func: %s --> return var: %s", node->value.func->name, node->name); break;
	case AST_OP:		printf("op: %d --> var: %s", node->value.op, node->name); break;
	default:			printf("error!"); exit(-1);
	}
	printf("\n");
	printASTTree(node->lc);
	printASTTree(node->rc);
	astIndent--;
}