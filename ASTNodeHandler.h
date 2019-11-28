#pragma once
#include "ASTNode.h"
#include "MyList.h"
#include "ASTNodeMemoryManage.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define ASTNODE_MEMORY_MANAGE_ENABLE

#define MEMORY_FREE_ENABLE

struct _ASTNodeHandler_ {
	ASTNode node;
	int refCnt;
};
typedef struct _ASTNodeHandler_* ASTNodeHandler;

ASTNodeHandler createASTNodeHandler(ASTNode node) {
	ASTNodeHandler handler = (ASTNodeHandler)malloc(sizeof(struct _ASTNodeHandler_));
	if (handler) {
		handler->node = node;
		handler->refCnt = 1;
	}
	return handler;
}

ASTNode getASTNode(ASTNodeHandler handler) {
	return handler->node;
}

void destroyASTNodeHandler(ASTNodeHandler handler) {
	handler->refCnt--;
	if (handler->refCnt == 0) {
#ifdef MEMORY_FREE_ENABLE
#ifdef ASTNODE_MEMORY_MANAGE_ENABLE
		handler->node->removeTag = 1;
#else
		clearASTNode(handler->node);
		free(handler->node);
#endif
		free(handler);
#endif
	}
}

ASTNodeHandler copyASTNodeHandler(ASTNodeHandler handler) {
	handler->refCnt++;
	return handler;
}

ASTNode createASTNode(int height, ASTNodeType type, ASTNodeValue value, char* name, ASTNode lc, ASTNode rc, ListHead parents, Type varType) {
#ifdef ASTNODE_MEMORY_MANAGE_ENABLE
	ASTNode node = mallocASTNodeSpace();
#else
	ASTNode node = (ASTNode)malloc(sizeof(struct _ASTNode_));
#endif
	if (node) {
		node->height = height;
		node->type = type;
		node->value = value;
		node->name = name;
		node->lc = lc;
		node->rc = rc;
		node->parents = parents;
		node->accessTag = 0;
		node->removeTag = 0;
		assignConstNone(&node->constValue);
		node->varType = varType;
	}
	return node;
}

int isExistASTNode(ASTNodeHandler handler) {
	return handler->node->removeTag == 0;
}
