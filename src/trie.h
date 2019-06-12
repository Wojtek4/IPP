#pragma once

typedef struct TrieNode* TrieTree;

struct TrieNode;

#include <stdint.h>
#include "vector.h"
#include "city.h"

struct TrieNode {
	City city;
	Vector t;
	TrieTree parent;
};

TrieTree newTrieRoot();
TrieTree newTrieNode(TrieTree parent);

void removeTrie(TrieTree trieTree);
TrieTree addPath(const char *history, TrieTree trieTree);
TrieTree findNode(const char *history, TrieTree trieTree);
City getCity(const char *history, TrieTree trieTree);

void deleteTrieTreeIfEmpty(TrieTree trieTree);
void writeName(TrieTree trieTree, char** array);
uint32_t getNameLength(TrieTree trieTree);