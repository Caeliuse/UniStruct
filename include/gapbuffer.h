#ifndef GAPBUFFER_H
#define GAPBUFFER_H

#include "array.h"
#include "variadic.h"
#include <stdio.h>

DEFINE_ARRAY_PROTOTYPE(char);

typedef struct{
	struct{
		usize begin;
		usize size;
	}gap;
	Array(char) string;
}GapBuffer;

#define WS(str) (Array(char)){.base = str, .size = (COUNTOF(str) - 1)}
#define WSS(str, len) (Array(char)){.base = str, .size = len}

#define MIN(_a, _b) ({	\
	typeof(_a) a = (_a);\
	typeof(_b) b = (_b);\
	(a < b)? a : b;		\
})						\

#define MAX(_a, _b) ({	\
	typeof(_a) a = (_a);\
	typeof(_b) b = (_b);\
	(a > b)? a : b;		\
})						\

#define EMPTY_GB (GapBuffer){.string = EMPTY_ARRAY(char), .gap.begin = 0, .gap.size = 0}

GapBuffer CreateGb(const Array(char));
void DestroyGb(GapBuffer*);

void GbInsert(GapBuffer*, const usize, const char);
void GbErase(GapBuffer*, const usize);

void GbAppend(GapBuffer*, const char);
void GbPrepend(GapBuffer*, const char);

char GbPop(GapBuffer*);
char GbPopFront(GapBuffer*);

void GbInsertString(GapBuffer*, const usize, const Array(char));
void GbEraseString(GapBuffer*, const usize, const usize);

usize GbSize(const GapBuffer*);
usize GbReserved(const GapBuffer*);

char GbGet(const GapBuffer*, const usize);
void GbSet(GapBuffer*, const usize, const char);

void GbReserve(GapBuffer*, const usize);
void GbShrink(GapBuffer*);

Array(char) GbStringMode(GapBuffer*);
char* GbCMode(GapBuffer*);

GapBuffer GbLoadUntil(FILE*, const char);
GapBuffer GbLoad(FILE*);
GapBuffer GbInput(const char*, ...);

#endif//GAPBUFFER_H