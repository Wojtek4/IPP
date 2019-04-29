#include <stdlib.h>
#include <inttypes.h>
#include "vector.h"
#include "trie.h"
#include "city.h"
#include "road.h"
#include "map.h"
#include "route.h"
#include "search.h"
#include <stdio.h>

struct Map {
    Vector routes;
    TrieTree trieTree;
};

Map* newMap() {
    Map* result = malloc(sizeof(Map));
    if (result == NULL)
        return NULL;
    result->routes = newVector();
    if (result->routes == NULL) {
        free(result);
        return NULL;
    }
    result->trieTree = newTrieRoot();
    if (result->trieTree == NULL) {
        clear(result->routes);
        free(result);
        return NULL;
    }
    return result;
}

void deleteMap(Map *map) {
    for (uint32_t i = 0; i < map->routes->numberOfElements; i++)
        deleteRoute(getElement(map->routes, i));
    clear(map->routes);
    removeTrie(map->trieTree);
    free(map);
}

bool badName(const char *city) {
    for (uint32_t i = 0; true; i++) {
        if (city[i] == 0)
            return false;
        if (city[i] == ';' || (city[i] < 32 && city[i] >= 0))
            return true;
    }
}

bool equalNames(const char *s1, const char *s2) {
    for (uint32_t i = 0; true; i++) {
        if (s1[i] != s2[i])
            return false;
        if (s1[i] == 0)
            return true;
    }
}

Road findEdge(City v1, City v2) {
    Vector w1 = v1->edges;
    for (uint32_t i = 0; i < w1->numberOfElements; i++) {
        Road r = getElement(w1, i);
        if (r->destination == v2)
            return r;
    }
    return NULL;
}

bool addRoad(Map *map, const char *city1, const char *city2, unsigned length, int builtYear) {

    if (badName(city1) || badName(city2) || builtYear == 0 || length == 0 || equalNames(city1, city2))
        return false;

    TrieTree v1 = findNode(city1, map->trieTree), v2 = findNode(city2, map->trieTree);

    if (v1 == NULL)
        v1 = addPath(city1, map->trieTree);
    if (v2 == NULL)
        v2 = addPath(city2, map->trieTree);
    if (v1 == NULL || v2 == NULL)
        return false;

    if (v1->city == NULL)
        v1->city = newCity(v1);
    if (v2->city == NULL)
        v2->city = newCity(v2);
    if (v1->city == NULL || v2->city == NULL)
        return false;
    City c1 = v1->city, c2 = v2->city;
    if (findEdge(c1 ,c2) != NULL)
        return false;

    Vector w1 = c1->edges, w2 = c2->edges;
    Road r1 = newRoad(c2, builtYear, length), r2 = newRoad(c1, builtYear, length);
    if (r1 == NULL || r2 == NULL) {
        deleteRoad(r1);
        deleteRoad(r2);
        deleteCityIfEmpty(c1);
        deleteCityIfEmpty(c2);
        return false;
    }

    addElement(w1, r1);
    addElement(w2, r2);
    return true;
}

bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear) {

    if (repairYear == 0)
        return false;

    City c1 = getCity(city1, map->trieTree), c2 = getCity(city2, map->trieTree);
    if (c1 == NULL || c2 == NULL)
        return false;

    Road r1 = findEdge(c1, c2), r2 = findEdge(c2, c1);
    if (r1 == NULL || r2 == NULL)
        return false;
    if (r1->year > repairYear || r2->year > repairYear)
        return false;

    r1->year = repairYear;
    r2->year = repairYear;
    return true;
}

bool newRoute(Map *map, unsigned routeId, const char *city1, const char *city2) {
    if (routeId == 0 || routeId >= 1000)
        return false;
    for (uint32_t i = 0; i < map->routes->numberOfElements; i++) {
        Route currRoute = getElement(map->routes, i);
        if (currRoute->id == routeId)
            return false;
    }
    City c1 = getCity(city1, map->trieTree), c2 = getCity(city2, map->trieTree);
    if (c1 == NULL || c2 == NULL || c1 == c2)
        return false;
    Route result = newRouteStruct(routeId);
    if (result == NULL)
        return false;
    result->objects = findPath(c1, c2);
    if (result->objects == NULL || result->objects->numberOfElements == 0) {
        deleteRoute(result);
        return false;
    }
    for (uint32_t i = 1; i < result->objects->numberOfElements; i+=2) {
        Road currEdge = getElement(result->objects, i);
        addElement(currEdge->listOfRoutes, result);
    }
    addElement(map->routes, result);
    return true;
}

bool extendRoute(Map *map, unsigned routeId, const char *city) {

    Route route = NULL;
    for (uint32_t i = 0; i < map->routes->numberOfElements; i++) {
        Route currRoute = getElement(map->routes, i);
        if (currRoute->id == routeId) {
            route = currRoute;
            break;
        }
    }

    if (route == NULL) {
        return false;
    }

    City c = getCity(city, map->trieTree);
    if (c == NULL)
        return false;

    City firstCity = getElement(route->objects, 0);
    City lastCity = getElement(route->objects, route->objects->numberOfElements - 1);
    if (firstCity == c || lastCity == c)
        return false;

    changePathWithLastElement(route, false);
    Vector path1 = findPath(c, firstCity);

    firstCity->isAllowed = false;
    lastCity->isAllowed = true;
    Vector path2 = findPath(lastCity, c);
    changePathWithFirstElement(route, true);

    Vector resultPath = getBetterPath(path1, path2);

    if (resultPath == NULL || resultPath->numberOfElements == 0) {
        clear(path1);
        clear(path2);
        return false;
    }

    for (uint32_t i = 1; i < resultPath->numberOfElements; i+=2) {
        Road currEdge = getElement(resultPath, i);
        addElement(currEdge->listOfRoutes, route);
    }

    if (getElement(resultPath, 0) == lastCity) {
        for (uint32_t i = 1; i < resultPath->numberOfElements; i++)
            addElement(route->objects, getElement(resultPath, i));
    }
    else {
        for (uint32_t i = 1; i < route->objects->numberOfElements; i++)
            addElement(resultPath, getElement(route->objects, i));

        Vector temp = resultPath;
        resultPath = route->objects;
        route->objects = temp;
    }
    clear(path1);
    clear(path2);
    clear(resultPath);
    return true;
}

