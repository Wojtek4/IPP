#include <stdlib.h>
#include "road.h"

Road newRoad(City destination, int32_t year, uint32_t length) {
    Road result = malloc(sizeof(struct road));
    result->length = length;
    result->destination = destination;
    result->year = year;
    result->listOfRoutes = newVector();
    return result;
}

void deleteRoad(Road road) {
    clear(road->listOfRoutes);
    free(road);
}