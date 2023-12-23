#include "test_array.h"
#include "test_list.h"
#include "test_gapbuffer.h"
#include "test_map.h"
#include "message.h"
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void SigSegvHandler(const int){
	Message("\nSegmentation fault\n", RED);
	exit(EXIT_FAILURE);
}

void SigAbrtHandler(const int){
	Message("\nAbort\n", RED);
	exit(EXIT_FAILURE);
}

int main(){
	signal(SIGSEGV, SigSegvHandler);
	signal(SIGABRT, SigAbrtHandler);

	TestArray();	putchar('\n');
	TestList();		putchar('\n');
	TestGb();		putchar('\n');
	TestMap();		putchar('\n');
	
	Message("--- All tests passed ---\n\n", GREEN);

	return 0;
}