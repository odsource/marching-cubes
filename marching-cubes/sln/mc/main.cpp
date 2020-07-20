#include "Window.hpp"
#include <fstream>
#include <sstream>
// #include <vector>
#include <time.h>
// #include <glm/gtc/type_ptr.hpp>
// #include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <cmath>
// #include "Marching_Cubes.hpp"
#include <helper_math.h>
#include <helper_cuda.h>
#include <stdio.h>
#include "marching_cubes_kernel.cuh"

//GLuint g_vertexArrayId[] = { 0, 1 };
//GLuint vertexBufferId[] = { 0, 0 };
//GLuint colorBufferId[] = { 0, 0 };

// VBOs
GLuint vbo1[4]; // ID of VBO for vertex arrays - 0 is reserved
				// glGenBuffers() will return non-zero id if success
				// vbo[0] - grid
				// vbo[1] - points
				// vbo[2] - geometry
				// vbo[3] - color
GLuint vbo2[4]; // ID of VBO for vertex arrays - 0 is reserved
				// glGenBuffers() will return non-zero id if success
				// vbo[0] - grid
				// vbo[1] - points
				// vbo[2] - geometry
				// vbo[3] - color

GLuint vao[2];  // ID of VAO
				// vao[0] - content of polygon
				// vao[1] - edge of polygon




GLuint g_shaderId = 0;
GLsizei num_grid = 0;
GLsizei num_points = 0;
GLsizei num_geom = 0;

GLsizei num_tri = 0;

