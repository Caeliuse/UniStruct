#include "test_list.h"
#include "message.h"
#include <stdio.h>
#include <list.h>
#include <assert.h>
#include <stdbool.h>

DEFINE_LIST_PROTOTYPE(int);
DEFINE_LIST_PROTOTYPE(double); // define an additional type to check the absence of conflicts

void CreateListTest(){
	const usize SIZE = 5;
	List(int) list = CreateList(int, 1, 2, 3, 4, 5);

	assert(list.size == SIZE);
	assert(list.head != NULL);
	assert(list.tail != NULL);
	assert(list.head != list.tail);

	ListNode(int)* curr = list.head;
	usize i = 0;

	while (curr != NULL){
		assert(curr->item == (int)(i + 1));
		curr = curr->next;
		++i;
	}

	assert(i == SIZE);
	DestroyList(int, &list);
}

void DestroyListTest(){
	List(int) list = CreateList(int, 10, 11, 4);
	DestroyList(int, &list);

	assert(list.size == 0);
	assert(list.head == NULL);
	assert(list.tail == NULL);

	list = EMPTY_LIST(int);
	DestroyList(int, &list); // check for absence of SEGFAULT
}

void ListAppendTest(){
	List(int) list = EMPTY_LIST(int);

	int value = 10;
	ListAppend(int, &list)->item = value;

	assert(list.size == 1);
	assert(list.head != NULL);
	assert(list.tail != NULL);
	assert(list.head == list.tail);
	assert(list.tail->item == value);

	value = 15;
	ListAppend(int, &list)->item = value;

	assert(list.size == 2);
	assert(list.head != NULL);
	assert(list.tail != NULL);
	assert(list.head != list.tail);
	assert(list.tail->item == value);

	DestroyList(int, &list);
}

void ListPrependTest(){
	List(int) list = EMPTY_LIST(int);

	int value = 10;
	ListPrepend(int, &list)->item = value;

	assert(list.size == 1);
	assert(list.head != NULL);
	assert(list.tail != NULL);
	assert(list.head == list.tail);
	assert(list.head->item == value);

	value = 15;
	ListPrepend(int, &list)->item = value;

	assert(list.size == 2);
	assert(list.head != NULL);
	assert(list.tail != NULL);
	assert(list.head != list.tail);
	assert(list.head->item == value);

	DestroyList(int, &list);
}

void ListInsertTest(){
	List(int) list = EMPTY_LIST(int);

	int value = 10;
	ListInsert(int, &list, list.tail)->item = value;

	assert(list.size == 1);
	assert(list.head != NULL);
	assert(list.tail != NULL);
	assert(list.head == list.tail);
	assert(list.head->item == value);

	value = 3;
	ListInsert(int, &list, list.head)->item = value;

	assert(list.size == 2);
	assert(list.head != NULL);
	assert(list.tail != NULL);
	assert(list.head != list.tail);
	assert(list.head->item == value);

	value = 44;
	ListInsert(int, &list, list.head->next)->item = value;

	assert(list.size == 3);
	assert(list.head != NULL);
	assert(list.tail != NULL);
	assert(list.head != list.tail);
	assert(list.head->next->item == value);

	DestroyList(int, &list);
}

void ListEraseTest(){
	List(int) list = EMPTY_LIST(int);

	ListAppend(int, &list)->item = 0;
	ListAppend(int, &list)->item = 1;
	ListAppend(int, &list)->item = 2;
	ListAppend(int, &list)->item = 3;
	ListAppend(int, &list)->item = 4;

	usize size = list.size;
	ListErase(int, &list, list.head);

	assert(list.head != NULL);
	assert(list.tail != NULL);
	assert(list.size == size - 1);
	assert(list.head->item == 1);

	size = list.size;
	ListErase(int, &list, list.tail);

	assert(list.head != NULL);
	assert(list.tail != NULL);
	assert(list.size == size - 1);
	assert(list.tail->item == 3);

	size = list.size;
	ListErase(int, &list, list.head->next);

	assert(list.head != NULL);
	assert(list.tail != NULL);
	assert(list.size == size - 1);
	assert(list.head->next->item == 3);

	DestroyList(int, &list);
}

void NodeAtTest(){
	List(int) list = CreateList(int, 10, 11, 12, 4, 2, 0);

	ListNode(int)* node = NodeAt(int, list, 0);
	assert(node == list.head);

	node = NodeAt(int, list, list.size - 1);
	assert(node == list.tail);

	node = NodeAt(int, list, 1);
	assert(node == list.head->next);

	DestroyList(int, &list);
}

void CopyListTest(){
	List(int) original = EMPTY_LIST(int);
	for (usize i = 0; i != 10; ++i)
		ListAppend(int, &original)->item = (int)i;

	List(int) copy = CopyList(int, original);
	assert(copy.size == original.size);

	usize i = 0;
	for (ListNode(int)* node = copy.head; node != NULL; node = node->next){
		assert(node->item == (int)i);
		++i;
	}

	DestroyList(int, &original);
	DestroyList(int, &copy);
}

void ListsEqualTest(){
	List(int) original = CreateList(int, 10, 1, 8, 2, 3);
	List(int) copy = CopyList(int, original);

	assert(ListsEqual(int, original, copy) == true);
	copy.head->item += 1;
	assert(ListsEqual(int, original, copy) == false);

	assert(ListsEqual(int, EMPTY_LIST(int), EMPTY_LIST(int)) == true);
	assert(ListsEqual(int, EMPTY_LIST(int), copy) == false);
	assert(ListsEqual(int, copy, EMPTY_LIST(int)) == false);

	DestroyList(int, &original);
	DestroyList(int, &copy);
}

void TestList(){
	printf("\'CreateList\'\ttest...");	CreateListTest();	Message("\t[passed]\n", GREEN);
	printf("\'DestroyList\'\ttest...");	DestroyListTest();	Message("\t[passed]\n", GREEN);
	printf("\'ListAppend\'\ttest...");	ListAppendTest();	Message("\t[passed]\n", GREEN);
	printf("\'ListPrepend\'\ttest...");	ListPrependTest();	Message("\t[passed]\n", GREEN);
	printf("\'ListInsert\'\ttest...");	ListInsertTest();	Message("\t[passed]\n", GREEN);
	printf("\'ListErase\'\ttest...");	ListEraseTest();	Message("\t[passed]\n", GREEN);
	printf("\'NodeAt\'\ttest...");		NodeAtTest();	Message("\t[passed]\n", GREEN);
	printf("\'CopyList\'\ttest...");	CopyListTest();		Message("\t[passed]\n", GREEN);
	printf("\'ListsEqual\'\ttest...");	ListsEqualTest();	Message("\t[passed]\n", GREEN);
}

DEFINE_LIST_IMPLEMENTATION(int);
DEFINE_LIST_IMPLEMENTATION(double);