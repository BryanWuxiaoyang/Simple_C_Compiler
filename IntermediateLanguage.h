#pragma once
#include "OP.h"
#include "MyList.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum ILOP {
	ILOP_NONE,
	ILOP_LABEL,
	ILOP_FUNCTION,
	ILOP_ASSIGN,
	ILOP_PLUS,
	ILOP_MINUS,
	ILOP_MUL,
	ILOP_DIV,
	ILOP_ADDR,
	ILOP_GOTO,
	ILOP_IF_G,
	ILOP_IF_GE,
	ILOP_IF_E,
	ILOP_IF_NE,
	ILOP_IF_L,
	ILOP_IF_LE,
	ILOP_RETURN,
	ILOP_DEC,
	ILOP_ARG,
	ILOP_CALL,
	ILOP_PARAM,
	ILOP_READ,
	ILOP_WRITE
};
typedef enum ILOP ILOP;

ILOP getIfRelop(OP op) {
	switch (op) {
	case OP_G:	return ILOP_IF_G;
	case OP_GE: return ILOP_IF_GE;
	case OP_E:	return ILOP_IF_E;
	case OP_NE:	return ILOP_IF_NE;
	case OP_L:	return ILOP_IF_L;
	case OP_LE:	return ILOP_IF_LE;
	default:	assert(0);
	}
	return ILOP_NONE;
}

ILOP getAlgorithmOP(OP op) {
	switch (op) {
	case OP_PLUS:	return ILOP_PLUS;
	case OP_MINUS:	return ILOP_MINUS;
	case OP_STAR:	return ILOP_MUL;
	case OP_DIV:	return ILOP_DIV;
	default:		assert(0);
	}
	return ILOP_NONE;
}

struct _InterCode_ {
	const char* arg1;
	const char* arg2;
	const char* target;
	ILOP op;
};
typedef struct _InterCode_* InterCode;

InterCode createInterCode(const char* arg1,const char* arg2, const char* target, ILOP op) {
	InterCode code = (InterCode)malloc(sizeof(struct _InterCode_));
	if (code) {
		code->arg1 = arg1;
		code->arg2 = arg2;
		code->target = target;
		code->op = op;
	}
	return code;
}

ListHead interCodeList;

struct _CodeIterator_ {
	ListIterator it;
};
typedef struct _CodeIterator_* CodeIterator;

void initIL(){
	interCodeList = MyList_createList();
}

CodeIterator createCodeIterator() {
	CodeIterator it = (CodeIterator)malloc(sizeof(struct _CodeIterator_));
	if (it) {
		it->it = MyList_createIterator(interCodeList);
	}
	return it;
}

int hasNextCode(CodeIterator it) {
	return MyList_hasNext(it->it);
}

InterCode getNextCode(CodeIterator it) {
	return (InterCode)MyList_getNext(it->it);
}

ListHead getInterCodeList() {
	return interCodeList;
}

void appendInterCode(InterCode code) {
	MyList_pushElem(interCodeList, code);
}

void insertInterCode(CodeIterator it, InterCode code) {
	MyList_insert(it->it, code);
}

InterCode removeInterCode_prev(CodeIterator it) {
	return (InterCode)MyList_removePrev(it->it);
}

InterCode removeInterCode_next(CodeIterator it) {
	return (InterCode)MyList_removeNext(it->it);
}

void backpatchCode(ListHead codeList, char* label) {
	ListIterator it = MyList_createIterator(codeList);
	while (MyList_hasNext(it)) {
		InterCode code = (InterCode)MyList_getNext(it);
		code->target = label;
	}
	MyList_destroyIterator(it);
}

void printInterCode(InterCode code) {
	const char* arg1 = code->arg1;
	const char* arg2 = code->arg2;
	const char* target = code->target;
	switch (code->op) {
	case	ILOP_LABEL:		printf("LABEL %s :", target); break;
	case 	ILOP_FUNCTION:	printf("FUNCTION %s :", target); break;
	case 	ILOP_ASSIGN:	printf("%s := %s", target, arg1); break;
	case 	ILOP_PLUS:		printf("%s := %s + %s", target, arg1, arg2); break;
	case 	ILOP_MINUS:		printf("%s := %s - %s", target, arg1, arg2); break;
	case 	ILOP_MUL:		printf("%s := %s * %s", target, arg1, arg2); break;
	case 	ILOP_DIV:		printf("%s := %s / %s", target, arg1, arg2); break;
	case 	ILOP_ADDR:		printf("%s := &%s", target, arg1); break;
	case 	ILOP_GOTO:		printf("GOTO %s\n", target); break;
	case 	ILOP_IF_G:		printf("IF %s > %s GOTO %s", arg1, arg2, target); break;
	case 	ILOP_IF_GE:		printf("IF %s >= %s GOTO %s", arg1, arg2, target); break;
	case 	ILOP_IF_E:		printf("IF %s == %s GOTO %s", arg1, arg2, target); break;
	case 	ILOP_IF_NE:		printf("IF %s != %s GOTO %s", arg1, arg2, target); break;
	case 	ILOP_IF_L:		printf("IF %s < %s GOTO %s", arg1, arg2, target); break;
	case 	ILOP_IF_LE:		printf("IF %s <= %s GOTO %s", arg1, arg2, target); break;
	case 	ILOP_RETURN:	printf("RETURN %s", target); break;
	case 	ILOP_DEC:		printf("DEC %s [ %s ]", target, arg1); break;
	case 	ILOP_ARG:		printf("ARG %s", target); break;
	case 	ILOP_CALL:		printf("%s := CALL %s", target, arg1); break;
	case 	ILOP_PARAM:		printf("PARAM %s", target); break;
	case 	ILOP_READ:		printf("READ %s", target); break;
	case 	ILOP_WRITE:		printf("WRITE %s", target); break;
	}
	printf("\n");
}

void printInterCodeList() {
	ListIterator it = MyList_createIterator(interCodeList);
	while (MyList_hasNext(it)) {
		InterCode code = (InterCode)MyList_getNext(it);
		printInterCode(code);
	}
	MyList_destroyIterator(it);
}