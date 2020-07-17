#include "Marching_Cubes.hpp"
#include <helper_math.h>
#include <vector>

// Rendering variables
float xmax = 10.0f;
float xmin = -10.0f;
int numPoints = 3;
int dim = 3;
int func = 0;

TRIANGLE_COLLECTION* marching_cubes(VOXEL* vox)
{
	GLfloat isolevel = 0;
	// Index anhand des Vorzeichens berechnen
	GLfloat density_values[8];
	for (int i = 0; i < 8; i++)
	{
		density_values[i] = vox->val[i];
	}

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
	printf("Edge entry: %d\r\n", edge_entry);
	int num_verts = numVertsTable[cubeindex];
	printf("Number of Vertices: %d\r\n", numVertsTable[cubeindex]);
	int num_tri = num_verts / 3;
	printf("Number of Triangles: %d\r\n", num_tri);

	// interpolation();

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

GLfloat density(VERTEX* v)
{
	if (func == 0)
	{
		return (sin(v->p[0]) - sin(v->p[1]) + sin(v->p[2]));
	}
}

VERTEX lin_interpolation(VERTEX v1, VERTEX v2, GLfloat value)
{
	return v1;
}

void generate_data() 
{
	unsigned int points_size;
	float4* points;
	unsigned int grid_size;
	float4* grid;
	unsigned int geom_size;
	float4* geom;

	// Allocate memory
	points_size = numPoints * numPoints * numPoints * sizeof(float4);
	points = (float4*)malloc(points_size);
	grid_size = (numPoints - 1) * (numPoints - 1) * (numPoints - 1) * 16
		* sizeof(float4);
	grid = (float4*)malloc(grid_size);
	geom_size = (numPoints - 1) * (numPoints - 1) * (numPoints - 1) * 15
		* sizeof(float4);
	geom = (float4*)malloc(geom_size);

	// Initialize data
	int v = -1;
	int sum = 0;
	// Initialize points data.
	float delta = (xmax - xmin) / (numPoints - 1);
	for (int i = 0; i < numPoints; i++) {
		for (int j = 0; j < numPoints; j++) {
			for (int k = 0; k < numPoints; k++) {

				int idx = i + j * numPoints + k * numPoints * numPoints;

				// Set initial position data
				points[idx].x = i; // xmin + delta * i;
				points[idx].y = j; // xmax - delta * j;
				points[idx].z = k; // xmin + delta * k;
				printf("%f, %f, %f\r\n", points[idx].x, points[idx].y, points[idx].z);
				v++;
				if (v == 2) {
					printf("\r\n");
					v = -1;
					sum += 1;
				}
			}
		}
	}
	printf("Summe: %d\r\n", sum);

	// Initialize grid data.
	for (int i = 0; i < (numPoints - 1); i++) {
		for (int j = 0; j < (numPoints - 1); j++) {
			for (int k = 0; k < (numPoints - 1); k++) {

				int idx_pt = i + j * numPoints + k * numPoints * numPoints;
				int idx_sq = idx_pt - j + k - 2 * k * numPoints;

				// Set initial position data
				grid[16 * idx_sq + 0] = points[idx_pt];
				grid[16 * idx_sq + 1] = points[idx_pt + 1];
				grid[16 * idx_sq + 2] = points[idx_pt + numPoints + 1];
				grid[16 * idx_sq + 3] = points[idx_pt + numPoints];

				grid[16 * idx_sq + 4] = points[idx_pt + numPoints * numPoints];
				grid[16 * idx_sq + 5] = points[idx_pt + numPoints * numPoints + 1];
				grid[16 * idx_sq + 6] = points[idx_pt + numPoints * numPoints + numPoints + 1];
				grid[16 * idx_sq + 7] = points[idx_pt + numPoints * numPoints + numPoints];

				grid[16 * idx_sq + 8] = points[idx_pt];
				grid[16 * idx_sq + 9] = points[idx_pt + 1];
				grid[16 * idx_sq + 10] = points[idx_pt + numPoints * numPoints + 1];
				grid[16 * idx_sq + 11] = points[idx_pt + numPoints * numPoints];

				grid[16 * idx_sq + 12] = points[idx_pt + numPoints];
				grid[16 * idx_sq + 13] = points[idx_pt + numPoints + 1];
				grid[16 * idx_sq + 14] = points[idx_pt + numPoints * numPoints + numPoints + 1];
				grid[16 * idx_sq + 15] = points[idx_pt + numPoints * numPoints + numPoints];

			}
		}
	}

	std::vector<float> mesh;

	for (int index = 0; index < (numPoints * numPoints * numPoints); index++)
	{
		mesh.push_back(points[index].x);
		mesh.push_back(points[index].y);
		mesh.push_back(points[index].z);
		//mesh.push_back(points[index].w);
	}
}