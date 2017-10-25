#pragma once

#ifndef _DRAWABLE_LINES_H
#define _DRAWABLE_LINES_H

#include "drawable.h"
#include <vector>

/*
The class representing lines to be drawn
*/
class DrawableLines : public Drawable 
{
public:

	// The number of elements to be drawn
	int _totalElements;

	/*
	Constructor of the class
	*/
	DrawableLines();

	/*
	Destructor of the class
	*/
	~DrawableLines();

	/*
	Defines the geometry of the element
	NOTE: DO NOT USE IT
	*/
	void DefineGeometry();

	/*
	Defines the geometry of pairs of lines using the given points
	@param std::vector<Point *> * points
	*/
	void DefinePairsGeometry(std::vector<Point *> * points);

	/*
	Defines the geometry of a sequence of lines using the given points
	@param std::vector<Point *> * points
	*/
	void DefineSequenceGeometry(std::vector<Point *> * points);

	/*
	Defines the shader for the element
	*/
	void DefineShaderProgram();

	/*
	Draws the element
	@param const glm::mat4 model
	@param const glm::mat4 view
	@param const glm::mat4 projection
	*/
	void Draw(const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection);
};

#endif

