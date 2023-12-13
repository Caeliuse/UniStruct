#include <alarmists.h>
#include <stdlib.h>

void* MallocAlarmist(const usize SIZE){
	void* p = malloc(SIZE);
	if (p == NULL)
		abort();
	return p;
}