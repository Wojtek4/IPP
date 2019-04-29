#include <stdlib.h>
#include <inttypes.h>
#include "search.h"
#include <stdio.h>

const uint64_t MAX_DIST = 18446744073709551615ull;
const int32_t MAX_AGE = 2147483647;

int32_t min(int32_t x1, int32_t x2) {
    return ((x1 < x2) ? x1 : x2);
}

void clearGraph(City c1, uint64_t setDistance) {
    c1->distance = setDistance;
    c1->numberOfPaths = 0;
    c1->isInQueue = false;
    c1->minAge = 0;
    for (uint32_t i = 0; i < c1->edges->numberOfElements; i++) {
        Road currRoad = getElement(c1->edges, i);
        City currDest = currRoad->destination;
        if (currDest->isAllowed == false ||
                (currDest->distance == setDistance &&
                currDest->numberOfPaths == 0 &&
                currDest->isInQueue == false &&
                currDest->minAge == 0))
            continue;
        clearGraph(currDest, setDistance);
    }
}

void findNumberOfPaths(City city, int minYear) {
    for (uint32_t i = 0; i < city->edges->numberOfElements; i++) {
        Road currEdge = getElement(city->edges, i);
        City currDest = currEdge->destination;
        if (currDest->isAllowed == false ||
            currDest->distance != city->distance + currEdge->length ||
            currEdge->year < minYear)
            continue;
        if (currDest->numberOfPaths < 2) {
            currDest->numberOfPaths += city->numberOfPaths;
            currDest->numberOfPaths = min(currDest->numberOfPaths, 2);
            findNumberOfPaths(currDest, minYear);
        }
    }
}

bool findRoute(City begin, City dest, Vector result, int minYear) {
    if (begin == dest) {
        addElement(result, dest);
        return true;
    }
    for (uint32_t i = 0; i < begin->edges->numberOfElements; i++) {
        Road currEdge = getElement(begin->edges, i);
        City currDest = currEdge->destination;
        if (currDest->isAllowed == false ||
            currEdge->year < minYear ||
            currDest->numberOfPaths == 2 ||
            currDest->distance != begin->distance + currEdge->length)
            continue;
        if (findRoute(currDest, dest, result, minYear) == true) {
            addElement(result, currEdge);
            addElement(result, begin);
            return true;
        }
    }
    return false;
}

Vector findPath(City c1, City c2) {
    if (c1 == c2 || c1->isAllowed == false || c2->isAllowed == false)
        return newVector();
    clearGraph(c1, MAX_DIST);
    Vector queue = newVector();
    c1->distance = 0;
    c1->numberOfPaths = 1;
    c1->minAge = MAX_AGE;
    addElement(queue, c1);
    c1->isInQueue = true;
    for (uint32_t i = 0; i < queue->numberOfElements; i++) {
        City currNode = getElement(queue, i);
        currNode->isInQueue = false;
        for (uint32_t j = 0; j < currNode->edges->numberOfElements; j++) {
            Road currEdge = getElement(currNode->edges, j);
            City currDest = currEdge->destination;
            int32_t newMinYear = min(currEdge->year, currNode->minAge);
            if (currDest->isAllowed == false ||
                currDest->distance < currNode->distance + currEdge->length ||
                    (currDest->distance == currNode->distance + currEdge->length &&
                    newMinYear <= currDest->minAge))
                continue;
            currDest->minAge = newMinYear;
            currDest->distance = currNode->distance + currEdge->length;
            if (currDest->isInQueue == false) {
                addElement(queue, currDest);
                currDest->isInQueue = true;
            }
        }
    }
    if (c2->distance == 0) {
        clear(queue);
        clearGraph(c1, 0);
        return newVector();
    }
    findNumberOfPaths(c1, c2->minAge);
    clear(queue);
    Vector result = newVector();
    findRoute(c1, c2, result, c2->minAge);
    reverseVector(result);
    clearGraph(c1, 0);
    return result;
}

void changePath(Vector objects, bool isAllowed, uint32_t startingPosition, uint32_t bias) {
    for (uint32_t i = startingPosition; i + bias < objects->numberOfElements; i+=2) {
        City currCity = getElement(objects, i);
        currCity->isAllowed = isAllowed;
    }
}

void changePathWithFirstElement(Route route, bool isAllowed) {
    changePath(route->objects, isAllowed, 0, 1);
}

void changePathWithLastElement(Route route, bool isAllowed) {
    changePath(route->objects, isAllowed, 2, 0);
}
void changePathWithLastAndFirstElement(Route route, bool isAllowed) {
    changePath(route->objects, isAllowed, 0, 0);
}

Vector fixRoute(Route route, Road road) {
    Vector objects = route->objects;
    City c1=NULL, c2=NULL;
    for (uint32_t i = 1; i < objects->numberOfElements; i+=2) {
        if (getElement(objects, i) == road) {
            c1 = getElement(objects, i - 1);
            c2 = getElement(objects, i + 1);
            break;
        }
    }
    if (c1 == NULL || c2 == NULL) {
        return NULL;
    }
    changePathWithLastAndFirstElement(route, false);
    c1->isAllowed = true;
    c2->isAllowed = true;
    deleteByValue(c1->edges, road);
    Vector ans = findPath(c1, c2);
    addElement(c1->edges, road);
    changePathWithLastAndFirstElement(route, true);
    return ans;
}

Vector getBetterPath(Vector path1, Vector path2) {
    if (path1 == NULL || path2 == NULL)
        return NULL;
    if (path1->numberOfElements == 0 && path2->numberOfElements == 0)
        return NULL;
    if (path1->numberOfElements == 0 && path2->numberOfElements > 0)
        return copyVector(path2);
    if (path1->numberOfElements > 0 && path2->numberOfElements == 0)
        return copyVector(path1);

    int64_t lengthOfPath1 = 0, lengthOfPath2 = 0;
    int minOfPath1 = MAX_AGE, minOfPath2 = MAX_AGE;
    for (uint32_t i = 1; i < path1->numberOfElements; i+=2) {
        Road currEdge = getElement(path1, i);
        minOfPath1 = min(minOfPath1, currEdge->year);
        lengthOfPath1 += currEdge->length;
    }
    for (uint32_t i = 1; i < path2->numberOfElements; i+=2) {
        Road currEdge = getElement(path2, i);
        minOfPath2 = min(minOfPath2, currEdge->year);
        lengthOfPath2 += currEdge->length;
    }
    if (lengthOfPath1 == lengthOfPath2 && minOfPath1 == minOfPath2)
        return NULL;
    if (lengthOfPath1 < lengthOfPath2 ||
            (lengthOfPath1 == lengthOfPath2 && minOfPath1 > minOfPath2)) {
        return copyVector(path1);
    }
    return copyVector(path2);
}

uint32_t calcArrayLength(Vector objects) {
    uint32_t length = 0;
    for (uint32_t i = 0; i < objects->numberOfElements; i+=2) {
        City currCity = getElement(objects, i);
        length += getNameLength(currCity->trieTree) + 1;
    }
    for (uint32_t i = 1; i < objects->numberOfElements; i+=2) {
        Road currRoad = getElement(objects, i);
        length += snprintf(NULL, 0, ";%" PRIu32 ";%" PRId32, currRoad->length, currRoad->year);
    }
    return length;
}