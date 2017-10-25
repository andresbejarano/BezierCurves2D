#pragma once

#ifndef _UTILS_H_
#define _UTILS_H_

#include "point.h"
#include <vector>

namespace Utils 
{
	/*
	Returns the vector with the points of the convex hull from the given points. 2D convex hull 
	is calculated using the Monotone chain method as described in: 
	https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
	NOTE: Given vector is sorted.
	@param std::vector<Point *> * points A presorted vector with the points
	@return std::vector<Point *> * A vector with the vertices of the convex hull
	*/
	std::vector<Point *> * ConvexHull(std::vector<Point *> * points);

	/*
	Deletes the content and the vector from memory.
	@param std::vector<Point *> * points
	*/
	void DeletePointVector(std::vector<Point *> * points);

	/*
	Tests whether a point p2 is left, on or right of a line defined by p0 and p1.
	@param Point * p The point to be tested
	@param Point * A The first point defining the line
	@param Point * B The second point defining the line
	@return >0 for p left of the line through A and B, =0 for p on the line, <0 for p right of the line
	*/
	double IsLeft(Point * p, Point * A, Point * B);

	/*
	Returns the linear interpolation between two points and a given parameter.
	@param Point * A
	@param Point * B
	@param double t
	@return Point * The pointer to the point from the linear interpolation
	*/
	Point * lerp(Point * A, Point * B, double t);

	/*
	@param double value
	@param double start1
	@param double stop1
	@param double start2
	@param double stop2
	@return
	*/
	double map(double value, double start1, double stop1, double start2, double stop2);

	/*
	Sorts the given vector of points
	@param std::vector<Point *> * points
	*/
	void Sort(std::vector<Point *> * points);

	/*
	From: https://stackoverflow.com/questions/236129/most-elegant-way-to-split-a-string
	*/
	template<typename Out>
	void split(const std::string &s, char delim, Out result);

	/*
	From From: https://stackoverflow.com/questions/236129/most-elegant-way-to-split-a-string
	*/
	std::vector<std::string> split(const std::string &s, char delim);

	/*
	From http://www.cplusplus.com/faq/sequences/strings/trim/
	*/
	std::string& trim_right_inplace(std::string& s, const std::string& delimiters = " \f\n\r\t\v");

	/*
	From http://www.cplusplus.com/faq/sequences/strings/trim/
	*/
	std::string& trim_left_inplace(std::string& s, const std::string& delimiters = " \f\n\r\t\v");

	/*
	From http://www.cplusplus.com/faq/sequences/strings/trim/
	*/
	std::string& trim_inplace(std::string& s, const std::string& delimiters = " \f\n\r\t\v");

}

#endif
