#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <inttypes.h>

#include "map.h"
#include "vector.h"
#include "route.h"
#include "road.h"

void error (const uint32_t* numberOfLines) {
	fprintf(stderr, "ERROR %" PRIu32 "\n", *numberOfLines);
}

char descriptionString[] = "getRouteDescription",
	repairString[] = "repairRoad",
	addString[] = "addRoad",
	newRouteString[] = "newRoute",
	extendRouteString[] = "extendRoute",
	removeRoadString[] = "removeRoad",
	removeRouteString[] = "removeRoute",
	zeroString[] = "0";

struct Word {
	int32_t sizeOfCharsArray;
	int32_t numberOfChars;
	char *charsPtr;
};

typedef struct Word* WordPtr;

void clearWord(WordPtr word) {
	free(word->charsPtr);
	free(word);
}

void addChar(WordPtr word, char c) {
	if (word->sizeOfCharsArray == word->numberOfChars) {
		word->sizeOfCharsArray *= 2;
		word->charsPtr = realloc(word->charsPtr, sizeof(char) * word->sizeOfCharsArray);
	}

	word->charsPtr[word->numberOfChars++] = c;
}

int64_t stringToNumber(const char *s) {

	if (strlen(s) > 12)
		return 0;

	if (s[0] == 0)
		return 0;

	if (s[0] != '-' && isdigit(s[0]) == 0)
		return 0;

	for (uint32_t i = 1; true; i++) {
		if (s[i] == 0)
			return atoll(s);
		if (isdigit(s[i]) == false)
			return 0;
	}
}

static bool badName(const char *city) {

	if (*city == 0)
		return true;

	for (uint32_t i = 0; true; i++) {

		if (city[i] == 0)
			return false;

		if (city[i] == ';' || (city[i] < 32 && city[i] >= 0))
			return true;
	}
}

