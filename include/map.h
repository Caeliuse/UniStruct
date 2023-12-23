#ifndef MAP_H
#define MAP_H

#include "array.h"

#define DEFAULT_MAP_RESERVE 16
#define MAP_LOAD_FACTOR_THRESHOLD 0.8
#define DEFAULT_MAP_EXTENSION_FACTOR 2

#define MapItem(K, T) MapItem##_##K##_##T
#define MapEntryNode(K, T) MapEntryNode##_##K##_##T
#define MapEntry(K, T) MapEntry##_##K##_##T
#define Map(K, T) Map##_##K##_##T

#define PrepareMap(K, T, size) PrepareMap##_##K##_##T(size)
#define DestroyMapEntryNode(K, T, node) DestroyMapEntryNode##_##K##_##T(node)
#define DestroyMap(K, T, map) DestroyMap##_##K##_##T(map)

#define MapLoadFactor(K, T, map) MapLoadFactor##_##K##_##T(map)
#define RehashMap(K, T, map, size) RehashMap##_##K##_##T(map, size)

#define PickMapEntry(K, T, map, key) PickMapEntry##_##K##_##T(map, key)
#define SearchMapEntry(K, T, entry, key) SearchMapEntry##_##K##_##T(entry, key)

#define MapAdd(K, T, map, key) MapAdd##_##K##_##T(map, key)
#define MapRemove(K, T, map, key) MapRemove##_##K##_##T(map, key)

#define MapAccess(K, T, map, key) MapAccess##_##K##_##T(map, key)
#define MapContains(K, T, map, key) MapContains##_##K##_##T(map, key)

#define EMPTY_MAP_ENTRY(K, T) (MapEntry(K, T)){.head = NULL}
#define EMPTY_MAP(K, T) (Map(K, T)){.entries = EMPTY_ARRAY(MapEntry##_##K##_##T), .size = 0}

