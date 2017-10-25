#pragma once

#ifndef _POINT_H_
#define _POINT_H_

/*
A class representing a point using 4 coordinates
*/
class Point
{
public:

	// The x coordinate of the point
	double x;

	// The y coordinate of the point
	double y;

	// The z coordinate of the point
	double z;

	// The w coordinate of the point
	double w;

	/*
	Constructor of the class. By default the point is set to (0, 0, 0)
	*/
	Point() : x(0.0), y(0.0), z(0.0), w(0.0) {}

	/*
	Constructor of the class.
	@param double v The value for the coordinates
	*/
	Point(double v) : x(v), y(v), z(v), w(v) {}

	/*
	Constructor of the class. Z coordinate is set to 0.0
	@param double x The x coordinate of the point
	@param double y The y coordinate of the point
	*/
	Point(double x, double y) : x(x), y(y), z(0.0), w(0.0) {}

	/*
	Constructor of the class
	@param double x The x coordinate of the point
	@param double y The y coordinate of the point
	@param double z The z coordinate of the point
	*/
	Point(double x, double y, double z) : x(x), y(y), z(z), w(0.0) {}

	/*
	Constructor of the class
	@param double x The x coordinate of the point
	@param double y The y coordinate of the point
	@param double z The z coordinate of the point
	@param double w The w coordinate of the point
	*/
	Point(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

	/*
	Constructor of the class
	@param point The pointer to another point
	*/
	Point(Point * P) : x(P->x), y(P->y), z(P->z), w(P->w) {}

	/*
	Destructor of the class
	*/
	~Point() {};

	/*
	Returns a copy of the point
	@return Point *
	*/
	Point * copy();

	/*
	Set the given value to the coordinates
	@param double v The value for the coordinates
	*/
	void set(double v);

	/*
	Set the coordinate of the point
	@param double x The X coordinate
	@param double y The Y coordinate
	@param double z The Z coordinate
	*/
	void set(double x, double y, double z);

	/*
	Set the coordinate of the point
	@param double x The X coordinate
	@param double y The Y coordinate
	@param double z The Z coordinate
	@param double z The W coordinate
	*/
	void set(double x, double y, double z, double w);

	/*
	Set the coordinate of the point
	@param point * The pointer to a point
	*/
	void set(Point * point);

	/*
	Set coordinate values with random numbers.
	@param int n The max random number
	*/
	void setRandom(int n);

	/*
	Calculates the dot product of the current point with the given point
	@param Point * p A pointer to a point
	@return double The dot product
	*/
	double dot(Point * P);

	/*
	Returns the magnitude of the vector defined by the point
	@return double The magnitude of the vector
	*/
	double magnitude();

	/*
	Normalizes the point
	*/
	void normalize();

	/*
	Returns the distance to the given point.
	@param Point * p A pointer to a point
	@return double The distance between the current point and p
	*/
	double distance(Point * p);

	/*
	Returns a pointer to the Point with the information of the direction vector between the current point and p.
	@param Point * p A pointer to a point
	@return Point * The pointer to the direction vector object
	*/
	Point * direction(Point * p);

	/*
	Multuply the point by the given scalar
	@param double s The scalar value
	*/
	void multiply(double s);

	/*
	Adds the given point to the current one
	@param Point * p A pointer to a point
	*/
	void add(Point * p);

	/*
	Returns the angle (in radians) between the current point and the given point.
	@param Point * p A pointer to a point
	@return double The angle between the points
	*/
	double angle(Point  * p);

};

#endif
