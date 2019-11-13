#pragma once
struct _Sym_ {
	char* name;
	Type type;
	int offset;
	void* instantValue;
};
typedef struct _Sym_* Sym;