#pragma once
#include "MyList.h"
#include "SymTable.h"
#include "FuncTable.h"
#include "TypeTable.h"
#include "SemanticErrorChecks.h"
#include "AbstractSyntaxTree.h"
#include "OP.h"
#include "ASTtoIL.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
	Other,
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
	SYN_ID,
	SYN_OP,
	SYN_INT,
	SYN_FLOAT
};

struct _Node_ {
	union {
		int int_val;
		float float_val;
		char* str_val;
		OP op;
	};
	int childNum;
	struct _Node_* child[8];

	int symCode;
	int expandNo;

	int lineno;
};
typedef struct _Node_* Node;

#define TEST_MODE

#ifdef TEST_MODE
int indent = -1;

const char* getSym(int code) {
	switch (code) {
	case Other:return "Other";
	case Program:return "Program";
	case ExtDefList:return "ExtDefList"; 
	case ExtDef:return "ExtDef";
	case ExtDecList:return "ExtDecList";
	case Specifier:return "Specifier";
	case StructSpecifier:return "StructSpecifier";
	case OptTag:return "OptTag";
	case Tag:return "Tag";
	case VarDec:return "VarDec";
	case FunDec:return "FunDec";
	case VarList:return "VarList";
	case ParamDec:return "ParamDec";
	case CompSt:return "CompSt";
	case StmtList:return "StmtList";
	case Stmt:return "Stmt";
	case DefList:return  "DefList";
	case Def:return "Def";
	case DecList:return  "DecList";
	case Dec:return "Dec";
	case Exp:return "Exp";
	case Args:return "Args";
	default:printf("enter error!\n"); exit(-1);
	}
}
void testEnterPrint(Node node) {
	indent++;
	for (int i = 0; i < indent; i++)
		printf("\t");
	printf("enter %s line %d expandNo %d\n", getSym(node->symCode), node->lineno, node->expandNo);
}
void testExitPrint(Node node) {
	for (int i = 0; i < indent; i++)
		printf("\t");
	printf("exit %s line %d expandNo %d\n", getSym(node->symCode), node->lineno, node->expandNo);
	indent--;
}
#endif

void putSyntaxTreeToFile(FILE* file, Node node) {
	int code = node->symCode;
	int no = node->expandNo;
	int lineno = node->lineno;
	fprintf(file, "%d %d %d %d", code, no, lineno, node->childNum);
	switch (node->symCode) {
	case SYN_ID: fprintf(file, " %s", node->str_val); break;
	case SYN_OP: fprintf(file, " %d", node->op); break;
	case SYN_INT: fprintf(file, " %d", node->int_val); break;
	case SYN_FLOAT: fprintf(file, " %f", node->float_val); break;
	}
	fprintf(file, "\n");
	for (int i = 0; i < node->childNum; i++) {
		putSyntaxTreeToFile(file, node->child[i]);
	}
}

Node getSyntaxTreeFromFile(FILE* file) {
	Node node = (Node)malloc(sizeof(struct _Node_));
	if (node == NULL) exit(-1);
	fscanf(file, "%d %d %d %d", &node->symCode, &node->expandNo, &node->lineno, &node->childNum);
	int t;
	switch (node->symCode) {
	case SYN_ID: node->str_val = (char*)malloc(sizeof(char) * 50); fscanf(file, "%s", node->str_val); break;
	case SYN_OP: fscanf(file, "%d", &t); node->op = (OP)t; break;
	case SYN_INT: fscanf(file, "%d", &node->int_val); break;
	case SYN_FLOAT: fscanf(file, "%f", &node->float_val); break;
	}
	for (int i = 0; i < node->childNum; i++) {
		node->child[i] = getSyntaxTreeFromFile(file);
	}
	return node;
}

