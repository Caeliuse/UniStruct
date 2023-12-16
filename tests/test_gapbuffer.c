#include "test_gapbuffer.h"
#include "format.h"
#include <gapbuffer.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define TEST_DATA "tests/test.txt"

void WsTest(){
	char s[] = "Some text";
	Array(char) w = WS(s);

	assert(w.base == s);
	assert(w.size == strlen(s));
}

void WssTest(){
	char s[] = "Not some text";
	const usize LEN = strlen(s);
	Array(char) w = WSS(s, LEN);

	assert(w.base == s);
	assert(w.size == LEN);
}

void CreateGbTest(){
	Array(char) ws = WS("Not text at all");
	GapBuffer gb = CreateGb(ws);

	assert(gb.string.base != NULL);
	assert(gb.string.base != ws.base);
	assert(GbSize(&gb) == ws.size);

	for (usize i = 0; i != ws.size; ++i)
		assert(GbGet(&gb, i) == ws.base[i]);
	DestroyGb(&gb);
}

void DestroyGbTest(){
	GapBuffer gb = CreateGb(WS("No text here"));
	DestroyGb(&gb);

	assert(gb.string.base == NULL);
	assert(GbSize(&gb) == 0);

	gb = EMPTY_GB;
	DestroyGb(&gb); // check for absence of SEGFAULT
}

void CopyGbTest(){
	GapBuffer original = CreateGb(WS("Some text"));
	GapBuffer copy = CopyGb(&original);

	assert(GbSize(&original) == GbSize(&copy));
	assert(original.string.base != copy.string.base);

	const usize SIZE = GbSize(&original);
	for (usize i = 0; i != SIZE; ++i)
		assert(GbGet(&original, i) == GbGet(&copy, i));

	DestroyGb(&original);
	DestroyGb(&copy);
}

void GbEqualTest(){
	GapBuffer a = CreateGb(WS("How are you?"));
	GapBuffer b = CopyGb(&a);

	assert(GbEqual(&a, &b));
	DestroyGb(&a);
	assert(GbEqual(&a, &b) == false);
	DestroyGb(&b);
	assert(GbEqual(&a, &b));
}

void GbInsertTest(){
	Array(char) expected = WS("01234");
	GapBuffer gb = EMPTY_GB;

	GbInsert(&gb, 0, '3'); // "3"
	GbInsert(&gb, 0, '1'); // "13"
	GbInsert(&gb, 1, '2'); // "123"
	GbInsert(&gb, 0, '0'); // "0123"
	GbInsert(&gb, GbSize(&gb), '4'); // "01234"

	assert(GbSize(&gb) == expected.size);
	for (usize i = 0; i != expected.size; ++i)
		assert(GbGet(&gb, i) == expected.base[i]);
	DestroyGb(&gb);
}

void GbEraseTest(){
	GapBuffer gb = CreateGb(WS("Hi! I guess?"));
	Array(char) expected = WS("! I");

	GbErase(&gb, 0); // "i! I guess?"
	GbErase(&gb, GbSize(&gb) - 1); // "i! I guess"
	GbErase(&gb, 0); // "! I guess"

	GbErase(&gb, 3); // "! Iguess"
	GbErase(&gb, 3); // "! Iuess"
	GbErase(&gb, GbSize(&gb) - 2); // "! Iues"
	GbErase(&gb, 4); // "! Ius"
	GbErase(&gb, 3); // "! Is"
	GbErase(&gb, 3); // "! I"

	assert(GbSize(&gb) == expected.size);
	for (usize i = 0; i != expected.size; ++i)
		assert(GbGet(&gb, i) == expected.base[i]);
	DestroyGb(&gb);
}

void GbAppendTest(){
	GapBuffer gb = EMPTY_GB;
	Array(char) expected = WS("qwerty");

	for (usize i = 0; i != expected.size; ++i)
		GbAppend(&gb, expected.base[i]);

	assert(GbSize(&gb) == expected.size);
	for (usize i = 0; i != expected.size; ++i)
		assert(GbGet(&gb, i) == expected.base[i]);
	DestroyGb(&gb);
}

void GbPrependTest(){
	GapBuffer gb = EMPTY_GB;
	Array(char) expected = WS("password");

	for (int i = expected.size - 1; i >= 0; --i)
		GbPrepend(&gb, expected.base[i]);

	assert(GbSize(&gb) == expected.size);
	for (usize i = 0; i != expected.size; ++i)
		assert(GbGet(&gb, i) == expected.base[i]);
	DestroyGb(&gb);
}

void GbPopTest(){
	Array(char) original = WS("Pop this text");
	Array(char) expected = WS("Pop");
	GapBuffer gb = CreateGb(original);

	for (usize i = GbSize(&gb) - 1; i >= expected.size; --i)
		assert(GbPop(&gb) == original.base[i]);

	assert(GbSize(&gb) == expected.size);
	for (usize i = 0; i != expected.size; ++i)
		assert(GbGet(&gb, i) == expected.base[i]);
	DestroyGb(&gb);
}

void GbPopFrontTest(){
	Array(char) original = WS("this text pop");
	Array(char) expected = WS("pop");
	GapBuffer gb = CreateGb(original);

	const usize ITERS = original.size - expected.size;
	for (usize i = 0; i != ITERS; ++i)
		assert(GbPopFront(&gb) == original.base[i]);

	assert(GbSize(&gb) == expected.size);
	for (usize i = 0; i != expected.size; ++i)
		assert(GbGet(&gb, i) == expected.base[i]);
	DestroyGb(&gb);
}

