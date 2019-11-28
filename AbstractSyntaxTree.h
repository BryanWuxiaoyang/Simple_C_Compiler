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
#include "ASTNode.h"
#include "ASTNodeHandler.h"

#define VAR_CONST_OPTMIZATION

#define CONST_VALUE_OPTMIZATION

#define SUB_EXP_OPTMIZATION

#define AST_MAX_HEIGHT 10

struct _ASTTable_ {
	ListHead handlerList;
};
typedef struct _ASTTable_* ASTTable;

struct _LeafASTNodeIterator_ {
	ListIterator handlerIt;
};
typedef struct _LeafASTNodeIterator_* LeafASTNodeIterator;

struct _InnerASTNodeIterator_ {
	ListIterator tableListIt;
	ListIterator handlerIt;
};
typedef struct _InnerASTNodeIterator_* InnerASTNodeIterator;

struct _AllASTNodeIterator_ {
	LeafASTNodeIterator leafIt;
	InnerASTNodeIterator innerIt;
};
typedef struct _AllASTNodeIterator_* AllASTNodeIterator;

ASTTable leafASTTable;
ListHead innerASTTableList;

LeafASTNodeIterator createLeafASTNodeIterator() {
	LeafASTNodeIterator handlerIt = (LeafASTNodeIterator)malloc(sizeof(struct _LeafASTNodeIterator_));
	if (handlerIt) {
		handlerIt->handlerIt = MyList_createIterator(leafASTTable->handlerList);
	}
	return handlerIt;
}

int hasNextLeafASTNode(LeafASTNodeIterator it) {
	return MyList_hasNext(it->handlerIt);
}

ASTNodeHandler getNextLeafASTNode(LeafASTNodeIterator it) {
	return (ASTNodeHandler)MyList_getNext(it->handlerIt);
}

void destroyLeafASTNodeIterator(LeafASTNodeIterator it) {
	MyList_destroyIterator(it->handlerIt);
	free(it);
}

InnerASTNodeIterator createInnerASTNodeIterator() {
	InnerASTNodeIterator it = (InnerASTNodeIterator)malloc(sizeof(struct _InnerASTNodeIterator_));
	if (it) {
		it->tableListIt = MyList_createIterator(innerASTTableList);
		ASTTable table = (ASTTable)MyList_getFront(innerASTTableList);
		it->handlerIt = MyList_createIterator(table->handlerList);
	}
	return it;
}

void destroyInnerASTNodeIterator(InnerASTNodeIterator it) {
	if (it->tableListIt) MyList_destroyIterator(it->tableListIt);
	if (it->handlerIt) MyList_destroyIterator(it->handlerIt);
	free(it);
}

int hasNextInnerASTNode(InnerASTNodeIterator it) {
	if (MyList_hasNext(it->handlerIt)) return 1;
	else{
		while (MyList_hasNext(it->tableListIt)) {
			ASTTable table = (ASTTable)MyList_getNext(it->tableListIt);
			MyList_destroyIterator(it->handlerIt);
			it->handlerIt = MyList_createIterator(table->handlerList);
			if (MyList_hasNext(it->handlerIt)) return 1;
		}
	}
	return 0;
}

ASTNodeHandler peekNextInnerASTNode(InnerASTNodeIterator it) {
	if (hasNextInnerASTNode(it)) {
		ASTNodeHandler handler = (ASTNodeHandler)MyList_peekNext(it->handlerIt);
		return handler;
	}
	assert(0);
	return NULL;
}

ASTNodeHandler getNextInnerASTNode(InnerASTNodeIterator it) {
	if (hasNextInnerASTNode(it)) {
		ASTNodeHandler handler = (ASTNodeHandler)MyList_getNext(it->handlerIt);
		return handler;
	}
	assert(0);
	return NULL;
}

void removeNextInnerASTNode(InnerASTNodeIterator it) {
	if (hasNextInnerASTNode(it)) {
		MyList_removeNext(it->handlerIt);
		return;
	}
	assert(0);
}

ASTTable createASTTable() {
	ASTTable table = (ASTTable)malloc(sizeof(struct _ASTTable_));
	if (table) {
		table->handlerList = MyList_createList();
	}
	return table;
}

void destroyASTTable(ASTTable table) {
	MyList_destroyList(table->handlerList);
	free(table);
}

AllASTNodeIterator createAllASTNodeIterator() {
	AllASTNodeIterator handlerIt = (AllASTNodeIterator)malloc(sizeof(struct _AllASTNodeIterator_));
	if (handlerIt) {
		handlerIt->innerIt = createInnerASTNodeIterator();
		handlerIt->leafIt = createLeafASTNodeIterator();
	}
	return handlerIt;
}

