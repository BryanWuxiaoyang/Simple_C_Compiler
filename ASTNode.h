#pragma once
#include <assert.h>

struct _ASTNodeHandler_;

enum ASTNodeType {
	AST_INTEGER,
	AST_FLOAT,
	AST_SYM,
	AST_FUNC,
	AST_OP
};
typedef enum ASTNodeType ASTNodeType;

int isInstantType(ASTNodeType type) {
	return type == AST_INTEGER || type == AST_FLOAT;
}

union ASTNodeValue {
	Sym sym;
	Func func;
	int intValue;
	float floatValue;
	enum OP op;
};
typedef union ASTNodeValue ASTNodeValue;

struct _ASTNode_ {
	int height;

	enum ASTNodeType type;
	union ASTNodeValue value;
	char* name;

	struct _ASTNode_* lc;
	struct _ASTNode_* rc;

	ListHead parents; // 用于子表达式优化，非必要项
	ConstValue constValue;  // 用于常量优化，非必要项

	int accessTag;

	struct _ASTNodeHandler_* handler;
};
typedef struct _ASTNode_* ASTNode;

struct _ASTNodeHandler_ {
	ASTNode node;
	int removeTag;
};
typedef struct _ASTNodeHandler_* ASTNodeHandler;

ASTNodeHandler createASTNodeHandler(ASTNode node) {
	if (node->handler == NULL) {
		node->handler = (ASTNodeHandler)malloc(sizeof(struct _ASTNodeHandler_));
		if (node->handler) {
			node->handler->node = node;
			node->handler->removeTag = 0;
		}
	}

	return node->handler;
}

ASTNode getASTNode(ASTNodeHandler handler) {
	if (handler->removeTag > 0) assert(0);
	else return handler->node;
	return NULL;
}

void destroyASTNodeHandler(ASTNodeHandler handler) {
	return;
}

void destroyASTNode(ASTNode node) {
	if (node->name) free(node->name);
	if (node->parents) MyList_destroyList(node->parents);
	if (node->handler) {
		node->handler->removeTag = 1;
		node->handler->node = NULL;
	}
}

ASTNode createASTNode(int height, ASTNodeType type, ASTNodeValue value, char* name, ASTNode lc, ASTNode rc, ListHead parents) {
	ASTNode node = (ASTNode)malloc(sizeof(struct _ASTNode_));
	if (node) {
		node->height = height;
		node->type = type;
		node->value = value;
		node->name = name;
		node->lc = lc;
		node->rc = rc;
		node->parents = parents;
		node->accessTag = 0;
		node->handler = NULL;
		assignConstNone(&node->constValue);

		node->handler = createASTNodeHandler(node);
	}
	return node;
}

int isExistASTNode(ASTNodeHandler handler) {
	return handler->removeTag == 0;
}
