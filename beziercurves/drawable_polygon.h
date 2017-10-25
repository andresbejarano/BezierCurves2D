#pragma once

#ifndef _DRAWABLE_POLYGON_H_
#define _DRAWABLE_POLYGON_H_

#include "drawable.h"
#include <vector>

/*
The class representing a polygon to be drawn
*/
class DrawablePolygon : public Drawable 
{
public:

	// The number of elements for drawing the polygon
	int _totalElements;

	/*
	Constructor of the class
	*/
	DrawablePolygon();

	/*
	Destructor of the class
	*/
	~DrawablePolygon();

	/*
	Defines the geometry of the element
	NOTE: DO NOT USE IT
	*/
	void DefineGeometry();

	/*
	Defines the geometry of the polygon.
	NOTE: Vertices must be defined CCW
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
