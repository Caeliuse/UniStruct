#ifndef VARIADIC_H
#define VARIADIC_H

#include <stdarg.h>

#define COUNTOF(collection) (sizeof(collection)/sizeof(collection[0])) 
#define ARGCOUNT(type, ...) (COUNTOF(((type[]){0, ##__VA_ARGS__})) - 1)

#endif//VARIADIC_H