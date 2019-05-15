#include <stdlib.h>
#include "vector.h"
#include <stdio.h>

Vector newVector() {

	Vector result = malloc(sizeof(struct vector));
	if (result == NULL)
		return NULL;

	result->arraySize = 1;
	result->array = malloc(result->arraySize * sizeof(void *));
	result->numberOfElements = 0;

	return result;
}

void addElement(Vector vector, void* element) {

	if (vector->arraySize == vector->numberOfElements) {
		vector->arraySize *= 2;
		vector->array = realloc(vector->array, vector->arraySize * sizeof(void *));
	}

	vector->array[vector->numberOfElements++] = element;
}

void* getElement(Vector vector, uint32_t index) {

	if (index >= vector->numberOfElements)
        return NULL;

	return vector->array[index];
}

void* popBack(Vector vector) {

	if (vector->numberOfElements == 0)
		return NULL;

    void* result = vector->array[vector->numberOfElements - 1];
	vector->numberOfElements--;

	if (vector->arraySize > 1 && vector->numberOfElements * 4 <= vector->arraySize) {
		vector->arraySize /= 2;
		vector->array = realloc(vector->array, vector->arraySize * sizeof(void *));
	}

	return result;
}

void clearAndDelete(Vector vector) {

    while(vector->numberOfElements > 0)
        free(popBack(vector));

    free(vector->array);
    free(vector);
}

void clear(Vector vector) {

    if (vector == NULL)
        return;

    free(vector->array);
    free(vector);
}

static void swap(void** e1, void** e2) {
    void* tmp = *e1;
    *e1 = *e2;
    *e2 = tmp;
}

void reverseVector(Vector vector) {

    for (uint32_t i = 0; i * 2 < vector->numberOfElements; i++)
        swap(vector->array + i, vector->array + vector->numberOfElements - 1 - i);
}

void replace(Vector vector, uint32_t index, Vector vectorToReplace) {

    Vector secondPart = newVector();

    for (uint32_t i = index + 1; i < vector->numberOfElements; i++)
        addElement(secondPart, getElement(vector, i));

    while(vector->numberOfElements > index)
        popBack(vector);

    for (uint32_t i = 0; i < vectorToReplace->numberOfElements; i++)
        addElement(vector, getElement(vectorToReplace, i));

    for (uint32_t i = 0; i < secondPart->numberOfElements; i++)
        addElement(vector, getElement(secondPart, i));

    clear(secondPart);
    clear(vectorToReplace);
}

void deleteByIndex(Vector vector, uint32_t index) {
    replace(vector, index, newVector());
}

void deleteByValue(Vector vector, void* value) {

    for (uint32_t i = 0; i < vector->numberOfElements; i++)
        if (getElement(vector, i) == value)
            return deleteByIndex(vector, i);
}

Vector copyVector(Vector vector) {

    Vector res = newVector();

    for (uint32_t i = 0; i < vector->numberOfElements; i++)
        addElement(res, getElement(vector, i));

    return res;
}

