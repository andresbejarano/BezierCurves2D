#include "drawable_lines.h"
#include "shaders.h"
#include <glm/gtc/type_ptr.hpp>

DrawableLines::DrawableLines() : 
	_totalElements(0)
{
	// Defines the shader program
	DefineShaderProgram();
}

DrawableLines::~DrawableLines()
{
}

void DrawableLines::DefineGeometry()
{
	// DO NOT DEFINE NOR USE!!!!!
}

void DrawableLines::DefinePairsGeometry(std::vector<Point*>* points)
{
	// Get the number of points
	int nPoints = points->size();

	// Initialize the array where the line points are stored (nPoints points x 3 coordinates per point)
	GLfloat * vertices = new GLfloat[nPoints * 3];

	// Traverse through the points
	for (int i = 0; i < nPoints; i += 1)
	{
		// Get the reference to the current point
		Point * p = points->at(i);

		// Define the index for the current point in the vertices array
		int index = i * 3;

		// Store the current vertex
		vertices[index + 0] = (GLfloat)p->x;
		vertices[index + 1] = (GLfloat)p->y;
		vertices[index + 2] = (GLfloat)p->z;
	}

	// Calculate the number of elements of the curve (nPoints / 2 lines X 2 points per line)
	_totalElements = nPoints;

	// Define the elements array for defining the lines of the curve 
	GLuint * elements = new GLuint[_totalElements];

	//
	for (int i = 0; i < nPoints; i += 1)
	{
		// Store the line elements
		elements[i] = i;
	}

	// Generate the VAO (if it hasn't yet)
	if ((int)_vao == -1)
	{
		glGenVertexArrays(1, &_vao);
	}

	// Bind the VAO
	glBindVertexArray(_vao);

	// Generate the VBO (if it hasn't yet)
	if ((int)_vbo == -1)
	{
		glGenBuffers(1, &_vbo);
	}

	// Bind and fill the VBO
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, nPoints * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	// Generate the EBO (if it hasn't yet)
	if ((int)_ebo == -1)
	{
		glGenBuffers(1, &_ebo);
	}

	// Bind and fill the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _totalElements * sizeof(GLuint), elements, GL_STATIC_DRAW);

	// Initialize the vertex attribute (if it hasn't yet)
	if (!_vertexAttributesInitialized)
	{
		// Enable the vertex array for passing the vertices to the shader
		glEnableVertexAttribArray(_pointLocation);

		// Set up the specifications of the vertex array
		glVertexAttribPointer(_pointLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		// Indicate the vertex attribute has been initialized
		_vertexAttributesInitialized = true;
	}

	// Delete the vertices and elements array (we don't need them anymore)
	delete vertices;
	delete elements;
}

void DrawableLines::DefineSequenceGeometry(std::vector<Point*>* points)
{
	// Get the number of points
	int nPoints = points->size();

	// Initialize the array where the line points are stored (nPoints points x 3 coordinates per point)
	GLfloat * vertices = new GLfloat[nPoints * 3];

	// Traverse through the points
	for (int i = 0; i < nPoints; i += 1)
	{
		// Get the reference to the current point
		Point * p = points->at(i);

		// Define the index for the current point in the vertices array
		int index = i * 3;

		// Store the current vertex
		vertices[index + 0] = (GLfloat)p->x;
		vertices[index + 1] = (GLfloat)p->y;
		vertices[index + 2] = (GLfloat)p->z;
	}

	// Calculate the number of elements of the curve (nPoints - 1 lines X 2 points per line)
	_totalElements = (nPoints - 1) * 2;

	// Define the elements array for defining the lines of the curve 
	GLuint * elements = new GLuint[_totalElements];

	//
	for (int i = 0; i < nPoints - 1; i += 1)
	{
		// Define the index for the current line elements
		int index = i * 2;

		// Store the line elements
		elements[index + 0] = i;
		elements[index + 1] = i + 1;
	}

	// Generate the VAO (if it hasn't yet)
	if ((int)_vao == -1)
	{
		glGenVertexArrays(1, &_vao);
	}

	// Bind the VAO
	glBindVertexArray(_vao);

	// Generate the VBO (if it hasn't yet)
	if ((int)_vbo == -1)
	{
		glGenBuffers(1, &_vbo);
	}

	// Bind and fill the VBO
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, nPoints * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	// Generate the EBO (if it hasn't yet)
	if ((int)_ebo == -1)
	{
		glGenBuffers(1, &_ebo);
	}

	// Bind and fill the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _totalElements * sizeof(GLuint), elements, GL_STATIC_DRAW);

	// Initialize the vertex attribute (if it hasn't yet)
	if (!_vertexAttributesInitialized)
	{
		// Enable the vertex array for passing the vertices to the shader
		glEnableVertexAttribArray(_pointLocation);

		// Set up the specifications of the vertex array
		glVertexAttribPointer(_pointLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		// Indicate the vertex attribute has been initialized
		_vertexAttributesInitialized = true;
	}

	// Delete the vertices and elements array (we don't need them anymore)
	delete vertices;
	delete elements;
}

void DrawableLines::DefineShaderProgram()
{
	// Generate the shader program
	GenerateShaderProgram(Shaders::basicVertexShaderSource, Shaders::basicFragmentShaderSource);

	// Specify the fragment shader output will be located in the default buffer (AKA 0)
	glBindFragDataLocation(_shaderProgram, 0, "outColor");

	// Get the identifier for the location of the point input variable of the shader
	_pointLocation = glGetAttribLocation(_shaderProgram, "point");

	// Get the location of the uniform variables in the shader program
	_modelMatrixLocation = glGetUniformLocation(_shaderProgram, "modelMatrix");
	_viewMatrixLocation = glGetUniformLocation(_shaderProgram, "viewMatrix");
	_projectionMatrixLocation = glGetUniformLocation(_shaderProgram, "projMatrix");
	_colorLocation = glGetUniformLocation(_shaderProgram, "color");
}

void DrawableLines::Draw(const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection)
{
	// If not initialized or not drawing then exit the function
	if (!_vertexAttributesInitialized || !_drawing)
	{
		return;
	}

	// Set the shader program
	glUseProgram(_shaderProgram);

	// Bind the VAO
	glBindVertexArray(_vao);

	// Set the color
	glUniform4f(_colorLocation, (float)_color->x, (float)_color->y, (float)_color->z, (float)_color->w);

	// Send matrices to shader
	glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(_projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projection));

	// Draw the lines
	glDrawElements(GL_LINES, _totalElements, GL_UNSIGNED_INT, 0);
}