void processLineFromInput(Map* m, uint32_t* numberOfLines) {

	int32_t firstChar = getchar();
	Vector pointersToSemicolons = newVector();
	uint32_t numberOfSemicolons = 0;

	while (firstChar == '#' || firstChar == '\n') {
		(*numberOfLines)++;
		while(firstChar != '\n' && firstChar != EOF)
			firstChar = getchar();
		if (firstChar != EOF)
			firstChar = getchar();
	}

	if (firstChar == EOF) {
		clear(pointersToSemicolons);
		deleteMap(m);
		exit(0);
	}

	WordPtr res = malloc(sizeof(struct Word));
	res->numberOfChars = 0;
	res->sizeOfCharsArray = 1;
	res->charsPtr = malloc(sizeof(char) * res->sizeOfCharsArray);

	while(firstChar != '\n' && firstChar != EOF) {
		addChar(res, firstChar);
		firstChar = getchar();
	}
	addChar(res, '\0');

	for (int32_t i = 0; i < res->numberOfChars; i++) {
		if (res->charsPtr[i] == ';')
			res->charsPtr[i] = '\0', addElement(pointersToSemicolons, res->charsPtr + i);
	}

	numberOfSemicolons = pointersToSemicolons->numberOfElements;

	if (numberOfSemicolons == 0) {
		error(numberOfLines);
	}

	else if (numberOfSemicolons == 1) {

		int64_t routeNumber = stringToNumber(getElement(pointersToSemicolons, 0) + 1);

		if (routeNumber < 0 ||
			(strcmp(getElement(pointersToSemicolons, 0) + 1, zeroString) != 0 && routeNumber == 0)) {
			error(numberOfLines);
		}

		else if (strcmp(descriptionString, res->charsPtr) == 0) {
			char* result = (char*)getRouteDescription(m, routeNumber);

			if (result == NULL) {
				error(numberOfLines);
			}
			else {
				printf("%s\n", result);
				free(result);
			}
		}

		else if (strcmp(removeRouteString, res->charsPtr) == 0) {
			bool result = removeRoute(m, routeNumber);

			if (result == false)
				error(numberOfLines);
		}

		else {
			error(numberOfLines);
		}
	}

	else if (numberOfSemicolons == 2) {

		if (strcmp(removeRoadString, res->charsPtr) == 0) {
			bool result = removeRoad(m,
					getElement(pointersToSemicolons, 0) + 1,
					getElement(pointersToSemicolons, 1) + 1);

			if (result == false)
				error(numberOfLines);
		}

		else if (strcmp(extendRouteString, res->charsPtr) == 0) {

			int64_t routeNumber = stringToNumber(getElement(pointersToSemicolons, 0) + 1);

			if (routeNumber > 0 && routeNumber <= MAX_ROUTE_ID) {
				bool result = extendRoute(m, routeNumber, getElement(pointersToSemicolons, 1) + 1);

				if (result == false)
					error(numberOfLines);
			}
			else {
				error(numberOfLines);
			}
		}

		else {
			error(numberOfLines);
		}
	}

	else if (numberOfSemicolons == 3) {

		if (strcmp(newRouteString, res->charsPtr) == 0) {

			int64_t routeNumber = stringToNumber(getElement(pointersToSemicolons, 0) + 1);

			if (routeNumber > 0 && routeNumber <= MAX_ROUTE_ID) {
				bool result = newRoute(m,
						routeNumber,
						getElement(pointersToSemicolons, 1) + 1,
						getElement(pointersToSemicolons, 2) + 1);

				if (result == false)
					error(numberOfLines);
			}
			else {
				error(numberOfLines);
			}
		}

		else {

			int64_t repairYear = stringToNumber(getElement(pointersToSemicolons, 2) + 1);

			bool result = false;

			if (strcmp(repairString, res->charsPtr) == 0 && repairYear <= INT32_MAX && repairYear >= INT32_MIN)
				result = repairRoad(m,
									getElement(pointersToSemicolons, 0) + 1,
									getElement(pointersToSemicolons, 1) + 1,
									(int) (repairYear));

			if (result == false)
				error(numberOfLines);
		}
	}

	else if (numberOfSemicolons == 4 && strcmp(res->charsPtr, addString) == 0) {

		int64_t builtYear = stringToNumber(getElement(pointersToSemicolons, 3) + 1),
				length = stringToNumber(getElement(pointersToSemicolons, 2) + 1);

		bool result = false;

		if (builtYear <= INT32_MAX && builtYear >= INT32_MIN && length > 0 && length <= UINT32_MAX)
			result = addRoad(m,
							 getElement(pointersToSemicolons, 0) + 1,
							 getElement(pointersToSemicolons, 1) + 1,
							 (unsigned) length,
							 (int)(builtYear));

		if (result == false)
			error(numberOfLines);
	}

	else if (numberOfSemicolons >= 4 && (numberOfSemicolons - 1) % 3 == 0) {

		int64_t route = stringToNumber(res->charsPtr);

		bool isNew = true;

		for (uint32_t i = 0; i < m->routes->numberOfElements; i++) {
			Route currRoute = getElement(m->routes, i);
			if (currRoute->id == route) {
				isNew = false;
				break;
			}
		}

		if (!isNew || route <= 0 || route > MAX_ROUTE_ID) {
			error(numberOfLines);
		}

		else {
			bool isAllGood = true;

			TrieTree namesOfCities = newTrieRoot();

			City setAsPresent = (City)1;

			for (uint32_t i = 0; i < numberOfSemicolons; i += 3) {
				if (badName(getElement(pointersToSemicolons, i) + 1)) {
					isAllGood = false;
					break;
				}

				TrieTree trieTree = addPath(getElement(pointersToSemicolons, i) + 1, namesOfCities);
				if (trieTree->city == setAsPresent)
					isAllGood = false;
				trieTree->city = setAsPresent;
			}

			for (uint32_t i = 0; i < numberOfSemicolons; i += 3) {
				if (badName(getElement(pointersToSemicolons, i) + 1)) {
					isAllGood = false;
					break;
				}

				TrieTree trieTree = findNode(getElement(pointersToSemicolons, i) + 1, namesOfCities);
				trieTree->city = NULL;
			}

			removeTrie(namesOfCities);

			for (uint32_t i = 0; i + 3 < numberOfSemicolons; i += 3) {

				int64_t builtYear = stringToNumber(getElement(pointersToSemicolons, i + 2) + 1),
						length = stringToNumber(getElement(pointersToSemicolons, i + 1) + 1);

				if (!(builtYear <= INT32_MAX && builtYear >= INT32_MIN && builtYear != 0
					&& length > 0 && length <= UINT32_MAX))
					isAllGood = false;

				City c1 = getCity(getElement(pointersToSemicolons, i) + 1, m->trieTree),
					c2 = getCity(getElement(pointersToSemicolons, i + 3) + 1, m->trieTree);
				if (c1 == NULL || c2 == NULL)
					continue;

				Road r1 = NULL;
				for (uint32_t j = 0; j < c1->edges->numberOfElements; j++) {
					Road currRoad = getElement(c1->edges, j);
					if (currRoad->destination == c2) {
						r1 = currRoad;
						break;
					}
				}

				if (r1 != NULL && (r1->length != length || r1->year > builtYear))
					isAllGood = false;
			}

			if (isAllGood == false) {
				error(numberOfLines);
			}
			else {

				for (uint32_t i = 0; i + 3 < pointersToSemicolons->numberOfElements; i += 3) {

					int64_t builtYear = stringToNumber(getElement(pointersToSemicolons, i + 2) + 1),
							length = stringToNumber(getElement(pointersToSemicolons, i + 1) + 1);

					addRoad(m,
							getElement(pointersToSemicolons, i) + 1,
							getElement(pointersToSemicolons, i + 3) + 1,
							(unsigned) length,
							(int)(builtYear));

					repairRoad(m,
							getElement(pointersToSemicolons, i) + 1,
							getElement(pointersToSemicolons, i + 3) + 1,
							(int)(builtYear));

					if (i == 0)
						addCityToRoute(m, getElement(pointersToSemicolons, i) + 1,
									   route);

					addRoadToRoute(m,
							getElement(pointersToSemicolons, i) + 1,
							getElement(pointersToSemicolons, i + 3) + 1,
							route);

					addCityToRoute(m, getElement(pointersToSemicolons, i + 3) + 1,
								   route);
				}
			}
		}
	}
	else
		error(numberOfLines);
	clear(pointersToSemicolons);
	clearWord(res);

	(*numberOfLines)++;

	if (firstChar == EOF)
		exit(0);
}


int main() {

	Map *m = newMap();
	uint32_t numberOfLines = 1;

	while (true)
		processLineFromInput(m, &numberOfLines);
}
