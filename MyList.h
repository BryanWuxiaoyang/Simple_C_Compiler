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
	ListIterator it = (ListIterator)malloc(sizeof(struct _ListIterator_));
	if (it) {
		it->head = head;
		it->next = head->next;
		it->prev = head;
	}
	return it;
}

ListIterator MyList_createReverseIterator(ListHead head) {
	ListIterator it = (ListIterator)malloc(sizeof(struct _ListIterator_));
	if(it) {
		it->head = head;
		it->next = head;
		it->prev = head->prev;
	}
	return it;
}

void* MyList_getNext(ListIterator it) {
	void* elem = it->next->elem;
	it->prev = it->next;
	it->next = it->next->next;
	return elem;
}

int MyList_hasNext(ListIterator it) {
	return (it->next == it->head) ? 0 : 1;
}

void* MyList_getPrev(ListIterator it) {
	void* elem = it->prev->elem;
	it->next = it->prev;
	it->prev = it->prev->prev;
	return elem;
}

int MyList_hasPrev(ListIterator it) {
	return (it->prev == it->head) ? 0 : 1;
}


void MyList_destroyIterator(ListIterator it) {
	free(it);
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

void* MyList_removePrev(ListIterator it) {
	ListNode node1 = it->prev->prev;
	ListNode node2 = it->prev;
	ListNode node3 = it->next;
	node1->next = node3;
	node3->prev = node1;
	it->prev = node1;
	it->next = node3;
	return node2->elem;
}

void* MyList_removeNext(ListIterator it) {
	ListNode node1 = it->prev;
	ListNode node2 = it->next;
	ListNode node3 = it->next->next;
	node1->next = node3;
	node3->prev = node1;
	it->prev = node1;
	it->next = node3;
	return node2->elem;
}

void MyList_clear(ListHead list) {
	ListNode p = list->next;
	while (p != list) {
		ListNode q = p;
		p = p->next;
		free(q);
	}
	list->next = list->prev = list;
}

void MyList_insert(ListIterator it, void* elem) {
	ListNode node1 = it->prev;
	ListNode node2 = MyList_wrapNode(elem);
	ListNode node3 = it->next;
	node2->prev = node1;
	node2->next = node3;
	node1->next = node2;
	node3->prev = node2;
	it->prev = node1;
	it->next = node2;
}

void MyList_destroyList(ListHead list) {
	MyList_clear(list);
	free(list);
}

void* MyList_getBack(ListHead list) {
	return list->prev->elem;
}

void* MyList_getFront(ListHead list) {
	return list->next->elem;
}