void GbInsertStringTest(){
	Array(char) expected = WS("three word sentence");
	GapBuffer gb = EMPTY_GB;

	const usize CONNECTIONS[] = {5, 10};

	Array(char) ins = (Array(char)){
		.base = expected.base + CONNECTIONS[0] + 1,
		.size = CONNECTIONS[1] - CONNECTIONS[0]
	};

	GbInsertString(&gb, 0, ins);

	ins = (Array(char)){
		.base = expected.base + 0,
		.size = CONNECTIONS[0] + 1
	};

	GbInsertString(&gb, 0, ins);

	ins = (Array(char)){
		.base = expected.base + CONNECTIONS[1] + 1,
		.size = expected.size - CONNECTIONS[1] - 1
	};

	GbInsertString(&gb, GbSize(&gb), ins);

	assert(GbSize(&gb) == expected.size);
	for (usize i = 0; i != expected.size; ++i)
		assert(GbGet(&gb, i) == expected.base[i]);
	DestroyGb(&gb);
}

void GbEraseStringTest(){
	GapBuffer gb = CreateGb(WS("Delete <not this> this junk"));
	Array(char) expected = WS("<not this>");

	GbEraseString(&gb, 0, 7); // "<not this> this junk"
	GbEraseString(&gb, 10, 10); // "<not this>"

	assert(GbSize(&gb) == expected.size);
	for (usize i = 0; i != expected.size; ++i)
		assert(GbGet(&gb, i) == expected.base[i]);
	DestroyGb(&gb);
}

void GbSizeTest(){
	Array(char) expected = WS("ABC");
	GapBuffer gb = CreateGb(expected);

	const usize SIZE = GbSize(&gb);
	assert(SIZE == expected.size);

	for (usize i = 0; i != SIZE; ++i)
		assert(GbGet(&gb, i) == expected.base[i]);
	DestroyGb(&gb);
}

void GbReservedTest(){
	GapBuffer gb = CreateGb(WS("placeholder"));
	GbPrepend(&gb, 'a');
	GbPrepend(&gb, ' ');

	assert(GbReserved(&gb) < gb.string.size);
	assert(GbReserved(&gb) == gb.gap.size);

	DestroyGb(&gb);
}

void GbGetTest(){
	Array(char) expected = WS("(*-*)");
	GapBuffer gb = CreateGb(expected);

	assert(GbSize(&gb) == expected.size);
	for (usize i = 0; i != expected.size; ++i)
		assert(GbGet(&gb, i) == expected.base[i]);
	DestroyGb(&gb);
}

void GbSetTest(){
	Array(char) expected = WS("123456");
	GapBuffer gb = CreateGb(WS("654321"));

	for (usize i = 0; i != expected.size; ++i)
		GbSet(&gb, i, expected.base[i]);
	assert(GbSize(&gb) == expected.size);
	for (usize i = 0; i != expected.size; ++i)
		assert(GbGet(&gb, i) == expected.base[i]);
	DestroyGb(&gb);
}

void GbReserveTest(){
	GapBuffer gb = EMPTY_GB;

	const usize TO_RESERVE = 10;
	GbReserve(&gb, TO_RESERVE);

	assert(GbReserved(&gb) == TO_RESERVE);
	assert(gb.string.base != NULL);
	assert(gb.string.size == TO_RESERVE);

	DestroyGb(&gb);
}

void GbShrinkTest(){
	GapBuffer gb = EMPTY_GB;

	GbAppend(&gb, 'a');
	GbReserve(&gb, 10);
	GbShrink(&gb);

	assert(GbReserved(&gb) == 0);
	assert(GbSize(&gb) == 1);
	assert(gb.string.base != NULL);

	DestroyGb(&gb);
}

void GbStringModeTest(){
	Array(char) expected = WS("Some text, not important");
	GapBuffer gb = CreateGb(expected);

	assert(ArraysEqual(char, GbStringMode(&gb), expected));

	DestroyGb(&gb);
}

void GbCModeTest(){
	char s[] = "Surprise!";
	GapBuffer gb = CreateGb(WS(s));

	assert(strcmp(s, GbCMode(&gb)) == 0);

	DestroyGb(&gb);
}

void GbLoadTest(){
	FILE* file = fopen(TEST_DATA, "r");
	assert(file != NULL);

	Array(char) expected = WS("this is test data");
	GapBuffer gb = GbLoad(file);

	assert(ArraysEqual(char, expected, GbStringMode(&gb)));

	DestroyGb(&gb);
}

void GbLoadUntilTest(){
	FILE* file = fopen(TEST_DATA, "r");
	assert(file != NULL);

	Array(char) expected = WS("this");
	GapBuffer gb = GbLoadUntil(file, ' ');

	assert(ArraysEqual(char, expected, GbStringMode(&gb)));

	DestroyGb(&gb);
}

void TestGb(){
	Test("WS", WsTest);
	Test("WSS", WssTest);
	Test("CreateGb", CreateGbTest);
	Test("DestroyGb", DestroyGbTest);
	Test("CopyGb", CopyGbTest);
	Test("GbEqual", GbEqualTest);
	Test("GbInset", GbInsertTest);
	Test("GbErase", GbEraseTest);
	Test("GbAppend", GbAppendTest);
	Test("GbPrepend", GbPrependTest);
	Test("GbPop", GbPopTest);
	Test("GbPopFront", GbPopFrontTest);
	Test("GbInsertString", GbInsertStringTest);
	Test("GbEraseString", GbEraseStringTest);
	Test("GbSize", GbSizeTest);
	Test("GbReserved", GbReservedTest);
	Test("GbGet", GbGetTest);
	Test("GbSet", GbSetTest);
	Test("GbReserve", GbReserveTest);
	Test("GbShrink", GbShrinkTest);
	Test("GbStringMode", GbStringModeTest);
	Test("GbCMode", GbCModeTest);
	Test("GbLoad", GbLoadTest);
	Test("GbLoadUntil", GbLoadUntilTest);
}