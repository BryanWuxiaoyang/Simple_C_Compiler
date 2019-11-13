#pragma once
#include "TypeTable.h"

struct _Sym_ {
	char* name;
	Type type;
	int offset;
};
typedef struct _Sym_* Sym;

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

ListHead symTableList = MyList_createList();
SymTable globalSymTable = createSymTable(FIELD_GLOBAL);
SymTable curSymTable = globalSymTable;

void initSymTable() {
	curSymTable = globalSymTable;
	MyList_pushElem(symTableList, globalSymTable);
}

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
	ListIterator it = MyList_createReverseIterator(symTableList);
	curSymTable = (SymTable)MyList_getPrev(it);
	MyList_destroyIterator(it);
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
	ListIterator it = MyList_createIterator(table->head);
	Sym res = NULL;
	while (MyList_hasNext(it)) {
		Sym sym = (Sym)MyList_getNext(it);
		if (strcmp(sym->name, name) == 0) {
			res = sym;
			break;
		}
	}
	MyList_destroyIterator(it);
	return res;
}

Sym findSym_all(const char* name) {
	ListIterator it = MyList_createReverseIterator(symTableList);
	Sym res = NULL;
	while (MyList_hasPrev(it)) {
		SymTable table = (SymTable)MyList_getPrev(it);
		res = findSym(table, name);
		if (res) break;
	}
	MyList_destroyIterator(it);
	return res;
}

void printSymTable() {
	printf("symbal tables:\n");
	ListIterator it = MyList_createIterator(symTableList);
	while (MyList_hasNext(it)) {
		SymTable table = (SymTable)MyList_getNext(it);
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
			printf("\t\tsymbal %s, type: %s, offset: %d\n", sym->name, sym->type->name, sym->offset);
		}
		MyList_destroyIterator(it2);
	}
	MyList_destroyIterator(it);
}