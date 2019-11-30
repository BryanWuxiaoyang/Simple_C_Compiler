#pragma once
#include "ASTNode.h"
#include <stdlib.h>
#include <stdio.h>

#define BLOCK_CAPACITY 100

struct _NodeCell_ {
	struct _ASTNode_ node;
	struct _NodeCell_* next;
};
typedef struct _NodeCell_* NodeCell;

struct _NodeBlock_ {
	struct _NodeCell_ cellList[BLOCK_CAPACITY];
	struct _NodeBlock_* next;
};
typedef struct _NodeBlock_* NodeBlock;

NodeBlock blockHead;

NodeBlock blockTail;

NodeCell availCell;

NodeBlock createNodeBlock() {
	NodeBlock block = (NodeBlock)malloc(sizeof(struct _NodeBlock_));
	if (block) {
		block->next = NULL;
		for (int i = 0; i < BLOCK_CAPACITY - 1; i++) {
			block->cellList[i].next = &block->cellList[i + 1];
		}
		block->cellList[BLOCK_CAPACITY - 1].next = NULL;
	}
	return block;
}

void initASTNodeMemoryManage() {
	blockHead = createNodeBlock();
	blockTail = blockHead;
	availCell = &blockHead->cellList[0];
}


void checkASTNodeMemoryManageInit() {
	if (blockHead == NULL) {
		initASTNodeMemoryManage();
	}
}

void freeNodeCell(NodeCell cell) {
	clearASTNode(&cell->node);
	cell->next = availCell;
	availCell = cell;
}

void freeNodeCells() {
	NodeBlock block = blockHead;
	while (block->next) {
		for (int i = 0; i < BLOCK_CAPACITY; i++) {
			if (block->cellList[i].node.removeTag > 0) {
				freeNodeCell(&block->cellList[i]);
			}
		}
		block = block->next;
	}
}

ASTNode mallocASTNodeSpace() {
	checkASTNodeMemoryManageInit();
	if (availCell->next == NULL) {
		freeNodeCells();
	}
	if (availCell->next == NULL) {
		NodeBlock newBlock = createNodeBlock();
		blockTail->next = newBlock;
		availCell->next = &newBlock->cellList[0];
	}

	ASTNode node = &availCell->node;
	availCell = availCell->next;
	return node;
}
