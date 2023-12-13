#include <array.h>
#include "alarmists.h"

void InsertSegment(void** original, const usize SIZE, const usize INDEX, const usize INS_SIZE){
	void* updated = MallocAlarmist(SIZE + INS_SIZE);

	void* src = *original;
	void* dst = updated;
	usize to_copy = INDEX;

	memcpy(dst, src, to_copy);

	src = src + to_copy;
	dst = dst + to_copy + INS_SIZE;
	to_copy = SIZE - to_copy;

	memcpy(dst, src, to_copy);

	free(*original);
	*original = updated;
}

void EraseSegment(void** original, const usize SIZE, const usize INDEX, const usize ERS_SIZE){
	void* updated = malloc(SIZE - ERS_SIZE);

	void* src = *original;
	void* dst = updated;
	usize to_copy = INDEX;

	memcpy(dst, src, to_copy);

	src = src + to_copy + ERS_SIZE;
	dst = dst + to_copy;
	to_copy = SIZE - to_copy - ERS_SIZE;

	memcpy(dst, src, to_copy);

	free(*original);
	*original = updated;
}