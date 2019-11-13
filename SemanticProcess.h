#pragma once
#include"SymTable.h"

//#define TEST_MODE



void printSemError(int type, int lineno, const char* description) {
	printf("Error type %d at Line %d: %s\n", type, lineno, description);
}

enum {
	Program,
	ExtDefList,
	ExtDef,
	ExtDecList,
	Specifier,
	StructSpecifier,
	OptTag,
	Tag,
	VarDec,
	FunDec,
	VarList,
	ParamDec,
	CompSt,
	StmtList,
	Stmt,
	DefList,
	Def,
	DecList,
	Dec,
	Exp,
	Args,
	Other
};

struct Node {
	char text[32];
	union {
		int int_val;
		float float_val;
		char* str_val;
	};
	int childNum;
	struct Node* child[8];

	int symCode;
	int expandNo;
	int inhSize;
	void* inhValues[10];
	int synSize;
	void* synValues[10];

	int lineno;
};

struct Node* head;

void semAnalysis(struct Node* head);

void normalDFS(struct Node* head) {
	for (int i = 0; i < head->childNum; i++) {
		semAnalysis(head->child[i]);
	}
}

void addInhValue(struct Node* node, void* value) {
	node->inhValues[node->inhSize] = value;
	node->inhSize++;
}

void addSynValue(struct Node* node, void* value) {
	node->synValues[node->synSize] = value;
	node->synSize++;
}

int structNameNo = 1;

char* giveDefaultStructName() {
	char* name = (char*)malloc(sizeof(char) * 10);
	if (name) {
		int i = 0;
		name[i++] = 's'; name[i++] = 't'; name[i++] = 'r'; name[i++] = 'u'; name[i++] = 'c'; name[i++] = 't';
		int no = structNameNo;
		name[i++] = '0' + no;
		name[i++] = '\0';
	}
	return name;
}
#ifdef TEST_MODE
int n = -1;

void testPrint(const char* msg){
	for(int i = 0; i < n; i++){
		printf("\t");
	}
	printf("msg: %s\n", msg);
}

#endif

int funcMode = 0;
VarListHead* funcModeVarList = NULL;

