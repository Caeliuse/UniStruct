#include "test_array.h"
#include "message.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <array.h>

DEFINE_ARRAY_PROTOTYPE(int);
DEFINE_ARRAY_PROTOTYPE(double); // define an additional type to check for conflicts

void AllocateArrayTest(){
	const usize SIZE = 10;
	Array(int) array = AllocateArray(int, SIZE);

	assert(array.base != NULL);
	assert(array.size == SIZE);

	for (usize i = 0; i != array.size; ++i){
		array.base[i] = i;
		assert(array.base[i] == (int)i);
	}

	DestroyArray(int, &array);
}

void CreateArrayTest(){
	Array(int) array = CreateArray(int, 1, 2, 3, 4, 5);

	assert(array.base != NULL);
	assert(array.size == 5);

	for (usize i = 0; i != array.size; ++i)
		assert(array.base[i] == (int)(i + 1));

	DestroyArray(int, &array);
}

void DestroyArrayTest(){
	Array(int) array = AllocateArray(int, 10);
	DestroyArray(int, &array);

	assert(array.base == NULL);
	assert(array.size == 0);

	array = EMPTY_ARRAY(int);
	DestroyArray(int, &array); // check for absence of SEGFAULT

	assert(array.base == NULL);
	assert(array.size == 0);
}

void InsertSubArrayTest(){
	// INITIAL = EMPTY_ARRAY(int);
	static const int STATE_1[] = {0, 0, 0};
	static const int STATE_2[] = {0, 0, 0, 1, 1};
	static const int STATE_3[] = {0, 0, 0, 2, 1, 1};
	static const int STATE_4[] = {0, 0, 0, 2, 1, 1}; // insert zero items
	static const int* STATES[] = {
		STATE_1,
		STATE_2,
		STATE_3,
		STATE_4
	};
	static const usize STATES_N = sizeof(STATES)/sizeof(STATES[0]);

	typedef struct{
		usize index;
		usize expected_size;
	}InsInfo;

	static const InsInfo INS_INFO[] = {
		(InsInfo){.index = 0, .expected_size = sizeof(STATE_1)/sizeof(STATE_1[0])},
		(InsInfo){.index = 3, .expected_size = sizeof(STATE_2)/sizeof(STATE_2[0])},
		(InsInfo){.index = 3, .expected_size = sizeof(STATE_3)/sizeof(STATE_3[0])},
		(InsInfo){.index = 0, .expected_size = sizeof(STATE_4)/sizeof(STATE_4[0])}
	};

	Array(int) array = EMPTY_ARRAY(int);

	for (usize ins = 0; ins != STATES_N; ++ins){
		const usize INDEX = INS_INFO[ins].index;
		const usize SIZE = INS_INFO[ins].expected_size - array.size;

		Array(int) sub = InsertSubArray(int, &array, INDEX, SIZE);

		assert(sub.size == SIZE);
		assert(sub.base != NULL);
		assert(sub.base == array.base + INDEX);
		assert(array.size == INS_INFO[ins].expected_size);

		for (usize i = 0; i != sub.size; ++i)
			sub.base[i] = ins;
		assert(memcmp(STATES[ins], array.base, array.size * sizeof(int)) == 0);
	}

	DestroyArray(int, &array);
}

void InsertArrayTest(){
	static const int STATE_1[] = {1, 2, 3};
	static const int STATE_2[] = {1, 2, 3, 9, 0};
	static const int STATE_3[] = {1, 2, 3, 2, 9, 0};
	static const int STATE_4[] = {1, 2, 3, 2, 9, 0}; // insert zero items
	static const int* STATES[] = {
		STATE_1,
		STATE_2,
		STATE_3,
		STATE_4
	};
	static const usize STATES_N = sizeof(STATES)/sizeof(STATES[0]);

	typedef struct{
		usize index;
		usize expected_size;
	}InsInfo;

	static const InsInfo INS_INFO[] = {
		(InsInfo){.index = 0, .expected_size = sizeof(STATE_1)/sizeof(STATE_1[0])},
		(InsInfo){.index = 3, .expected_size = sizeof(STATE_2)/sizeof(STATE_2[0])},
		(InsInfo){.index = 3, .expected_size = sizeof(STATE_3)/sizeof(STATE_3[0])},
		(InsInfo){.index = 0, .expected_size = sizeof(STATE_4)/sizeof(STATE_4[0])}
	};

	Array(int) array = EMPTY_ARRAY(int);

	for (usize ins = 0; ins != STATES_N; ++ins){
		const usize INDEX = INS_INFO[ins].index;
		const usize SIZE = INS_INFO[ins].expected_size - array.size;

		const Array(int) TO_INSERT = (Array(int)){.size = SIZE, .base = STATES[ins] + INDEX};
		Array(int) sub = InsertArray(int, &array, INDEX, TO_INSERT);

		assert(sub.size == SIZE);
		assert(sub.base != NULL);
		assert(sub.base == array.base + INDEX);
		assert(array.size == INS_INFO[ins].expected_size);

		assert(memcmp(STATES[ins], array.base, array.size * sizeof(int)) == 0);
	}

	DestroyArray(int, &array);
}

