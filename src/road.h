#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "vector.h"
#include "city.h"

typedef struct road* Road;

struct road {
    int32_t year;
    City destination;
    uint32_t length;
    Vector listOfRoutes;
};

Road newRoad(City destination, int32_t year, uint32_t length);
void deleteRoad(Road road);