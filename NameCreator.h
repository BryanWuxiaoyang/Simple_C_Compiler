#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int symVarCnt = 0;
int funcVarCnt = 0;
int tempVarCnt = 0;
int labelCnt = 0;

char* createName_sym() {
	symVarCnt++;
	char* name = (char*)malloc(sizeof(char) * 10);
	if (name) sprintf(name, "v%d", symVarCnt);
	return name;
}

char* createName_func() {
	funcVarCnt++;
	char* name = (char*)malloc(sizeof(char) * 10);
	if (name) sprintf(name, "f%d", funcVarCnt);
	return name;
}

char* createName_temp() {
	tempVarCnt++;
	char* name = (char*)malloc(sizeof(char) * 10);
	if (name) sprintf(name, "t%d", tempVarCnt);
	return name;
}

char* createName_label(){
	labelCnt++;
	char* name = (char*)malloc(sizeof(char) * 10);
	if (name) {
		sprintf(name, "L%d", labelCnt);
	}

	
	return name;
}