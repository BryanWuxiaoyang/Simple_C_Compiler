#pragma once
struct _Sym_ {
	char* name;
	Type type;
	int offset;
};
typedef struct _Sym_* Sym;