void printTables() {
	printSymTable();
	printTypeTable();
	printFuncTable();
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

void init() {
	initSymTable();
	initFuncTable();
	initTypeTable();
	initAST();
}

void SM_Program(Node node);
void SM_ExtDefList(Node node);
void SM_ExtDef(Node node);
void SM_ExtDecList(Node node, Type decType);
void SM_Specifier(Node node, Type* ret_specType);
void SM_StructSpecifier(Node node, Type* ret_specType);
void SM_OptTag(Node node, char** ret_name);
void SM_Tag(Node node, char** ret_name);
void SM_VarDec(Node node, Type type, Sym* ret_var);
void SM_FunDec(Node node, Type specType, int isDef);
void SM_VarList(Node node, ListHead ret_list);
void SM_ParamDec(Node node, Sym* ret_var);
void SM_CompSt(Node node, Type returnType);
void SM_StmtList(Node node, Type returnType);
void SM_Stmt(Node node, Type returnType);
void SM_DefList(Node node);
void SM_Def(Node node);
void SM_DecList(Node node, Type specType);
void SM_Dec(Node node, Type specType, Sym* ret_var);
void SM_Exp(Node node, ASTNode* ret_astNode);
void SM_Args(Node node, ListHead ret_list);

void semAnalysis(Node node) {
	init();
	SM_Program(node);
}

void SM_Program(Node node) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// ExtDefList
		SM_ExtDefList(node->child[0]);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_ExtDefList(Node node) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// ExtDef ExtDefList
		SM_ExtDef(node->child[0]);
		SM_ExtDefList(node->child[1]);
	}
	else if (node->expandNo == 2) {// none
		
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_ExtDef(Node node) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// Specifier ExtDecList SEMI
		Node specifierNode = node->child[0];
		Node extDecListNode = node->child[1];
		Type specType = NULL;
		SM_Specifier(specifierNode, &specType);
		SM_ExtDecList(extDecListNode, specType);
	}
	else if (node->expandNo == 2) {// Specifier SEMI
		Node specifierNode = node->child[0];
		SM_Specifier(specifierNode, NULL);
	}
	else if (node->expandNo == 3) {// Specifier FuncDec CompSt
		Node specifierNode = node->child[0];
		Node funDecNode = node->child[1];
		Node compStNode = node->child[2];
		Type specType = NULL;
		SM_Specifier(specifierNode, &specType);
		SM_FunDec(funDecNode, specType, 1);
		SM_CompSt(compStNode, specType);
	}
	else if (node->expandNo == 4) {// Specifier FunDec SEMI
		Node specifierNode = node->child[0];
		Node funDecNode = node->child[1];
		Type specType = NULL;
		SM_Specifier(specifierNode, &specType);
		SM_FunDec(funDecNode, specType, 0);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_ExtDecList(Node node, Type decType) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// VarDec
		Node varDecNode = node->child[0];
		Sym sym = NULL;
		SM_VarDec(varDecNode, decType, &sym);
		insertSym(getCurSymTable(), sym);
	}
	else if (node->expandNo == 2) {// VarDec COMMA ExtDecList
		Node varDecNode = node->child[0];
		Node extDecListNode = node->child[2];
		Sym sym = NULL;
		SM_VarDec(varDecNode, decType, &sym);
		insertSym(getCurSymTable(), sym);
		SM_ExtDecList(extDecListNode, decType);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_Specifier(Node node, Type* ret_specType) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// TYPE_FLOAT
		*ret_specType = floatType;
	}
	else if (node->expandNo == 2) {// TYPE_INT
		if(ret_specType) *ret_specType = integerType;
	}
	else if (node->expandNo == 3) {// StructSpecifier
		Node structSpecifierNode = node->child[0];
		SM_StructSpecifier(structSpecifierNode, ret_specType);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_StructSpecifier(Node node, Type* ret_specType) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// STRUCT OptTag LC DefList RC
		Node optTagNode = node->child[1];
		Node defListNode = node->child[3];
		char* name = NULL;

		SM_OptTag(optTagNode, &name);
		if (name == NULL) name = giveDefaultStructName();

		pushSymTable(createSymTable(FIELD_STRUCT));
		pushTypeTable(createTypeTable(FIELD_STRUCT));
		SM_DefList(defListNode);
		ListHead fieldList = getCurSymTable()->head;
		popSymTable();
		popTypeTable();

		Type type = createType_structure(name, fieldList);
		insertType(getCurTypeTable(), type);
		if(ret_specType) *ret_specType = type;
	}
	else if (node->expandNo == 2) {// STRUCT Tag
		Node tagNode = node->child[1];
		char* name = NULL;
		SM_Tag(tagNode, &name);
		*ret_specType = findType_all(name);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_OptTag(Node node, char** ret_name) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// ID
		Node idNode = node->child[0];
		if(ret_name) *ret_name = idNode->str_val;
	}
	else if (node->expandNo == 2) {// none
		if(ret_name) *ret_name = NULL;
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_Tag(Node node, char** ret_name) {
	if(node->expandNo == 1){// ID
		Node idNode = node->child[0];
		if(ret_name) *ret_name = idNode->str_val;
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_VarDec(Node node, Type type, Sym* ret_var) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// ID
		Node idNode = node->child[0];
		char* name = idNode->str_val;
		if(ret_var) *ret_var = createSym(name, type);
	}
	else if (node->expandNo == 2) {// VarDec LB INT RB
		Node varDecNode = node->child[0];
		Node intNode = node->child[2];
		int size = intNode->int_val;
		Type arrayType = createType_array("", type, size);
		SM_VarDec(varDecNode, arrayType, ret_var);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_FunDec(Node node, Type specType, int isDef) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	char* name = NULL;
	ListHead paramList = MyList_createList();
	if (node->expandNo == 1) {// ID LP VarList RP
		Node idNode = node->child[0];
		Node varListNode = node->child[2];
		name = idNode->str_val;
		paramList = MyList_createList();
		SM_VarList(varListNode, paramList);
	}
	else if (node->expandNo == 2) {// ID LP RP
		Node idNode = node->child[0];
		name = idNode->str_val;
	}

	if (isDef) {// to define
		Func func = findFunc_all(name);
		if (func == NULL) {
			func = createFunc(name, paramList, specType);
			insertFunc(getCurFuncTable(), func);
			declareFunc(func, node->lineno);
		}
		defineFunc(func, node->lineno);
	}
	else {// to declare
		Func func = findFunc_all(name);
		if (func == NULL) {
			func = createFunc(name, paramList, specType);
			insertFunc(getCurFuncTable(), func); 
			declareFunc(func, node->lineno);
		}
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_VarList(Node node, ListHead ret_list) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// ParamDec COMMA VarList
		Node paramDecNode = node->child[0];
		Node varListNode = node->child[2];
		Sym sym = NULL;
		SM_ParamDec(paramDecNode, &sym);
		if (ret_list) MyList_pushElem(ret_list, sym);
		SM_VarList(varListNode, ret_list);
	}
	else if (node->expandNo == 2){ //ParamDec
		Node paramDecNode = node->child[0];
		Sym sym = NULL;
		SM_ParamDec(paramDecNode, &sym);
		if (ret_list) MyList_pushElem(ret_list, sym);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_ParamDec(Node node, Sym* ret_var) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// Specifier VarDec
		Node specifierNode = node->child[0];
		Node varDecNode = node->child[1];
		Type specType = NULL;
		SM_Specifier(specifierNode, &specType);
		SM_VarDec(varDecNode, specType, ret_var);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_CompSt(Node node, Type returnType) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// LC DefList StmtList RC
		Node defListNode = node->child[1];
		Node stmtListNode = node->child[2];

		pushSymTable(createSymTable(FIELD_COMPST));
		pushTypeTable(createTypeTable(FIELD_COMPST));
		SM_DefList(defListNode);
		SM_StmtList(stmtListNode, returnType);
		popSymTable();
		popTypeTable();
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_StmtList(Node node, Type returnType) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// Stmt StmtList
		Node stmtNode = node->child[0];
		Node stmtListNode = node->child[1];
		SM_Stmt(stmtNode, returnType);
		SM_StmtList(stmtListNode, returnType);
	}
	else if (node->expandNo == 2) {//none

	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_Stmt(Node node, Type returnType) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// Exp SEMI
		Node expNode = node->child[0];
		ASTNode astNode = NULL;
		SM_Exp(expNode, &astNode);
		ASTtoIL(astNode);
		printASTTree(astNode);
		printf("\n\n");
	}
	else if (node->expandNo == 2) {// CompSt
		Node compStNode = node->child[0];
		SM_CompSt(compStNode, returnType);
	}
	else if (node->expandNo == 3) {// RETURN Exp SEMI
		Node expNode = node->child[1];
		SM_Exp(expNode, NULL);
	}
	else if (node->expandNo == 4) {// IF LP Exp RP Stmt
		Node expNode = node->child[2];
		Node stmtNode = node->child[4];
		SM_Exp(expNode, NULL);
		SM_Stmt(stmtNode, returnType);
	}
	else if (node->expandNo == 5) {// IF LP Exp RP Stmt ELSE Stmt
		Node expNode = node->child[2];
		Node stmtNode1 = node->child[4];
		Node stmtNode2 = node->child[6];
		SM_Exp(expNode, NULL);
		SM_Stmt(stmtNode1, returnType);
		SM_Stmt(stmtNode2, returnType);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_DefList(Node node) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// Def DefList
		Node defNode = node->child[0];
		Node defListNode = node->child[1];
		SM_Def(defNode);
		SM_DefList(defListNode);
	}
	else if (node->expandNo == 2) {// none

	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_Def(Node node) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// Specifier DecList SEMI
		Node specifierNode = node->child[0];
		Node decListNode = node->child[1];
		Type specType = NULL;
		SM_Specifier(specifierNode, &specType);
		SM_DecList(decListNode, specType);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_DecList(Node node, Type specType) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// Dec
		Node decNode = node->child[0];
		Sym sym = NULL;
		SM_Dec(decNode, specType, &sym);
		insertSym(getCurSymTable(), sym);
	}
	else if (node->expandNo == 2) {// Dec COMMA DecList
		Node decNode = node->child[0];
		Node decListNode = node->child[2];
		Sym sym = NULL;
		SM_Dec(decNode, specType, &sym);
		insertSym(getCurSymTable(), sym);
		SM_DecList(decListNode, specType);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_Dec(Node node, Type specType, Sym* ret_var) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// VarDec
		Node varDecNode = node->child[0];
		SM_VarDec(varDecNode, specType, ret_var);
	}
	else if (node->expandNo == 2) {// VarDec ASSIGNOP Exp
		Node varDecNode = node->child[0];
		Node expNode = node->child[2];
		SM_VarDec(varDecNode, specType, ret_var);
		SM_Exp(expNode, NULL);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_Exp(Node node, ASTNode* ret_astNode) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// Exp ASSIGNOP Exp
		Node expNode1 = node->child[0];
		Node expNode2 = node->child[2];

		if (ret_astNode == NULL) {
			SM_Exp(expNode1, NULL);
			SM_Exp(expNode2, NULL);
		}
		else {
			ASTNode astNode1 = NULL;
			ASTNode astNode2 = NULL;
			SM_Exp(expNode1, &astNode1);
			SM_Exp(expNode2, &astNode2);
			*ret_astNode = createASTNode_op(OP_ASSIGN, astNode1, astNode2);
		}
	}
	else if (node->expandNo == 4) {// Exp RELOP Exp
		Node expNode1 = node->child[0];
		Node opNode = node->child[1];
		Node expNode2 = node->child[2];
		
		if (ret_astNode == NULL) {
			SM_Exp(expNode1, NULL);
			SM_Exp(expNode2, NULL);
		}
		else {// TODO: change it
			OP op = opNode->op;
			ASTNode astNode1 = NULL;
			ASTNode astNode2 = NULL;
			SM_Exp(expNode1, &astNode1);
			SM_Exp(expNode2, &astNode2);
			*ret_astNode = createASTNode_op(op, astNode1, astNode2);
		}
	}
	else if (
		node->expandNo == 2 ||// Exp AND Exp
		node->expandNo == 3 ||// Exp OR Exp
		node->expandNo == 5 ||// Exp PLUS Exp
		node->expandNo == 6 ||// Exp MINUS Exp
		node->expandNo == 7 ||// Exp STAR Exp
		node->expandNo == 8// Exp DIV Exp
		) {
		Node expNode1 = node->child[0];
		Node opNode = node->child[1];
		Node expNode2 = node->child[2];

		if (ret_astNode == NULL) {
			SM_Exp(expNode1, NULL);
			SM_Exp(expNode2, NULL);
		}
		else {
			OP op = opNode->op;
			ASTNode astNode1 = NULL;
			ASTNode astNode2 = NULL;
			SM_Exp(expNode1, &astNode1);
			SM_Exp(expNode2, &astNode2);
			*ret_astNode = createASTNode_op(op, astNode1, astNode2);
		}
	}
	else if (node->expandNo == 9) {// LP Exp RP
		Node expNode = node->child[1];
		if (ret_astNode == NULL) {
			SM_Exp(expNode, NULL);
		}
		else {
			SM_Exp(expNode, ret_astNode);
		}
	}
	else if (node->expandNo == 10) {// NEG Exp
		Node expNode = node->child[1];

		if (ret_astNode == NULL) {
			SM_Exp(expNode, NULL);
		}
		else {
			ASTNode astNode = NULL;
			SM_Exp(expNode, &astNode);
			*ret_astNode = createASTNode_op(OP_NEG, astNode, NULL);
		}
	}
	else if (node->expandNo == 11) {// NOT Exp
		Node expNode = node->child[1];

		if (ret_astNode == NULL) {
			SM_Exp(expNode, NULL);
		}
		else {
			ASTNode astNode = NULL;
			SM_Exp(expNode, &astNode);
			*ret_astNode = createASTNode_op(OP_NOT, astNode, NULL);
		}
	}
	else if (node->expandNo == 12) {// ID LP Args RP
		Node idNode = node->child[0];
		Node argsNode = node->child[2];
		char* name = idNode->str_val;
		ListHead argList = MyList_createList();
		SM_Args(argsNode, argList);
		Func func = findFunc_all(name);
		
		if (ret_astNode)* ret_astNode = createASTNode_func(func);
	}
	else if (node->expandNo == 13) {// ID LP RP
		Node idNode = node->child[0];
		char* name = idNode->str_val;
		Func func = findFunc_all(name);

		if (ret_astNode)* ret_astNode = createASTNode_func(func);
	}
	else if (node->expandNo == 14) {// Exp LB Exp RB
		Node expNode1 = node->child[0];
		Node expNode2 = node->child[2];

		if (ret_astNode == NULL) {
			SM_Exp(expNode1, NULL);
			SM_Exp(expNode2, NULL);
		}
		else {
			ASTNode astNode1;
			ASTNode astNode2;
			SM_Exp(expNode1, &astNode1);
			SM_Exp(expNode2, &astNode2);
			// TODO:
		}
	}
	else if (node->expandNo == 15) {// Exp DOT ID
		Node expNode = node->child[0];
		Node idNode = node->child[2];
		char* name = idNode->str_val;

		SM_Exp(expNode, NULL);
	}
	else if (node->expandNo == 16) {// ID
		Node idNode = node->child[0];
		char* name = idNode->str_val;
		Sym sym = findSym_all(name);

		if (ret_astNode)* ret_astNode = createASTNode_sym(sym);
	}
	else if (node->expandNo == 17) {// INT
		Node intNode = node->child[0];
		int val = intNode->int_val;

		if (ret_astNode)* ret_astNode = createASTNode_integer(val);
	}
	else if (node->expandNo == 18) {// FLOAT
		Node floatNode = node->child[0];
		float val = floatNode->float_val;

		if (ret_astNode)* ret_astNode = createASTNode_float(val);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_Args(Node node, ListHead ret_list) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// Exp COMMA Args
		Node expNode = node->child[0];
		Node argsNode = node->child[2];
		SM_Exp(expNode, NULL);
		SM_Args(argsNode, ret_list);
	}
	else if (node->expandNo == 2) {// Exp
		Node expNode = node->child[0];
		SM_Exp(expNode, NULL);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}