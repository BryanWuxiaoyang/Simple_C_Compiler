#pragma once
#include "MyList.h"
enum TypeKind{ BASIC, ARRAY, STRUCTURE, ADDR };
typedef enum TypeKind TypeKind;

struct _Type_ {
	char* name;
	TypeKind kind;
	union {
		int basic;
		struct { struct _Type_* elemType; int size; } array;
		ListHead fieldList;
		struct _Type_* targetType;
	} u;
	int size;
};
typedef struct _Type_* Type;

Type integerType;
Type floatType;
Type errorType;
