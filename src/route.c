#include "route.h"
#include <stdlib.h>

Route newRouteStruct(uint32_t id) {

	Route result = malloc(sizeof(struct route));
	if (result == NULL)
		return NULL;

	result->objects = newVector();
	if (result->objects == NULL) {
		free(result);
		return NULL;
	}

	result->id = id;

	return result;
}

void deleteRoute(Route route) {

	if (route == NULL)
		return;

	clear(route->objects);
	free(route);
}