#pragma once

#ifndef _SQUARE_H_
#define _SQUARE_H_

#include "drawable_polygon.h"

/*
The class representing a square drawn using a drawable polygon
*/
class Square : public DrawablePolygon 
{

public:

	// The size of the square
	double _size;

	// Half the size of the square
	double _halfSize;

	/*
	Constructor of the class
	@param double size The length of each side of the square
	*/
	Square(double size);

	/*
	Destructor of the class
	*/
	~Square();

};

#endif
