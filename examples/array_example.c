#include <stdio.h>
#include <array.h>

// Note: you can not define template using type with more than one word in it, like:
// - unsigned int
// - const float
// - char*
// and so on, but if you need to, you can use a typedef, for example:
// - unsigned int -> uint (totaly valid for a template)
DEFINE_ARRAY_PROTOTYPE(int);
DEFINE_ARRAY_IMPLEMENTATION(int);

void PrintIntArray(const Array(int) ARRAY){
	if (ARRAY.size == 0){
		puts("[]");
		return;
	}

	putchar('[');
	for (size_t i = 0; i != ARRAY.size - 1; ++i)
		printf("%d, ", ARRAY.base[i]);
	printf("%d]\n", ARRAY.base[ARRAY.size - 1]);
}

int main(){
	Array(int) lut = CreateArray(int, 0, 1, 1, 2, 3, 5, 8, 13);
	// 'CreateArray(type, ...)' macro allows you to create array with initial values 
	// the amount of values is not limited
	// Note: if you don't want to initialize elements, you can use the 'AllocateArray(type, size)' function
	// Note: if you don't want any items at all, you can use the 'EMPTY_ARRAY(type)' macro

	printf("Initial state: "); PrintIntArray(lut);

	Array(int) sub = InsertSubArray(int, &lut, lut.size, 10);
	// 'InsertSubArray(type, array, index, size)'
	// function allows to insert new segment to array
	// function returns 'sub-array', size of which is the size of the inserted segment, -
	// and its base pointer points to the beggining of the inserted segment
	// Note: sub-arrays don't have to be destroyed, they are not separate objects
	// Note: if you want to insert another array, you can use the 'InsertArray(type, array, index, other)' function

	memset(sub.base, 0, sub.size * sizeof(int));

	printf("Sub-array: ");   PrintIntArray(sub);
	printf("Whole array: "); PrintIntArray(lut);

	EraseSubArray(int, &lut, sub); // 'EraseSubArray(type)(array, sub_array)'
	printf("Whole array after erasing the sub-array: "); PrintIntArray(lut);

	Array(int) copy = CopyArray(int, lut); // Note: copies have to be destroyed, they are separate objects
	printf("Copy: "); PrintIntArray(copy);
	printf("copy == lut?: %d\n", ArraysEqual(int, lut, copy));

	DestroyArray(int, &copy);
	DestroyArray(int, &lut);

	return 0;
}