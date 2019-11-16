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

void printInterCode(InterCode code);

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
	ListIterator handlerIt;
};
typedef struct _CodeIterator_* CodeIterator;

void initIL(){
	interCodeList = MyList_createList();
}

CodeIterator createCodeIterator() {
	CodeIterator handlerIt = (CodeIterator)malloc(sizeof(struct _CodeIterator_));
	if (handlerIt) {
		handlerIt->handlerIt = MyList_createIterator(interCodeList);
	}
	return handlerIt;
}

int hasNextCode(CodeIterator handlerIt) {
	return MyList_hasNext(handlerIt->handlerIt);
}

InterCode getNextCode(CodeIterator handlerIt) {
	return (InterCode)MyList_getNext(handlerIt->handlerIt);
}

ListHead getInterCodeList() {
	return interCodeList;
}

void appendInterCode(InterCode code) {
	MyList_pushElem(interCodeList, code);
}

void insertInterCode(CodeIterator handlerIt, InterCode code) {
	MyList_insert(handlerIt->handlerIt, code);
}

InterCode removeInterCode_prev(CodeIterator handlerIt) {
	return (InterCode)MyList_removePrev(handlerIt->handlerIt);
}

InterCode removeInterCode_next(CodeIterator handlerIt) {
	return (InterCode)MyList_removeNext(handlerIt->handlerIt);
}

void backpatchCode(ListHead codeList, const char* label) {
	ListIterator handlerIt = MyList_createIterator(codeList);
	while (MyList_hasNext(handlerIt)) {
		InterCode code = (InterCode)MyList_getNext(handlerIt);
		code->target = label;
	}
	MyList_destroyIterator(handlerIt);
}

InterCode getLastCode() {
	return (InterCode)MyList_getBack(interCodeList);
}

int removeCode(InterCode code) {
	int suc = 0;
	ListIterator it = MyList_createIterator(getInterCodeList());
	while (MyList_hasNext(it)) {
		InterCode code2 = (InterCode)MyList_getNext(it);
		if (code == code2) {
			MyList_removePrev(it);
			suc = 1;
			break;
		}
	}
	MyList_destroyIterator(it);
	return suc;
}

void printInterCode(InterCode code, FILE* file, char* buffer) {
	const char* arg1 = code->arg1;
	const char* arg2 = code->arg2;
	const char* target = code->target;
	char buf[100];
	switch (code->op) {
	case	ILOP_LABEL:		sprintf(buf, "LABEL %s :", target); break;
	case 	ILOP_FUNCTION:	sprintf(buf, "FUNCTION %s :", target); break;
	case 	ILOP_ASSIGN:	sprintf(buf, "%s := %s", target, arg1); break;
	case 	ILOP_PLUS:		sprintf(buf, "%s := %s + %s", target, arg1, arg2); break;
	case 	ILOP_MINUS:		sprintf(buf, "%s := %s - %s", target, arg1, arg2); break;
	case 	ILOP_MUL:		sprintf(buf, "%s := %s * %s", target, arg1, arg2); break;
	case 	ILOP_DIV:		sprintf(buf, "%s := %s / %s", target, arg1, arg2); break;
	case 	ILOP_ADDR:		sprintf(buf, "%s := &%s", target, arg1); break;
	case 	ILOP_GOTO:		sprintf(buf, "GOTO %s", target); break;
	case 	ILOP_IF_G:		sprintf(buf, "IF %s > %s GOTO %s", arg1, arg2, target); break;
	case 	ILOP_IF_GE:		sprintf(buf, "IF %s >= %s GOTO %s", arg1, arg2, target); break;
	case 	ILOP_IF_E:		sprintf(buf, "IF %s == %s GOTO %s", arg1, arg2, target); break;
	case 	ILOP_IF_NE:		sprintf(buf, "IF %s != %s GOTO %s", arg1, arg2, target); break;
	case 	ILOP_IF_L:		sprintf(buf, "IF %s < %s GOTO %s", arg1, arg2, target); break;
	case 	ILOP_IF_LE:		sprintf(buf, "IF %s <= %s GOTO %s", arg1, arg2, target); break;
	case 	ILOP_RETURN:	sprintf(buf, "RETURN %s", target); break;
	case 	ILOP_DEC:		sprintf(buf, "DEC %s %s", target, arg1); break;
	case 	ILOP_ARG:		sprintf(buf, "ARG %s", target); break;
	case 	ILOP_CALL:		sprintf(buf, "%s := CALL %s", target, arg1); break;
	case 	ILOP_PARAM:		sprintf(buf, "PARAM %s", target); break;
	case 	ILOP_READ:		sprintf(buf, "READ %s", target); break;
	case 	ILOP_WRITE:		sprintf(buf, "WRITE %s", target); break;
	}

	if (file) {
		fprintf(file, "%s\n", buf);
	}
	else if (buffer) {
		sprintf(buffer, "%s\n", buf);
	}
	else {
		printf("%s\n", buf);
	}
}

void printInterCodeList(FILE* file, char* buffer) {
	ListIterator handlerIt = MyList_createIterator(interCodeList);
	while (MyList_hasNext(handlerIt)) {
		InterCode code = (InterCode)MyList_getNext(handlerIt);
		printInterCode(code, file, buffer);
	}
	MyList_destroyIterator(handlerIt);
}