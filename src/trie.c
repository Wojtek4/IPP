#include <stdlib.h>
#include <stdint.h>
#include "trie.h"
#include <stdio.h>

typedef struct trieEdge* TrieEdge;

struct trieEdge {
    char label;
    TrieTree node;
};

TrieEdge newTrieEdge(const char label, TrieTree node) {
    TrieEdge result = malloc(sizeof(struct trieEdge));
    result->node = node;
    result->label = label;
    return result;
}

TrieTree newTrieNode(TrieTree parent) {

	TrieTree newTrieTree = malloc(sizeof(struct TrieNode));
	if (newTrieTree == NULL)
		exit(1);

	newTrieTree->t = newVector();
	newTrieTree->city = NULL;
	newTrieTree->parent = parent;

	return newTrieTree;
}

TrieTree newTrieRoot() {
    return newTrieNode(NULL);
}

TrieTree addPath(const char *history, TrieTree trieTree) {

    //printf("add path %s\n", history);

	while (*history != 0) {
		TrieTree nextNode = NULL;

		for (uint32_t i = 0; i < trieTree->t->numberOfElements; i++) {
		    TrieEdge currEdge = (TrieEdge) getElement(trieTree->t, i);
		    if (currEdge->label == *history) {
                //printf("ide dalej %d, %s\n", *history, history);
                nextNode = currEdge->node;
                break;
            }
		}

		if (nextNode == NULL) {
		    //printf("nowy %d, %s\n", *history, history);
		    nextNode = newTrieNode(trieTree);
            addElement(trieTree->t, newTrieEdge(*history, nextNode));
        }

		trieTree = nextNode;
		history++;
	}
	//printf("koniec\n");
	return trieTree;
}

void removeTrie(TrieTree trieTree) {

	TrieTree *stack = malloc(sizeof(TrieTree));
	stack[0] = trieTree;
	uint32_t sizeOfStackMemory = 1, numberOfPtrsOnStack = 1;

	for (uint32_t i = 0; i < numberOfPtrsOnStack; i++) {

		for (uint32_t j = 0; j < stack[i]->t->numberOfElements; j++) {

            TrieEdge currEdge = (TrieEdge) getElement(stack[i]->t, j);

			if (currEdge == NULL)
				continue;

			if (sizeOfStackMemory == numberOfPtrsOnStack) {
				sizeOfStackMemory *= 2;
				stack = realloc(stack, sizeOfStackMemory * sizeof(TrieTree));
				if (stack == NULL)
					exit(1);
			}

			stack[numberOfPtrsOnStack++] = currEdge->node;
		}
        clearAndDelete(stack[i]->t);
		if (stack[i]->city != NULL)
		    deleteCity(stack[i]->city);
		free(stack[i]);
	}

	free(stack);
}

TrieTree findNode(const char *history, TrieTree trieTree) {

    //printf("find node %s\n", history);

	while (*history != 0) {
	    bool stop = true;
        for (uint32_t i = 0; i < trieTree->t->numberOfElements; i++) {
            TrieEdge currEdge = getElement(trieTree->t, i);
            //printf("Jestem w %d, %s\n", currEdge->label, history);
            if (currEdge->label == *history) {
                trieTree = currEdge->node;
                history++;
                //printf("IDE %d, %s\n", *history, history);
                stop = false;
                break;
            }
        }
        if (stop) {
            //printf("Nieznalazlem, %s\n", history);
            return NULL;
        }
	}
	//printf("Znalazlem\n\n");
	return trieTree;
}

City getCity(const char *history, TrieTree trieTree) {
    trieTree = findNode(history, trieTree);
    if (trieTree == NULL) {
        //printf("NIEZNALAZLEM\n");
        return NULL;
    }
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
