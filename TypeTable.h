#pragma once
#include "MyList.h"
#include "defs.h"
#include "Sym.h"
#include "Type.h"
#include <string.h>



struct _TypeTable_ {
	ListHead head;
	int type;
};
typedef struct _TypeTable_* TypeTable;


TypeTable createTypeTable(FieldType type) {
	TypeTable table = (TypeTable)malloc(sizeof(struct _TypeTable_));
	if (table) {
		table->head = MyList_createList();
		table->type = type;
	}
	return table;
}

ListHead typeTableList;
TypeTable globalTypeTable;
TypeTable curTypeTable;

void pushTypeTable(TypeTable table) {
	if (table) {
		MyList_pushElem(typeTableList, table);
		curTypeTable = table;
	}
}

TypeTable popTypeTable() {
	TypeTable table = (TypeTable)MyList_pop(typeTableList);
	ListIterator it = MyList_createReverseIterator(typeTableList);
	curTypeTable = (TypeTable)MyList_getPrev(it);
	MyList_destroyIterator(it);
	return table;
}

TypeTable getCurTypeTable() {
	return curTypeTable;
}

TypeTable getGlobalTypeTable() {
	return globalTypeTable;
}

Type findType(TypeTable table, const char* name) {
	ListIterator it = MyList_createIterator(table->head);
	Type res = NULL;
	while (MyList_hasNext(it)) {
		Type type = (Type)MyList_getNext(it);
		if (strcmp(type->name, name) == 0) {
			res = type;
			break;
		}
	}
	MyList_destroyIterator(it);
	return res;
}

Type findType_all(const char* name) {
	ListIterator it = MyList_createReverseIterator(typeTableList);
	Type res = NULL;
	while (MyList_hasPrev(it)) {
		TypeTable table = (TypeTable)MyList_getPrev(it);
		res = findType(table, name);
		if (res) break;
	}
	MyList_destroyIterator(it);
	return res;
}

void fillType_basic(Type type, const char* name, int basicId) {
	type->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(type->name, name);
	type->kind = BASIC;
	type->u.basic = basicId;
}

void fillType_array(Type type, const char* name, Type elemType, int size) {
	type->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(type->name, name);
	type->kind = ARRAY;
	type->u.array.elemType = elemType;
	type->u.array.size = size;
}

void fillType_structure(Type type, const char* name, ListHead fieldList) {
	type->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(type->name, name);
	type->kind = STRUCTURE;
	type->u.fieldList = fieldList;
}

Type createType_basic(const char* name, int basicId) {
	Type type = (Type)malloc(sizeof(struct _Type_));
	if (type) {
		fillType_basic(type, name, basicId);
	}
	return type;
}

Type createType_array(const char* name, Type elemType, int size) {
	Type type = (Type)malloc(sizeof(struct _Type_));
	if (type) {
		fillType_array(type, name, elemType, size);
	}
	return type;
}

Type createType_structure(const char* name, ListHead fieldList) {
	Type type = (Type)malloc(sizeof(struct _Type_));
	if (type) {
		fillType_structure(type, name, fieldList);
	}
	return type;
}


void insertType(TypeTable table, Type type) {
	MyList_pushElem(table->head, type);
}

Type integerType;
Type floatType;
Type errorType;


void initTypeTable() {
	typeTableList = MyList_createList();
	globalTypeTable = createTypeTable(FIELD_GLOBAL);
	curTypeTable = globalTypeTable;
	MyList_pushElem(typeTableList, globalTypeTable);

	integerType = createType_basic("integer", 1);
	floatType = createType_basic("float", 2);
	errorType = createType_basic("erorr", 3);
	insertType(getCurTypeTable(), integerType);
	insertType(getCurTypeTable(), floatType);
	insertType(getCurTypeTable(), errorType);
}

void printTypeTable() {
	printf("type tables:\n");
	ListIterator it = MyList_createIterator(typeTableList);
	while (MyList_hasNext(it)) {
		TypeTable table = (TypeTable)MyList_getNext(it);
		printf("\typetable: ");
		switch (table->type) {
		case FIELD_GLOBAL: printf("global"); break;
		case FIELD_STRUCT: printf("struct"); break;
		case FIELD_COMPST: printf("compst"); break;
		}
		printf("\n");

		ListIterator it2 = MyList_createIterator(table->head);
		while (MyList_hasNext(it2)) {
			Type type = (Type)MyList_getNext(it2);
			printf("\t\ttype %s, kind ", type->name);
			if (type->kind == BASIC) {
				printf("basic, %d\n", type->u.basic);
			}
			else if (type->kind == ARRAY) {
				printf("array, size %d, elem type %s\n", type->u.array.size, type->u.array.elemType->name);
			}
			else if (type->kind == STRUCTURE) {
				printf("struct, fieldList:\n");
				ListIterator it3 = MyList_createIterator(type->u.fieldList);
				while (MyList_hasNext(it3)) {
					Sym sym = (Sym)MyList_getNext(it3);
					printf("\t\t\tsymbal %s, type %s, offset %d\n", sym->name, sym->type->name, sym->offset);
				}
				MyList_destroyIterator(it3);
			}
		}
		MyList_destroyIterator(it2);
	}
	MyList_destroyIterator(it);
}