# CTL (C Template Library):

A simple library that provides basic data structures as templates

## Dependencies:
- C/C++ compiler (GCC is preferable)
- archiver (ar, often preinstalled)
- make

## Build:
- To build the library, use the 'Makefile' file without any additional arguments:
commad: 'make'

- To build the tests, use the 'Makefile' file with additional argument 'test':
command: 'make test'

## Usage:
- You can find more examples in the 'examples' folder

* Array example:
~~~c
	#include "array.h"

	DEFINE_ARRAY_PROTOTYPE(int);
	DEFINE_ARRAY_IMPLEMENTATION(int);

	...
	Array(int) array = CreateArray(int, 1, 2, 3); // create array with initial values
	Array(int) sub = InsertSubArray(int, &array, 0, 4); // insert segment of size 4 in index 0
	...
~~~

- List example:
~~~c
	#include "list.h"

	DEFINE_LIST_PROTOTYPE(int);
	DEFINE_LIST_IMPLEMENTATION(int);

	...
	List(int) list = CreateList(int, 1, 2, 3); // create list with initial values
	ListNode(int)* node = NodeAt(list, 1); // get node at index 1
	...
~~~

## License:
This project is licensed under the [MIT License](LICENSE.md)