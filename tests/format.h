#ifndef FORMAT_H
#define FORMAT_H

#include "message.h"

#define Test(name, f) ({			\
	printf("\'%s\'\ttest: ", name);	\
	f();							\
	Message("\t[passed]\n", GREEN);	\
})									\

#endif//FORMAT_H