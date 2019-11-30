#pragma once
#include "defs.h"
#include "Type.h"
#include "Sym.h"
#include <string.h>

struct _SymTable_ {
	ListHead head;
	int type;
};
typedef struct _SymTable_* SymTable;

SymTable createSymTable(FieldType type) {
	SymTable table = (SymTable)malloc(sizeof(struct _SymTable_));
	if (table) {
		table->head = MyList_createList();
		table->type = type;
	}
	return table;
}

ListHead symTableList;
SymTable globalSymTable;
SymTable curSymTable;

SymTable getCurSymTable() {
	return curSymTable;
}

SymTable getGlobalSymTable() {
	return globalSymTable;
}

void pushSymTable(SymTable table) {
	MyList_pushElem(symTableList, table);
	curSymTable = table;
}

SymTable popSymTable() {
	SymTable table = (SymTable)MyList_pop(symTableList);
	ListIterator handlerIt = MyList_createReverseIterator(symTableList);
	curSymTable = (SymTable)MyList_getPrev(handlerIt);
	MyList_destroyIterator(handlerIt);
	return table;
}

Sym createSym(const char* name, Type type) {
	Sym sym = (Sym)malloc(sizeof(struct _Sym_));
	if (sym) {
		sym->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
		if(sym->name) strcpy(sym->name, name);
		sym->type = type;
		sym->offset = 0;
	}
	return sym;
}

void insertSym(SymTable table, Sym sym) {
	MyList_pushElem(table->head, sym);
}

Sym findSym(SymTable table, const char* name) {
	ListIterator handlerIt = MyList_createIterator(table->head);
	Sym res = NULL;
	while (MyList_hasNext(handlerIt)) {
		Sym sym = (Sym)MyList_getNext(handlerIt);
		if (strcmp(sym->name, name) == 0) {
			res = sym;
			break;
		}
	}
	MyList_destroyIterator(handlerIt);
	return res;
}

Sym findSym_all(const char* name) {
	ListIterator handlerIt = MyList_createReverseIterator(symTableList);
	Sym res = NULL;
	while (MyList_hasPrev(handlerIt)) {
		SymTable table = (SymTable)MyList_getPrev(handlerIt);
		res = findSym(table, name);
		if (res) break;
	}
	MyList_destroyIterator(handlerIt);
	return res;
}

void initSymTable() {
	symTableList = MyList_createList();
	globalSymTable = createSymTable(FIELD_GLOBAL);
	curSymTable = globalSymTable;
	MyList_pushElem(symTableList, globalSymTable);
}

void printSymTable() {
	printf("symbal tables:\n");
	ListIterator handlerIt = MyList_createIterator(symTableList);
	while (MyList_hasNext(handlerIt)) {
		SymTable table = (SymTable)MyList_getNext(handlerIt);
		printf("\tsymtable: ");
		switch (table->type) {
		case FIELD_GLOBAL: printf("global"); break;
		case FIELD_STRUCT: printf("struct"); break;
		case FIELD_COMPST: printf("compst"); break;
		}
		printf("\n");

		ListIterator it2 = MyList_createIterator(table->head);
		while (MyList_hasNext(it2)) {
			Sym sym = (Sym)MyList_getNext(it2);
			printf("\t\tsymbal %s, type: %s, type size: %d, offset: %d\n", sym->name, sym->type->name, sym->type->size, sym->offset);
		}
		MyList_destroyIterator(it2);
	}
	MyList_destroyIterator(handlerIt);
}