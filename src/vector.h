#pragma once

#include <stdint.h>

typedef struct vector* Vector;

struct vector{
	uint32_t arraySize;
	uint32_t numberOfElements;
	void** array;
};

Vector newVector();
void addElement(Vector vector, void* element);
void* getElement(Vector vector, uint32_t index);
void swapWithLast(Vector vector, uint32_t index);
void* popBack(Vector vector);
void clear(Vector vector);
void clearAndDelete(Vector vector);
void reverseVector(Vector vector);
void replace(Vector vector, uint32_t index, Vector vectorToReplace);
void deleteByIndex(Vector vector, uint32_t index);
void deleteByValue(Vector vector, void* value);
Vector copyVector(Vector vector);