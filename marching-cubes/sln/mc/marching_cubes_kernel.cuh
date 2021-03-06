#pragma once
// Implementation of the marching cubes algorithm
#include "Window.hpp"
#include <fstream>
#include <sstream>
#include <time.h>
#include <cstdlib>
#include <cmath>
// #include "source/MCTable.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <helper_cuda.h>    // includes for helper CUDA functions
#include <helper_math.h>
#include <stdio.h>
#include <string.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <cuda_gl_interop.h>



#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

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

void createVBOs(GLuint* vao, GLuint* vbo1, GLuint* vbo2);
int getNumPoints();
void setNumPoints(int n);