#include "route.h"
#include <stdlib.h>

Route newRouteStruct(uint32_t id) {
    Route result = malloc(sizeof(struct route));
    result->objects = newVector();
    result->id = id;
    return result;
}

void deleteRoute(Route route) {
    clear(route->objects);
    free(route);
}