#pragma once
#include <stdlib.h>
#include <stdio.h>

struct _ListNode_ {
	void* elem;
	struct _ListNode_* next;
	struct _ListNode_* prev;
};
typedef struct _ListNode_* ListNode;
typedef struct _ListNode_* ListHead;

struct _ListIterator_ {
	ListHead head;
	ListNode next;
	ListNode prev;
};
typedef struct _ListIterator_* ListIterator;

ListIterator MyList_createIterator(ListHead head) {
	ListIterator handlerIt = (ListIterator)malloc(sizeof(struct _ListIterator_));
	if (handlerIt) {
		handlerIt->head = head;
		handlerIt->next = head->next;
		handlerIt->prev = head;
	}
	return handlerIt;
}

ListIterator MyList_createReverseIterator(ListHead head) {
	ListIterator handlerIt = (ListIterator)malloc(sizeof(struct _ListIterator_));
	if(handlerIt) {
		handlerIt->head = head;
		handlerIt->next = head;
		handlerIt->prev = head->prev;
	}
	return handlerIt;
}

void* MyList_getNext(ListIterator handlerIt) {
	void* elem = handlerIt->next->elem;
	handlerIt->prev = handlerIt->next;
	handlerIt->next = handlerIt->next->next;
	return elem;
}

void* MyList_peekNext(ListIterator handlerIt) {
	return handlerIt->next->elem;
}

int MyList_hasNext(ListIterator handlerIt) {
	return (handlerIt->next == handlerIt->head) ? 0 : 1;
}

void* MyList_getPrev(ListIterator handlerIt) {
	void* elem = handlerIt->prev->elem;
	handlerIt->next = handlerIt->prev;
	handlerIt->prev = handlerIt->prev->prev;
	return elem;
}

void* MyList_peekPrev(ListIterator handlerIt) {
	return handlerIt->prev->elem;
}

int MyList_hasPrev(ListIterator handlerIt) {
	return (handlerIt->prev == handlerIt->head) ? 0 : 1;
}


void MyList_destroyIterator(ListIterator handlerIt) {
	free(handlerIt);
}



ListHead MyList_createList() {
	ListHead head = (ListHead)malloc(sizeof(struct _ListNode_));
	if (head) {
		head->elem = NULL;
		head->next = head;
		head->prev = head;
	}
	return head;
}

ListNode MyList_wrapNode(void* elem) {
	ListNode node = (ListNode)malloc(sizeof(struct _ListNode_));
	if (node) {
		node->elem = elem;
		node->next = node;
		node->prev = node;
	}
	return node;
}

void MyList_pushNode(ListHead head, ListNode node) {
	node->next = head;
	node->prev = head->prev;
	head->prev->next = node;
	head->prev = node;
}

void MyList_pushElem(ListHead head, void* elem) {
	MyList_pushNode(head, MyList_wrapNode(elem));
}

int MyList_isEmpty(ListHead head) {
	return head->next == head ? 1 : 0;
}

void* MyList_pop(ListHead head) {
	void* elem = NULL;
	if (MyList_isEmpty(head) == 0) {
		ListNode p = head->prev;
		elem = p->elem;
		ListNode q = head->prev->prev;
		q->next = head;
		head->prev = q;
		free(p);
	}
	return elem;
}

void* MyList_removePrev(ListIterator handlerIt) {
	ListNode node1 = handlerIt->prev->prev;
	ListNode node2 = handlerIt->prev;
	ListNode node3 = handlerIt->next;
	node1->next = node3;
	node3->prev = node1;
	handlerIt->prev = node1;
	handlerIt->next = node3;
	return node2->elem;
}

void* MyList_removeNext(ListIterator handlerIt) {
	ListNode node1 = handlerIt->prev;
	ListNode node2 = handlerIt->next;
	ListNode node3 = handlerIt->next->next;
	node1->next = node3;
	node3->prev = node1;
	handlerIt->prev = node1;
	handlerIt->next = node3;
	return node2->elem;
}

void MyList_clear(ListHead handlerList) {
	ListNode p = handlerList->next;
	while (p != handlerList) {
		ListNode q = p;
		p = p->next;
		free(q);
	}
	handlerList->next = handlerList->prev = handlerList;
}

void MyList_insert(ListIterator handlerIt, void* elem) {
	ListNode node1 = handlerIt->prev;
	ListNode node2 = MyList_wrapNode(elem);
	ListNode node3 = handlerIt->next;
	node2->prev = node1;
	node2->next = node3;
	node1->next = node2;
	node3->prev = node2;
	handlerIt->prev = node1;
	handlerIt->next = node2;
}

void MyList_destroyList(ListHead handlerList) {
	MyList_clear(handlerList);
	free(handlerList);
}

void* MyList_getBack(ListHead handlerList) {
	return handlerList->prev->elem;
}

void* MyList_getFront(ListHead handlerList) {
	return handlerList->next->elem;
}

void MyList_mergeList_intersect(ListHead dstList, ListHead srcList) {
	ListIterator dstIt = MyList_createIterator(dstList);
	while (MyList_hasNext(dstIt)) {
		void* dstElem = MyList_getNext(dstIt);
		int suc = 0;

		ListIterator srcIt = MyList_createIterator(srcList);
		while (MyList_hasNext(srcIt)) {
			void* srcElem = MyList_getNext(srcIt);
			if (dstElem == srcElem) {
				suc = 1;
				break;
			}
		}
		MyList_destroyIterator(srcIt);

		if (suc == 0) MyList_removePrev(dstIt);
	}

	MyList_destroyIterator(dstIt);
}