void EraseSubArrayTest(){
	static int INITIAL[] = {0, 1, 2, 3, 4, 5};
	static int STATE_1[] = {0, 1, 2, 3};
	static int STATE_2[] = {1, 2, 3};
	static int STATE_3[] = {1, 3};
	static int STATE_4[] = {1, 3}; // erase zero items

	static int* STATES[] = {
		STATE_1,
		STATE_2,
		STATE_3,
		STATE_4
	};
	const usize STATES_N = sizeof(STATES)/sizeof(STATES[0]);

	typedef struct{
		usize index;
		usize size;
	}ErsInfo;

	static const ErsInfo ERS_INFO[] = {
		(ErsInfo){.index = 4, .size = 2},
		(ErsInfo){.index = 0, .size = 1},
		(ErsInfo){.index = 1, .size = 1},
		(ErsInfo){.index = 0, .size = 0}
	};

	Array(int) array = AllocateArray(int, sizeof(INITIAL)/sizeof(INITIAL[0]));
	memcpy(array.base, INITIAL, array.size * sizeof(int));

	for (usize ers = 0; ers != STATES_N; ++ers){
		const usize INDEX = ERS_INFO[ers].index;
		const usize SIZE = ERS_INFO[ers].size;

		const usize PREV_SIZE = array.size;
		const Array(int) TO_ERASE = (Array(int)){.base = array.base + INDEX, .size = SIZE};
		EraseSubArray(int, &array, TO_ERASE);

		assert(array.size == PREV_SIZE - SIZE);
		assert(memcmp(STATES[ers], array.base, array.size * sizeof(int)) == 0);
	}

	DestroyArray(int, &array);
}

void CopyArrayTest(){
	Array(int) original = AllocateArray(int, 10); // no need to init, the values are not important in this test
	Array(int) copy = CopyArray(int, original);

	assert(copy.base != NULL);
	assert(copy.size == original.size);
	assert(memcmp(copy.base, original.base, copy.size * sizeof(int)) == 0);

	DestroyArray(int, &original);
	DestroyArray(int, &copy);
}

void ArrayEqualsTest(){
	Array(int) a = AllocateArray(int, 10);
	Array(int) b = AllocateArray(int, a.size);
	memcpy(b.base, a.base, b.size * sizeof(int)); // 'b' is copy of array 'a'

	assert(ArraysEqual(int, a, b) == true);
	assert(ArraysEqual(int, EMPTY_ARRAY(int), EMPTY_ARRAY(int)) == true);
	assert(ArraysEqual(int, EMPTY_ARRAY(int), a) == false);
	assert(ArraysEqual(int, a, EMPTY_ARRAY(int)) == false);

	DestroyArray(int, &a);
	DestroyArray(int, &b);
}

void TestArray(){
	printf("\'AllocateArray\'\ttest..."); 	AllocateArrayTest();	Message("\t[passed]\n", GREEN);
	printf("\'CreateArray\'\ttest...");		CreateArrayTest();		Message("\t[passed]\n", GREEN);
	printf("\'DestroyArray\'\ttest...");	DestroyArrayTest();		Message("\t[passed]\n", GREEN);
	printf("\'InsertSubArray\'\ttest...");	InsertSubArrayTest();	Message("\t[passed]\n", GREEN);
	printf("\'InsertArray\'\ttest...");		InsertArrayTest();		Message("\t[passed]\n", GREEN);
	printf("\'EraseSubArray\'\ttest...");	EraseSubArrayTest();	Message("\t[passed]\n", GREEN);
	printf("\'CopyArray\'\ttest...");		CopyArrayTest();		Message("\t[passed]\n", GREEN);
	printf("\'ArrayEquals\'\ttest...");		ArrayEqualsTest();		Message("\t[passed]\n", GREEN);
}

DEFINE_ARRAY_IMPLEMENTATION(int);
DEFINE_ARRAY_IMPLEMENTATION(double);