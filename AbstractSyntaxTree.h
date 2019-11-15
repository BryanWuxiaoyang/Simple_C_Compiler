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

//#define VAR_CONST_OPTMIZATION

//#define CONST_VALUE_OPTMIZATION

#define SUB_EXP_OPTMIZATION

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
	int removeTag;
};
typedef struct _ASTNode_* ASTNode;

struct _ASTTable_ {
	ListHead list;
};
typedef struct _ASTTable_* ASTTable;

struct _InnerASTTableListIterator_ {
	ListIterator tableListIt;
	ListIterator nodeIt;
};
typedef struct _InnerASTTableListIterator_* InnerASTNodeIterator;

ASTTable leafASTTable;
ListHead innerASTTableList;

InnerASTNodeIterator createInnerASTNodeIterator() {
	InnerASTNodeIterator it = (InnerASTNodeIterator)malloc(sizeof(struct _InnerASTTableListIterator_));
	if (it) {
		it->tableListIt = MyList_createIterator(innerASTTableList);
		ASTTable table = (ASTTable)MyList_getFront(innerASTTableList);
		it->nodeIt = MyList_createIterator(table->list);
	}
	return it;
}

void destroyInnerASTNodeIterator(InnerASTNodeIterator it) {
	if (it->tableListIt) MyList_destroyIterator(it->tableListIt);
	if (it->nodeIt) MyList_destroyIterator(it->nodeIt);
	free(it);
}

int hasNextInnerASTNode(InnerASTNodeIterator it) {
	if (MyList_hasNext(it->nodeIt)) return 1;
	else if (MyList_hasNext(it->tableListIt)) return 1;
	else return 0;
}

ASTNode getNextInnerASTNode(InnerASTNodeIterator it) {
	if (MyList_hasNext(it->nodeIt)) return (ASTNode)MyList_getNext(it->nodeIt);
	else if (MyList_hasNext(it->tableListIt)) {
		MyList_destroyIterator(it->nodeIt);
		ASTTable table;
		do {
			table = (ASTTable)MyList_getNext(it->tableListIt);
		} while (MyList_isEmpty(table->list) == 1);
		it->nodeIt = MyList_createIterator(table->list);
		return (ASTNode)MyList_getNext(it->nodeIt);
	}
	else assert(0);
	return NULL;
}

ASTTable createASTTable() {
	ASTTable table = (ASTTable)malloc(sizeof(struct _ASTTable_));
	if (table) {
		table->list = MyList_createList();
	}
	return table;
}

void initAST() {
	leafASTTable = createASTTable();
	innerASTTableList = MyList_createList();
	MyList_pushElem(innerASTTableList, createASTTable());
}

ASTTable getLeafASTTable() {
	return leafASTTable;
}

ASTTable getCurInnerASTTable() {
	return (ASTTable)MyList_getBack(innerASTTableList);
}

void pushInnerASTTable() {
	MyList_pushElem(innerASTTableList, createASTTable());
}

void popInnerASTTable() {
	MyList_pop(innerASTTableList);
}

void removeASTNodes() {
	ListIterator tableListIt = MyList_createIterator(innerASTTableList);
	while (MyList_hasNext(tableListIt)) {
		ASTTable table = (ASTTable)MyList_getNext(tableListIt);

		ListIterator nodeIt = MyList_createIterator(table->list);
		while (MyList_hasNext(nodeIt)) {
			ASTNode node = (ASTNode)MyList_getNext(nodeIt);
			if (node->removeTag > 0) MyList_removePrev(nodeIt);
		}
		MyList_destroyIterator(nodeIt);
	}
	MyList_destroyIterator(tableListIt);
}

int maxHeight(ASTNode h1, ASTNode h2) {
	if (h1 && h2) return h1->height > h2->height ? h1->height : h2->height;
	else if (h1) return h1->height;
	else if (h2) return h2->height;
	else return 0;
}


int isConstASTNode(ASTNode node) {
#ifdef CONST_VALUE_OPTMIZATION
	return node->constValue.type != CONST_NONE;
#else
	return 0;
#endif
}

ConstValue getConstValue(ASTNode node) {
	return node->constValue;
}


ListHead getASTNodeList(int height) {
	if (height == 0) return getLeafASTTable()->list;
	else return getCurInnerASTTable()->list;
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
			suc = (node->removeTag == 0 && node->type == AST_INTEGER && node->value.intValue == value.intValue && node->lc == lc && node->rc == rc) ? 1 : 0;
			break;
		case AST_FLOAT:
			suc = (node->removeTag == 0 && node->type == AST_FLOAT && node->value.floatValue == value.floatValue && node->lc == lc && node->rc == rc) ? 1 : 0;
			break;
		case AST_SYM:
			suc = (node->removeTag == 0 && node->type == AST_SYM && node->value.sym == value.sym && node->lc == lc && node->rc == rc) ? 1 : 0;
			break;
		case AST_FUNC:
			suc = (node->removeTag == 0 && node->type == AST_FUNC && node->value.func == value.func && node->lc == lc && node->rc == rc) ? 1 : 0;
			break;
		case AST_OP:
			suc = (node->removeTag == 0 && node->type == AST_OP && node->value.op == value.op && node->lc == lc && node->rc == rc) ? 1 : 0;
			break;
		default:assert(0);
		}
		if (suc) {
			res = node;
			break;
		}
	}
	MyList_destroyIterator(it);
	return res;
}

ASTNode findASTNode_sym(Sym sym) {
	ASTNodeValue value;
	value.sym = sym;
	return findASTNode(0, AST_SYM, value, NULL, NULL);
}

