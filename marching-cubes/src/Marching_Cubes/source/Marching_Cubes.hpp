#pragma once
// Implementation of the marching cubes algorithm
#include "MCTable.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

typedef struct {
	std::vector<int> p[3];
	std::vector<int> norm;
} TRIANGLE;

typedef struct {
	std::vector<int> p[8];
	double val[8];
} GRID;

