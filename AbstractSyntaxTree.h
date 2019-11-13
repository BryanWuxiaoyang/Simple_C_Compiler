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
	Integer integer;
	Float float_;
	enum OP op;
};
typedef union ASTNodeValue ASTNodeValue;

struct _ASTNode_ {
	int height;

	enum ASTNodeType nodeType;
	union ASTNodeValue nodeValue;
	char* name;

	struct _ASTNode_* lc;
	struct _ASTNode_* rc;
	ListHead parents;

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
			suc = (node->nodeType == AST_INTEGER && node->nodeValue.integer->value == value.integer->value && node->lc == lc && node->rc == rc) ? 1 : 0;
			break;
		case AST_FLOAT:
			suc = (node->nodeType == AST_FLOAT && node->nodeValue.float_->value == value.float_->value && node->lc == lc && node->rc == rc) ? 1 : 0;
			break;
		case AST_SYM:
			suc = (node->nodeType == AST_SYM && node->nodeValue.sym == value.sym && node->lc == lc && node->rc == rc) ? 1 : 0;
			break;
		case AST_FUNC:
			suc = (node->nodeType == AST_FUNC && node->nodeValue.func == value.func && node->lc == lc && node->rc == rc) ? 1 : 0;
			break;
		case AST_OP:
			suc = (node->nodeType == AST_OP && node->nodeValue.op == value.op && node->lc == lc && node->rc == rc) ? 1 : 0;
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

ASTNode createASTNode_raw(int height, ASTNodeType nodeType, ASTNodeValue nodeValue, char* name, ASTNode lc, ASTNode rc, ListHead parents) {
	ASTNode node = (ASTNode)malloc(sizeof(struct _ASTNode_));
	if (node) {
		node->height = height;
		node->nodeType = nodeType;
		node->nodeValue = nodeValue;
		node->name = name;
		node->lc = lc;
		node->rc = rc;
		node->parents = parents;
		node->accessTag = 0;
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
	node = findASTNode(0, AST_FUNC, value, NULL, NULL);
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
	value.integer = createInteger(v);
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
		insertASTNode(node);
	}
	return node;
}

ASTNode createASTNode_float(float v) {
	ASTNode node = NULL;
	ASTNodeValue value;
	value.float_ = createFloat(v);
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
			op == OP_ASSIGN ? lc->name : createName_temp(),
			lc,
			rc,
			MyList_createList()
		);
		if(lc) MyList_pushElem(lc->parents, node);
		if(rc && rc != lc) MyList_pushElem(rc->parents, node);
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

char* getASTNodeStr(ASTNode node) {
	char* str = NULL;
	if (node->nodeType == AST_INTEGER) str = itostr(node->nodeValue.integer->value);
	else if (node->nodeType == AST_FLOAT) str = ftostr(node->nodeValue.float_->value);
	else str = node->name;
	return str;
}

int isInstantASTNode(ASTNode node, BasicType* ret_type, void** ret_value) {
	if (node->nodeType == AST_INTEGER) {
		if (ret_type)* ret_type = BASIC_INTEGER;
		if (ret_value)* ret_value = node->nodeValue.integer;
		return 1;
	}
	else if (node->nodeType == AST_FLOAT) {
		if (ret_type)* ret_type = BASIC_FLOAT;
		if (ret_value)* ret_value = node->nodeValue.float_;
		return 1;
	}
	else if(node->nodeType == AST_SYM && node->nodeValue.sym->type->kind == BASIC) {
		Sym sym = node->nodeValue.sym;
		if (sym->type == integerType) {
			if(ret_type)*ret_type = BASIC_INTEGER;
			if (ret_value)* ret_value = sym->instantValue;
		}
		else if (sym->type == floatType) {
			if(ret_type)*ret_type = BASIC_FLOAT;
			if (ret_value)* ret_value = sym->instantValue;
		}
		return 1;
	}
	else return 0;
}


int astIndent = -1;
void printASTTree(ASTNode node) {
	if (node == NULL) return;
	astIndent++;
	for (int i = 0; i < astIndent; i++)
		printf("\t");
	switch (node->nodeType) {
	case AST_INTEGER:	printf("integer: %s", itostr(node->nodeValue.integer->value)); break;
	case AST_FLOAT:		printf("float: %s", ftostr(node->nodeValue.float_->value)); break;
	case AST_SYM:		printf("symbal: %s --> var: %s", node->nodeValue.sym->name, node->name); break;
	case AST_FUNC:		printf("func: %s --> return var: %s", node->nodeValue.func->name, node->name); break;
	case AST_OP:		printf("op: %d --> var: %s", node->nodeValue.op, node->name); break;
	default:			printf("error!"); exit(-1);
	}
	printf("\n");
	printASTTree(node->lc);
	printASTTree(node->rc);
	astIndent--;
}