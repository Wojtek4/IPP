#include <stdlib.h>
#include "city.h"
#include "road.h"
#include <stdio.h>

City newCity(TrieTree trieTree) {
    City result = malloc(sizeof(struct city));
    result->edges = newVector();
    result->trieTree = trieTree;
    result->isAllowed = true;
    result->distance = 0;
    result->isInQueue = false;
    result->minAge = 0;
    return result;
}

void deleteCityIfEmpty(City city) {
    if (city->edges->numberOfElements == 0) {
        //printf("TAK\n");
        clear(city->edges);
        city->trieTree->city = NULL;
        deleteTrieTreeIfEmpty(city->trieTree);
        free(city);
    }
}

void deleteCity(City city) {
    for (uint32_t i = 0; i < city->edges->numberOfElements; i++)
        deleteRoad(getElement(city->edges, i));
    clear(city->edges);
    free(city);
}