int hasNextAllASTNode(AllASTNodeIterator it) {
	if (hasNextLeafASTNode(it->leafIt)) return 1;
	else if (hasNextInnerASTNode(it->innerIt)) return 1;
	else return 0;
}

ASTNodeHandler getNextAllASTNode(AllASTNodeIterator it) {
	if (hasNextLeafASTNode(it->leafIt)) return getNextLeafASTNode(it->leafIt);
	else if (hasNextInnerASTNode(it->innerIt)) return getNextInnerASTNode(it->innerIt);
	else assert(0);
	return NULL;
}

void destroyAllASTNodeIterator(AllASTNodeIterator it) {
	destroyInnerASTNodeIterator(it->innerIt);
	destroyLeafASTNodeIterator(it->leafIt);
	free(it);
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

int maxHeight(ASTNode h1, ASTNode h2) {
	if (h1 && h2) return h1->height > h2->height ? h1->height : h2->height;
	else if (h1) return h1->height;
	else if (h2) return h2->height;
	else return -1;
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
	if (height == 0) return getLeafASTTable()->handlerList;
	else return getCurInnerASTTable()->handlerList;
}

ListHead getBackUpInnerTableList() {// ��ȡ�ӱ���ʽ�������屸�ݣ��Թ�ifelse�ķ�֧ʹ��
	ListHead resTableList = MyList_createList();
	ListIterator tableIt = MyList_createIterator(innerASTTableList);
	while (MyList_hasNext(tableIt)) {
		ASTTable table = (ASTTable)MyList_getNext(tableIt);
		ASTTable resTable = createASTTable();
		MyList_pushElem(resTableList, resTable);

		ListIterator handlerIt = MyList_createIterator(table->handlerList);
		while (MyList_hasNext(handlerIt)) {
			ASTNodeHandler handler = (ASTNodeHandler)MyList_getNext(handlerIt);
			MyList_pushElem(resTable->handlerList, handler);
		}
		MyList_destroyIterator(handlerIt);
	}
	MyList_destroyIterator(tableIt);
	return resTableList;
}

void switchInnerTableList(ListHead* tableList) {// ��tableList�ı���Ϊ��ǰ�ӱ���ʽ��������֮ǰ���ӱ���ʽ������tableList
	ListHead tmp = innerASTTableList;
	innerASTTableList = *tableList;
	*tableList = tmp;
}

void destroyInnerTableList(ListHead tableList) {
	ListIterator tableIt = MyList_createIterator(tableList);
	while (MyList_hasNext(tableIt)) {
		ASTTable table = (ASTTable)MyList_getNext(tableIt);
		destroyASTTable(table);
	}
	MyList_destroyIterator(tableIt);

	MyList_destroyList(tableList);
}

void mergeInnerTableList_intersect(ListHead tableList) {// ���ý����ķ�ʽ���ϲ�����tableList
	ListIterator dstTableIt = MyList_createIterator(innerASTTableList);
	ListIterator srcTableIt = MyList_createIterator(tableList);

	while (MyList_hasNext(dstTableIt) && MyList_hasNext(srcTableIt)) {
		ASTTable dstTable = (ASTTable)MyList_getNext(dstTableIt);
		ASTTable srcTable = (ASTTable)MyList_getNext(srcTableIt);

		ListIterator dstHandlerIt = MyList_createIterator(dstTable->handlerList);
		while (MyList_hasNext(dstHandlerIt)) {
			ASTNodeHandler dstHandler = (ASTNodeHandler)MyList_getNext(dstHandlerIt);
			int suc = 0;

			ListIterator srcHandlerIt = MyList_createIterator(srcTable->handlerList);
			while (MyList_hasNext(srcHandlerIt)) {
				ASTNodeHandler srcHandler = (ASTNodeHandler)MyList_getNext(srcHandlerIt);
				
				if (getASTNode(dstHandler) == getASTNode(srcHandler)) {
					suc = 1;
					break;
				}
			}
			MyList_destroyIterator(srcHandlerIt);

			if (suc == 0) {
				MyList_removePrev(dstHandlerIt);
			}
		}
		MyList_destroyIterator(dstHandlerIt);
		
	}
	MyList_destroyIterator(dstTableIt);
	MyList_destroyIterator(srcTableIt);
}

void clearInnerASTNodes() { // ɾ���ӱ���ʽ�ڵ�handler����ɾ��table
	ListIterator tableIt = MyList_createIterator(innerASTTableList);
	while (MyList_hasNext(tableIt)) {
		ASTTable table = (ASTTable)MyList_getNext(tableIt);
		MyList_clear(table->handlerList);
	}
	MyList_destroyIterator(tableIt);
}

ASTNodeHandler findASTNode(ASTNodeType type, ASTNodeValue value, ASTNode lc, ASTNode rc) {
	if (type == AST_OP) {
		InnerASTNodeIterator handlerIt = createInnerASTNodeIterator();
		while (hasNextInnerASTNode(handlerIt)) {
			ASTNodeHandler handler = getNextInnerASTNode(handlerIt);
			ASTNode node = getASTNode(handler);
			if (node->type == AST_OP && node->value.op == value.op && node->lc == lc && node->rc == rc){
				destroyInnerASTNodeIterator(handlerIt);
				return createASTNodeHandler(node);
			}
		}
		destroyInnerASTNodeIterator(handlerIt);
	}
	else {
		LeafASTNodeIterator handlerIt = createLeafASTNodeIterator();
		while (hasNextLeafASTNode(handlerIt)) {
			ASTNodeHandler handler = getNextLeafASTNode(handlerIt);
			ASTNode node = getASTNode(handler);

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
			default:assert(0);
			}

			if (suc) {
				destroyLeafASTNodeIterator(handlerIt);
				return createASTNodeHandler(node);
			}
		}
		destroyLeafASTNodeIterator(handlerIt);
	}
	return NULL;
}

