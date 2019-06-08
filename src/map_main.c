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

void error (const uint32_t* numberOfLines) {
	fprintf(stderr, "ERROR %" PRIu32 "\n", *numberOfLines);
}

char descriptionString[] = "getRouteDescription",
	repairString[] = "repairRoad",
	addString[] = "addRoad";

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

void processLineFromInput(Map* m, uint32_t* numberOfLines) {

	int32_t firstChar = getchar();
	Vector pointersToSemicolons = newVector();
	int32_t numberOfSemicolons = 0;

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

		char* result = NULL;

		if (strcmp(descriptionString, res->charsPtr) == 0 && routeNumber > 0 && routeNumber <= MAX_ROUTE_ID)
			result = (char*)getRouteDescription(m, routeNumber);

		if (result == NULL) {
			error(numberOfLines);
		}
		else {
			printf("%s\n", result);
			free(result);
		}
	}

	else if (numberOfSemicolons == 3) {

		int64_t repairYear = stringToNumber(getElement(pointersToSemicolons, 2) + 1);

		bool result = false;

		if (strcmp(repairString, res->charsPtr) == 0 && repairYear <= INT32_MAX && repairYear >= INT32_MIN)
			result = repairRoad(m,
				getElement(pointersToSemicolons, 0) + 1,
				getElement(pointersToSemicolons, 1) + 1,
				(int)(repairYear));

		if (result == false)
			error(numberOfLines);
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

		if (isNew && route > 0 && route <= MAX_ROUTE_ID) {

			for (uint32_t i = 0; i + 3 < pointersToSemicolons->numberOfElements; i += 3) {

				int64_t builtYear = stringToNumber(getElement(pointersToSemicolons, i + 2) + 1),
					length = stringToNumber(getElement(pointersToSemicolons, i + 1) + 1);

				bool result = false;

				if (builtYear <= INT32_MAX && builtYear >= INT32_MIN && length > 0 && length <= UINT32_MAX) {

					addRoad(m,
						getElement(pointersToSemicolons, i) + 1,
						getElement(pointersToSemicolons, i + 3) + 1,
						(unsigned) length,
						(int)(builtYear));

					result = repairRoad(m,
						getElement(pointersToSemicolons, i) + 1,
						getElement(pointersToSemicolons, i + 3) + 1,
						(int)(builtYear));
				}

				if (result == false) {
					error(numberOfLines);
					break;
				}

				if (i == 0)
					addCityToRoute(m, getElement(pointersToSemicolons, i) + 1,
					route);

				if (addRoadToRoute(m, getElement(pointersToSemicolons, i) + 1,
					getElement(pointersToSemicolons, i + 3) + 1,
					route) == false) {
					error(numberOfLines);
					break;
				}

				addCityToRoute(m, getElement(pointersToSemicolons, i + 3) + 1,
					route);
			}
		}
		else {
			error(numberOfLines);
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
