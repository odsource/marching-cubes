#include "Marching_Cubes.hpp"

TRIANGLE_COLLECTION* marching_cubes(VOXEL* vox)
{
	GLfloat isolevel = 0;
	// Index anhand des Vorzeichens berechnen
	GLfloat density_values[8];
	for (int i = 0; i < 8; i++)
	{
		density_values[i] = vox->val[i];
		printf("%f\r\n", density_values[i]);
	}
	printf("\r\n");

	int cubeindex = 0;
	if (density_values[0] >= isolevel) cubeindex |= 1;
	if (density_values[1] >= isolevel) cubeindex |= 2;
	if (density_values[2] >= isolevel) cubeindex |= 4;
	if (density_values[3] >= isolevel) cubeindex |= 8;
	if (density_values[4] >= isolevel) cubeindex |= 16;
	if (density_values[5] >= isolevel) cubeindex |= 32;
	if (density_values[6] >= isolevel) cubeindex |= 64;
	if (density_values[7] >= isolevel) cubeindex |= 128;

	printf("Cubeindex: %d\r\n", cubeindex);
	int edge_entry = edgeTable[cubeindex];
	printf("Number of Vertices: %d\r\n", numVertsTable[cubeindex]);
	int num_verts = numVertsTable[cubeindex];
	int num_tri = num_verts / 3;

	int t[16];
	int i = 0;
	for (int triangle : triTable[cubeindex])
	{
		t[i] = triangle;
		i++;
	}
	

	TRIANGLE_COLLECTION tri = {};
	for (int i = 0; i < num_tri; i++) {
		tri.t[i].v1 = vox->v[t[3*i]];
		printf("%f, %f, %f \n", tri.t[i].v1.p[0], tri.t[i].v1.p[1], tri.t[i].v1.p[2]);
		tri.t[i].v2 = vox->v[t[3*i+1]];
		tri.t[i].v3 = vox->v[t[3*i+2]];
	}
	tri.num = num_tri;
	return &tri;
};

GLfloat density(VERTEX* v, int fun)
{
	if (fun == 0)
	{
		return (sin(v->p[0]) - sin(v->p[1]) + sin(v->p[2]));
	}
}

VERTEX lin_interpolation(VERTEX v1, VERTEX v2, GLfloat value)
{
	return v1;
}