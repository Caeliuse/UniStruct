#include <stdio.h>
#include <list.h>

// Note: you can not define template using type with more than one word in it, like:
// - unsigned int
// - const float
// - char*
// and so on, but if you need to, you can use typedefs, for example:
// - unsigned int -> uint (totaly valid for a template)
DEFINE_LIST_PROTOTYPE(int);
DEFINE_LIST_IMPLEMENTATION(int);

void PrintIntList(const List(int) LIST){
	if (LIST.size == 0){
		puts("[]");
		return;
	}

	putchar('[');
	for (ListNode(int) const* curr = LIST.head; curr->next != NULL; curr = curr->next)
		printf("%d, ", curr->item);
	printf("%d]\n", LIST.tail->item);
}

int main(){
	List(int) numbers = CreateList(int, 1, 2, 3);
	// 'CreateList(type, ...)'
	// macro allows you to create list with initial values
	// Note: if you don't want to initialize list with any values, you can use the 'EMPTY_LIST(type)' macro

	printf("Initial list: "); PrintIntList(numbers);

	ListNode(int)* inserted = ListAppend(int, &numbers);
	inserted->item = 4;
	// 'ListAppend(type, list)'
	// the function inserts a node to end of the list and -
	// (as every other list insertion function) returns pointer to the newly-inserted node

	ListPrepend(int, &numbers)->item = 0;
	// 'ListPrepend(type)(list)'
	// the function inserts element to beggining of the list

	printf("List after append + prepend: "); PrintIntList(numbers);

	ListErase(int, &numbers, inserted); // 'ListErase(type)(list, node)' erases node
	printf("List after erase: "); PrintIntList(numbers);

	ListInsert(int, &numbers, NodeAt(int, numbers, numbers.size))->item = 4;
	// 'ListInsert(type, list, node)'
	// the function inserts a new node to the list
	// Note: a new node will be inserted before the node which was passed to the function
	// Note: ListInsert(type, list, NULL) is valid, and is the same thing as ListAppend(type)(list)
	// 'NodeAt(type, list, index)'
	// returns pointer to the node on the passed index (NULL if index == list.size)

	printf("List after insertion: "); PrintIntList(numbers);

	List(int) copy = CopyList(int, numbers); // Note: copies have to be destroyed, they are separate objects
	printf("Copy: "); PrintIntList(copy);
	printf("copy == numbers?: %d\n", ListsEqual(int, copy, numbers));

	DestroyList(int, &copy);
	DestroyList(int, &numbers);

	return 0;
}