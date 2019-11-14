#pragma once
#include <assert.h>

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

void assignConstValue(ConstValue* src, ConstValue* dst) {
	switch (src->type) {
	case CONST_INTEGER: assignConstInteger(src->v.constInteger, dst); break;
	case CONST_FLOAT:	assignConstFloat(src->v.constFloat, dst); break;
	default:			exit(-1);
	}
}

int getConstInt(ConstValue* v) {
	assert(v->type == CONST_INTEGER);
	return v->v.constInteger;
}

float getConstFloat(ConstValue* v) {
	assert(v->type == CONST_FLOAT);
	return v->v.constFloat;
}