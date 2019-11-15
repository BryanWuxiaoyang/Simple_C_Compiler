#pragma once
#include "MyList.h"
#include "SymTable.h"
#include "FuncTable.h"
#include "TypeTable.h"
#include "SemanticErrorChecks.h"
#include "AbstractSyntaxTree.h"
#include "OP.h"
#include "IntermediateLanguage.h"
#include "TranslateASTTree.h"

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

const char* getSymCodeStr(int code) {
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
	default:printf("enter error!\n"); assert(0);
	}
	return NULL;
}
void testEnterPrint(Node node) {
	indent++;
	for (int i = 0; i < indent; i++)
		printf("\t");
	printf("enter %s line %d expandNo %d\n", getSymCodeStr(node->symCode), node->lineno, node->expandNo);
}
void testExitPrint(Node node) {
	for (int i = 0; i < indent; i++)
		printf("\t");
	printf("exit %s line %d expandNo %d\n", getSymCodeStr(node->symCode), node->lineno, node->expandNo);
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
	if (node == NULL) assert(0);
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
	initIL();
}

void SM_Program(Node node);
void SM_ExtDefList(Node node);
void SM_ExtDef(Node node);
void SM_ExtDecList(Node node, Type decType);
void SM_Specifier(Node node, Type* ret_specType);
void SM_StructSpecifier(Node node, Type* ret_specType);
void SM_OptTag(Node node, char** ret_name);
void SM_Tag(Node node, char** ret_name);
void SM_VarDec(Node node, Type type, Sym* ret_var, ASTNode* ret_astNode);
void SM_FunDec(Node node, Type specType, int isDef);
void SM_VarList(Node node, ListHead ret_list);
void SM_ParamDec(Node node, Sym* ret_var, ASTNode* ret_astNode);
void SM_CompSt(Node node, Type returnType, ListHead nextList);
void SM_StmtList(Node node, Type returnType, ListHead nextList);
void SM_Stmt(Node node, Type returnType, ListHead nextList);
void SM_DefList(Node node);
void SM_Def(Node node);
void SM_DecList(Node node, Type specType);
void SM_Dec(Node node, Type specType, Sym* ret_var);
void SM_Exp(Node node, ASTNode* ret_astNode, ListHead trueList, ListHead falseList, int trueFall, int falseFall);
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
		ListHead nextList = MyList_createList();
		
		pushSymTable(createSymTable(FIELD_COMPST));
		pushTypeTable(createTypeTable(FIELD_COMPST));
		pushInnerASTTable();

		SM_Specifier(specifierNode, &specType);
		SM_FunDec(funDecNode, specType, 1);
		SM_CompSt(compStNode, specType, nextList);

		popSymTable();
		popTypeTable();
		popInnerASTTable();
		
		if (MyList_isEmpty(nextList) == 0) {
			char* label = createName_label();
			backpatchCode(nextList, label);
			appendInterCode(createInterCode(NULL, NULL, label, ILOP_LABEL));
		}
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
		SM_VarDec(varDecNode, decType, &sym, NULL);
		insertSym(getCurSymTable(), sym);
	}
	else if (node->expandNo == 2) {// VarDec COMMA ExtDecList
		Node varDecNode = node->child[0];
		Node extDecListNode = node->child[2];
		Sym sym = NULL;
		SM_VarDec(varDecNode, decType, &sym, NULL);
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

void SM_VarDec(Node node, Type type, Sym* ret_var, ASTNode* ret_astNode) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// ID
		Node idNode = node->child[0];
		char* name = idNode->str_val;
		Sym sym = createSym(name, type);
		ASTNode astNode = createASTNode_sym(sym);

		if (ret_var)* ret_var = sym;
		if (ret_astNode)* ret_astNode = astNode;
	}
	else if (node->expandNo == 2) {// VarDec LB INT RB
		Node varDecNode = node->child[0];
		Node intNode = node->child[2];
		int size = intNode->int_val;
		Type arrayType = createType_array("", type, size);
		SM_VarDec(varDecNode, arrayType, ret_var, ret_astNode);
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
			
			// insert params into local symTable
			ListIterator it = MyList_createIterator(paramList);
			while (MyList_hasNext(it)) {
				Sym param = (Sym)MyList_getNext(it);
				insertSym(getCurSymTable(), param);
			}
			MyList_destroyIterator(it);

			appendInterCode(createInterCode(NULL, NULL, func->name, ILOP_FUNCTION));
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

void SM_VarList(Node node, ListHead ret_symList) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// ParamDec COMMA VarList
		Node paramDecNode = node->child[0];
		Node varListNode = node->child[2];
		Sym sym = NULL;
		ASTNode astNode = NULL;

		SM_ParamDec(paramDecNode, &sym, &astNode);

		appendInterCode(createInterCode(NULL, NULL, getASTNodeStr_l(astNode), ILOP_PARAM));

		if (ret_symList) MyList_pushElem(ret_symList, sym);
		SM_VarList(varListNode, ret_symList);
	}
	else if (node->expandNo == 2){ //ParamDec
		Node paramDecNode = node->child[0];
		Sym sym = NULL;
		ASTNode astNode = NULL;

		SM_ParamDec(paramDecNode, &sym, &astNode);

		appendInterCode(createInterCode(NULL, NULL, getASTNodeStr_l(astNode), ILOP_PARAM));

		if (ret_symList) MyList_pushElem(ret_symList, sym);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_ParamDec(Node node, Sym* ret_var, ASTNode* ret_astNode) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// Specifier VarDec
		Node specifierNode = node->child[0];
		Node varDecNode = node->child[1];
		Type specType = NULL;
		SM_Specifier(specifierNode, &specType);
		SM_VarDec(varDecNode, specType, ret_var, ret_astNode);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_CompSt(Node node, Type returnType, ListHead nextList) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// LC DefList StmtList RC
		Node defListNode = node->child[1];
		Node stmtListNode = node->child[2];

		SM_DefList(defListNode);
		SM_StmtList(stmtListNode, returnType, nextList);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_StmtList(Node node, Type returnType, ListHead nextList) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// Stmt StmtList
		Node stmtNode = node->child[0];
		Node stmtListNode = node->child[1];
		ListHead tmpNextList = MyList_createList();

		SM_Stmt(stmtNode, returnType, tmpNextList);

		
		if (MyList_isEmpty(tmpNextList) == 0) {
			char* label = createName_label();
			backpatchCode(tmpNextList, label);
			appendInterCode(createInterCode(NULL, NULL, label, ILOP_LABEL));
		}
		MyList_destroyList(tmpNextList);
		
		SM_StmtList(stmtListNode, returnType, nextList);
	}
	else if (node->expandNo == 2) {//none
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_Stmt(Node node, Type returnType, ListHead nextList) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// Exp SEMI
		Node expNode = node->child[0];
		SM_Exp(expNode, NULL, NULL, NULL, 0, 0);
	}
	else if (node->expandNo == 2) {// CompSt
		Node compStNode = node->child[0];

		pushSymTable(createSymTable(FIELD_COMPST));
		pushTypeTable(createTypeTable(FIELD_COMPST));
		SM_CompSt(compStNode, returnType, nextList);
		popSymTable();
		popTypeTable();
	}
	else if (node->expandNo == 3) {// RETURN Exp SEMI
		Node expNode = node->child[1];
		ASTNode astNode = NULL;
		SM_Exp(expNode, &astNode, NULL, NULL, 0, 0);
		InterCode code = createInterCode(NULL, NULL, getASTNodeStr_r(astNode), ILOP_RETURN);
		appendInterCode(code);
	}
	else if (node->expandNo == 4) {// IF LP Exp RP Stmt
		Node expNode = node->child[2];
		Node stmtNode = node->child[4];
		ASTNode astNode = NULL;
		ListHead trueList = MyList_createList();
		SM_Exp(expNode, &astNode, trueList, nextList, 1, 0);

		if (isConstASTNode(astNode) && getConstInt(getConstValue(astNode)) == 1) {
				SM_Stmt(stmtNode, returnType, nextList);
		}
		else {
			if (MyList_isEmpty(trueList) == 0) {
				char* label = createName_label();
				backpatchCode(trueList, label);
				appendInterCode(createInterCode(NULL, NULL, label, ILOP_LABEL));
			}

			pushInnerASTTable();
			SM_Stmt(stmtNode, returnType, nextList);
			popInnerASTTable();
			removeASTNodes();
		}

		MyList_destroyList(trueList);
	}
	else if (node->expandNo == 5) {// IF LP Exp RP Stmt ELSE Stmt
		Node expNode = node->child[2];
		Node stmtNode1 = node->child[4];
		Node stmtNode2 = node->child[6];
		ASTNode astNode = NULL;
		ListHead trueList = MyList_createList();
		ListHead falseList = MyList_createList();
		char* label1 = createName_label();
		char* label2 = createName_label();

		printASTNodes();
		SM_Exp(expNode, &astNode, trueList, falseList, 1, 0);

		if (isConstASTNode(astNode)) {
			ConstValue value = getConstValue(astNode);
			if(getConstInt(value)){
				SM_Stmt(stmtNode1, returnType, falseList);
			}
			else {
				SM_Stmt(stmtNode2, returnType, nextList);
			}
		}
		else {
			ListHead removeList = MyList_createList();

			if (MyList_isEmpty(trueList) == 0) {
				appendInterCode(createInterCode(NULL, NULL, label1, ILOP_LABEL));
			}
			
			removeASTNodes(); // 清除掉之前的remove标记
			pushInnerASTTable();
			printASTNodes();
			SM_Stmt(stmtNode1, returnType, falseList);
			popInnerASTTable();
			printASTNodes();

			InnerASTNodeIterator it = createInnerASTNodeIterator();
			while (hasNextInnerASTNode(it)) {
				ASTNode node = getNextInnerASTNode(it);
				if (node->removeTag > 0) {
					MyList_pushElem(removeList, node);
					node->removeTag = 0;
				}
			}
			destroyInnerASTNodeIterator(it);
			
			if (MyList_isEmpty(falseList) == 0) {
				appendInterCode(createInterCode(NULL, NULL, label2, ILOP_LABEL));
			}

			pushInnerASTTable();
			SM_Stmt(stmtNode2, returnType, nextList);
			popInnerASTTable();
			printASTNodes();

			ListIterator removeIt = MyList_createIterator(removeList);
			while (MyList_hasNext(removeIt)) {
				ASTNode node = (ASTNode)MyList_getNext(removeIt);
				node->removeTag = 1;
			}
			MyList_destroyIterator(removeIt);
			
			removeASTNodes();
			printASTNodes();

			backpatchCode(trueList, label1);
			backpatchCode(falseList, label2);
		}

		MyList_destroyList(trueList);
		MyList_destroyList(falseList);
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
		ASTNode astNode = NULL;
		SM_VarDec(varDecNode, specType, ret_var, &astNode);

		if (getCurSymTable()->type != FIELD_STRUCT) {
			char* size = (char*)malloc(sizeof(char) * 10);
			if (size) {
				sprintf(size, "%d", (*ret_var)->type->size);
				appendInterCode(createInterCode(size, NULL, getASTNodeStr_l(astNode), ILOP_DEC));
			}
		}
	}
	else if (node->expandNo == 2) {// VarDec ASSIGNOP Exp
		Node varDecNode = node->child[0];
		Node expNode = node->child[2];
		ASTNode astNode1 = NULL;
		ASTNode astNode2 = NULL;
		SM_VarDec(varDecNode, specType, ret_var, &astNode1);
		SM_Exp(expNode, &astNode2, NULL, NULL, 0, 0);

		char* size = (char*)malloc(sizeof(char) * 10);
		if (size) {
			sprintf(size, "%d", (*ret_var)->type->size);
			appendInterCode(createInterCode(size, NULL, getASTNodeStr_l(astNode1), ILOP_DEC));
		}
		InterCode code = createInterCode(getASTNodeStr_r(astNode2), NULL, getASTNodeStr_l(astNode1), ILOP_ASSIGN);
		appendInterCode(code);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_Exp(Node node, ASTNode* ret_astNode, ListHead trueList, ListHead falseList, int trueFall, int falseFall) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// Exp ASSIGNOP Exp
		Node expNode1 = node->child[0];
		Node expNode2 = node->child[2];

		ASTNode astNode1 = NULL;
		ASTNode astNode2 = NULL;
		SM_Exp(expNode1, &astNode1, NULL, NULL, 0, 0);
		SM_Exp(expNode2, &astNode2, NULL, NULL, 0, 0);

		ASTNode resNode = createASTNode_op(OP_ASSIGN, astNode1, astNode2);
		if (ret_astNode)* ret_astNode = resNode;
		appendInterCode(createInterCode(getASTNodeStr_r(astNode2), NULL, getASTNodeStr_l(astNode1), ILOP_ASSIGN));
	}
	else if (node->expandNo == 2) {// Exp AND Exp
		Node expNode1 = node->child[0];
		Node expNode2 = node->child[2];
		ASTNode astNode1 = NULL;
		ASTNode astNode2 = NULL;
		ListHead tmpTrueList = MyList_createList();

		SM_Exp(expNode1, &astNode1, tmpTrueList, falseList, 1, 0);

		if (MyList_isEmpty(tmpTrueList) == 0) {
			char* label = createName_label();
			backpatchCode(tmpTrueList, label);
			appendInterCode(createInterCode(NULL, NULL, label, ILOP_LABEL));
		}

		if (isConstASTNode(astNode1) && getConstInt(getConstValue(astNode1)) == 0) {
			astNode2 = createASTNode_integer(0);
		}
		else {
			SM_Exp(expNode2, &astNode2, trueList, falseList, trueFall, falseFall);
		}

		MyList_destroyList(tmpTrueList);
		if (ret_astNode)* ret_astNode = createASTNode_op(OP_AND, astNode1, astNode2);
	}
	else if (node->expandNo == 3) {// Exp OR Exp
		Node expNode1 = node->child[0];
		Node expNode2 = node->child[2];
		ASTNode astNode1 = NULL;
		ASTNode astNode2 = NULL;
		

		ListHead tmpFalseList = MyList_createList();
		SM_Exp(expNode1, &astNode1, trueList, tmpFalseList, 0, 1);

		if (MyList_isEmpty(tmpFalseList) == 0) {
			char* label = createName_label();
			appendInterCode(createInterCode(NULL, NULL, label, ILOP_LABEL));
			backpatchCode(tmpFalseList, createName_label());
		}

		if (isConstASTNode(astNode1) && getConstInt(getConstValue(astNode1)) == 1) {
			astNode2 = createASTNode_integer(1);
		}
		else {
			SM_Exp(expNode2, &astNode2, trueList, falseList, trueFall, falseFall);
		}

		MyList_destroyList(tmpFalseList);
		if (ret_astNode)* ret_astNode = createASTNode_op(OP_OR, astNode1, astNode2);
	}
	else if (node->expandNo == 4) {// Exp RELOP Exp
		Node expNode1 = node->child[0];
		Node opNode = node->child[1];
		Node expNode2 = node->child[2];

		OP op = opNode->op;
		ASTNode astNode1 = NULL;
		ASTNode astNode2 = NULL;
		SM_Exp(expNode1, &astNode1, NULL, NULL, 0, 0);
		SM_Exp(expNode2, &astNode2, NULL, NULL, 0, 0);
		ASTNode resNode = createASTNode_op(op, astNode1, astNode2);
		if (ret_astNode)* ret_astNode = resNode;

		if (isConstASTNode(resNode)) {
			ConstValue value = getConstValue(resNode);
			if (getConstInt(value) != 0 && trueFall == 0) {
				InterCode code = createInterCode(NULL, NULL, NULL, ILOP_GOTO);
				appendInterCode(code);
				MyList_pushElem(trueList, code);
			}
			else if (getConstInt(value) == 0 && falseFall == 0) {
				InterCode code = createInterCode(NULL, NULL, NULL, ILOP_GOTO);
				appendInterCode(code);
				MyList_pushElem(falseList, code);
			}
		}
		else {
			if (trueFall == 0 && falseFall == 0) {
				ILOP ilop = getIfRelop(op);
				InterCode code1 = createInterCode(getASTNodeStr_r(astNode1), getASTNodeStr_r(astNode2), NULL, ilop);
				InterCode code2 = createInterCode(NULL, NULL, NULL, ILOP_GOTO);
				appendInterCode(code1);
				appendInterCode(code2);
				MyList_pushElem(trueList, code1);
				MyList_pushElem(falseList, code2);
			}
			else if (trueFall == 0) {
				ILOP ilop = getIfRelop(op);
				InterCode code = createInterCode(getASTNodeStr_r(astNode1), getASTNodeStr_r(astNode2), NULL, ilop);
				appendInterCode(code);
				MyList_pushElem(trueList, code);
			}
			else if (falseFall == 0) {
				op = getReverseRelop(op);
				ILOP ilop = getIfRelop(op);
				InterCode code = createInterCode(getASTNodeStr_r(astNode1), getASTNodeStr_r(astNode2), NULL, ilop);
				appendInterCode(code);
				MyList_pushElem(falseList, code);
			}
			else assert(0);
		}
	}
	else if (	
		node->expandNo == 5 ||// Exp PLUS Exp
		node->expandNo == 6 ||// Exp MINUS Exp
		node->expandNo == 7 ||// Exp STAR Exp
		node->expandNo == 8// Exp DIV Exp
		) {
		Node expNode1 = node->child[0];
		Node opNode = node->child[1];
		Node expNode2 = node->child[2];

		OP op = opNode->op;
		ASTNode astNode1 = NULL;
		ASTNode astNode2 = NULL;
		SM_Exp(expNode1, &astNode1, NULL, NULL, 0, 0);
		SM_Exp(expNode2, &astNode2, NULL, NULL, 0, 0);
		int nodeExist = findASTNode_op(op, astNode1, astNode2) ? 1 : 0;
		ASTNode resNode = createASTNode_op(op, astNode1, astNode2);
		if(ret_astNode)*ret_astNode = resNode;

		if (nodeExist == 0 && isConstASTNode(resNode) == 0) {
			ILOP ilop = getAlgorithmOP(op);
			InterCode code = createInterCode(getASTNodeStr_r(astNode1), getASTNodeStr_r(astNode2), getASTNodeStr_r(resNode), ilop);
			appendInterCode(code);
		}
	}
	else if (node->expandNo == 9) {// LP Exp RP
		Node expNode = node->child[1];
		SM_Exp(expNode, ret_astNode, trueList, falseList, trueFall, falseFall);
	}
	else if (node->expandNo == 10) {// NEG Exp
		Node expNode = node->child[1];
		ASTNode astNode = NULL;
		SM_Exp(expNode, &astNode, NULL, NULL, 0, 0);
		int nodeExist = findASTNode_op(OP_NEG, astNode, NULL) ? 1 : 0;
		ASTNode resNode = createASTNode_op(OP_NEG, astNode, NULL);
		if (ret_astNode)* ret_astNode = resNode;
		
		if (nodeExist == 0 && isConstASTNode(resNode) == 0) {
			InterCode code = createInterCode("0", getASTNodeStr_r(astNode), getASTNodeStr_l(resNode), ILOP_MINUS);
			appendInterCode(code);
		}
	}
	else if (node->expandNo == 11) {// NOT Exp
		Node expNode = node->child[1];

		ASTNode astNode = NULL;
		SM_Exp(expNode, &astNode, falseList, trueList, 0, 0);
		ASTNode resNode = createASTNode_op(OP_NOT, astNode, NULL);
		if (ret_astNode)* ret_astNode = resNode;
	} 
	else if (node->expandNo == 12) {// ID LP Args RP
		Node idNode = node->child[0];
		Node argsNode = node->child[2];
		char* name = idNode->str_val;
		ListHead argList = MyList_createList();
		SM_Args(argsNode, argList);
		Func func = findFunc_all(name);
		
		ASTNode resNode = createASTNode_func(func);
		if (ret_astNode)* ret_astNode = resNode;

		InterCode code = createInterCode(func->name, NULL, getASTNodeStr_l(resNode), ILOP_CALL);
		appendInterCode(code);

	}
	else if (node->expandNo == 13) {// ID LP RP
		Node idNode = node->child[0];
		char* name = idNode->str_val;
		Func func = findFunc_all(name);

		ASTNode resNode = createASTNode_func(func);
		if (ret_astNode)* ret_astNode = resNode;

		InterCode code = createInterCode(func->name, NULL, getASTNodeStr_l(resNode), ILOP_CALL);
		appendInterCode(code);
	}
	else if (node->expandNo == 14) {// Exp LB Exp RB
		Node expNode1 = node->child[0];
		Node expNode2 = node->child[2];

		//TODO:
	}
	else if (node->expandNo == 15) {// Exp DOT ID
		Node expNode = node->child[0];
		Node idNode = node->child[2];
		char* name = idNode->str_val;
		ASTNode structSymNode = NULL;
		SM_Exp(expNode, &structSymNode, NULL, NULL, 0, 0);
		Sym structSym = structSymNode->value.sym;
		Sym fieldSym = NULL;

		ListIterator it = MyList_createIterator(structSym->type->u.fieldList);
		while (MyList_hasNext(it)) {
			Sym sym = (Sym)MyList_getNext(it);
			if (strcmp(sym->name, name) == 0) {
				fieldSym = sym;
				break;
			}
		}
		MyList_destroyIterator(it);

		if (fieldSym == NULL) assert(0);
		int offset = fieldSym->offset;
		ASTNode refNode = createASTNode_op(OP_REF, structSymNode, NULL);
		ASTNode addNode = NULL;
		if (offset == 0) {
			addNode = refNode;
		}
		else {
			ASTNode offsetNode = createASTNode_integer(offset);
			addNode = createASTNode_op(OP_PLUS, refNode, offsetNode);
		}
		ASTNode derefNode = createASTNode_op(OP_DEREF, addNode, NULL);

		translateASTTree(derefNode);
		if (ret_astNode)* ret_astNode = derefNode;
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

		ASTNode astNode = NULL;
		SM_Exp(expNode, &astNode, NULL, NULL, 0, 0);

		MyList_pushElem(ret_list, astNode->value.sym);

		SM_Args(argsNode, ret_list);

		appendInterCode(createInterCode(NULL, NULL, getASTNodeStr_r(astNode), ILOP_ARG));
	}
	else if (node->expandNo == 2) {// Exp
		Node expNode = node->child[0];
		
		ASTNode astNode = NULL;
		SM_Exp(expNode, &astNode, NULL, NULL, 0, 0);
		MyList_pushElem(ret_list, astNode->value.sym);
		appendInterCode(createInterCode(NULL, NULL, getASTNodeStr_r(astNode), ILOP_ARG));
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}