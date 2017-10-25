#include "drawable.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Drawable::Drawable() :
	_drawing(true), 
	_vertexAttributesInitialized(false), 
	_vao(-1),
	_vbo(-1),
	_ebo(-1),
	_shaderProgram(-1),
	_modelMatrixLocation(-1),
	_viewMatrixLocation(-1),
	_projectionMatrixLocation(-1),
	_pointLocation(-1),
	_colorLocation(-1), 
	_color(new Point())
{
}

Drawable::~Drawable()
{
	// Delete the shader program
	glUseProgram(0);
	glDeleteProgram(_shaderProgram);
}

bool Drawable::IsDrawing()
{
	return _drawing;
}

void Drawable::SetColor(double r, double g, double b, double a)
{
	_color->set(r, g, b, a);
}

void Drawable::SetDrawing(bool d)
{
	_drawing = d;
}

void Drawable::ToggleDrawing()
{
	_drawing = !_drawing;
}

GLuint Drawable::GetShader(const char * source, GLenum type)
{
	// Create the shader
	GLuint shader = glCreateShader(type);

	// Set the shader source
	glShaderSource(shader, 1, &source, NULL);

	// Compile the shader
	glCompileShader(shader);

	// Get the compile status
	GLint status = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	// If there was a problem during compilation then post it on console
	if (status != GL_TRUE)
	{
		// Get the error message
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);

		// Write the error message
		std::cout << "Error while compiling the shader: " << std::endl << buffer << std::endl;
	}

	// Return the shader identifier
	return shader;
}

void Drawable::GenerateShaderProgram(const char * vertexShaderCode, const char * fragmentShaderCode)
{
	// Define the vertex shader
	GLuint vertexShader = GetShader(vertexShaderCode, GL_VERTEX_SHADER);

	// Define the fragment shader
	GLuint fragmentShader = GetShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

	// Initialize the shader program
	_shaderProgram = glCreateProgram();

	// Attach both the vertex shader and the fragment shader to the shader program
	glAttachShader(_shaderProgram, vertexShader);
	glAttachShader(_shaderProgram, fragmentShader);

	// Link the shader program
	glLinkProgram(_shaderProgram);

	// Delete shaders (we don't need them anymore since the shader program is already built)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
