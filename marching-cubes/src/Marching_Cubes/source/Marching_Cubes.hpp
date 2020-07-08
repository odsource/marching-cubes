#pragma once
// Implementation of the marching cubes algorithm
#include "Window.hpp"
#include "MCTable.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef struct {
	GLfloat p[3];
} VERTEX;

typedef struct {
	VERTEX v1;
	VERTEX v2;
	VERTEX v3;
} TRIANGLE;

typedef struct {
	TRIANGLE t[16];
	GLsizei num;
} TRIANGLE_COLLECTION;

typedef struct {
	VERTEX v[8];
	GLfloat val[8];
} VOXEL;

TRIANGLE_COLLECTION* marching_cubes(VOXEL* vox);
GLfloat density(VERTEX* v, int fun);
VERTEX lin_interpolation(VERTEX v1, VERTEX v2, GLfloat value);
