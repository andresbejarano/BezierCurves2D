#pragma once

#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

#include <GL/gl3w.h>
#include <glm/mat4x4.hpp>
#include "point.h"

/*
The abstract class for any element to be rendered in the window
*/
class Drawable
{

public:

	// Indicates whether drawing the element is enabled or disabled
	bool _drawing;

	// Indicates whether the vertex attributes have been initialized
	bool _vertexAttributesInitialized;

	// The identifier for the vertex array object
	GLuint _vao;

	// The identifier for the vertex buffer object
	GLuint _vbo;

	// The identifier for the element buffer object
	GLuint _ebo;

	// The identifier to the shader program
	GLuint _shaderProgram;

	// The identifier for the location of the model matrix in the shader program
	GLint _modelMatrixLocation;

	// The identifier for the location of the view matrix in the shader program
	GLint _viewMatrixLocation;

	// The identifier for the location of the projection matrix in the shader program
	GLint _projectionMatrixLocation;

	// The identifier for the location of the vertex point in the shader program
	GLint _pointLocation;

	// The identifier for the location of the color variable in the shader program
	GLint _colorLocation;

	// The color for the object
	// NOTE: Use the coordinates (X, Y, Z) as the (R, G, B) color components
	Point * _color;

	/*
	Constructor of the class
	*/
	Drawable();

	/*
	Destructor of the class
	*/
	~Drawable();

	/*
	Indicates whether the drawing is enabled or disabled for the element
	@return bool The drawing state
	*/
	bool IsDrawing();

	/*
	Sets the color components for drawing the object
	@param double r
	@param double g
	@param double b
	@param double a
	*/
	void SetColor(double r, double g, double b, double a);

	/*
	Sets the drawing state
	@param bool d The drawing state
	*/
	void SetDrawing(bool d);

	/*
	Toggles the drawing state
	*/
	void ToggleDrawing();

	/*
	Defines a shader and returns its identifier
	@param const char * source The source code of the shader
	@param GLenum type The requested type (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
	*/
	GLuint GetShader(const char * source, GLenum type);

	/*
	Defines the shader program for th given vertex and fragment shader codes
	@param const char * vertexShaderCode The source code of the vertex shader
	@param const char * fragmentShader Code The source code of the fragment shader
	*/
	void GenerateShaderProgram(const char * vertexShaderCode, const char * fragmentShaderCode);

	/*
	Defines the shader for the element
	*/
	virtual void DefineShaderProgram() = 0;

	/*
	Defines the geometry of the element
	*/
	virtual void DefineGeometry() = 0;

	/*
	Draws the element
	@param const glm::mat4 model
	@param const glm::mat4 view
	@param const glm::mat4 projection
	*/
	virtual void Draw(const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection) = 0;

};

#endif
