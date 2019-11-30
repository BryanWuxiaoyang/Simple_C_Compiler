#pragma once
#include "assert.h"
enum OP {
	OP_ASSIGN,
	OP_AND,
	OP_OR,
	OP_G,
	OP_GE,
	OP_E,
	OP_NE,
	OP_L,
	OP_LE,
	OP_PLUS,
	OP_MINUS,
	OP_STAR,
	OP_DIV,
	OP_NEG,
	OP_NOT,
	OP_REF,
	OP_DEREF
};
typedef enum OP OP;

int isRelop(OP op) {
	switch (op) {
	case OP_G:
	case OP_GE:
	case OP_E:
	case OP_NE:
	case OP_L:
	case OP_LE:	return 1;
	default:	return 0;
	}
	return 0;
}

int isCondOp(OP op) {
	switch (op) {
	case OP_AND:
	case OP_OR:	
	case OP_NOT:	return 1;
	default:		return 0;
	}
	return 0;
}

int isAlgorithmOp(OP op) {
	switch (op) {
		case OP_PLUS:
		case OP_MINUS:
		case OP_STAR:
		case OP_DIV:
		case OP_NEG:	return 1;
		default:		return 0;
	}
	return 0;
}

OP getReverseRelop(OP op) {
	switch (op) {
	case OP_G:	return OP_LE;
	case OP_GE:	return OP_L;
	case OP_E:	return OP_NE;
	case OP_NE:	return OP_E;
	case OP_L:	return OP_GE;
	case OP_LE:	return OP_G;
	default:	assert(0);
	}
	return OP_E;
}

int procOP1_int(OP op, int arg) {
	switch (op) {
	case OP_NEG:	return -arg;
	case OP_NOT:	return !arg;
	default:		assert(0);
	}
	return 0;
}

float procOP1_float(OP op, float arg){
	switch (op) {
	case OP_NEG:	return -arg;
	default:		assert(0);
	}
	return 0;
}

int procOP2_int(OP op, int arg1, int arg2) {
	switch (op) {
	case OP_PLUS:	return arg1 + arg2;
	case OP_MINUS:	return arg1 - arg2;
	case OP_STAR:	return arg1 * arg2;
	case OP_DIV:	return arg1 / arg2;
	case OP_G:		return arg1 > arg2;
	case OP_GE:		return arg1 >= arg2;
	case OP_E:		return arg1 == arg2;
	case OP_NE:		return arg1 != arg2;
	case OP_L:		return arg1 < arg2;
	case OP_LE:		return arg1 <= arg2;
	case OP_AND:	return arg1 && arg2;
	case OP_OR:		return arg1 || arg2;
	default:		assert(0);
	}
	return 0;
}


float procOP2_float(OP op, float arg1, float arg2) {
	switch (op) {
	case OP_PLUS:	return arg1 + arg2;
	case OP_MINUS:	return arg1 - arg2;
	case OP_STAR:	return arg1 * arg2;
	case OP_DIV:	return arg1 / arg2;
	case OP_G:		return arg1 > arg2;
	case OP_GE:		return arg1 >= arg2;
	case OP_E:		return arg1 == arg2;
	case OP_NE:		return arg1 != arg2;
	case OP_L:		return arg1 < arg2;
	case OP_LE:		return arg1 <= arg2;
	case OP_AND:	return arg1 && arg2;
	case OP_OR:		return arg1 || arg2;
	default:		assert(0);
	}
	return 0;
}