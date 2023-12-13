#ifndef ARRAY_H
#define ARRAY_H

#include "variadic.h"
#include "alarmists.h"
#include "defines.h"
#include <stdlib.h>
#include <string.h>

#define Array(type) Array##_##type

#define AllocateArray(type, size) AllocateArray##_##type(size)
#define DestroyArray(type, arrp) DestroyArray##_##type(arrp)

#define InsertSubArray(type, arrp, index, size) InsertSubArray##_##type(arrp, index, size)
#define InsertArray(type, arrp, index, content) InsertArray##_##type(arrp, index, content)
#define EraseSubArray(type, arrp, sub) EraseSubArray##_##type(arrp, sub)

#define CopyArray(type, original) CopyArray##_##type(original)
#define ArraysEqual(type, a, b) ArraysEqual##_##type(a, b)

#define EMPTY_ARRAY(type) (Array(type)){.base = NULL, .size = 0}

#define CreateArray(type, ...) ({									\
	const usize COUNT = ARGCOUNT(type, __VA_ARGS__);				\
	Array(type) array = AllocateArray(type, COUNT);					\
	memcpy(array.base, (type[]){__VA_ARGS__}, COUNT * sizeof(type));\
	array;															\
})																	\

#ifdef __cplusplus
extern "C"{
#endif
	void InsertSegment(void**, const usize, const usize, const usize);
	void EraseSegment(void**, const usize, const usize, const usize);
#ifdef __cplusplus
}
#endif

#define DEFINE_ARRAY_PROTOTYPE(type)	\
	typedef struct{						\
		type* base;						\
		usize size;						\
	}Array(type);						\
										\
	Array(type) AllocateArray(type, const usize);	\
	void DestroyArray(type, Array(type)*);			\
													\
	Array(type) InsertSubArray(type, Array(type)*, const usize, const usize);	\
	Array(type) InsertArray(type, Array(type)*, const usize, const Array(type));\
	void EraseSubArray(type, Array(type)*, const Array(type));					\
																				\
	Array(type) CopyArray(type, const Array(type));								\
	bool ArraysEqual(type, const Array(type), const Array(type));				\

#define DEFINE_ARRAY_IMPLEMENTATION(type)					\
	Array(type) AllocateArray(type, const usize SIZE){		\
		return (Array(type)){								\
			.size = SIZE,									\
			.base = MallocAlarmist(SIZE * sizeof(type))		\
		};													\
	}														\
															\
	void DestroyArray(type, Array(type)* array){			\
		free(array->base);									\
		*array = EMPTY_ARRAY(type);							\
	}														\
															\
	Array(type) InsertSubArray(type, Array(type)* array, const usize INDEX, const usize SIZE){		\
		const usize ARRAY_BYTE_SIZE = array->size * sizeof(type);									\
		const usize BYTE_INDEX = INDEX * sizeof(type);												\
		const usize SUB_BYTE_SIZE = SIZE * sizeof(type);											\
		void** basep = (void**)&array->base;														\
																									\
		InsertSegment(basep, ARRAY_BYTE_SIZE, BYTE_INDEX, SUB_BYTE_SIZE);							\
		array->size += SIZE;																		\
																									\
		return (Array(type)){																		\
			.size = SIZE,																			\
			.base = array->base + INDEX 															\
		};																							\
	}																								\
																									\
	Array(type) InsertArray(type, Array(type)* array, const usize INDEX, const Array(type) CONTENT){	\
		Array(type) sub = InsertSubArray(type, array, INDEX, CONTENT.size);								\
		memcpy(sub.base, CONTENT.base, sub.size * sizeof(type));										\
		return sub;																						\
	}																									\
																										\
	void EraseSubArray(type, Array(type)* array, const Array(type) SUB){	\
		const usize INDEX = SUB.base - array->base;							\
		const usize SIZE = SUB.size;										\
																			\
		const usize ARRAY_BYTE_SIZE = array->size * sizeof(type);			\
		const usize BYTE_INDEX = INDEX * sizeof(type);						\
		const usize SUB_BYTE_SIZE = SIZE * sizeof(type);					\
		void** basep = (void**)&array->base;								\
																			\
		EraseSegment(basep, ARRAY_BYTE_SIZE, BYTE_INDEX, SUB_BYTE_SIZE);	\
		array->size -= SIZE;												\
	}																		\
																			\
	Array(type) CopyArray(type, const Array(type) ORIGINAL){				\
		Array(type) copy = AllocateArray(type, ORIGINAL.size);				\
		memcpy(copy.base, ORIGINAL.base, copy.size * sizeof(type));			\
		return copy;														\
	}																		\
																			\
	bool ArraysEqual(type, const Array(type) A, const Array(type) B){		\
		if (A.size != B.size)												\
			return false;													\
		if (A.base == NULL || B.base == NULL)								\
			return A.base == B.base;										\
		return memcmp(A.base, B.base, A.size * sizeof(type)) == 0;			\
	}																		\

#endif//ARRAY_H