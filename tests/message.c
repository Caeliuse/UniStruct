#include "message.h"
#include <stdio.h>

void Message(const char* TEXT, const int COLOR){
	printf("\033[1;%dm%s\033[0m", COLOR, TEXT);
}