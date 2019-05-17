#include <stdlib.h>
#include "road.h"

Road newRoad(City destination, int32_t year, uint32_t length) {

	Road result = malloc(sizeof(struct road));
	if (result == NULL)
		return NULL;

	result->length = length;
	result->destination = destination;
	result->year = year;
	result->listOfRoutes = newVector();

	return result;
}

void deleteRoad(Road road) {
	if (road == NULL)
		return;

	clear(road->listOfRoutes);
	free(road);
}