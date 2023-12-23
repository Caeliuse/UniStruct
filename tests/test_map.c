#include "test_map.h"
#include <map.h>
#include <assert.h>
#include "message.h"
#include "format.h"

DEFINE_MAP_PROTOTYPE(int, char);
DEFINE_MAP_PROTOTYPE(float, float); // chech for conflicts

void PrepareMapTest(){
	const usize SIZE = 11;
	Map(int, char) map = PrepareMap(int, char, SIZE);

	assert(map.size == 0);
	assert(map.entries.size == SIZE);
	assert(map.entries.base != NULL);

	for (usize i = 0; i != map.entries.size; ++i)
		assert(map.entries.base[i].head == NULL);

	DestroyMap(int, char, &map);
}

void DestroyMapTest(){
	Map(int, char) map = PrepareMap(int, char, 10);
	DestroyMap(int, char, &map);

	assert(map.size == 0);
	assert(map.entries.size == 0);
	assert(map.entries.base == NULL);

	map = EMPTY_MAP(int, char);
	DestroyMap(int, char, &map); // chech for absence of SEGFAULT
}

void MapAddTest(){
	typedef struct{
		int key;
		char value;
	}Pair;

	static const Pair PAIRS[] = {
		(Pair){.key = 0, .value = 'a'},
		(Pair){.key = 1, .value = 'b'},
		(Pair){.key = 3, .value = 'c'},
		(Pair){.key = 4, .value = 'd'},
		(Pair){.key = 5, .value = 'e'},
		(Pair){.key = 6, .value = '1'},
		(Pair){.key = 7, .value = '2'},
		(Pair){.key = 8, .value = '3'},
		(Pair){.key = 9, .value = '4'},
		(Pair){.key = 10, .value = '5'},
		(Pair){.key = 11, .value = '5'},
		(Pair){.key = 12, .value = '5'},
		(Pair){.key = 13, .value = '5'},
		(Pair){.key = 14, .value = '5'},
		(Pair){.key = 15, .value = '5'},
		(Pair){.key = 16, .value = '5'},
		(Pair){.key = 17, .value = '5'},
		(Pair){.key = 18, .value = '5'},
	};
	static const usize PAIRS_N = sizeof(PAIRS)/sizeof(Pair);
	
	Map(int, char) map = EMPTY_MAP(int, char);

	for (usize i = 0; i != PAIRS_N; ++i)
		MapAdd(int, char, &map, PAIRS[i].key)->value = PAIRS[i].value;

	assert(map.size == PAIRS_N);
	assert(map.entries.base != NULL);
	assert(MapAccess(int, char, map, PAIRS_N + 21) == NULL);

	for (usize i = 0; i != PAIRS_N; ++i){
		assert(MapContains(int, char, map, PAIRS[i].key));
		assert(MapAccess(int, char, map, PAIRS[i].key)->value == PAIRS[i].value);
	}

	const usize PREV_SIZE = map.size;
	const usize REPLACE_KEY = 0;
	const char REPLACE_VALUE = '-';

	MapAdd(int, char, &map, PAIRS[REPLACE_KEY].key)->value = REPLACE_VALUE;
	assert(map.size == PREV_SIZE);
	assert(MapAccess(int, char, map, REPLACE_KEY)->value == REPLACE_VALUE);

	DestroyMap(int, char, &map);
}

