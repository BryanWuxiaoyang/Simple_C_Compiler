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
	initTypeTable();
	initFuncTable();
	initSymTable();
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
void SM_VarDec(Node node, Type type, Sym* ret_var, ASTNodeHandler* ret_handler);
void SM_FunDec(Node node, Type specType, int isDef);
void SM_VarList(Node node, ListHead ret_list);
void SM_ParamDec(Node node, Sym* ret_var, ASTNodeHandler* ret_handler);
void SM_CompSt(Node node, Type returnType, ListHead nextList);
void SM_StmtList(Node node, Type returnType, ListHead nextList);
void SM_Stmt(Node node, Type returnType, ListHead nextList);
void SM_DefList(Node node);
void SM_Def(Node node);
void SM_DecList(Node node, Type specType);
void SM_Dec(Node node, Type specType, Sym* ret_var);
void SM_Exp(Node node, ASTNodeHandler* ret_handler, ListHead trueList, ListHead falseList, int trueFall, int falseFall);
void SM_Args(Node node, ListHead ret_handlerList);

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

void SM_VarDec(Node node, Type type, Sym* ret_var, ASTNodeHandler* ret_handler) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// ID
		Node idNode = node->child[0];
		char* name = idNode->str_val;
		Sym sym = createSym(name, type);
		ASTNodeHandler handler = createASTNode_sym(sym);
		if (ret_var)* ret_var = sym;
		if (ret_handler)* ret_handler = handler;
	}
	else if (node->expandNo == 2) {// VarDec LB INT RB
		Node varDecNode = node->child[0];
		Node intNode = node->child[2];
		int size = intNode->int_val;
		Type arrayType = createType_array("", type, size);
		SM_VarDec(varDecNode, arrayType, ret_var, ret_handler);
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
		appendInterCode(createInterCode(NULL, NULL, name, ILOP_FUNCTION));
		SM_VarList(varListNode, paramList);
	}
	else if (node->expandNo == 2) {// ID LP RP
		Node idNode = node->child[0];
		name = idNode->str_val;
		appendInterCode(createInterCode(NULL, NULL, name, ILOP_FUNCTION));
	}

	if (isDef) {// to define
		Func func = findFunc_all(name);
		if (func == NULL) {
			func = createFunc(name, paramList, specType);
			insertFunc(getCurFuncTable(), func);
			declareFunc(func, node->lineno);

			// insert params into local symTable
			ListIterator handlerIt = MyList_createIterator(paramList);
			while (MyList_hasNext(handlerIt)) {
				Sym param = (Sym)MyList_getNext(handlerIt);
				insertSym(getCurSymTable(), param);
			}
			MyList_destroyIterator(handlerIt);
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
		ASTNodeHandler handler = NULL;

		SM_ParamDec(paramDecNode, &sym, &handler);

		if (sym->type->kind == STRUCTURE) {// ���ṹ���͵Ĳ���ת������ָ��
			sym->type = createType_addr(sym->type);
			getASTNode(handler)->varType = sym->type;
			insertType(getCurTypeTable(), sym->type);
		}
		appendInterCode(createInterCode(NULL, NULL, getASTNodeStr_l(getASTNode(handler)), ILOP_PARAM));
		if (ret_symList) MyList_pushElem(ret_symList, sym);

		SM_VarList(varListNode, ret_symList);
	}
	else if (node->expandNo == 2){ //ParamDec
		Node paramDecNode = node->child[0];
		Sym sym = NULL;
		ASTNodeHandler handler = NULL;

		SM_ParamDec(paramDecNode, &sym, &handler);

		if (sym->type->kind == STRUCTURE) {
			sym->type = createType_addr(sym->type);
			getASTNode(handler)->varType = sym->type;
			insertType(getCurTypeTable(), sym->type);
		}

		appendInterCode(createInterCode(NULL, NULL, getASTNodeStr_l(getASTNode(handler)), ILOP_PARAM));

		if (ret_symList) MyList_pushElem(ret_symList, sym);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_ParamDec(Node node, Sym* ret_var, ASTNodeHandler* ret_handler) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// Specifier VarDec
		Node specifierNode = node->child[0];
		Node varDecNode = node->child[1];
		Type specType = NULL;
		SM_Specifier(specifierNode, &specType);
		SM_VarDec(varDecNode, specType, ret_var, ret_handler);
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
		ASTNodeHandler handler = NULL;
		SM_Exp(expNode, &handler, NULL, NULL, 0, 0);
		InterCode code = createInterCode(NULL, NULL, getASTNodeStr_r(getASTNode(handler)), ILOP_RETURN);
		appendInterCode(code);
	}
	else if (node->expandNo == 4) {// IF LP Exp RP Stmt
		Node expNode = node->child[2];
		Node stmtNode = node->child[4];
		ASTNodeHandler handler = NULL;
		ListHead trueList = MyList_createList();
		SM_Exp(expNode, &handler, trueList, nextList, 1, 0);

		if (isConstASTNode(getASTNode(handler)) && getConstInt(getConstValue(getASTNode(handler))) == 1) {
			if (MyList_isEmpty(trueList) == 0) {
				ListIterator it = MyList_createIterator(trueList);
				while (MyList_hasNext(it)) {
					InterCode code = (InterCode)MyList_getNext(it);
					removeInterCode(code);
				}
				MyList_destroyIterator(it);
			}
			SM_Stmt(stmtNode, returnType, nextList);
		}
		else {
			if (MyList_isEmpty(trueList) == 0) {
				char* label = createName_label();
				backpatchCode(trueList, label);
				appendInterCode(createInterCode(NULL, NULL, label, ILOP_LABEL));
			}

#ifdef VAR_CONST_OPTMIZATION
			// ��������֧ǰ�Ľڵ�ĳ���״̬
			struct _ConstVar_ {
				ASTNodeHandler handler;
				ConstValue value;
				int remove;
			};
			typedef struct _ConstVar_* ConstVar;
			ListHead constList = MyList_createList();
			AllASTNodeIterator it = createAllASTNodeIterator();
			while (hasNextAllASTNode(it)) {
				ASTNodeHandler handler = getNextAllASTNode(it);
				ConstVar constVar = (ConstVar)malloc(sizeof(struct _ConstVar_));
				if (constVar == NULL) assert(0);
				constVar->handler = createASTNodeHandler(getASTNode(handler));
				assignConstValue(&getASTNode(handler)->constValue, &constVar->value);
				MyList_pushElem(constList, constVar);
			}
			destroyAllASTNodeIterator(it);
#endif

			pushInnerASTTable();
			SM_Stmt(stmtNode, returnType, nextList);
			popInnerASTTable();

#ifdef VAR_CONST_OPTMIZATION
			// �����б��ı䳣��״̬�Ľڵ���Ϊ�ǳ���
			ListIterator constIt = MyList_createIterator(constList);
			while (MyList_hasNext(constIt)) {
				ConstVar constVar = (ConstVar)MyList_getNext(constIt);
				if (isExistASTNode(constVar->handler) && isEqualConstValue(constVar->value, getASTNode(constVar->handler)->constValue) == 0) {
					assignConstNone(&getASTNode(constVar->handler)->constValue);
				}
				free(constVar);
			}
			MyList_destroyIterator(constIt);
			MyList_destroyList(constList);
#endif
		}

		MyList_destroyList(trueList);
	}
	else if (node->expandNo == 5) {// IF LP Exp RP Stmt ELSE Stmt
		Node expNode = node->child[2];
		Node stmtNode1 = node->child[4];
		Node stmtNode2 = node->child[6];
		ListHead trueList = MyList_createList();
		ListHead falseList = MyList_createList();
		char* label1 = createName_label();
		char* label2 = createName_label();
		ASTNodeHandler handler = NULL;

		SM_Exp(expNode, &handler, trueList, falseList, 1, 0);

		if (isConstASTNode(getASTNode(handler))) {
			ConstValue value = getConstValue(getASTNode(handler));
			if(getConstInt(value)){
				if (MyList_isEmpty(trueList) == 0) {
					ListIterator it = MyList_createIterator(trueList);
					while (MyList_hasNext(it)) {
						InterCode code = (InterCode)MyList_getNext(it);
						removeInterCode(code);
					}
					MyList_destroyIterator(it);
				}
				if (MyList_isEmpty(falseList) == 0) {
					ListIterator it = MyList_createIterator(falseList);
					while (MyList_hasNext(it)) {
						InterCode code = (InterCode)MyList_getNext(it);
						removeInterCode(code);
					}
					MyList_destroyIterator(it);
				}

				SM_Stmt(stmtNode1, returnType, nextList);
			}
			else {
				if (MyList_isEmpty(trueList) == 0) {
					ListIterator it = MyList_createIterator(trueList);
					while (MyList_hasNext(it)) {
						InterCode code = (InterCode)MyList_getNext(it);
						removeInterCode(code);
					}
					MyList_destroyIterator(it);
				}
				if (MyList_isEmpty(falseList) == 0) {
					ListIterator it = MyList_createIterator(falseList);
					while (MyList_hasNext(it)) {
						InterCode code = (InterCode)MyList_getNext(it);
						removeInterCode(code);
					}
					MyList_destroyIterator(it);
				}

				SM_Stmt(stmtNode2, returnType, nextList);
			}
		}
		else {
			if (MyList_isEmpty(trueList) == 0) {
				appendInterCode(createInterCode(NULL, NULL, label1, ILOP_LABEL));
			}

#ifdef VAR_CONST_OPTMIZATION
			// ��������֧ǰ�Ľڵ�ĳ���״̬
			struct _ConstVar_ {
				ASTNodeHandler handler;
				ConstValue value;
				int remove;
			};
			typedef struct _ConstVar_* ConstVar;
			ListHead constList = MyList_createList();
			AllASTNodeIterator it = createAllASTNodeIterator();
			while (hasNextAllASTNode(it)) {
				ASTNodeHandler handler = getNextAllASTNode(it);
				ConstVar constVar = (ConstVar)malloc(sizeof(struct _ConstVar_));
				if (constVar == NULL) assert(0);
				constVar->handler = createASTNodeHandler(getASTNode(handler));
				assignConstValue(&getASTNode(handler)->constValue, &constVar->value);
				MyList_pushElem(constList, constVar);
			}
			destroyAllASTNodeIterator(it);
#endif

#ifdef SUB_EXP_OPTMIZATION
			// �洢��ǰ���ӱ���ʽ�б�
			ListHead backUpInnerTableList = getBackUpInnerTableList();
#endif

			pushInnerASTTable();
			SM_Stmt(stmtNode1, returnType, falseList);
			popInnerASTTable();


#ifdef VAR_CONST_OPTMIZATION
			// ��ʱ�ָ��ڵ�ĳ�����״̬������������״̬�����ĵĽڵ�����Ԥɾ�����
			ListIterator constIt = MyList_createIterator(constList);
			while (MyList_hasNext(constIt)) {
				ConstVar constVar = (ConstVar)MyList_getNext(constIt);
				if (isExistASTNode(constVar->handler) && isEqualConstValue(constVar->value, getASTNode(constVar->handler)->constValue) == 0) {
					constVar->remove = 1;
				}
				assignConstValue(&constVar->value, &getASTNode(constVar->handler)->constValue);
			}
			MyList_destroyIterator(constIt);
#endif


#ifdef SUB_EXP_OPTMIZATION
			// ��һ����֧������������ǰ���ͱ��ݱ�����ʱ���ݱ��д����һ����֧��������ı�
			switchInnerTableList(&backUpInnerTableList);
#endif

			InterCode nextCode = createInterCode(NULL, NULL, NULL, ILOP_GOTO); // ��һ����֧Ū�������stmt��next
			appendInterCode(nextCode);
			MyList_pushElem(nextList, nextCode);
			
			if (MyList_isEmpty(falseList) == 0) {
				appendInterCode(createInterCode(NULL, NULL, label2, ILOP_LABEL));
			}

			pushInnerASTTable();
			SM_Stmt(stmtNode2, returnType, nextList);
			popInnerASTTable();
			
#ifdef VAR_CONST_OPTMIZATION
			// ���������������֧�У�����״̬�ı���Ľڵ�ĳ���״̬
			constIt = MyList_createIterator(constList);
			while (MyList_hasNext(constIt)) {
				ConstVar constVar = (ConstVar)MyList_getNext(constIt);
				if (isExistASTNode(constVar->handler) && (constVar->remove > 0 || isEqualConstValue(constVar->value, getASTNode(constVar->handler)->constValue) == 0)) {
					assignConstNone(&getASTNode(constVar->handler)->constValue);
				}
				free(constVar);
			}
			MyList_destroyIterator(constIt);
			MyList_destroyList(constList);
#endif

#ifdef SUB_EXP_OPTMIZATION
			// �����ݱ�����һ����֧������ı����뵱ǰ�����ڶ�����֧������ı����ϲ���ȡ��������Ϊ���η�֧��ʣ����ӱ���ʽ��
			mergeInnerTableList_intersect(backUpInnerTableList);
			destroyInnerTableList(backUpInnerTableList);
#endif

			backpatchCode(trueList, label1);
			backpatchCode(falseList, label2);
		}

		MyList_destroyList(trueList);
		MyList_destroyList(falseList);
	} else if (node->expandNo == 6) {// WHILE LP Exp RP Stmt
		Node expNode = node->child[2];
		Node stmtNode = node->child[4];
		ListHead trueList = MyList_createList();
		const char* label1 = createName_label();
		const char* label2 = createName_label();
		ASTNodeHandler handler = NULL;
		InterCode code1 = NULL;

		if (getLastCode()->op == ILOP_LABEL) {
			label1 = getLastCode()->target;
		}
		else {
			code1 = createInterCode(NULL, NULL, label1, ILOP_LABEL);
			appendInterCode(code1);
		}

#ifdef SUB_EXP_OPTMIZATION
		// ���е��ⲿ�ӱ���ʽ������while������ʹ��
		clearInnerASTNodes();
#endif

#ifdef VAR_CONST_OPTMIZATION
		// ���з����������ⲿ����״̬��������while������ʹ��
		AllASTNodeIterator it = createAllASTNodeIterator();
		while (hasNextAllASTNode(it)) {
			ASTNodeHandler handler = getNextAllASTNode(it);
			if (isConstASTNode(getASTNode(handler)) && isInstantType(getASTNode(handler)->type) == 0) {
				assignConstNone(&getASTNode(handler)->constValue);
				clearParents(getASTNode(handler));
			}
		}
		destroyAllASTNodeIterator(it);
#endif

		SM_Exp(expNode, &handler, trueList, nextList, 1, 0);

		if (MyList_isEmpty(trueList) == 0) {
			appendInterCode(createInterCode(NULL, NULL, label2, ILOP_LABEL));
			backpatchCode(trueList, label2);
		}

		if (isConstASTNode(getASTNode(handler)) && getConstInt(getConstValue(getASTNode(handler))) == 0) {
			// do nothing, remove label
			ListIterator codeIt = MyList_createIterator(getInterCodeList());
			while (MyList_hasNext(codeIt)) {
				InterCode code =(InterCode)MyList_getNext(codeIt);
				if (code == code1) {
					MyList_removePrev(codeIt);
					break;
				}
			}
			MyList_destroyIterator(codeIt);

			ListIterator nextListIt = MyList_createIterator(nextList);
			while (MyList_hasNext(nextListIt)) {
				InterCode nextListCode = (InterCode)MyList_getNext(nextListIt);
				removeInterCode(nextListCode);
			}
			MyList_destroyIterator(nextListIt);
			MyList_clear(nextList);
		}
		else {
			ListHead assignedHandlerList = MyList_createList();

			pushInnerASTTable();
			SM_Stmt(stmtNode, returnType, nextList);
			popInnerASTTable();

			appendInterCode(createInterCode(NULL, NULL, label1, ILOP_GOTO));
		}

#ifdef VAR_CONST_OPTMIZATION
		// ������while���������ɵĳ��������ܱ�ʹ��
		it = createAllASTNodeIterator();
		while (hasNextAllASTNode(it)) {
			ASTNodeHandler handler = getNextAllASTNode(it);
			if (isConstASTNode(getASTNode(handler)) && isInstantType(getASTNode(handler)->type) == 0) {
				assignConstNone(&getASTNode(handler)->constValue);
				clearParents(getASTNode(handler));
			}
		}
		destroyAllASTNodeIterator(it);
#endif
	
		MyList_destroyList(trueList);
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
		ASTNodeHandler handler = NULL;
		SM_VarDec(varDecNode, specType, ret_var, &handler);
		
		if (((*ret_var)->type->kind == STRUCTURE || (*ret_var)->type->kind == ARRAY) && getCurSymTable()->type != FIELD_STRUCT) {
			char* size = (char*)malloc(sizeof(char) * 10);
			if (size) {
				sprintf(size, "%d", (*ret_var)->type->size);
				appendInterCode(createInterCode(size, NULL, getASTNodeStr_l(getASTNode(handler)), ILOP_DEC));
			}
		}
	}
	else if (node->expandNo == 2) {// VarDec ASSIGNOP Exp
		Node varDecNode = node->child[0];
		Node expNode = node->child[2];
		ASTNodeHandler handler1 = NULL;
		ASTNodeHandler handler2 = NULL;
		SM_VarDec(varDecNode, specType, ret_var, &handler1);
		SM_Exp(expNode, &handler2, NULL, NULL, 0, 0);
		createASTNode_op(OP_ASSIGN, getASTNode(handler1), getASTNode(handler2));
		
		if (((*ret_var)->type->kind == STRUCTURE || (*ret_var)->type->kind == ARRAY) && getCurSymTable()->type != FIELD_STRUCT) {
			char* size = (char*)malloc(sizeof(char) * 10);
			if (size) {
				sprintf(size, "%d", (*ret_var)->type->size);
				appendInterCode(createInterCode(size, NULL, getASTNodeStr_l(getASTNode(handler1)), ILOP_DEC));
			}
		}

		InterCode code = createInterCode(getASTNodeStr_r(getASTNode(handler2)), NULL, getASTNodeStr_l(getASTNode(handler1)), ILOP_ASSIGN);
		appendInterCode(code);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_Exp(Node node, ASTNodeHandler* ret_handler, ListHead trueList, ListHead falseList, int trueFall, int falseFall) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// Exp ASSIGNOP Exp
		Node expNode1 = node->child[0];
		Node expNode2 = node->child[2];

		ASTNodeHandler handler1 = NULL;
		ASTNodeHandler handler2 = NULL;
		SM_Exp(expNode1, &handler1, NULL, NULL, 0, 0);
		SM_Exp(expNode2, &handler2, NULL, NULL, 0, 0);

		ASTNodeHandler resHandler = createASTNode_op(OP_ASSIGN, getASTNode(handler1), getASTNode(handler2));
		if (ret_handler)* ret_handler = resHandler;
		appendInterCode(createInterCode(getASTNodeStr_r(getASTNode(handler2)), NULL, getASTNodeStr_l(getASTNode(handler1)), ILOP_ASSIGN));
	}
	else if (node->expandNo == 2) {// Exp AND Exp
		Node expNode1 = node->child[0];
		Node expNode2 = node->child[2];
		ASTNodeHandler handler1 = NULL;
		ASTNodeHandler handler2 = NULL;
		ListHead tmpTrueList = MyList_createList();

		SM_Exp(expNode1, &handler1, tmpTrueList, falseList, 1, 0);

		if (MyList_isEmpty(tmpTrueList) == 0) {
			char* label = createName_label();
			backpatchCode(tmpTrueList, label);
			appendInterCode(createInterCode(NULL, NULL, label, ILOP_LABEL));
		}

		if (isConstASTNode(getASTNode(handler1)) && getConstInt(getConstValue(getASTNode(handler1))) == 0) {
			handler2 = createASTNode_integer(0);
		}
		else {
			SM_Exp(expNode2, &handler2, trueList, falseList, trueFall, falseFall);
		}

		MyList_destroyList(tmpTrueList);
		if (ret_handler)* ret_handler = createASTNode_op(OP_AND, getASTNode(handler1), getASTNode(handler2));
	}
	else if (node->expandNo == 3) {// Exp OR Exp
		Node expNode1 = node->child[0];
		Node expNode2 = node->child[2];
		ASTNodeHandler handler1 = NULL;
		ASTNodeHandler handler2 = NULL;
		
		ListHead tmpFalseList = MyList_createList();
		SM_Exp(expNode1, &handler1, trueList, tmpFalseList, 0, 1);

		if (MyList_isEmpty(tmpFalseList) == 0) {
			char* label = createName_label();
			appendInterCode(createInterCode(NULL, NULL, label, ILOP_LABEL));
			backpatchCode(tmpFalseList, createName_label());
		}

		if (isConstASTNode(getASTNode(handler1)) && getConstInt(getConstValue(getASTNode(handler2))) == 1) {
			handler2 = createASTNode_integer(1);
		}
		else {
			SM_Exp(expNode2, &handler2, trueList, falseList, trueFall, falseFall);
		}

		MyList_destroyList(tmpFalseList);
		
		ASTNodeHandler resHandler = createASTNode_op(OP_OR, getASTNode(handler1), getASTNode(handler2));
		if (ret_handler)* ret_handler = resHandler;
	}
	else if (node->expandNo == 4) {// Exp RELOP Exp
		Node expNode1 = node->child[0];
		Node opNode = node->child[1];
		Node expNode2 = node->child[2];
		OP op = opNode->op;
		ASTNodeHandler handler1 = NULL;
		ASTNodeHandler handler2 = NULL;

		SM_Exp(expNode1, &handler1, NULL, NULL, 0, 0);
		SM_Exp(expNode2, &handler2, NULL, NULL, 0, 0);
		ASTNodeHandler resHandler = createASTNode_op(op, getASTNode(handler1), getASTNode(handler2));
		if (ret_handler)* ret_handler = resHandler;

		if (isConstASTNode(getASTNode(resHandler))) {
			ConstValue value = getConstValue(getASTNode(resHandler));
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
				InterCode code1 = createInterCode(getASTNodeStr_r(getASTNode(handler1)), getASTNodeStr_r(getASTNode(handler2)), NULL, ilop);
				InterCode code2 = createInterCode(NULL, NULL, NULL, ILOP_GOTO);
				appendInterCode(code1);
				appendInterCode(code2);
				MyList_pushElem(trueList, code1);
				MyList_pushElem(falseList, code2);
			}
			else if (trueFall == 0) {
				ILOP ilop = getIfRelop(op);
				InterCode code = createInterCode(getASTNodeStr_r(getASTNode(handler1)), getASTNodeStr_r(getASTNode(handler2)), NULL, ilop);
				appendInterCode(code);
				MyList_pushElem(trueList, code);
			}
			else if (falseFall == 0) {
				op = getReverseRelop(op);
				ILOP ilop = getIfRelop(op);
				InterCode code = createInterCode(getASTNodeStr_r(getASTNode(handler1)), getASTNodeStr_r(getASTNode(handler2)), NULL, ilop);
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
		ASTNodeHandler handler1 = NULL;
		ASTNodeHandler handler2 = NULL;
		SM_Exp(expNode1, &handler1, NULL, NULL, 0, 0);
		SM_Exp(expNode2, &handler2, NULL, NULL, 0, 0);
		int nodeExist = findASTNode_op(op, getASTNode(handler1), getASTNode(handler2)) ? 1 : 0;

		ASTNodeHandler resHandler = createASTNode_op(op, getASTNode(handler1), getASTNode(handler2));
		if(ret_handler)*ret_handler = resHandler;

		if (nodeExist == 0 && isConstASTNode(getASTNode(resHandler)) == 0) {
			ILOP ilop = getAlgorithmOP(op);
			InterCode code = createInterCode(getASTNodeStr_r(getASTNode(handler1)), getASTNodeStr_r(getASTNode(handler2)), getASTNodeStr_r(getASTNode(resHandler)), ilop);
			appendInterCode(code);
		}
	}
	else if (node->expandNo == 9) {// LP Exp RP
		Node expNode = node->child[1];
		SM_Exp(expNode, ret_handler, trueList, falseList, trueFall, falseFall);
	}
	else if (node->expandNo == 10) {// NEG Exp
		Node expNode = node->child[1];
		ASTNodeHandler handler = NULL;
		SM_Exp(expNode, &handler, NULL, NULL, 0, 0);
		int nodeExist = findASTNode_op(OP_NEG, getASTNode(handler), NULL) ? 1 : 0;
		ASTNodeHandler resHandler = createASTNode_op(OP_NEG, getASTNode(handler), NULL);
		if (ret_handler)* ret_handler = resHandler;
		
		if (nodeExist == 0 && isConstASTNode(getASTNode(resHandler)) == 0) {
			InterCode code = createInterCode("0", getASTNodeStr_r(getASTNode(handler)), getASTNodeStr_l(getASTNode(resHandler)), ILOP_MINUS);
			appendInterCode(code);
		}
	}
	else if (node->expandNo == 11) {// NOT Exp
		Node expNode = node->child[1];

		ASTNodeHandler handler = NULL;
		SM_Exp(expNode, &handler, falseList, trueList, 0, 0);
		ASTNodeHandler resHandler = createASTNode_op(OP_NOT, getASTNode(handler), NULL);
		if (ret_handler)* ret_handler = resHandler;
	} 
	else if (node->expandNo == 12) {// ID LP Args RP
		Node idNode = node->child[0];
		Node argsNode = node->child[2];
		char* name = idNode->str_val;
		ListHead argHandlerList = MyList_createList();
		SM_Args(argsNode, argHandlerList);
		Func func = findFunc_all(name);
		
		ASTNodeHandler resHandler = createASTNode_func(func);
	
		if (func == writeFunc) {
			ASTNodeHandler argHandler = (ASTNodeHandler)MyList_getFront(argHandlerList);
			InterCode code = createInterCode(NULL, NULL, getASTNodeStr_r(getASTNode(argHandler)), ILOP_WRITE);
			appendInterCode(code);
		}
		else {
			ListIterator handlerIt = MyList_createIterator(argHandlerList);
			while (MyList_hasNext(handlerIt)) {
				ASTNodeHandler handler = (ASTNodeHandler)MyList_getNext(handlerIt);
				appendInterCode(createInterCode(NULL, NULL, getASTNodeStr_r(getASTNode(handler)), ILOP_ARG));
			}
			InterCode code = createInterCode(func->name, NULL, getASTNodeStr_l(getASTNode(resHandler)), ILOP_CALL);
			appendInterCode(code);
		}

		MyList_destroyList(argHandlerList);
		if (ret_handler)* ret_handler = resHandler;
	}
	else if (node->expandNo == 13) {// ID LP RP
		Node idNode = node->child[0];
		char* name = idNode->str_val;
		Func func = findFunc_all(name);

		ASTNodeHandler resHandler = createASTNode_func(func);
		if (ret_handler)* ret_handler = resHandler;

		if (func == readFunc) {
			InterCode code = createInterCode(NULL, NULL, getASTNodeStr_l(getASTNode(resHandler)), ILOP_READ);
			appendInterCode(code);
		}
		else {
			InterCode code = createInterCode(func->name, NULL, getASTNodeStr_l(getASTNode(resHandler)), ILOP_CALL);
			appendInterCode(code);
		}	
	}
	else if (node->expandNo == 14) {// Exp LB Exp RB
		Node expNode1 = node->child[0];
		Node expNode2 = node->child[2];
		ASTNodeHandler handler1 = NULL;
		ASTNodeHandler handler2 = NULL;
		//printf("Cannot translate: Code contains variables or parameters of structure type.\n");
		SM_Exp(expNode1, &handler1, NULL, NULL, 0, 0);
		SM_Exp(expNode2, &handler2, NULL, NULL, 0, 0);

		Type type = getASTNode(handler1)->varType;

		ASTNodeHandler refHandler = createASTNode_op(OP_REF, getASTNode(handler1), NULL);
		if (getASTNode(handler1)->type == AST_OP && (
			(getASTNode(handler1)->value.op == OP_DEREF && getASTNodeStr_r(getASTNode(handler1))[0]=='*')|| 
			(getASTNode(handler1)->value.op == OP_REF && getASTNodeStr_r(getASTNode(handler1))[0]=='&')
			)
			) {
			char* newName = createName_temp();
			appendInterCode(createInterCode(getASTNodeStr_r(getASTNode(handler1)), NULL, newName, ILOP_ASSIGN));
			getASTNode(handler1)->name = newName;
		}

		ASTNodeHandler elemSizeHandler = createASTNode_integer(type->u.array.elemType->size);
		ASTNodeHandler offsetHandler = createASTNode_op(OP_STAR, getASTNode(handler2), getASTNode(elemSizeHandler));
		ASTNodeHandler addHandler = createASTNode_op(OP_PLUS, getASTNode(refHandler), getASTNode(offsetHandler));
		ASTNodeHandler derefHandler = createASTNode_op(OP_DEREF, getASTNode(addHandler), NULL);
		if (getASTNode(derefHandler)->varType->kind == ARRAY) {
			getASTNode(derefHandler)->varType = getASTNode(derefHandler)->varType->u.targetType;
		}

		getASTNode(handler1)->accessTag = 1;
		getASTNode(handler2)->accessTag = 1;// 不需要再次输出计算过的东西
		translateASTTree(getASTNode(derefHandler));
		
		if (ret_handler)* ret_handler = derefHandler;
	}
	else if (node->expandNo == 15) {// Exp DOT ID
		Node expNode = node->child[0];
		Node idNode = node->child[2];
		char* name = idNode->str_val;
		ASTNodeHandler structSymHandler = NULL;
		SM_Exp(expNode, &structSymHandler, NULL, NULL, 0, 0);

		Sym fieldSym = NULL;
		Type varType = getASTNode(structSymHandler)->varType;
		Type structType = NULL;
		ASTNodeHandler refHandler = NULL;
		
		if (varType->kind == ADDR) {
			refHandler = structSymHandler;
			structType = varType->u.targetType;
		}
		else if(varType->kind == STRUCTURE){
			if (getASTNode(structSymHandler)->type == AST_OP && 
				(
					(getASTNode(structSymHandler)->value.op == OP_DEREF && getASTNodeStr_r(getASTNode(structSymHandler))[0] == '*')|| 
					(getASTNode(structSymHandler)->value.op == OP_REF && getASTNodeStr_r(getASTNode(structSymHandler))[0] == '&')
					)
			){
				char* newName = createName_temp();
				appendInterCode(createInterCode(getASTNodeStr_r(getASTNode(structSymHandler)), NULL, newName, ILOP_ASSIGN));
				getASTNode(structSymHandler)->name = newName;
			}
			refHandler = createASTNode_op(OP_REF, getASTNode(structSymHandler), NULL);
			structType = varType;
		}
		else {
			assert(0);
		}

		ListIterator handlerIt = MyList_createIterator(structType->u.fieldList);
		while (MyList_hasNext(handlerIt)) {
			Sym sym = (Sym)MyList_getNext(handlerIt);
			if (strcmp(sym->name, name) == 0) {
				fieldSym = sym;
				break;
			}
		}
		MyList_destroyIterator(handlerIt);

		if (fieldSym == NULL) assert(0);
		int offset = fieldSym->offset;
		ASTNodeHandler offsetHandler = NULL;
		ASTNodeHandler addHandler = NULL;
		if (structType->kind == ADDR && offset == 0) {
			addHandler = refHandler;
		}
		else {
			offsetHandler = createASTNode_integer(offset);
			addHandler = createASTNode_op(OP_PLUS, getASTNode(refHandler), getASTNode(offsetHandler));
		}
		
		ASTNodeHandler derefHandler = createASTNode_op(OP_DEREF, getASTNode(addHandler), NULL);

		translateASTTree(getASTNode(derefHandler));
		if (ret_handler)* ret_handler = derefHandler;
	}
	else if (node->expandNo == 16) {// ID
		Node idNode = node->child[0];
		char* name = idNode->str_val;
		Sym sym = findSym_all(name);

		if (ret_handler)* ret_handler = createASTNode_sym(sym);
	}
	else if (node->expandNo == 17) {// INT
		Node intNode = node->child[0];
		int val = intNode->int_val;

		if (ret_handler)* ret_handler = createASTNode_integer(val);
	}
	else if (node->expandNo == 18) {// FLOAT
		Node floatNode = node->child[0];
		float val = floatNode->float_val;

		if (ret_handler)* ret_handler = createASTNode_float(val);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}

void SM_Args(Node node, ListHead ret_handlerList) {
#ifdef TEST_MODE
	testEnterPrint(node);
#endif
	if (node->expandNo == 1) {// Exp COMMA Args
		Node expNode = node->child[0];
		Node argsNode = node->child[2];

		ASTNodeHandler handler = NULL;
		SM_Exp(expNode, &handler, NULL, NULL, 0, 0);

		if (getASTNode(handler)->value.sym->type->kind == STRUCTURE) {
			clearParents(getASTNode(handler));
			handler = createASTNode_op(OP_REF, getASTNode(handler), NULL);
		}

		MyList_pushElem(ret_handlerList, handler);

		SM_Args(argsNode, ret_handlerList);
	}
	else if (node->expandNo == 2) {// Exp
		Node expNode = node->child[0];
		
		ASTNodeHandler handler = NULL;
		SM_Exp(expNode, &handler, NULL, NULL, 0, 0);

		if (getASTNode(handler)->type == AST_SYM && getASTNode(handler)->value.sym->type->kind == STRUCTURE) {
			clearParents(getASTNode(handler));
			handler = createASTNode_op(OP_REF, getASTNode(handler), NULL);
		}

		MyList_pushElem(ret_handlerList, handler);
	}
#ifdef TEST_MODE
	testExitPrint(node);
#endif
}
