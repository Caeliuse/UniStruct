#ifndef LIST_H
#define LIST_H

#include "variadic.h"
#include "alarmists.h"
#include "defines.h"
#include <stdlib.h>
#include <string.h>

#define ListNode(type) ListNode##_##type
#define List(type) List##_##type

#define CreateListNode(type, prev, next) CreateListNode##_##type(prev, next)
#define DestroyListNode(type, node) DestroyListNode##_##type(node)

#define DestroyList(type, list) DestroyList##_##type(list)

#define ListAppend(type, list) ListAppend##_##type(list)
#define ListPrepend(type, list) ListPrepend##_##type(list)

#define ListInsert(type, list, node) ListInsert##_##type(list, node)
#define ListErase(type, list, node) ListErase##_##type(list, node)

#define NodeAt(type, list, index) NodeAt##_##type(list, index)
#define CopyList(type, list) CopyList##_##type(list)
#define ListsEqual(type, a, b) ListsEqual##_##type(a, b)

#define EMPTY_LIST(type) (List(type)){.head = NULL, .tail = NULL, .size = 0}

#define CreateList(type, ...) ({						\
	const usize COUNT = ARGCOUNT(type, __VA_ARGS__);	\
	const type* ARGS = (type[]){__VA_ARGS__};			\
	List(type) list = EMPTY_LIST(type);					\
														\
	for (usize i = 0; i != COUNT; ++i)					\
		ListAppend(type, &list)->item = ARGS[i];		\
	list;												\
})														\

#define DEFINE_LIST_PROTOTYPE(type)		\
	typedef struct ListNode(type){		\
		struct ListNode(type)* next;	\
		struct ListNode(type)* prev;	\
		type item;						\
	}ListNode(type);					\
										\
	typedef struct{						\
		ListNode(type)* head;			\
		ListNode(type)* tail;			\
		usize size;						\
	}List(type);						\
										\
	ListNode(type)* CreateListNode(type, ListNode(type)*, ListNode(type)*);	\
	ListNode(type)* DestroyListNode(type, ListNode(type)*);					\
																			\
	void DestroyList(type, List(type)*);									\
																			\
	ListNode(type)* ListAppend(type, List(type)*);							\
	ListNode(type)* ListPrepend(type, List(type)*);							\
																			\
	ListNode(type)* ListInsert(type, List(type)*, ListNode(type)*);			\
	void ListErase(type, List(type)*, ListNode(type)*);						\
																			\
	ListNode(type)* NodeAt(type, List(type), usize);						\
	List(type) CopyList(type, const List(type));							\
	bool ListsEqual(type, const List(type), const List(type));				\

#define DEFINE_LIST_IMPLEMENTATION(type)												\
	ListNode(type)* CreateListNode(type, ListNode(type)* prev, ListNode(type)* next){	\
		ListNode(type)* node = MallocAlarmist(sizeof(ListNode(type)));					\
																						\
		node->prev = prev;																\
		node->next = next;																\
																						\
		return node;																	\
	}																					\
																						\
	ListNode(type)* DestroyListNode(type, ListNode(type)* node){	\
		ListNode(type)* next = node->next;							\
		free(node);													\
		return next;												\
	}																\
																	\
	void DestroyList(type, List(type)* list){						\
		while (list->head) 											\
			list->head = DestroyListNode(type, list->head);			\
		*list = EMPTY_LIST(type);									\
	}																\
																	\
	ListNode(type)* ListAppend(type, List(type)* list){				\
		ListNode(type)* tail = list->tail;							\
		ListNode(type)* node = CreateListNode(type, tail, NULL);	\
																	\
		if (tail) tail->next = node;								\
		else list->head = node;										\
																	\
		list->tail = node;											\
		++list->size;												\
																	\
		return node;												\
	}																\
																	\
	ListNode(type)* ListPrepend(type, List(type)* list){			\
		ListNode(type)* head = list->head;							\
		ListNode(type)* node = CreateListNode(type, NULL, head);	\
																	\
		if (head) head->prev = node;								\
		else list->tail = node;										\
																	\
		list->head = node;											\
		++list->size;												\
																	\
		return node;												\
	}																\
																	\
	ListNode(type)* ListInsert(type, List(type)* list, ListNode(type)* next){	\
		if (next == NULL) return ListAppend(type, list);						\
		if (next->prev == NULL) return ListPrepend(type, list);					\
																				\
		ListNode(type)* prev = next->prev;										\
		ListNode(type)* node = CreateListNode(type, prev, next);				\
																				\
		prev->next = node;														\
		next->prev = node;														\
																				\
		++list->size;															\
		return node;															\
	}																			\
																				\
	void ListErase(type, List(type)* list, ListNode(type)* node){		\
		ListNode(type)* next = node->next;								\
		ListNode(type)* prev = node->prev;								\
																		\
		if (next) next->prev = prev;									\
		else list->tail = prev;											\
																		\
		if (prev) prev->next = next;									\
		else list->head = next;											\
																		\
		DestroyListNode(type, node);									\
		--list->size;													\
	}																	\
																		\
	ListNode(type)* NodeAt(type, List(type) list, usize index){			\
		ListNode(type)* NextNode(ListNode(type)* node){					\
			return node->next;											\
		}																\
																		\
		ListNode(type)* PrevNode(ListNode(type)* node){					\
			return node->prev;											\
		}																\
																		\
		if (index >= list.size)											\
			return NULL;												\
																		\
		typedef ListNode(type)*(*ShiftFunc)(ListNode(type)*);			\
																		\
		ShiftFunc shift;												\
		ListNode(type)* start;											\
																		\
		if (index < list.size / 2){										\
			shift = NextNode;											\
			start = list.head;											\
		}																\
		else{															\
			shift = PrevNode;											\
			start = list.tail;											\
			index = list.size - index - 1;								\
		}																\
																		\
		while (index--)													\
			start = shift(start);										\
		return start;													\
	}																	\
																						\
	List(type) CopyList(type, const List(type) LIST){									\
		List(type) copy = EMPTY_LIST(type);												\
		for (ListNode(type)* curr = LIST.head; curr != NULL; curr = curr->next)			\
			ListAppend(type, &copy)->item = curr->item;									\
		return copy;																	\
	}																					\
																						\
	bool ListsEqual(type, const List(type) A, const List(type) B){	\
		if (A.size != B.size)										\
			return false;											\
																	\
		ListNode(type)* a = A.head;									\
		ListNode(type)* b = B.head;									\
																	\
		while (a != NULL){											\
			if (memcmp(&a->item, &b->item, sizeof(type)) != 0)		\
				return false;										\
			a = a->next;											\
			b = b->next;											\
		}															\
		return true;												\
	}																\

#endif//LIST_H