void MapRemoveTest(){
	typedef struct{
		int key;
		char value;
	}Pair;

	static const Pair PAIRS[] = {
		(Pair){.key = 0, .value = 'a'},
		(Pair){.key = 1, .value = 'b'},
		(Pair){.key = 3, .value = 'c'},
		(Pair){.key = 4, .value = 'd'},
		(Pair){.key = 5, .value = 'e'},
		(Pair){.key = 6, .value = '1'},
		(Pair){.key = 7, .value = '2'},
		(Pair){.key = 8, .value = '3'},
		(Pair){.key = 9, .value = '4'},
		(Pair){.key = 10, .value = '5'},
		(Pair){.key = 11, .value = '5'},
		(Pair){.key = 12, .value = '5'},
		(Pair){.key = 13, .value = '5'},
		(Pair){.key = 14, .value = '5'},
		(Pair){.key = 15, .value = '5'},
		(Pair){.key = 16, .value = '5'},
		(Pair){.key = 17, .value = '5'},
		(Pair){.key = 18, .value = '5'},
	};
	static const usize PAIRS_N = sizeof(PAIRS)/sizeof(Pair);
	
	Map(int, char) map = EMPTY_MAP(int, char);

	for (usize i = 0; i != PAIRS_N; ++i)
		MapAdd(int, char, &map, PAIRS[i].key)->value = PAIRS[i].value;


	for (usize i = 0; i != PAIRS_N; ++i){
		const int KEY = PAIRS[i].key;
		const char VALUE = PAIRS[i].value;
		const usize PREV_SIZE = map.size;

		assert(MapAccess(int, char, map, KEY)->value == VALUE);
		MapRemove(int, char, &map, KEY);
		assert(map.size == PREV_SIZE - 1);
		assert(MapAccess(int, char, map, KEY) == NULL);
	}

	assert(map.size == 0);

	const usize PREV_SIZE = map.size;
	MapRemove(int, char, &map, PAIRS_N + 1939); // remove an item which does not exist
	assert(map.size == PREV_SIZE);

	DestroyMap(int, char, &map);
}

void RehashMapTest(){
	Map(int, char) map = EMPTY_MAP(int, char);

	usize rehash_size = 29;
	RehashMap(int, char, &map, rehash_size);

	assert(map.size == 0);
	assert(map.entries.base != NULL);
	assert(map.entries.size == rehash_size);

	const int KEY = 9;
	const char VALUE = 'a';
	MapAdd(int, char, &map, KEY)->value = VALUE;

	rehash_size = 1;
	RehashMap(int, char, &map, rehash_size);

	assert(map.size == 1);
	assert(map.entries.base != NULL);
	assert(map.entries.size == rehash_size);
	assert(MapAccess(int, char, map, KEY)->value == VALUE);

	rehash_size = 0;
	RehashMap(int, char, &map, rehash_size);

	assert(map.size == 0);
	assert(map.entries.base == NULL);
	assert(map.entries.size == rehash_size);
	assert(MapAccess(int, char, map, KEY) == NULL);

	DestroyMap(int, char, &map);
}

void MapAccessTest(){
	Map(int, char) map = EMPTY_MAP(int, char);

	int key = 0;
	char value = 'a';

	MapAdd(int, char, &map, key)->value = value;
	MapItem(int, char)* item = MapAccess(int, char, map, key);
	
	assert(item != NULL);
	assert(item->key == key);
	assert(item->value == value);

	item = MapAccess(int, char, map, 389891);
	assert(item == NULL);

	DestroyMap(int, char, &map);
}

void MapContainsTest(){
	Map(int, char) map = EMPTY_MAP(int, char);

	int key = 0;
	char value = 'a';

	MapAdd(int, char, &map, key)->value = value;
	assert(MapContains(int, char, map, key) == true);
	assert(MapContains(int, char, map, 34947940) == false);

	DestroyMap(int, char, &map);
}
void TestMap(){
	Test("PrepareMap", PrepareMapTest);
	Test("DestroyMap", DestroyMapTest);
	Test("MapAdd", MapAddTest);
	Test("MapRemove", MapRemoveTest);
	Test("RehashMap", RehashMapTest);
	Test("MapAccess", MapAccessTest);
	Test("MapContains", MapContainsTest);
}

bool IntsEqual(const int A, const int B){return A == B;}
usize IntHash(const int N){return N;}

bool FloatsEqual(const float A, const float B){return A == B;}
usize FloatHash(const float X){return X * 1000;}

DEFINE_MAP_IMPLEMENTATION(int, char, IntsEqual, IntHash);
DEFINE_MAP_IMPLEMENTATION(float, float, FloatsEqual, FloatHash);