bool removeRoad(Map *map, const char *city1, const char *city2) {
    City c1 = getCity(city1, map->trieTree), c2 = getCity(city2, map->trieTree);
    if (c1 == NULL || c2 == NULL)
        return false;

    Road r1 = findEdge(c1, c2), r2 = findEdge(c2, c1);
    if (r1 == NULL || r2 == NULL)
        return false;

    Vector vectorOfFixes = newVector();

    for (uint32_t routeNumber = 0; routeNumber < r1->listOfRoutes->numberOfElements; routeNumber++) {
        Route route = getElement(r1->listOfRoutes, routeNumber);
        addElement(vectorOfFixes, fixRoute(route, r1));
    }
    for (uint32_t routeNumber = 0; routeNumber < r2->listOfRoutes->numberOfElements; routeNumber++) {
        Route route = getElement(r2->listOfRoutes, routeNumber);
        addElement(vectorOfFixes, fixRoute(route, r2));
    }
    bool isSomethingWrong = false;
    for (uint32_t i = 0; i < vectorOfFixes->numberOfElements; i++) {
        Vector currFix = getElement(vectorOfFixes, i);
        if (currFix == NULL || currFix->numberOfElements == 0) {
            isSomethingWrong = true;
            break;
        }
        popBack(currFix);
        deleteByIndex(currFix, 0);
    }
    if (isSomethingWrong) {
        for (uint32_t i = 0; i < vectorOfFixes->numberOfElements; i++)
            clear(getElement(vectorOfFixes, i));
        clear(vectorOfFixes);
        return false;
    }
    for (uint32_t j = 0; j < vectorOfFixes->numberOfElements; j++) {
        Vector resultPath = getElement(vectorOfFixes, j);
        Route currRoute = NULL;
        if (j < r1->listOfRoutes->numberOfElements)
            currRoute = getElement(r1->listOfRoutes, j);
        else
            currRoute = getElement(r2->listOfRoutes, j - r1->listOfRoutes->numberOfElements);
        for (uint32_t i = 0; i < resultPath->numberOfElements; i += 2) {
            Road currEdge = getElement(resultPath, i);
            addElement(currEdge->listOfRoutes, currRoute);
        }
    }
    for (uint32_t i = 0; i < r1->listOfRoutes->numberOfElements; i++) {
        Route route = getElement(r1->listOfRoutes, i);
        uint32_t indexOfEdge = 1;
        while(getElement(route->objects, indexOfEdge) != r1)
            indexOfEdge+=2;
        replace(route->objects, indexOfEdge, getElement(vectorOfFixes, i));
    }
    for (uint32_t i = 0; i < r2->listOfRoutes->numberOfElements; i++) {
        Route route = getElement(r2->listOfRoutes, i);
        uint32_t indexOfEdge = 1;
        while(getElement(route->objects, indexOfEdge) != r2)
            indexOfEdge+=2;
        replace(route->objects, indexOfEdge, getElement(vectorOfFixes, r1->listOfRoutes->numberOfElements + i));
    }
    clear(vectorOfFixes);
    deleteByValue(c1->edges, r1);
    deleteByValue(c2->edges, r2);
    clear(r1->listOfRoutes);
    clear(r2->listOfRoutes);
    free(r1);
    free(r2);
    deleteCityIfEmpty(c1);
    deleteCityIfEmpty(c2);
    return true;
}

char const* getRouteDescription(Map *map, unsigned routeId) {
    Route route = NULL;
    for (uint32_t i = 0; i < map->routes->numberOfElements; i++) {
        Route currRoute = getElement(map->routes, i);
        if (currRoute->id == routeId) {
            route = currRoute;
            break;
        }
    }
    if (route == NULL) {
        char* array = calloc(1, sizeof(char));
        return array;
    }
    uint32_t arrayLength = calcArrayLength(route->objects) + snprintf(NULL, 0, "%" PRIu32, route->id) + 1;
    char* arrayResult = calloc(arrayLength, sizeof(char));
    char* array = arrayResult;
    if (array == NULL)
        return NULL;
    sprintf(array, "%" PRIu32, route->id);
    while (*array != 0)
        array++;
    for (uint32_t i = 0; i < route->objects->numberOfElements; i++) {
        sprintf(array, ";");
        array++;
        if (i % 2 == 0) {
            City currCity = getElement(route->objects, i);
            writeName(currCity->trieTree, &array);
        }
        else {
            Road currRoad = getElement(route->objects, i);
            sprintf(array, "%" PRIu32 ";%" PRId32, currRoad->length, currRoad->year);
            while (*array != 0)
                array++;
        }
    }
    return arrayResult;
}
