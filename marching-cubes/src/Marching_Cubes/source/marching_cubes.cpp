#include "Marching_Cubes.hpp"

TRIANGLE* marching_cubes(GLfloat* density_values) 
{
	TRIANGLE tri[1] = {};
	GLfloat isolevel = 0;
	int cubeindex = 0;
	if (density_values[0] < isolevel) cubeindex |= 1;
	if (density_values[1] < isolevel) cubeindex |= 2;
	if (density_values[2] < isolevel) cubeindex |= 4;
	if (density_values[3] < isolevel) cubeindex |= 8;
	if (density_values[4] < isolevel) cubeindex |= 16;
	if (density_values[5] < isolevel) cubeindex |= 32;
	if (density_values[6] < isolevel) cubeindex |= 64;
	if (density_values[7] < isolevel) cubeindex |= 128;
	return tri;
};

GLfloat density(VEC* p, std::string fun)
{
	// TODO: GPU implementieren
	return sin(p->X);
}

VEC lin_interpolation(VEC v1, VEC v2, GLfloat value)
{
	return v1;
}