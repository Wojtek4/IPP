#pragma once

typedef struct TrieNode* TrieTree;

struct TrieNode;

#include <stdint.h>
#include "vector.h"
#include "city.h"

/* TrieNode is a node of trie tree. It stores ptr - pointer
to its DsuNode container - a dsu node witch contains this
TrieNode; orderInDsu - index of this trie node in *ptr;
t - array of children of this trie node, t[x] is connected to
this node with a vertex labeled by x + 32 */

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