#define DEFINE_MAP_PROTOTYPE(K, T)			\
	typedef struct{							\
		K key;								\
		T value;							\
	}MapItem(K, T);							\
											\
	typedef struct MapEntryNode(K, T){		\
		struct MapEntryNode(K, T)* next;	\
		MapItem(K, T) item;					\
	}MapEntryNode(K, T);					\
											\
	typedef struct{							\
		MapEntryNode(K, T)* head;			\
	}MapEntry(K, T);						\
											\
	DEFINE_ARRAY_PROTOTYPE(MapEntry##_##K##_##T);\
											\
	typedef struct{							\
		Array(MapEntry##_##K##_##T) entries;\
		usize size;							\
	}Map(K, T);								\
											\
	Map(K, T) PrepareMap(K, T, const usize);\
	void DestroyMap(K, T, Map(K, T)*);		\
											\
	float MapLoadFactor(K, T, const Map(K, T));			\
	void RehashMap(K, T, Map(K, T)*, const usize);		\
														\
	MapItem(K, T)* MapAdd(K, T, Map(K, T)*, const K);	\
	void MapRemove(K, T, Map(K, T)*, const K);			\
														\
	MapItem(K, T)* MapAccess(K, T, Map(K, T), const K);	\
	bool MapContains(K, T, const Map(K, T), const K);	\

#define DEFINE_MAP_IMPLEMENTATION(K, T, KeysEqual, Hash)	\
	DEFINE_ARRAY_IMPLEMENTATION(MapEntry##_##K##_##T);		\
															\
	Map(K, T) PrepareMap(K, T, const usize SIZE){			\
		Map(K, T) map = EMPTY_MAP(K, T);					\
		map.entries = AllocateArray(MapEntry##_##K##_##T, SIZE);\
															\
		for (usize i = 0; i != map.entries.size; ++i)		\
			map.entries.base[i] = EMPTY_MAP_ENTRY(K, T);	\
		return map;											\
	}														\
															\
	MapEntryNode(K, T)* DestroyMapEntryNode(K, T, MapEntryNode(K, T)* node){	\
		MapEntryNode(K, T)* next = node->next;									\
		free(node);																\
		return next;															\
	}																			\
																				\
	void DestroyMap(K, T, Map(K, T)* map){								\
		void DestroyMapEntry(MapEntry(K, T)* entry){					\
			while (entry->head != NULL)									\
				entry->head = DestroyMapEntryNode(K, T, entry->head);	\
			*entry = EMPTY_MAP_ENTRY(K, T);								\
		}																\
																		\
		for (usize i = 0; i != map->entries.size; ++i)					\
			DestroyMapEntry(&map->entries.base[i]);						\
		DestroyArray(MapEntry##_##K##_##T, &map->entries);				\
		*map = EMPTY_MAP(K, T);											\
	}																	\
																		\
	float MapLoadFactor(K, T, const Map(K, T) map){						\
		return map.size / (float)map.entries.size;						\
	}																	\
																		\
	void RehashMap(K, T, Map(K, T)* map, const usize NEW_SIZE){							\
		if (NEW_SIZE == 0){																\
			DestroyMap(K, T, map);														\
			return;																		\
		}																				\
																						\
		Map(K, T) rehashed = PrepareMap(K, T, NEW_SIZE);								\
																						\
		for (usize i = 0; i != map->entries.size; ++i){									\
			MapEntry(K, T) entry = map->entries.base[i];								\
			while (entry.head != NULL){													\
				MapItem(K, T)* item = MapAdd(K, T, &rehashed, entry.head->item.key);	\
				item->value = entry.head->item.value;									\
				entry.head = entry.head->next;											\
			}																			\
		}																				\
																						\
		DestroyMap(K, T, map);															\
		*map = rehashed;																\
	}																					\
																						\
	MapEntry(K, T)* PickMapEntry(K, T, Map(K, T) map, const K KEY){	\
		if (map.entries.size == 0)									\
			return NULL;											\
		const usize HASH = Hash(KEY);								\
		const usize ENTRY_INDEX = HASH % map.entries.size;			\
		return &map.entries.base[ENTRY_INDEX];						\
	}																\
																	\
	MapEntryNode(K, T)* SearchMapEntry(K, T, MapEntry(K, T) entry, const K KEY){	\
		while (entry.head != NULL){													\
			if (KeysEqual(entry.head->item.key, KEY))								\
				return entry.head;													\
			entry.head = entry.head->next;											\
		}																			\
		return NULL;																\
	}																				\
																					\
	MapItem(K, T)* MapAdd(K, T, Map(K, T)* map, const K KEY){							\
		MapEntryNode(K, T)* CreateMapEntryNode(const K KEY, MapEntryNode(K, T)* next){	\
			MapEntryNode(K, T)* node = MallocAlarmist(sizeof(MapEntryNode(K, T)));		\
			node->item.key = KEY;														\
			node->next = next;															\
			return node;																\
		}																				\
																						\
		if (map->entries.base == NULL)													\
			*map = PrepareMap(K, T, DEFAULT_MAP_RESERVE);								\
		if (MapLoadFactor(K, T, *map) >= MAP_LOAD_FACTOR_THRESHOLD)						\
			RehashMap(K, T, map, map->entries.size * DEFAULT_MAP_EXTENSION_FACTOR);		\
																						\
		MapEntry(K, T)* entry = PickMapEntry(K, T, *map, KEY);							\
		MapEntryNode(K, T)* node = SearchMapEntry(K, T, *entry, KEY);					\
																						\
		if (node != NULL)																\
			return &node->item;															\
																						\
		entry->head = CreateMapEntryNode(KEY, entry->head);								\
		++map->size;																	\
		return &entry->head->item;														\
	}																					\
																						\
	void MapRemove(K, T, Map(K, T)* map, const K KEY){			\
		bool MapEntryRemove(MapEntry(K, T)* entry, const K){	\
			if (entry->head == NULL)							\
				return false;									\
																\
			if (KeysEqual(entry->head->item.key, KEY)){			\
				MapEntryNode(K, T)* second = entry->head->next;	\
				DestroyMapEntryNode(K, T, entry->head);			\
				entry->head = second;							\
				return true;									\
			}													\
																\
			MapEntryNode(K, T)* prev = entry->head;				\
			MapEntryNode(K, T)* curr = prev->next;				\
																\
			while (curr != NULL){								\
				if (KeysEqual(curr->item.key, KEY)){			\
					MapEntryNode(K, T)* next = curr->next;		\
					DestroyMapEntryNode(K, T, curr);			\
					prev->next = next;							\
					return true;								\
				}												\
			}													\
																\
			return false;										\
		}														\
																\
		if (map->size == 0)										\
			return;												\
																\
		MapEntry(K, T)* entry = PickMapEntry(K, T, *map, KEY);	\
		if (MapEntryRemove(entry, KEY))							\
			--map->size;										\
	}															\
																\
	MapItem(K, T)* MapAccess(K, T, Map(K, T) map, const K KEY){			\
		MapEntry(K, T)* entry = PickMapEntry(K, T, map, KEY);			\
		if (entry == NULL)												\
			return NULL;												\
																		\
		MapEntryNode(K, T)* node = SearchMapEntry(K, T, *entry, KEY);	\
		if (node == NULL)												\
			return NULL;												\
		return &node->item;												\
	}																	\
																		\
	bool MapContains(K, T, const Map(K, T) MAP, const K KEY){			\
		return MapAccess(K, T, MAP, KEY) != NULL;						\
	}																	\

#endif//MAP_H