#pragma once

#ifndef _DRAWABLE_POINTS_H
#define _DRAWABLE_POINTS_H

#include "drawable.h"
#include <vector>

/*
The class representing points to be drawn
*/
class DrawablePoints : public Drawable 
{
public:

	// The number of points to be drawn
	int _totalPoints;

	/*
	Constructor of the class
	*/
	DrawablePoints();

	/*
	Destructor of the class
	*/
	~DrawablePoints();

	/*
	Defines the geometry of the element
	NOTE: DO NOT USE IT
	*/
	void DefineGeometry();

	/*
	Defines the geometry of pairs of lines using the given points
	@param std::vector<Point *> * points
	*/
	void DefineGeometry(std::vector<Point *> * points);

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
