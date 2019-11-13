#pragma once
#include "SymTable.h"

struct _Func_ {
	char* name;
	ListHead paramList;
	Type returnType;
	int state; // 0: unimplemented, 1: implemented
	int decLineno;
	int defLineno;
};
typedef struct _Func_* Func;

struct _FuncTable_ {
	ListHead head;
};
typedef struct _FuncTable_* FuncTable;

FuncTable createFuncTable() {
	FuncTable table = (FuncTable)malloc(sizeof(struct _FuncTable_));
	if (table) {
		table->head = MyList_createList();
	}
	return table;
}

FuncTable funcTable = createFuncTable();

void initFuncTable(){}

FuncTable getCurFuncTable() {
	return funcTable;
}

FuncTable getGlobalFuncTable() {
	return funcTable;
}

Func createFunc_raw() {
	return (Func)malloc(sizeof(struct _Func_));
}

Func createFunc(const char* name, ListHead paramList, Type returnType) {
	Func func = (Func)malloc(sizeof(struct _Func_));
	if (func) {
		func->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
		if(func->name) strcpy(func->name, name);
		func->paramList = paramList;
		func->returnType = returnType;
		func->state = 0;
		func->decLineno = 0;
		func->defLineno = 0;
	}
	return func;
}

void fillFunc(Func func, const char* name, ListHead paramList, Type returnType) {
	strcpy(func->name, name);
	func->paramList = paramList;
	func->returnType = returnType;
	func->state = 0;
	func->decLineno = 0;
	func->defLineno = 0;
}

Func findFunc(FuncTable table, const char* name) {
	ListIterator it = MyList_createIterator(table->head);
	Func res = NULL;
	while (MyList_hasNext(it)) {
		Func sym = (Func)MyList_getNext(it);
		if (strcmp(sym->name, name) == 0) {
			res = sym;
			break;
		}
	}
	MyList_destroyIterator(it);
	return res;
}

Func findFunc_all(const char* name) {
	return findFunc(getCurFuncTable(), name);
}

void insertFunc(FuncTable table, Func func) {
	MyList_pushElem(table->head, func);
}

void declareFunc(Func func, int decLineno) {
	func->state = 0;
	func->decLineno = decLineno;
}

void defineFunc(Func func, int defLineno) {
	func->state = 1;
	func->defLineno = defLineno;
}

int isDeclaredFunc(Func func) {
	if (func && func->state == 0) return 1;
	else return 0;
}

int isDefinedFunc(Func func) {
	if (func && func->state == 1) return 1;
	else return 0;
}

void printFuncTable() {
	printf("func table:\n");
	ListIterator it = MyList_createIterator(funcTable->head);
	while (MyList_hasNext(it)) {
		Func func = (Func)MyList_getNext(it);
		printf("\tname %s, return type %s, param list:\n", func->name, func->returnType->name);
		ListIterator it2 = MyList_createIterator(func->paramList);
		while (MyList_hasNext(it2)) {
			Sym sym = (Sym)MyList_getNext(it2);
			printf("\t\tsymbal %s, type %s, offset %d\n", sym->name, sym->type->name, sym->offset);
		}
		MyList_destroyIterator(it2);
	}
	MyList_destroyIterator(it);
}