#include <map.h>

#define SQUARE 0
#define TRIANGLE 1
#define CIRCLE 2

typedef struct{
	int x;
	int y;
}Point;

bool PointsEqual(const Point A, const Point B){
	return A.x == B.x && A.y == B.y;
}

usize PointHash(const Point POINT){
	return POINT.x + POINT.y;
}

DEFINE_MAP_PROTOTYPE(Point, int);
DEFINE_MAP_IMPLEMENTATION(Point, int, PointsEqual, PointHash);

int main(){
	Map(Point, int) objects = EMPTY_MAP(Point, int);
		// if you want to reserve memory ahead, you can use the "MapPrepare(K, T, size)" function

	MapAdd(Point, int, &objects, ((Point){0, 0}))->value = CIRCLE;
	MapAdd(Point, int, &objects, ((Point){101, 555}))->value = SQUARE;
	MapAdd(Point, int, &objects, ((Point){11, -11}))->value = TRIANGLE;
		// if you want to resize the map you can use the "RehashMap(K, T, map, size)" function

	printf("amount of objects: %u\n", objects.size);

	MapItem(Point, int)* object = MapAccess(Point, int, objects, ((Point){0, 0}));
	if (object != NULL)
		printf("(%d, %d): %d\n", object->key.x, object->key.y, object->value);

	Point some_point = (Point){1, 3};
	if (MapContains(Point, int, objects, some_point))
		printf("Map does not contain the (%d, %d) point\n", some_point.x, some_point.y);
	else
		printf("Map does contain the (%d, %d) point\n", some_point.x, some_point.y);

	DestroyMap(Point, int, &objects);

	return 0;
}