ASTNode findASTNode_integer(int v) {
	ASTNodeValue value;
	value.intValue = v;
	return findASTNode(0, AST_INTEGER, value, NULL, NULL);
}

ASTNode findASTNode_float(float v) {
	ASTNodeValue value;
	value.floatValue = v;
	return findASTNode(0, AST_FLOAT, value, NULL, NULL);
}

ASTNode findASTNode_op(OP op, ASTNode lc, ASTNode rc) {
	ASTNodeValue value;
	value.op = op;
	return findASTNode(1 + maxHeight(lc, rc), AST_OP, value, lc, rc);
}

void insertASTNode(ASTNode node) {
	int height = node->height;
	ListHead list = getASTNodeList(height);
	MyList_pushElem(list, node);
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
#ifdef CONST_VALUE_OPTMIZATION
	if (isConstASTNode(node)) {
		switch (node->constValue.type) {
		case CONST_INTEGER:	return itostr(getConstInt(&node->constValue));
		case CONST_FLOAT:	return ftostr(getConstFloat(&node->constValue));
		}
	}
#endif
	return node->name;
}

char* getASTNodeStr_l(ASTNode node) {
	return node->name;
}

int isConstCond(OP op, ASTNode node1, ASTNode node2) {
	switch (op) {
	case OP_AND:	return (isConstASTNode(node1) && getConstInt(getConstValue(node1)) == 0) || (isConstASTNode(node2) && getConstInt(getConstValue(node2)) == 0);
	case OP_OR:		return (isConstASTNode(node1) && getConstInt(getConstValue(node1)) == 1) || (isConstASTNode(node2) && getConstInt(getConstValue(node2)) == 1);
	case OP_NOT:	return isConstASTNode(node1);
	default:		assert(0);
	}
	return 0;
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
				//MyList_removePrev(list_it);
				list_node->removeTag = 1;
				break;
			}
		}
		MyList_destroyIterator(list_it);

	}
	MyList_destroyIterator(parents_it);
	MyList_clear(lc->parents);
}

void calConstValue(OP op, ConstValue* lv, ConstValue* rv, ConstValue* ret_value) {
	ConstType type = lv->type;
	ret_value->type = type;
	if (lv && rv) {
		if (type == CONST_INTEGER) {
			assignConstInteger(procOP2_int(op, getConstInt(*lv), getConstInt(*rv)), ret_value);
		}
		else if (type == CONST_FLOAT) {
			assignConstFloat(procOP2_float(op, getConstFloat(*lv), getConstFloat(*rv)), ret_value);
		}
	}
	else if (lv) {
		if (type == CONST_INTEGER) {
			assignConstInteger(procOP1_int(op, getConstInt(*lv)), ret_value);
		}
		else if (type == CONST_FLOAT) {
			assignConstFloat(procOP1_float(op, getConstFloat(*lv)), ret_value);
		}
	}
	else {
		assert(0);
	}
}

int astNodeIdCnt = 1;

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
		node->removeTag = 0;
		assignConstNone(&node->constValue);
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
			itostr(v),
			NULL,
			NULL,
			MyList_createList()
		);

#ifdef CONST_VALUE_OPTMIZATION
		assignConstInteger(v, &node->constValue);
#endif

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
			ftostr(v),
			NULL,
			NULL,
			MyList_createList()
		);

#ifdef CONST_VALUE_OPTMIZATION
		assignConstFloat(v, &node->constValue);
#endif

		insertASTNode(node);
	}
	return node;
}

ASTNode createASTNode_op(OP op, ASTNode lc, ASTNode rc) {
	ASTNode node = NULL;
	ASTNodeValue value;
	value.op = op;
	if (op == OP_ASSIGN) {
		clearParents(lc);
	}

#ifdef SUB_EXP_OPTMIZATION
	node = findASTNode(maxHeight(lc, rc) + 1, AST_OP, value, lc, rc);
#endif

	if (node == NULL) {
		node = createASTNode_raw(
			maxHeight(lc, rc) + 1,
			AST_OP,
			value,
			op == OP_ASSIGN ? getASTNodeStr_r(rc) :
			op == OP_DEREF ? concatStr("*", getASTNodeStr_l(lc)):
			createName_temp(),
			lc,
			rc,
			MyList_createList()
		);

#ifdef CONST_VALUE_OPTMIZATION
		if (op == OP_ASSIGN) {
			if (isConstASTNode(rc)) {
				assignConstValue(&rc->constValue, &node->constValue);

#ifdef VAR_CONST_OPTMIZATION
				assignConstValue(&rc->constValue, &lc->constValue);
#endif
			}
			else {
				assignConstNone(&lc->constValue);
				assignConstNone(&node->constValue);
			}
		}
		else if (lc && rc && isAlgorithmOp(op) && isConstASTNode(lc) && isConstASTNode(rc)) {
			calConstValue(op, &lc->constValue, &rc->constValue, &node->constValue);
		}
		else if (lc && rc && (isRelop(op) || (isCondOp(op) && isConstCond(op, lc, rc)))) {
			calConstValue(op, &lc->constValue, &rc->constValue, &node->constValue);
		}
		else if (lc && (rc == NULL) && (isAlgorithmOp(op) || isCondOp(op)) && isConstASTNode(lc)) {
			calConstValue(op, &lc->constValue, NULL, &node->constValue);
		} 
#endif

		if (lc) MyList_pushElem(lc->parents, node);
		if (rc && rc != lc) MyList_pushElem(rc->parents, node);
		insertASTNode(node);
	}
	return node;
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
	default:			printf("error!"); assert(0);
	}
	printf("\n");
	printASTTree(node->lc);
	printASTTree(node->rc);
	astIndent--;
}