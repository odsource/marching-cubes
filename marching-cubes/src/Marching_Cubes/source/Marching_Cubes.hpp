#pragma once
// Implementation of the marching cubes algorithm
#include "Window.hpp"
#include "MCTable.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef struct {
	GLfloat X;
	GLfloat Y;
	GLfloat Z;
} VEC;

typedef struct {
	VEC v1;
	VEC v2;
	VEC v3;
	VEC v4;
	VEC v5;
	VEC v6;
	VEC v7;
	VEC v8;
} CUBE;

typedef struct {
	VEC v;
	VEC norm;
} TRIANGLE;

typedef struct {
	CUBE c;
	double val[8];
} VOXEL;

TRIANGLE* marching_cubes(GLfloat* density_values);
GLfloat density(VEC* p, std::string fun);
VEC lin_interpolation(VEC v1, VEC v2, GLfloat value);
