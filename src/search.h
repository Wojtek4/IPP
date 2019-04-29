#pragma once

#include "vector.h"
#include "city.h"
#include "route.h"
#include "road.h"

Vector findPath(City c1, City c2);
Vector fixRoute(Route route, Road road);
Vector getBetterPath(Vector path1, Vector path2);

void changePath(Vector objects, bool isAllowed, uint32_t startingPosition, uint32_t bias);

void changePathWithFirstElement(Route route, bool isAllowed);
void changePathWithLastElement(Route route, bool isAllowed);
void changePathWithLastAndFirstElement(Route route, bool isAllowed);

uint32_t calcArrayLength(Vector objects);