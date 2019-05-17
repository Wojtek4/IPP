#pragma once

#include "vector.h"

typedef struct route* Route;

struct route{
	Vector objects;
	uint32_t id;
};

Route newRouteStruct(uint32_t id);
void deleteRoute(Route route);