ASTNodeHandler findASTNode_sym(Sym sym) {
	ASTNodeValue value;
	value.sym = sym;
	return findASTNode(AST_SYM, value, NULL, NULL);
}

ASTNodeHandler findASTNode_integer(int v) {
	ASTNodeValue value;
	value.intValue = v;
	return findASTNode(AST_INTEGER, value, NULL, NULL);
}

ASTNodeHandler findASTNode_float(float v) {
	ASTNodeValue value;
	value.floatValue = v;
	return findASTNode(AST_FLOAT, value, NULL, NULL);
}

ASTNodeHandler findASTNode_op(OP op, ASTNode lc, ASTNode rc) {
	ASTNodeValue value;
	value.op = op;
	return findASTNode(AST_OP, value, lc, rc);
}

void insertASTLeafNode(ASTNodeHandler handler) {
	ListHead list = getLeafASTTable()->handlerList;
	MyList_pushElem(list, handler);
}

void insertASTInnerNode(ASTNodeHandler handler) {
	ListHead list = getCurInnerASTTable()->handlerList;
	MyList_pushElem(list, handler);
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
		case CONST_INTEGER:	return itostr(getConstInt(node->constValue));
		case CONST_FLOAT:	return ftostr(getConstFloat(node->constValue));
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

void clearParents(ASTNode node) { // ����ӱ���ʽ��¼����������ڵ㱾��
	ListIterator parents_it = MyList_createIterator(node->parents);
	while (MyList_hasNext(parents_it)) {
		ASTNode pNode = (ASTNode)MyList_getNext(parents_it);
		
		InnerASTNodeIterator innerIt = createInnerASTNodeIterator();
		while (hasNextInnerASTNode(innerIt)) {
			ASTNodeHandler handler = peekNextInnerASTNode(innerIt);
			if (pNode == getASTNode(handler)) {
				destroyASTNodeHandler(handler);
				removeNextInnerASTNode(innerIt);
				break;
			}
			else {
				getNextInnerASTNode(innerIt);
			}
		}
		destroyInnerASTNodeIterator(innerIt);

		clearParents(pNode);
	}
	MyList_destroyIterator(parents_it);
	MyList_clear(node->parents);
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

ASTNodeHandler createASTNode_sym(Sym sym) {
	ASTNodeValue value;
	value.sym = sym;

	ASTNodeHandler handler = findASTNode(AST_SYM, value, NULL, NULL);
	if(handler == NULL){
		ASTNode node = createASTNode(
			0,
			AST_SYM,
			value,
			createName_sym(),
			NULL,
			NULL,
			MyList_createList(),
			sym->type
		);

		handler = createASTNodeHandler(node);
		insertASTLeafNode(handler);
	}
	return copyASTNodeHandler(handler);
}

ASTNodeHandler createASTNode_func(Func func) {
	ASTNodeValue value;
	value.func = func;
	ASTNode node = createASTNode(
			0,
			AST_FUNC,
			value,
			createName_temp(),
			NULL,
			NULL,
			MyList_createList(),
			func->returnType
		);
	ASTNodeHandler handler = createASTNodeHandler(node);
	insertASTLeafNode(handler);
	return copyASTNodeHandler(handler);
}

ASTNodeHandler createASTNode_integer(int v) {
	ASTNodeValue value;
	value.intValue = v;

	ASTNodeHandler handler = findASTNode(AST_INTEGER, value, NULL, NULL);

	if (handler == NULL){
		ASTNode node = createASTNode(
			0,
			AST_INTEGER,
			value,
			itostr(v),
			NULL,
			NULL,
			MyList_createList(),
			integerType
		);
		handler = createASTNodeHandler(node);

#ifdef CONST_VALUE_OPTMIZATION
		assignConstInteger(v, &node->constValue);
#endif

		insertASTLeafNode(handler);
	}
	return copyASTNodeHandler(handler);
}

ASTNodeHandler createASTNode_float(float v) {
	ASTNodeValue value;
	value.floatValue = v;

	ASTNodeHandler handler = findASTNode(AST_FLOAT, value, NULL, NULL);

	if (handler == NULL){
		ASTNode node = createASTNode(
			0,
			AST_FLOAT,
			value,
			ftostr(v),
			NULL,
			NULL,
			MyList_createList(),
			floatType
		);
		handler = createASTNodeHandler(node);

#ifdef CONST_VALUE_OPTMIZATION
		assignConstFloat(v, &node->constValue);
#endif

		insertASTLeafNode(handler);
	}
	return copyASTNodeHandler(handler);
}

ASTNodeHandler createASTNode_op(OP op, ASTNode lc, ASTNode rc) {
	ASTNodeValue value;
	value.op = op;
	if (op == OP_ASSIGN) {
		clearParents(lc);
	}
	ASTNodeHandler handler = NULL;
	ASTNode node = NULL;
#ifdef SUB_EXP_OPTMIZATION
	handler = findASTNode(AST_OP, value, lc, rc);
#endif
	if (handler) {
		node = getASTNode(handler);
	}
	else{
		node = createASTNode(
			maxHeight(lc, rc) + 1,
			AST_OP,
			value,
			op == OP_ASSIGN ? getASTNodeStr_r(rc) :
			op == OP_DEREF ? concatStr("*", getASTNodeStr_l(lc)) :
			op == OP_REF ? concatStr("&", getASTNodeStr_l(lc)) :
			createName_temp(),
			lc,
			rc,
			MyList_createList(),
			op == OP_DEREF ? lc->varType->u.targetType:
			op == OP_REF ? createType_addr(lc->varType):
			lc->varType
		);
		handler = createASTNodeHandler(node);
		if (lc) MyList_pushElem(lc->parents, node);
		if (rc && rc != lc) MyList_pushElem(rc->parents, node);
#ifdef SUB_EXP_OPTMIZATION
		insertASTInnerNode(handler);
#endif
	}

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
	else if (lc && rc && (isAlgorithmOp(op) || isRelop(op)) && isConstASTNode(lc) && isConstASTNode(rc)) {
		calConstValue(op, &lc->constValue, &rc->constValue, &node->constValue);
	}
	else if (lc && rc && (isCondOp(op) && isConstCond(op, lc, rc))) {
		calConstValue(op, &lc->constValue, &rc->constValue, &node->constValue);
	}
	else if (lc && (rc == NULL) && (isAlgorithmOp(op) || isCondOp(op)) && isConstASTNode(lc)) {
		calConstValue(op, &lc->constValue, NULL, &node->constValue);
	}
	else {
		assignConstNone(&node->constValue);
	}
#endif

#ifdef SUB_EXP_OPTMIZATION
	return copyASTNodeHandler(handler);
#else
	return handler;
#endif
}

int astIndent = -1;

void printASTNode(ASTNode node) {
	for (int i = 0; i < astIndent; i++)
		printf("\t");
	switch (node->type) {
	case AST_INTEGER:	printf("integer: %s", itostr(node->value.intValue)); break;
	case AST_FLOAT:		printf("float: %s", ftostr(node->value.floatValue)); break;
	case AST_SYM:		printf("symbal: %s --> var: %s", node->value.sym->name, node->name); break;
	case AST_FUNC:		printf("func: %s --> return var: %s", node->value.func->name, node->name); break;
	case AST_OP:		printf("op: %d --> var: %s, lc: %s, rc: %s", node->value.op, node->name, getASTNodeStr_r(node->lc), getASTNodeStr_r(node->rc)); break;
	default:			printf("error!"); assert(0);
	}
	printf("isConst: %d", isConstASTNode(node));
	printf("\n");
}

void printASTTree(ASTNode node) {
	if (node == NULL) return;
	astIndent++;
	printASTNode(node);
	printASTTree(node->lc);
	printASTTree(node->rc);
	astIndent--;
}

void printASTNodes() {
	printf("astNodes:\n");
	ListIterator handlerIt = MyList_createIterator(getLeafASTTable()->handlerList);
	while (MyList_hasNext(handlerIt)) {
		ASTNodeHandler handler = (ASTNodeHandler)MyList_getNext(handlerIt);
		printASTNode(getASTNode(handler));
	}
	MyList_destroyIterator(handlerIt);

	InnerASTNodeIterator it2 = createInnerASTNodeIterator();
	while (hasNextInnerASTNode(it2)) {
		ASTNodeHandler handler = getNextInnerASTNode(it2);
		printASTNode(getASTNode(handler));
	}
	destroyInnerASTNodeIterator(it2);
}