void semAnalysis(struct Node* head) {
	int code = head->symCode;
	int no = head->expandNo;
#ifdef TEST_MODE
	n++;
	char text[40];
	switch (code) {
	case Program: strcpy(text, "Program"); break;
	case ExtDefList: strcpy(text, "ExtDefList"); break;
	case ExtDef:strcpy(text, "ExtDef"); break;
	case ExtDecList:strcpy(text, "ExtDecList"); break;
	case Specifier:strcpy(text, "Specifier"); break;
	case StructSpecifier:strcpy(text, "StructSpecifier"); break;
	case OptTag:strcpy(text, "OptTag"); break;
	case Tag:strcpy(text, "Tag"); break;
	case VarDec:strcpy(text, "VarDec"); break;
	case FunDec:strcpy(text, "FunDec"); break;
	case VarList:strcpy(text, "VarList"); break;
	case ParamDec:strcpy(text, "ParamDec"); break;
	case CompSt:strcpy(text, "CompSt"); break;
	case StmtList:strcpy(text, "StmtList"); break;
	case Stmt:strcpy(text, "Stmt"); break;
	case DefList:strcpy(text, "DefList"); break;
	case Def:strcpy(text, "Def"); break;
	case DecList:strcpy(text, "DecList"); break;
	case Dec:strcpy(text, "Dec"); break;
	case Exp:strcpy(text, "Exp"); break;
	case Args:strcpy(text, "Args"); break;
	default:strcpy(text, "Error"); break;
	}
	for (int i = 0; i < n; i++)
		printf("\t");
	printf("enter unit %s no %d line %d\n", text, no, head->lineno);
#endif

	switch (code) {
	case Program: {
		init();
		#ifdef TEST_MODE
		printTables();
		#endif
		if (no == 1) {// ExtDefList
			normalDFS(head);
			{// check func define
				FuncNode* p = __funcList.next;
				while (p) {
					if (p->func->state == 0) {// undefined
						char desc[40];
						sprintf(desc, "decleared but not defined func: %s", p->func->name);
						printSemError(18, p->func->decLineno, desc);
					}
					p = p->next;
				}
				#ifdef TEST_MODE
				printTables();
				#endif
			}
		}

		break;
	}
	case ExtDefList: {
		if (no == 1) {// ExtDef ExtDefList
			normalDFS(head);
		}
		else if (no == 2) {// none
			normalDFS(head);
		}

		break;
	}
	case ExtDef: {
		if (no == 1) { // Specifier ExtDecList SEMI // var declaration
			// might be: int a; or struct Node a or struct{int t} a;
			struct Node* specifierNode = head->child[0];
			struct Node* extDecListNode = head->child[1];
			struct Node* semiNode = head->child[2];
			semAnalysis(specifierNode);
			{
				VarTypeP varType = (VarTypeP)specifierNode->synValues[0]; // get syn value
				addInhValue(extDecListNode, varType);
			}
			semAnalysis(extDecListNode);
		}
		else if (no == 2) {// Specifier SEMI // struct declaration
			// be: struct{int a}
			struct Node* specifierNode = head->child[0];
			semAnalysis(specifierNode);
		}
		else if (no == 3) {// Specifier FunDec CompSt // func implementation
			struct Node* specifierNode = head->child[0];
			struct Node* funDecNode = head->child[1];
			struct Node* compStNode = head->child[2];
			semAnalysis(specifierNode);
			{
				VarTypeP returnType = (VarTypeP)specifierNode->synValues[0];
				int* decOrImp = (int*)malloc(sizeof(int));
				*decOrImp = 1;
				addInhValue(funDecNode, returnType);
				addInhValue(funDecNode, decOrImp);
				addInhValue(compStNode, returnType);
			}
			semAnalysis(funDecNode);
			semAnalysis(compStNode);
		}
		else if (no == 4) {// Specifier FunDec SEMI // func declaration, unimplemented
			struct Node* specifierNode = head->child[0];
			struct Node* funDecNode = head->child[1];
			struct Node* semiNode = head->child[2];
			semAnalysis(specifierNode);
			{
				VarTypeP varType = (VarTypeP)specifierNode->synValues[0];
				int* decOrImp = (int*)malloc(sizeof(int));
				*decOrImp = 0;
				addInhValue(funDecNode, varType);
				addInhValue(funDecNode, decOrImp);
			}
			semAnalysis(funDecNode);
		}
		break;
	}
	case ExtDecList: {// inhValues 1: VarTypeP decType; synValues 0
		VarTypeP decType = (VarTypeP)head->inhValues[0];
		if (no == 1) {// VarDec
			struct Node* varDecNode = head->child[0];
			{
				int* enGlobalInstall = (int*)malloc(sizeof(int));
				*enGlobalInstall = 1;
				VarListHead* varList = NULL;
				addInhValue(varDecNode, enGlobalInstall);
				addInhValue(varDecNode, varList);
				addInhValue(varDecNode, decType);
			}
			semAnalysis(varDecNode);
		}
		else if (no == 2) {// VarDec COMMA ExtDecList
			struct Node* varDecNode = head->child[0];
			struct Node* commaNode = head->child[1];
			struct Node* extDecListNode = head->child[2];
			{
				int* enGlobalInstall = (int*)malloc(sizeof(int));
				*enGlobalInstall = 1;
				VarListHead* varList = NULL;
				addInhValue(varDecNode, enGlobalInstall);
				addInhValue(varDecNode, varList);
				addInhValue(varDecNode, decType);
				addInhValue(extDecListNode, decType);
			}
			semAnalysis(varDecNode);
			semAnalysis(commaNode);
			semAnalysis(extDecListNode);
		}
		break;
	}
	case Specifier: {// inhValues 0; synValues 1: VarTypeP varType
		if (no == 1) { // TYPE_FLOAT
			addSynValue(head, floatType);
		}
		else if (no == 2) { // TYPE_INT
			addSynValue(head, integerType);
			#ifdef TEST_MODE
			char msg[40];
			sprintf(msg, "synValues 1: varType %s", integerType->name);
			testPrint(msg);
			#endif
		}
		else if (no == 3) {// StructSpecifier
			struct Node* structSpecifierNode = head->child[0];
			semAnalysis(structSpecifierNode);
			{
				VarTypeP varType = (VarTypeP)structSpecifierNode->synValues[0];
				addSynValue(head, varType);
			}
		}
		break;
	}
	case StructSpecifier: {// inhValues 0; synValues 1: VarTypeP varType;
		if (no == 1) {// STRUCT OptTag LC DefList RC
			struct Node* structNode = head->child[0];
			struct Node* optTagNode = head->child[1];
			struct Node* lcNode = head->child[2];
			struct Node* defListNode = head->child[3];
			struct Node* rcNode = head->child[4];

			int* enGlobalInstall = (int*)malloc(sizeof(int));
			*enGlobalInstall = 0;
			VarListHead* fieldList = createVarList();
			char* id;

			semAnalysis(optTagNode);
			{
				addInhValue(defListNode, enGlobalInstall);
				addInhValue(defListNode, fieldList);
			}
			semAnalysis(defListNode);
			{
				char* id = (char*)optTagNode->synValues[0];
				if (id == NULL) {
					id = giveDefaultStructName();
				}

				VarTypeP type = errorType;
				int error = 0;
				if (1) {// check id redefine
					VarTypeP checkVarType = findVarType(id);
					VarP checkVar = findVar(id);
					FuncP checkFunc = findFunc(id);

					if (checkVarType != NULL || checkVar != NULL || checkFunc != NULL) {
						char desc[40];
						sprintf(desc, "redefined symbal: %s", id);
						printSemError(16, head->lineno, desc);
						error = 1;
					}
				}

				if (error == 0) {
					installStruct_2(id, fieldList);
					type = findVarType(id);
				}

				addSynValue(head, type);
			}
		}
		else if (no == 2) {// STRUCT Tag
			struct Node* structNode = head->child[0];
			struct Node* tagNode = head->child[1];
			semAnalysis(tagNode);
			{
				char* id = (char*)tagNode->synValues[0];
				VarTypeP type = findVarType(id);

				if (type == NULL || type->kind != STRUCTURE) {//check type exist
					char desc[40];
					sprintf(desc, "undefined struct: %s", id);
					printSemError(17, head->lineno, desc);
					type = errorType;
				}

				addSynValue(head, type);
			}
		}
		break;
	}
	case OptTag: {// inhValues 0; synValues 1: char* id;
		char* id;
		if (no == 1) {// ID
			struct Node* idNode = head->child[0];
			id = idNode->str_val;
			addSynValue(head, id);
		}
		else if (no == 2) {// none
			id = NULL;
			addSynValue(head, NULL);
		}
		break;
	}
	case Tag: {// inhValues 0; synValues 1: char* id;
		if (no == 1) {// ID
			struct Node* idNode = head->child[0];
			char* id = idNode->str_val;
			addSynValue(head, id);
		}
		break;
	}
	case VarDec: {// inhValues 3: int enGolbalInstall, VarListHead* varList, VarTypeP varType; synValues 1: char* id;
		// might be: a or a[1] or a[1][2] or a[]
		int* enGlobalInstall = (int*)head->inhValues[0];
		VarListHead* varList = (VarListHead*)head->inhValues[1];
		VarTypeP varType = (VarTypeP)head->inhValues[2];

		if (no == 1) {// ID
			struct Node* idNode = head->child[0];
			char* id = idNode->str_val;
			if (*enGlobalInstall == 1) {
				VarP checkVar = findVar(id);
				VarTypeP checkVarType = findVarType(id);
				FuncP checkFunc = findFunc(id);
				if (checkVar != NULL || checkVarType != NULL || checkFunc != NULL) {
					char desc[40];
					sprintf(desc, "redefined variable: %s", id);
					printSemError(3, head->lineno, desc);
				}
				else {
					installVar(id, varType);
					#ifdef TEST_MODE
					char msg[40];
					sprintf(msg, "install var %s, type %s", id, varType->name);
					testPrint(msg);
					#endif
				}
			}
			else if(*enGlobalInstall == 0){
				
				int error = 0;

				if(1){// check redefined field
					VarP redefVar = findVarInVarList(id, varList);
					VarP checkVar = findVar(id);
					FuncP checkFunc = findFunc(id);
					VarTypeP checkVarType = findVarType(id);
					if(redefVar != NULL){
						char desc[40];
						sprintf(desc, "redefined field '%s'", id);
						printSemError(15, head->lineno, desc);
						error = 1;
					}
					else if(checkFunc != NULL || checkVarType != NULL){
						char desc[40];
						sprintf(desc, "redefined variable: %s", id);
						printSemError(3, head->lineno, desc);
						error = 2;
					}
					else if(checkVar != NULL){
						if(funcMode == 0 || (funcMode == 1 && findVarInVarList(id, funcModeVarList) == NULL)){
							char desc[40];
							sprintf(desc, "redefined variable: %s", id);
							printSemError(3, head->lineno, desc);
							error = 2;
						}
					}
				}

				if(error == 0){
					// to be deleted
					installVar(id, varType);

					VarP var = findVar(id);

					appendVarToList(var, varList);
					#ifdef TEST_MODE
					char msg[40];
					sprintf(msg, "append var %s, type %s to list", id, varType->name);
					testPrint(msg);
					#endif
				}
			}
			addSynValue(head, id);
		}
		else if (no == 2) {// VarDec LB INT RB
			struct Node* varDecNode = head->child[0];
			struct Node* lbNode = head->child[1];
			struct Node* intNode = head->child[2];
			struct Node* rbNode = head->child[3];
			{
				int arraySize = intNode->int_val;
				VarTypeP arrayType = createArrayType("", varType, arraySize);
				addInhValue(varDecNode, enGlobalInstall);
				addInhValue(varDecNode, varList);
				addInhValue(varDecNode, arrayType);
			}
			semAnalysis(varDecNode);
			{
				char* id = (char*)varDecNode->synValues[0];
				addSynValue(head, id);
			}
		}
		/*else if (no == 3) {// VarDec LB RB
			struct Node* varDecNode = head->child[0];
			struct Node* lbNode = head->child[1];
			struct Node* rbNode = head->child[2];
			{
				int arraySize = 0;
				VarTypeP arrayType = createArrayType("", varType, arraySize);
				addInhValue(varDecNode, (int)enGlobalInstall);
				addInhValue(varDecNode, (int)varList);
				addInhValue(varDecNode, (int)arrayType);
			}
			semAnalysis(varDecNode);
			{
				char* id = (char*)varDecNode->synValues[0];
				addSynValue(head, (int)id);
			}
		}*/
		break;
	}
	case FunDec: {// inhValues 2: VarTypeP varType, int decOrImp; synValues 1: FuncP func;
		VarTypeP returnType = (VarTypeP)head->inhValues[0];
		int* decOrImp = (int*)head->inhValues[1];
		if (no == 1) {// ID LP VarList RP
			struct Node* idNode = head->child[0];
			struct Node* lpNode = head->child[1];
			struct Node* varListNode = head->child[2];
			struct Node* rpNode = head->child[3];
			char* id = idNode->str_val;
			FuncP func = findFunc(id);
			VarListHead* argList = createVarList();
			{
				addInhValue(varListNode, argList);
				if(func != NULL){
					funcMode = 1;
					funcModeVarList = func->argList;
				}
			}
			semAnalysis(varListNode);
			{
				funcMode = 0;
				funcModeVarList = NULL;

				int error = 0;
				if (error == 0) { // dec mode, check declaration match
					if (*decOrImp == 0 && func != NULL) {
						VarListHead* realArgList = func->argList;
						int wrong = 0;
						if(isEqualVarType(returnType, func->returnType) == 0) wrong = 1; // return type not match
						else if (argList->size != realArgList->size) wrong = 1; // arg size not match
						else {// arg types not match
							VarNode* p1 = argList->next;
							VarNode* p2 = realArgList->next;
							while (p1 && p2) {
								if (isEqualVarType(p1->var->type, p2->var->type) == 0) {
									wrong = 1;
								}
								p1 = p1->next;
								p2 = p2->next;
							}
						}

						if (wrong == 1) {
							char desc[40];
							sprintf(desc, "declaration of function '%s' not match", id);
							printSemError(19, head->lineno, desc);
							error = 1;
						}
					}
				}
				if (error == 0) {// imp mode, check redefine
					if (*decOrImp == 1 && func != NULL) {
						if (isImplemented(func) == 1) { // redefinition
							char desc[40];
							sprintf(desc, "redefined func: %s", id);
							printSemError(4, head->lineno, desc);
							error = 2;
						}
						else{// check arg match
							VarListHead* realArgList = func->argList;
							int wrong = 0;
							if(isEqualVarType(returnType, func->returnType) == 0) wrong = 1; // return type not match
							else if (argList->size != realArgList->size) wrong = 1; // arg size not match
							else {// arg types not match
								VarNode* p1 = argList->next;
								VarNode* p2 = realArgList->next;
								while (p1 && p2) {
									if (isEqualVarType(p1->var->type, p2->var->type) == 0) {
										wrong = 1;
									}
									p1 = p1->next;
									p2 = p2->next;
								}
							}

							if (wrong == 1) {// imp func not match dec func, alter it to imp func
								char desc[80];
								sprintf(desc, "declaration of function '%s' not match", id);
								printSemError(19, head->lineno, desc);
								error = 3;
								func->returnType = returnType;
								func->argList = argList;
								implementFunc(func);
								func->decLineno = func->defLineno = head->lineno;
							}
						}
					}
				}

				if(error == 0 && func == NULL){// checkName
					VarTypeP checkVarType = findVarType(id);
					FuncP checkFunc = findFunc(id);
					if(checkVarType != NULL || checkFunc != NULL){
						char desc[40];
						sprintf(desc, "redefined func: %s", id);
						printSemError(3, head->lineno, desc);
						error = 4;
					}
				}

				if (error == 0) {
					if (*decOrImp == 0 && func == NULL) {
						installFunc_2(id, argList, returnType);
						func = findFunc(id);
						func->decLineno = head->lineno;
						func->defLineno = 0;
					}
					else if (*decOrImp == 0 && func != NULL) {

					}
					else if (*decOrImp == 1 && func == NULL) {
						installFunc_2(id, argList, returnType);
						func = findFunc(id);
						implementFunc(func);
						func->decLineno = func->defLineno = head->lineno;
					}
					else if (*decOrImp == 1 && func != NULL) {
						implementFunc(findFunc(id));
						func->defLineno = head->lineno;
					}
				}
				func = findFunc(id);
				addSynValue(head, func);
			}
		}
		else if (no == 2) {// ID LP RP
			struct Node* idNode = head->child[0];
			struct Node* lpNode = head->child[1];
			struct Node* rpNode = head->child[2];
			{
				char* id = idNode->str_val;
				FuncP func = findFunc(id);
				VarListHead* argList = createVarList();
				int error = 0;
				if (error == 0) { // dec mode, check declaration match
					if (*decOrImp == 0 && func != NULL) {
						VarListHead* realArgList = func->argList;
						int wrong = 0;
						if(isEqualVarType(returnType, func->returnType) == 0) wrong = 1; // return type not match
						else if (argList->size != realArgList->size) wrong = 1; // arg size not match
						else {// arg types not match
							VarNode* p1 = argList->next;
							VarNode* p2 = realArgList->next;
							while (p1 && p2) {
								if (isEqualVarType(p1->var->type, p2->var->type) == 0) {
									wrong = 1;
								}
								p1 = p1->next;
								p2 = p2->next;
							}
						}

						if (wrong == 1) {
							char desc[40];
							sprintf(desc, "declaration of function '%s' not match", id);
							printSemError(19, head->lineno, desc);
							error = 1;
						}
					}
				}
				if (error == 0) {// imp mode, check redefine
					if (*decOrImp == 1 && func != NULL) {
						if (isImplemented(func) == 1) { // redefinition
							char desc[40];
							sprintf(desc, "redefined func: %s", id);
							printSemError(4, head->lineno, desc);
							error = 2;
						}
						else{// check arg match
							VarListHead* realArgList = func->argList;
							int wrong = 0;
							if(isEqualVarType(returnType, func->returnType) == 0) wrong = 1; // return type not match
							else if (argList->size != realArgList->size) wrong = 1; // arg size not match
							else {// arg types not match
								VarNode* p1 = argList->next;
								VarNode* p2 = realArgList->next;
								while (p1 && p2) {
									if (isEqualVarType(p1->var->type, p2->var->type) == 0) {
										wrong = 1;
									}
									p1 = p1->next;
									p2 = p2->next;
								}
							}

							if (wrong == 1) {// imp func not match dec func, alter it to imp func
								char desc[40];
								sprintf(desc, "declaration of function '%s' not match", id);
								printSemError(19, head->lineno, desc);
								error = 1;
								func->returnType = returnType;
								func->argList = argList;
								implementFunc(func);
								func->decLineno = func->defLineno = head->lineno;
							}
						}
					}
				}

				if(error == 0 && func == NULL){// checkName
					VarTypeP checkVarType = findVarType(id);
					FuncP checkFunc = findFunc(id);
					if(checkVarType != NULL || checkFunc != NULL){
						char desc[40];
						sprintf(desc, "redefined func: %s", id);
						printSemError(3, head->lineno, desc);
						error = 4;
					}
				}

				if (error == 0) {
					if (*decOrImp == 0 && func == NULL) {
						installFunc_2(id, argList, returnType);
						func = findFunc(id);
						func->decLineno = head->lineno;
						func->defLineno = 0;
					}
					else if (*decOrImp == 0 && func != NULL) {

					}
					else if (*decOrImp == 1 && func == NULL) {
						installFunc_2(id, argList, returnType);
						func = findFunc(id);
						implementFunc(func);
						func->decLineno = func->defLineno = head->lineno;
					}
					else if (*decOrImp == 1 && func != NULL) {
						implementFunc(findFunc(id));
						func->defLineno = head->lineno;
					}
				}
				func = findFunc(id);
				addSynValue(head, func);
			}
		}
		break;
	}
	case VarList: {// inhValues 1: VarListHead* varList; synValues 0;
		VarListHead* varList = (VarListHead*)head->inhValues[0];
		if (no == 1) {// ParamDec COMMA VarList
			struct Node* paramDecNode = head->child[0];
			struct Node* commaNode = head->child[1];
			struct Node* varListNode = head->child[2];

			{
				addInhValue(paramDecNode, varList);
				addInhValue(varListNode, varList);
			}
			semAnalysis(paramDecNode);
			semAnalysis(varListNode);
		}
		else if (no == 2) {// ParamDec
			struct Node* paramDecNode = head->child[0];
			{
				addInhValue(paramDecNode, varList);
			}
			semAnalysis(paramDecNode);
		}
		break;
	}
	case ParamDec: {// inhValues 1: VarListHead* varList; synValues 0;
		VarListHead* varList = (VarListHead*)head->inhValues[0];
		if (no == 1) {// Specifier VarDec
			struct Node* specifierNode = head->child[0];
			struct Node* varDecNode = head->child[1];

			semAnalysis(specifierNode);
			{
				int* enGlobalInstall = (int*)malloc(sizeof(int));
				*enGlobalInstall = 0;
				VarTypeP varType = (VarTypeP)specifierNode->synValues[0];
				addInhValue(varDecNode, enGlobalInstall);
				addInhValue(varDecNode, varList);
				addInhValue(varDecNode, varType);
			}
			semAnalysis(varDecNode);
		}
		break;
	}
	case CompSt: {// inhValues 1: VarTypeP returnType; synValues 0;
		VarTypeP returnType = (VarTypeP)head->inhValues[0];
		if (no == 1) {// LC DefList StmtList RC
			struct Node* lcNode = head->child[0];
			struct Node* defListNode = head->child[1];
			struct Node* stmtListNode = head->child[2];
			struct Node* rcNode = head->child[3];

			{
				int* enGlobalInstall = (int*)malloc(sizeof(int));
				*enGlobalInstall = 1;
				addInhValue(defListNode, enGlobalInstall);
				addInhValue(defListNode, NULL);
				addInhValue(stmtListNode, returnType);
			}
			semAnalysis(defListNode);
			semAnalysis(stmtListNode);
		}
		break;
	}
	case StmtList: {// inhValues 1: VarTypeP returnType; synValues 0;
		VarTypeP returnType = (VarTypeP)head->inhValues[0];
		if (no == 1) {// Stmt StmtList
			struct Node* stmtNode = head->child[0];
			struct Node* stmtListNode = head->child[1];
			{
				addInhValue(stmtNode, returnType);
				addInhValue(stmtListNode, returnType);
			}
			semAnalysis(stmtNode);
			semAnalysis(stmtListNode);
		}
		else if (no == 2) {// none

		}
		break;
	}
	case Stmt: {// inhValues 1: VarTypeP returnType; synValues 0;
		VarTypeP returnType = (VarTypeP)head->inhValues[0];
		if (no == 1) {// Exp SEMI
			struct Node* expNode = head->child[0];
			struct Node* semiNode = head->child[1];
			semAnalysis(expNode);
		}
		else if (no == 2) {// CompSt
			struct Node* compStNode = head->child[0];
			{
				addInhValue(compStNode, returnType);
			}
			semAnalysis(compStNode);
		}
		else if (no == 3) {// RETURN Exp SEMI
			struct Node* returnNode = head->child[0];
			struct Node* expNode = head->child[1];
			struct Node* semiNode = head->child[2];
			semAnalysis(expNode);
			{
				VarTypeP realReturnType = (VarTypeP)expNode->synValues[0];
				if (isEqualVarType(returnType, realReturnType) == 0) {
					char desc[50];
					sprintf(desc, "wrong return type: %s, expecting: %s", realReturnType->name, returnType->name);
					printSemError(8, head->lineno, desc);
				}
			}
		}
		else if (no == 4) {// IF LP Exp RP Stmt
			struct Node* ifNode = head->child[0];
			struct Node* lpNode = head->child[1];
			struct Node* expNode = head->child[2];
			struct Node* rpNode = head->child[3];
			struct Node* stmtNode = head->child[4];

			{
				addInhValue(stmtNode, returnType);
			}
			semAnalysis(expNode);
			semAnalysis(stmtNode);
		}
		else if (no == 5) {// IF LP Exp RP Stmt ELSE Stmt
			struct Node* ifNode = head->child[0];
			struct Node* lpNode = head->child[1];
			struct Node* expNode = head->child[2];
			struct Node* rpNode = head->child[3];
			struct Node* stmtNode1 = head->child[4];
			struct Node* elseNode = head->child[5];
			struct Node* stmtNode2 = head->child[6];

			{
				addInhValue(stmtNode1, returnType);
				addInhValue(stmtNode2, returnType);
			}
			semAnalysis(expNode);
			semAnalysis(stmtNode1);
			semAnalysis(stmtNode2);
		}
		else if (no == 6) {// WHILE LP Exp RP Stmt
			struct Node* whileNode = head->child[0];
			struct Node* lpNode = head->child[1];
			struct Node* expNode = head->child[2];
			struct Node* rpNode = head->child[3];
			struct Node* stmtNode = head->child[4];
			{
				addInhValue(stmtNode, returnType);
			}
			semAnalysis(expNode);
			semAnalysis(stmtNode);
		}
		break;
	}
	case DefList: {// inhValues 2: int enGlobalInstall, VarListHead* varList; synValues 0;
		if (no == 1) {// Def DefList
			struct Node* defNode = head->child[0];
			struct Node* defListNode = head->child[1];
			{
				addInhValue(defNode, head->inhValues[0]);
				addInhValue(defNode, head->inhValues[1]);
				addInhValue(defListNode, head->inhValues[0]);
				addInhValue(defListNode, head->inhValues[1]);
			}
			semAnalysis(defNode);
			semAnalysis(defListNode);
		}
		else if (no == 2) {// none

		}
		break;
	}
	case Def: {// inhValues 2: int enGlobalInstall, VarListHead* varList; synValues 0;
		if (no == 1) {// Specifier DecList SEMI
			struct Node* specifierNode = head->child[0];
			struct Node* decListNode = head->child[1];
			struct Node* semiNode = head->child[2];

			semAnalysis(specifierNode);
			{
				VarTypeP varType = (VarTypeP)specifierNode->synValues[0];
				addInhValue(decListNode, head->inhValues[0]);
				addInhValue(decListNode, head->inhValues[1]);
				addInhValue(decListNode, varType);
			}
			semAnalysis(decListNode);
		}
		break;
	}
	case DecList: {// inhValues 3: int enGlobalInstall, VarListHead* varList, VarTypeP varType; synValues 0;
		int* enGlobalInstall = (int*)head->inhValues[0];
		VarListHead* varList = (VarListHead*)head->inhValues[1];
		VarTypeP varType = (VarTypeP)head->inhValues[2];
		if (no == 1) {// Dec
			struct Node* decNode = head->child[0];

			{
				addInhValue(decNode, enGlobalInstall);
				addInhValue(decNode, varList);
				addInhValue(decNode, varType);
			}
			semAnalysis(decNode);
		}
		else if (no == 2) {// Dec COMMA DecList
			struct Node* decNode = head->child[0];
			struct Node* commaNode = head->child[1];
			struct Node* decListNode = head->child[2];

			{
				addInhValue(decNode, enGlobalInstall);
				addInhValue(decNode, varList);
				addInhValue(decNode, varType);
				addInhValue(decListNode, enGlobalInstall);
				addInhValue(decListNode, varList);
				addInhValue(decListNode, varType);
			}
			semAnalysis(decNode);
			semAnalysis(commaNode);
			semAnalysis(decListNode);
		}
		break;
	}
	case Dec: {// inhValues 3: int enGlobalInstall, VarListHead* varList, VarTypeP varType; synValues 0;
		int* enGlobalInstall = (int*)head->inhValues[0];
		VarListHead* varList = (VarListHead*)head->inhValues[1];
		VarTypeP varType = (VarTypeP)head->inhValues[2];
		if (no == 1) {// VarDec
			struct Node* varDecNode = head->child[0];
			{
				addInhValue(varDecNode, enGlobalInstall);
				addInhValue(varDecNode, varList);
				addInhValue(varDecNode, varType);
			}
			semAnalysis(varDecNode);
		}
		else if (no == 2) {// VarDec ASSIGNOP Exp
			struct Node* varDecNode = head->child[0];
			struct Node* assignopNode = head->child[1];
			struct Node* expNode = head->child[2];
			{
				addInhValue(varDecNode, enGlobalInstall);
				addInhValue(varDecNode, varList);
				addInhValue(varDecNode, varType);
			}
			semAnalysis(varDecNode);
			semAnalysis(expNode);
			{
				char* id = (char*)varDecNode->synValues[0];
				VarP var = findVar(id); // it might be an array, so we cannot use varType directly
				VarTypeP expType = (VarTypeP)expNode->synValues[0];
				int error = 0;

				if(1){// check assignable
					if(*enGlobalInstall == 0){
						char desc[40];
						sprintf(desc, "not assignable for variable '%s'", id);
						printSemError(15, head->lineno, desc);
						error = 1;
					}
				}

				if(error == 0){// check type not match
					VarTypeP assignVarType = (var == NULL ? errorType : var->type);
					if(var == NULL || isEqualVarType(assignVarType, expType) == 0){
						char desc[40];
						sprintf(desc, "type not match: %s and %s", assignVarType->name, expType->name);
						printSemError(5, head->lineno, desc);
						error = 2;
					}
				}
			}
		}
		break;
	}
	case Exp: {// inhValues 0; synValues 2: VarTypeP valType, int isLeftValue;
		// might be: 1 op 2, op 1,  a, (1), a(1, 2), a(), a.b, a[1],  
		if (no == 1) {// Exp ASSIGNOP Exp
			struct Node* expNode1 = head->child[0];
			struct Node* assignopNode = head->child[1];
			struct Node* expNode2 = head->child[2];

			semAnalysis(expNode1);
			semAnalysis(expNode2);
			{
				VarTypeP varType1 = (VarTypeP)expNode1->synValues[0];
				int* isLeftValue1 = (int*)expNode1->synValues[1];
				VarTypeP varType2 = (VarTypeP)expNode2->synValues[0];

				if (isEqualVarType(varType1, varType2) == 0) {// type not match
					char desc[50];
					sprintf(desc, "unmatched type: %s and %s", varType1->name, varType2->name);
					printSemError(5, head->lineno, desc);
				}
				if (*isLeftValue1 == 0) {
					printSemError(6, head->lineno, "left value at place for right value");
				}

				if (varType1 && varType1 != errorType) addSynValue(head, varType1);
				else if (varType2 && varType2 != errorType) addSynValue(head, varType2);
				else addSynValue(head, errorType);

				int* isLeftValue = (int*)malloc(sizeof(int));
				*isLeftValue = 0;
				addSynValue(head, isLeftValue);
			}
		}
		else if (no == 2) {// Exp AND Exp
			struct Node* expNode1 = head->child[0];
			struct Node* andNode = head->child[1];
			struct Node* expNode2 = head->child[2];

			semAnalysis(expNode1);
			semAnalysis(expNode2);
			{
				VarTypeP varType1 = (VarTypeP)expNode1->synValues[0];
				VarTypeP varType2 = (VarTypeP)expNode2->synValues[0];

				if (isEqualVarType(varType1, varType2) == 0) {
					char desc[50];
					sprintf(desc, "unmatched type: %s and %s", varType1->name, varType2->name);
					printSemError(7, head->lineno, desc);
				}

				if (varType1 && varType1 != errorType) addSynValue(head, varType1);
				else if (varType2 && varType2 != errorType) addSynValue(head, varType2);
				else addSynValue(head, errorType);
				int* isLeftValue = (int*)malloc(sizeof(int));
				*isLeftValue = 0;
				addSynValue(head, isLeftValue);
			}
		}
		else if (no == 3) {// Exp OR Exp
			struct Node* expNode1 = head->child[0];
			struct Node* opNode = head->child[1];
			struct Node* expNode2 = head->child[2];

			semAnalysis(expNode1);
			semAnalysis(expNode2);
			{
				VarTypeP varType1 = (VarTypeP)expNode1->synValues[0];
				VarTypeP varType2 = (VarTypeP)expNode2->synValues[0];

				if (isEqualVarType(varType1, varType2) == 0) {
					char desc[50];
					sprintf(desc, "unmatched type: %s and %s", varType1->name, varType2->name);
					printSemError(7, head->lineno, desc);
				}

				if (varType1 && varType1 != errorType) addSynValue(head, varType1);
				else if (varType2 && varType2 != errorType) addSynValue(head, varType2);
				else addSynValue(head, errorType);
				int* isLeftValue = (int*)malloc(sizeof(int));
				*isLeftValue = 0;
				addSynValue(head, isLeftValue);
			}
		}
		else if (no == 4) {// Exp RELOP Exp
			struct Node* expNode1 = head->child[0];
			struct Node* relopNode = head->child[1];
			struct Node* expNode2 = head->child[2];

			semAnalysis(expNode1);
			semAnalysis(expNode2);
			{
				VarTypeP varType1 = (VarTypeP)expNode1->synValues[0];
				VarTypeP varType2 = (VarTypeP)expNode2->synValues[0];

				if (isEqualVarType(varType1, varType2) == 0) {
					char desc[50];
					sprintf(desc, "unmatched type: %s and %s", varType1->name, varType2->name);
					printSemError(7, head->lineno, desc);
				}

				if (varType1 && varType1 != errorType) addSynValue(head, varType1);
				else if (varType2 && varType2 != errorType) addSynValue(head, varType2);
				else addSynValue(head, errorType);
				int* isLeftValue = (int*)malloc(sizeof(int));
				*isLeftValue = 0;
				addSynValue(head, isLeftValue);
			}
		}
		else if (no == 5) {// Exp PLUS Exp
			struct Node* expNode1 = head->child[0];
			struct Node* plusNode = head->child[1];
			struct Node* expNode2 = head->child[2];

			semAnalysis(expNode1);
			semAnalysis(expNode2);
			{
				VarTypeP varType1 = (VarTypeP)expNode1->synValues[0];
				VarTypeP varType2 = (VarTypeP)expNode2->synValues[0];

				if (isEqualVarType(varType1, varType2) == 0) {
					char desc[50];
					sprintf(desc, "unmatched type: %s and %s", varType1->name, varType2->name);
					printSemError(7, head->lineno, desc);
				}

				if (varType1 && varType1 != errorType) addSynValue(head, varType1);
				else if (varType2 && varType2 != errorType) addSynValue(head, varType2);
				else addSynValue(head, errorType);
				int* isLeftValue = (int*)malloc(sizeof(int));
				*isLeftValue = 0;
				addSynValue(head, isLeftValue);
			}
		}
		else if (no == 6) {// Exp MINUS Exp
			struct Node* expNode1 = head->child[0];
			struct Node* minusNode = head->child[1];
			struct Node* expNode2 = head->child[2];

			semAnalysis(expNode1);
			semAnalysis(expNode2);
			{
				VarTypeP varType1 = (VarTypeP)expNode1->synValues[0];
				VarTypeP varType2 = (VarTypeP)expNode2->synValues[0];

				if (isEqualVarType(varType1, varType2) == 0) {
					char desc[50];
					sprintf(desc, "unmatched type: %s and %s", varType1->name, varType2->name);
					printSemError(7, head->lineno, desc);
				}

				if (varType1 && varType1 != errorType) addSynValue(head, varType1);
				else if (varType2 && varType2 != errorType) addSynValue(head, varType2);
				else addSynValue(head, errorType);
				int* isLeftValue = (int*)malloc(sizeof(int));
				*isLeftValue = 0;
				addSynValue(head, isLeftValue);
			}
		}
		else if (no == 7) {// Exp STAR Exp
			struct Node* expNode1 = head->child[0];
			struct Node* starNode = head->child[1];
			struct Node* expNode2 = head->child[2];

			semAnalysis(expNode1);
			semAnalysis(expNode2);
			{
				VarTypeP varType1 = (VarTypeP)expNode1->synValues[0];
				VarTypeP varType2 = (VarTypeP)expNode2->synValues[0];

				if (isEqualVarType(varType1, varType2) == 0) {
					char desc[50];
					sprintf(desc, "unmatched type: %s and %s", varType1->name, varType2->name);
					printSemError(7, head->lineno, desc);
				}

				if (varType1 && varType1 != errorType) addSynValue(head, varType1);
				else if (varType2 && varType2 != errorType) addSynValue(head, varType2);
				else addSynValue(head, errorType);
				int* isLeftValue = (int*)malloc(sizeof(int));
				*isLeftValue = 0;
				addSynValue(head, isLeftValue);
			}
		}
		else if (no == 8) {// Exp DIV Exp
			struct Node* expNode1 = head->child[0];
			struct Node* divNode = head->child[1];
			struct Node* expNode2 = head->child[2];

			semAnalysis(expNode1);
			semAnalysis(expNode2);
			{
				VarTypeP varType1 = (VarTypeP)expNode1->synValues[0];
				VarTypeP varType2 = (VarTypeP)expNode2->synValues[0];

				if (isEqualVarType(varType1, varType2) == 0) {
					char desc[50];
					sprintf(desc, "unmatched type: %s and %s", varType1->name, varType2->name);
					printSemError(7, head->lineno, desc);
				}

				if (varType1 && varType1 != errorType) addSynValue(head, varType1);
				else if (varType2 && varType2 != errorType) addSynValue(head, varType2);
				else addSynValue(head, errorType);
				int* isLeftValue = (int*)malloc(sizeof(int));
				*isLeftValue = 0;
				addSynValue(head, isLeftValue);
			}
		}
		else if (no == 9) {// LP Exp RP
			struct Node* lpNode = head->child[0];
			struct Node* expNode = head->child[1];
			struct Node* rpNode = head->child[2];

			semAnalysis(expNode);
			{
				VarTypeP varType = (VarTypeP)expNode->synValues[0];
				int* isLeftValue = (int*)expNode->synValues[1];
				addSynValue(head, varType);
				addSynValue(head, isLeftValue);
			}
		}
		else if (no == 10) {// NEG Exp
			struct Node* negNode = head->child[0];
			struct Node* expNode = head->child[1];

			semAnalysis(expNode);
			{
				VarTypeP varType = (VarTypeP)expNode->synValues[0];

				addSynValue(head, varType);
				int* isLeftValue = (int*)malloc(sizeof(int));
				*isLeftValue = 0;
				addSynValue(head, isLeftValue);
			}
		}
		else if (no == 11) {// NOT Exp
			struct Node* notNode = head->child[0];
			struct Node* expNode = head->child[1];

			semAnalysis(expNode);
			{
				VarTypeP varType = (VarTypeP)expNode->synValues[0];

				addSynValue(head, varType);
				int* isLeftValue = (int*)malloc(sizeof(int));
				*isLeftValue = 0;
				addSynValue(head, isLeftValue);
			}
		}
		else if (no == 12) {// ID LP Args RP
			struct Node* idNode = head->child[0];
			struct Node* lpNode = head->child[1];
			struct Node* argsNode = head->child[2];
			struct Node* rpNode = head->child[3];
			VarTypeListHead* varTypeList = createVarTypeList();

			{
				addInhValue(argsNode, varTypeList);
			}
			semAnalysis(argsNode);
			{
				char* id = idNode->str_val;
				FuncP func = findFunc(id);
				VarTypeP returnType = errorType;
				int error = 0;
				int funcExist = 1;
				if (1) { // check func exist
					if (func == NULL) {
						VarP var = findVar(id);
						if (var == NULL) { // no symble match
							char desc[40];
							sprintf(desc, "undecleared function: %s", id);
							printSemError(2, head->lineno, desc);
							returnType = errorType;
							error = 1;
						}
						else {// var match
							char desc[40];
							sprintf(desc, "func call used on normal variable: %s", var->name);
							printSemError(11, head->lineno, desc);
							error = 2;
						}
						funcExist = 0;
					}
				}

				/*if(funcExist == 1){
					if(isImplemented(func) == 0){// check func defined
						char desc[40];
						sprintf(desc, "declared, but undefined function: %s", id);
						printSemError(2, head->lineno, desc);
						error = 3;
					}
				}*/

				int argSizeMatch = 1;
				if (funcExist == 1) {// check arg size match
					int expectArgSize = func->argList->size;
					int realArgSize = varTypeList->size;
					if (expectArgSize != realArgSize) {
						char desc[40];
						sprintf(desc, "arg num not match: %d, expecting: %d", realArgSize, expectArgSize);
						printSemError(9, head->lineno, desc);
						error = 4;
						argSizeMatch = 0;
					}
				}

				if (funcExist == 1 && argSizeMatch == 1) {// check arg types match
					VarNode* p1 = func->argList->next;
					VarTypeNode* p2 = varTypeList->next;
					while (p1 && p2) {
						VarTypeP type1 = p1->var->type;
						VarTypeP type2 = p2->type;
						if (isEqualVarType(type1, type2) == 0) {
							char desc[40];
							sprintf(desc, "arg '%s' type not match: %s, expecting %s", p1->var->name, type2->name, type1->name);
							printSemError(9, head->lineno, desc);
							error = 5;
						}
						p1 = p1->next;
						p2 = p2->next;
					}
				}

				if (error == 0) {
					FuncP rFunc = findFunc(id);
					returnType = rFunc->returnType;
				}

				addSynValue(head, returnType);
				int* isLeftValue = (int*)malloc(sizeof(int));
				*isLeftValue = 0;
				addSynValue(head, isLeftValue);
			}
		}
		else if (no == 13) {// ID LP RP
			struct Node* idNode = head->child[0];
			struct Node* lpNode = head->child[1];
			struct Node* rpNode = head->child[2];

			{
				char* id = idNode->str_val;
				FuncP func = findFunc(id);
				VarTypeP returnType = errorType;
				int error = 0;

				if (1) {// check define
					if (func == NULL) {
						VarP var = findVar(id);
						if (var == NULL) { // no symble match
							char desc[40];
							sprintf(desc, "undefined function: %s", id);
							printSemError(2, head->lineno, desc);
							returnType = errorType;
							error = 1;
						}
						else {// var match
							char desc[40];
							sprintf(desc, "func call used on normal variable: %s", var->name);
							printSemError(11, head->lineno, desc);
							error = 2;
						}
					}
				}
				if (1 && func) {// check arg num match
					int expectArgNum = func->argList->size;
					int realArgNum = 0;
					if (expectArgNum != realArgNum) {
						char desc[40];
						sprintf(desc, "arg num not match: %d, expecting %d", realArgNum, expectArgNum);
						error = 3;
					}
				}

				if (error == 0) {
					returnType = func->returnType;
				}

				addSynValue(head, returnType);
				int* isLeftValue = (int*)malloc(sizeof(int));
				*isLeftValue = 0;
				addSynValue(head, isLeftValue);
			}
		}
		else if (no == 14) {// Exp LB Exp RB
			struct Node* expNode1 = head->child[0];
			struct Node* lbNode = head->child[1];
			struct Node* expNode2 = head->child[2];
			struct Node* rbNode = head->child[3];

			semAnalysis(expNode1);
			semAnalysis(expNode2);
			{
				VarTypeP varType1 = (VarTypeP)expNode1->synValues[0];
				VarTypeP varType2 = (VarTypeP)expNode2->synValues[0];

				VarTypeP elemType = errorType;
				int error = 0;
				if (1) {// check being array type
					if (varType1->kind != ARRAY) {
						char desc[40];
						sprintf(desc, "type not array: %s", varType1->name);
						printSemError(10, head->lineno, desc);
						error = 1;
					}
				}

				if (1) {// check element index type
					if (isEqualVarType(varType2, integerType) == 0) {
						char desc[40];
						sprintf(desc, "element index type not integer: %s", varType2->name);
						printSemError(12, head->lineno, desc);
						error = 2;
					}
				}

				if (error == 0) {
					elemType = varType1->u.array.elemType;
				}

				addSynValue(head, elemType);
				int* isLeftValue = (int*)malloc(sizeof(int));
				*isLeftValue = 1;
				addSynValue(head, isLeftValue);
			}
		}
		else if (no == 15) {// Exp DOT ID
			struct Node* expNode = head->child[0];
			struct Node* dotNode = head->child[1];
			struct Node* idNode = head->child[2];

			semAnalysis(expNode);
			{
				VarTypeP varType = (VarTypeP)expNode->synValues[0];
				char* id = idNode->str_val;
				VarTypeP  fieldType = errorType;
				int error = 0;
				if (1) {// check exp type being struct
					if (varType->kind != STRUCTURE) {
						char desc[40];
						sprintf(desc, "not a struct type: %s", varType->name);
						printSemError(13, head->lineno, desc);
						error = 1;
					}
				}

				
				if (error == 0) {// check field exist in fieldList
					VarListHead* fieldList = varType->u.structure;
					VarP field = findVarInVarList(id, fieldList);
					if (field == NULL) {	
						char desc[40];
						sprintf(desc, "field %s not in fieldList of struct %s", id, varType->name);
						printSemError(14, head->lineno, desc);
						error = 2;
					}
				}
				
				if (error == 0) {
					VarListHead* fieldList = varType->u.structure;
					VarP field = findVarInVarList(id, fieldList);
					fieldType = field->type;
				}

				addSynValue(head, fieldType);
				int* isLeftValue = (int*)malloc(sizeof(int));
				*isLeftValue = 1;
				addSynValue(head, isLeftValue);
			}
		}
		else if (no == 16) {// ID
			struct Node* idNode = head->child[0];
			char* id = idNode->str_val;
			{
				VarP var = findVar(id);
				VarTypeP varType = errorType;
				if (var == NULL) {
					char description[40];
					sprintf(description, "undefined variable %s", id);
					printSemError(1, head->lineno, description);
					varType = errorType;
				}
				else {
					varType = var->type;
				}

				#ifdef TEST_MODE
				char msg[40];
				//printTables();
				sprintf(msg, "var: %s, type %s", var == NULL ? "none" : var->name, varType->name);
				testPrint(msg);
				#endif

				addSynValue(head, varType);
				int* isLeftValue = (int*)malloc(sizeof(int));
				*isLeftValue = 1;
				addSynValue(head, isLeftValue);
			}
		}
		else if (no == 17) {// INT
			struct Node* intNode = head->child[0];
			{
				int val = intNode->int_val;
				// TODO:

				VarTypeP varType = integerType;
				addSynValue(head, varType);
				int* isLeftValue = (int*)malloc(sizeof(int));
				*isLeftValue = 0;
				addSynValue(head, isLeftValue);
			}
		}
		else if (no == 18) {// FLOAT
			struct Node* floatNode = head->child[0];
			{
				float val = floatNode->float_val;
				// TODO:

				VarTypeP varType = floatType;
				addSynValue(head, floatType);
				int* isLeftValue = (int*)malloc(sizeof(int));
				*isLeftValue = 0;
				addSynValue(head, isLeftValue);
			}
		}
		break;
	}
	case Args: {// inhValues 1: VarTypeListHead* varList; synValues 0;
		VarTypeListHead* varTypeList = (VarTypeListHead*)head->inhValues[0];
		if (no == 1) {// Exp COMMA Args
			struct Node* expNode = head->child[0];
			struct Node* commaNode = head->child[1];
			struct Node* argsNode = head->child[2];

			semAnalysis(expNode);
			{
				// TODO:
				VarTypeP varType = (VarTypeP)expNode->synValues[0];
				appendVarTypeToList(varType, varTypeList);
				addInhValue(argsNode, varTypeList);
			}
			semAnalysis(argsNode);

		}
		else if (no == 2) {// Exp
			struct Node* expNode = head->child[0];

			semAnalysis(expNode);
			{
				// TODO:
				VarTypeP varType = (VarTypeP)expNode->synValues[0];
				appendVarTypeToList(varType, varTypeList);
			}
		}
		break;
	}
	}

#ifdef TEST_MODE
	for (int i = 0; i < n; i++)
		printf("\t");
	printf("exit unit line %d\n", head->lineno);
	n--;
#endif
}