#include <stdlib.h>
#include <stdio.h>
#include "city.h"
#include "road.h"

City newCity(TrieTree trieTree) {

    City result = malloc(sizeof(struct city));
    if (result == NULL)
    	return NULL;

    result->edges = newVector();
    result->trieTree = trieTree;
    result->isAllowed = true;
    result->distance = 0;
    result->isInQueue = false;
    result->minAge = 0;
    result->numberOfPaths = 0;

    return result;
}

void deleteCityIfEmpty(City city) {
    if (city->edges->numberOfElements == 0) {
        clear(city->edges);
        city->trieTree->city = NULL;
        deleteTrieTreeIfEmpty(city->trieTree);
        free(city);
    }
}

void deleteCity(City city) {

	if (city == NULL)
		return;

    for (uint32_t i = 0; i < city->edges->numberOfElements; i++)
        deleteRoad(getElement(city->edges, i));
    clear(city->edges);
    free(city);
}
