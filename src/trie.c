#include <stdlib.h>
#include <stdint.h>
#include "trie.h"
#include <stdio.h>

typedef struct trieEdge* TrieEdge;

struct trieEdge {
	char label;
	TrieTree node;
};

static TrieEdge newTrieEdge(const char label, TrieTree node) {

	TrieEdge result = malloc(sizeof(struct trieEdge));
	if (result == NULL)
		return NULL;

	result->node = node;
	result->label = label;

	return result;
}

TrieTree newTrieNode(TrieTree parent) {

	TrieTree newTrieTree = malloc(sizeof(struct TrieNode));
	if (newTrieTree == NULL)
		return NULL;

	newTrieTree->t = newVector();
	newTrieTree->city = NULL;
	newTrieTree->parent = parent;

	return newTrieTree;
}

TrieTree newTrieRoot() {
	return newTrieNode(NULL);
}

TrieTree addPath(const char *history, TrieTree trieTree) {

	while (*history != 0) {
		TrieTree nextNode = NULL;

		for (uint32_t i = 0; i < trieTree->t->numberOfElements; i++) {
			TrieEdge currEdge = (TrieEdge) getElement(trieTree->t, i);
			if (currEdge->label == *history) {
				nextNode = currEdge->node;
				break;
			}
		}

		if (nextNode == NULL) {
			nextNode = newTrieNode(trieTree);
			if (nextNode == NULL)
				return NULL;

			TrieEdge newEdge = newTrieEdge(*history, nextNode);
			if (newEdge == NULL) {
				deleteTrieTreeIfEmpty(nextNode);
				return NULL;
			}

			addElement(trieTree->t, newEdge);
		}

		trieTree = nextNode;
		history++;
	}

	return trieTree;
}

void removeTrie(TrieTree trieTree) {

	for (uint32_t i = 0; i < trieTree->t->numberOfElements; i++) {
		TrieEdge currEdge = (TrieEdge) getElement(trieTree->t, i);
		removeTrie(currEdge->node);
	}

	if (trieTree->city != NULL)
		deleteCity(trieTree->city);
	clearAndDelete(trieTree->t);
	free(trieTree);
}

TrieTree findNode(const char *history, TrieTree trieTree) {

	while (*history != 0) {
		bool stop = true;
		for (uint32_t i = 0; i < trieTree->t->numberOfElements; i++) {
			TrieEdge currEdge = getElement(trieTree->t, i);
			if (currEdge->label == *history) {
				trieTree = currEdge->node;
				history++;
				stop = false;
				break;
			}
		}
		if (stop) {
			return NULL;
		}
	}
	return trieTree;
}

City getCity(const char *history, TrieTree trieTree) {

	trieTree = findNode(history, trieTree);

	if (trieTree == NULL)
		return NULL;

	return trieTree->city;
}

void deleteTrieTreeIfEmpty(TrieTree trieTree) {

	if (trieTree->city == NULL && trieTree->t->numberOfElements == 0 && trieTree->parent != NULL) {

		TrieTree parent = trieTree->parent;

		for (uint32_t i = 0; i < parent->t->numberOfElements; i++) {

			TrieEdge edge = getElement(parent->t, i);

			if (edge->node == trieTree) {

				clearAndDelete(trieTree->t);
				free(trieTree);

				clear(getElement(parent->t, i));
				deleteByIndex(parent->t, i);

				return deleteTrieTreeIfEmpty(parent);
			}
		}
	}
}

void writeName(TrieTree trieTree, char** array) {

	if (trieTree->parent == NULL)
		return;

	TrieTree parent = trieTree->parent;

	for (uint32_t i = 0; i < parent->t->numberOfElements; i++) {

		TrieEdge edge = getElement(parent->t, i);

		if (edge->node == trieTree) {

			writeName(parent, array);

			**array = edge->label;
			(*array)++;

			return;
		}
	}
}

uint32_t getNameLength(TrieTree trieTree) {

	uint32_t res = 0;

	while (trieTree->parent != NULL) {

		res++;
		trieTree = trieTree->parent;
	}

	return res;
}
