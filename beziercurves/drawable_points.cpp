#include "drawable_points.h"
#include "shaders.h"
#include <glm/gtc/type_ptr.hpp>

DrawablePoints::DrawablePoints() : 
	_totalPoints(0)
{
	// Define the shader program for drawing the points
	DefineShaderProgram();
}

DrawablePoints::~DrawablePoints()
{
}

void DrawablePoints::DefineGeometry()
{
}

void DrawablePoints::DefineGeometry(std::vector<Point*>* points)
{
	// Get the number of points to be drawn 
	_totalPoints = points->size();

	// Define an array for storing the points (_totalPoints points x 3 coordinates per point)
	GLfloat * vertices = new GLfloat[_totalPoints * 3];

	// Traverse through the point
	for (int i = 0; i < _totalPoints; i += 1)
	{
		// Get the reference to the current point
		Point * point = points->at(i);

		// Define the global index in the vertices array for the current vertex
		int index = i * 3;

		// Store the information of the current vertex
		vertices[index + 0] = (GLfloat)point->x;
		vertices[index + 1] = (GLfloat)point->y;
		vertices[index + 2] = (GLfloat)point->z;
	}

	// Generate the VAO (if it hasn't yet)
	if (_vao == -1)
	{
		glGenVertexArrays(1, &_vao);
	}

	// Bind the VAO
	glBindVertexArray(_vao);

	// Generate the VBO (if it hasn't yet)
	if (_vbo == -1)
	{
		glGenBuffers(1, &_vbo);
	}

	// Bind and fill the VBO
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _totalPoints * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

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

	// Delete the vertices vector
	delete vertices;
}

void DrawablePoints::DefineShaderProgram()
{
	// Generate the shader program
	GenerateShaderProgram(Shaders::pointsVertexShaderSource, Shaders::pointsFragmentShaderSource);

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

void DrawablePoints::Draw(const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection)
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

	// Draw the points
	glDrawArrays(GL_POINTS, 0, _totalPoints);
}