GLuint loadShaders( const char * vertex_file_path, const char * fragment_file_path )
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader( GL_VERTEX_SHADER );
	GLuint FragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream( vertex_file_path, std::ios::in );
	if( VertexShaderStream.is_open() )
	{
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else
	{
		printf( "Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path );
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream( fragment_file_path, std::ios::in );
	if( FragmentShaderStream.is_open() )
	{
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf( "Compiling shader : %s\n", vertex_file_path );
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource( VertexShaderID, 1, &VertexSourcePointer, NULL );
	glCompileShader( VertexShaderID );

	// Check Vertex Shader
	glGetShaderiv( VertexShaderID, GL_COMPILE_STATUS, &Result );
	glGetShaderiv( VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength );
	if( InfoLogLength > 0 )
	{
		std::vector<char> VertexShaderErrorMessage( InfoLogLength + 1 );
		glGetShaderInfoLog( VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[ 0 ] );
		printf( "%s\n", &VertexShaderErrorMessage[ 0 ] );
	}

	// Compile Fragment Shader
	printf( "Compiling shader : %s\n", fragment_file_path );
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource( FragmentShaderID, 1, &FragmentSourcePointer, NULL );
	glCompileShader( FragmentShaderID );

	// Check Fragment Shader
	glGetShaderiv( FragmentShaderID, GL_COMPILE_STATUS, &Result );
	glGetShaderiv( FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength );
	if( InfoLogLength > 0 )
	{
		std::vector<char> FragmentShaderErrorMessage( InfoLogLength + 1 );
		glGetShaderInfoLog( FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[ 0 ] );
		printf( "%s\n", &FragmentShaderErrorMessage[ 0 ] );
	}

	// Link the program
	printf( "Linking program\n" );
	GLuint ProgramID = glCreateProgram();
	glAttachShader( ProgramID, VertexShaderID );
	glAttachShader( ProgramID, FragmentShaderID );
	glLinkProgram( ProgramID );

	// Check the program
	glGetProgramiv( ProgramID, GL_LINK_STATUS, &Result );
	glGetProgramiv( ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength );
	if( InfoLogLength > 0 )
	{
		std::vector<char> ProgramErrorMessage( InfoLogLength + 1 );
		glGetProgramInfoLog( ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[ 0 ] );
		printf( "%s\n", &ProgramErrorMessage[ 0 ] );
	}

	glDetachShader( ProgramID, VertexShaderID );
	glDetachShader( ProgramID, FragmentShaderID );

	glDeleteShader( VertexShaderID );
	glDeleteShader( FragmentShaderID );

	return ProgramID;
}
/*
std::vector<GLfloat> generateMesh()
{
	// createVBOs(vbo);
	
	GLsizei const size = 45;
	num_points = size;

	GLfloat const vertices[size] = {
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		-2.0f, 1.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		2.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		-2.0f, -1.0f, -2.0f,
		-2.0f, -1.0f, 2.0f,
		2.0f, -1.0f, 2.0f,

		-2.0f, -1.0f, -2.0f,
		2.0f, -1.0f, 2.0f,
		2.0f, -1.0f, -2.0f
	};


	std::vector<GLfloat> mesh;

	for (int index = 0; index < size; index++)
	{
		mesh.push_back(vertices[index]);
	}
	
	
	return mesh;
}

std::vector<GLfloat> generateColorData(int vao)
{
	std::vector<GLfloat> colors;
	if (vao == 0)
	{
		for (int i = 0; i < num_points; i++)
		{
			colors.push_back(1.0f);
			colors.push_back(1.0f);
			colors.push_back(1.0f);
		}
	}
	else
	{
		for (int i = 0; i < num_points; i++)
		{
			colors.push_back(0.0f);
			colors.push_back(0.0f);
			colors.push_back(0.0f);
		}
	}
	

	return colors;
}
*/
void initializeOpenGL()
{
	glEnable(GL_DEPTH_TEST);
	
	//glGenVertexArrays(1, &g_vertexArrayId[0]);
	//glBindVertexArray(g_vertexArrayId[0]);
	//
	printf("\r\n");
	printf("Kernel call initialized!\r\n");
	setNumPoints(100);
	createVBOs(vao, vbo1, vbo2);
	printf("Kernel call ended successfully!\r\n");
	printf("\r\n");
	/*
	//
	// Done in createVBOs()
	glGenBuffers(1, &vertexBufferId[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId[0]);
	std::vector<GLfloat> const mesh = generateMesh();
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh.size(), &mesh[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)NULL);
	
	glGenBuffers(1, &colorBufferId[0]);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferId[0]);
	std::vector<GLfloat> const colors = generateColorData(0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colors.size(), &colors[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferId[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)NULL);
	
	glGenVertexArrays(1, &g_vertexArrayId[1]);
	glBindVertexArray(g_vertexArrayId[1]);
	
	glGenBuffers(1, &vertexBufferId[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh.size(), &mesh[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)NULL);
	
	glGenBuffers(1, &colorBufferId[1]);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferId[1]);
	std::vector<GLfloat> const colors2 = generateColorData(1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colors2.size(), &colors2[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferId[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)NULL);

	glBindVertexArray(0);
	*/
	g_shaderId = loadShaders("vertex.glsl", "fragment.glsl");
}

void drawOpenGL( Window const * const _window, clock_t const & _lastInterval )
{
	glUseProgram(g_shaderId);

	GLfloat const rotationAngle = static_cast<GLfloat>(_lastInterval) / 1000.0f * 20.0f;
	glm::mat4x4 const scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 100.0f));
	glm::mat4x4 const rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(DegreeAngle(0/*rotationAngle*/).toFloat()), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4x4 const modelMatrix = rotationMatrix * scalingMatrix;

	Camera const camera = _window->getCamera();
	glm::mat4x4 const viewMatrix = camera.viewMatrix();
	glm::mat4x4 const projectionMatrix = camera.projectionMatrix();

	glm::mat4x4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

	GLuint matrixUniform = glGetUniformLocation( g_shaderId, "modelViewPerspective" );
	glUniformMatrix4fv( matrixUniform, 1, GL_FALSE, glm::value_ptr( modelViewProjectionMatrix ) );

	num_points = getNumPoints();
	num_grid = (num_points - 1) * (num_points - 1) * (num_points - 1) * 16;
	num_points = num_points * num_points * num_points;
	num_geom = (num_points - 1) * (num_points - 1) * (num_points - 1) * 15;

	GLsizei n = num_geom;
	
	//glBindVertexArray( g_vertexArrayId[0] );
	glBindVertexArray(vao[0]);
	// glPolygonMode gl_line anstatt gl_fill
	// mit 2 vao zeichnen eins schwarz mit line und eins mit gl_fill normal (selbe meshdaten)
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glDrawArrays(GL_QUADS, 0, n);
	glDrawArrays(GL_TRIANGLES, 0, n);
	
	glBindVertexArray( 0 );

	//glBindVertexArray(g_vertexArrayId[1]);
	/*
	glBindVertexArray(vao[1]);

	// glPolygonMode gl_line anstatt gl_fill
	// mit 2 vao zeichnen eins schwarz mit line und eins mit gl_fill normal (selbe meshdaten)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, n);
	
	glBindVertexArray(0);
	*/
	glUseProgram(0);
}

int main( int _argc, char ** _argv )
{
	Window * window = Window::getInstance();

	window->open( "Marching Cubes", 800, 600 );
	window->setEyePoint( glm::vec4( 0.0f, 0.0f, 1000.0f, 1.0f ) );
	window->setActive();

	initializeOpenGL();

	clock_t lastInterval = clock();

	while( window->isOpen() )
	{
		window->setActive();

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		drawOpenGL( window, lastInterval );

		lastInterval = clock();

		window->swapBuffer();
	}
}
