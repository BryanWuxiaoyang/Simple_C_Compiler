#pragma once

#include <stdlib.h>
struct _Integer_ {
	int value;
};
typedef struct _Integer_* Integer;

Integer createInteger(int value) {
	Integer integer = (Integer)malloc(sizeof(struct _Integer_));
	if (integer) {
		integer->value = value;
	}
	return integer;
}