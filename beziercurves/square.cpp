#include "square.h"
#include "utils.h"

Square::Square(double size) : 
	_size((double)size), 
	_halfSize(_size / 2.0)
{
	// Define the shader program for the polygon representing the square
	DefineShaderProgram();

	// Initialize the vector where the square vertices will be stored
	std::vector<Point *> * points = new std::vector<Point *>();

	// Generate and push the point representing the bottom left corner
	points->push_back(new Point(-_halfSize, -_halfSize));

	// Generate and push the point representing the bottom right corner
	points->push_back(new Point(_halfSize, -_halfSize));

	// Generate and push the point representing the top right corner
	points->push_back(new Point(_halfSize, _halfSize));

	// Generate and push the point representing the top left corner
	points->push_back(new Point(-_halfSize, _halfSize));

	// Define the geometry of the square
	DefineGeometry(points);

	// Delete the points vector
	Utils::DeletePointVector(points);
}

Square::~Square()
{
}
