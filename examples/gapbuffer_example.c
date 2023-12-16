#include <gapbuffer.h>
#include <stdio.h>

int main(){
	GapBuffer input = GbInput("Input something (%d lines): ", 1);

	GbPrepend(&input, '-');
	GbAppend(&input, '-');

	char* c_str = GbCMode(&input);
	printf("Your input but with '-': %s\n", c_str);

	return 0;
}