#pragma once

struct city;

typedef struct city* City;

#include <stdbool.h>
#include "vector.h"
#include "trie.h"

struct city{
	Vector edges;
	bool isAllowed, isInQueue;
	TrieTree trieTree;
	uint64_t distance;
	uint32_t numberOfPaths;
	int32_t minAge;
};

City newCity(TrieTree trieTree);
void deleteCityIfEmpty(City city);
void deleteCity(City city);