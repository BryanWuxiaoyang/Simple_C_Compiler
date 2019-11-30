#pragma once
#include <assert.h>
#include <string.h>

enum FieldType{ FIELD_GLOBAL, FIELD_STRUCT, FIELD_COMPST };
typedef enum FieldType FieldType;

enum BasicType{ BASIC_INTEGER, BASIC_FLOAT, BASIC_ERROR };
typedef enum BasicType BasicType;

enum ConstType{ CONST_NONE, CONST_INTEGER, CONST_FLOAT, CONST_BOOLEAN };
typedef enum ConstType ConstType;

struct ConstValue {
	ConstType type;
	union {
		int constInteger;
		float constFloat;
	} v;
};
typedef struct ConstValue ConstValue;

void assignConstInteger(int value, ConstValue* dst) {
	dst->type = CONST_INTEGER;
	dst->v.constInteger = value;
}

void assignConstFloat(float value, ConstValue* dst) {
	dst->type = CONST_FLOAT;
	dst->v.constFloat = value;
}

void assignConstNone(ConstValue* dst) {
	dst->type = CONST_NONE;
}

void assignConstValue(ConstValue* src, ConstValue* dst) {
	switch (src->type) {
	case CONST_INTEGER: assignConstInteger(src->v.constInteger, dst); break;
	case CONST_FLOAT:	assignConstFloat(src->v.constFloat, dst); break;
	case CONST_NONE:	assignConstNone(dst);	break;
	default:			assert(0);
	}
}

int getConstInt(ConstValue v) {
	assert(v.type == CONST_INTEGER);
	return v.v.constInteger;
}

float getConstFloat(ConstValue v) {
	assert(v.type == CONST_FLOAT);
	return v.v.constFloat;
}

char* concatStr(const char* str1, const char* str2) {
	char* str = (char*)malloc(sizeof(char) * (strlen(str1) + strlen(str2) + 2));
	if(str) sprintf(str, "%s%s", str1, str2);
	return str;
}

int isEqualConstValue(ConstValue v1, ConstValue v2) {
	if (v1.type != v2.type) return 0;
	else {
		switch (v1.type) {
		case CONST_INTEGER:	return v1.v.constInteger == v2.v.constInteger;
		case CONST_FLOAT:	return v1.v.constFloat == v2.v.constFloat;
		case CONST_NONE:	return	1;
		}
	}
	assert(0);
	return 0;
}