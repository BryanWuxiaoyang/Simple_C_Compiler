#pragma once
#include "MyList.h"
#include "SymTable.h"
#include "FuncTable.h"
#include "TypeTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printSemError(int type, int lineno, const char* description) {
	printf("Error type %d at Line %d: %s\n", type, lineno, description);
}
