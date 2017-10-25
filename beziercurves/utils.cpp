#include "utils.h"
#include <sstream>
#include <iterator>

std::vector<Point*>* Utils::ConvexHull(std::vector<Point*>* points)
{
	// Sort the points
	Sort(points);

	// Initialize the vector where the lower vertices of the convex hull will be stored
	std::vector<Point *> * lower = new std::vector<Point *>();

	// Get the number of points
	int nPoints = points->size();

	// Traverse through the points from left to right
	for (int i = 0; i < nPoints; i += 1) 
	{
		// 
		while (lower->size() >= 2 && IsLeft(points->at(i), lower->at(lower->size() - 2), lower->at(lower->size() - 1)) <= 0) 
		{
			// Remove the last point in the lower vector
			lower->pop_back();
		}

		// Insert the current point
		lower->push_back(points->at(i)->copy());
	}

	// Initialize the vector where the upper vertices of the convex hull will be stored
	std::vector<Point *> * upper = new std::vector<Point *>();

	// Traverse through the points from right to left
	for (int i = nPoints - 1; i >= 0; i -= 1) 
	{
		while (upper->size() >= 2 && IsLeft(points->at(i), upper->at(upper->size() - 2), upper->at(upper->size() - 1)) <= 0) 
		{
			// Removes the last point in the upper vector
			upper->pop_back();
		}

		// Insert the current point
		upper->push_back(points->at(i)->copy());
	}

	// Remove the last stored points in both vectors
	upper->pop_back();
	lower->pop_back();

	// Get the number of points in the upper vector
	int nUpper = upper->size();

	// Traverse through the upper vector and add its points to the lower vector
	for (int i = 0; i < nUpper; i += 1) 
	{
		lower->push_back(upper->at(i)->copy());
	}

	// Delete the upper vector
	DeletePointVector(upper);

	// Return the lower vector
	return lower;
}

void Utils::DeletePointVector(std::vector<Point*>* points)
{
	// Get the number of points in the vector
	int nPoints = points->size();

	// Traverse through the points and delete them
	for (int i = 0; i < nPoints; i += 1) 
	{
		delete points->at(i);
	}

	// Delete the vector
	delete points;
}

double Utils::IsLeft(Point * p, Point * A, Point * B)
{
	return (A->x - p->x) * (B->y - p->y) - (B->x - p->x) * (A->y - p->y);
}

Point * Utils::lerp(Point * A, Point * B, double t)
{
	// Initialize the new point
	Point * p = new Point();

	// Calculate 1 - t
	double _t = 1.0 - t;

	// Find the coordinates of the interpolated point
	p->x = (_t * A->x) + (t * B->x);
	p->y = (_t * A->y) + (t * B->y);
	p->z = (_t * A->z) + (t * B->z);

	// Return the pointer to the interpolated point
	return p;
}

double Utils::map(double value, double start1, double stop1, double start2, double stop2)
{
	double mapped = start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
	return mapped;
}

void Utils::Sort(std::vector<Point*>* points)
{
	// Get the number of points
	int nPoints = points->size();

	// Traverse through the points and run BubbleSort
	// NOTE: I know it is not optimal but given the nature of this assignment I don't see why complicating with fancier methods
	for (int i = 0; i < nPoints; i += 1) 
	{
		for (int j = i + 1; j < nPoints; j += 1) 
		{
			// Get the reference to the point at index i
			Point * pi = points->at(i);

			// Get the reference to the point at index j
			Point * pj = points->at(j);

			// Check if pi.x is greater than pj.x
			if (pi->x > pj->x) 
			{
				// Swap pi with pj
				Point * pt = pi;
				points->at(i) = points->at(j);
				points->at(j) = pt;
			}
			else if (pi->x == pj->x) 
			{
				// Check if pi.y is greater than pj.y
				if (pi->y > pj->y) 
				{
					// Swap pi with pj
					Point * pt = pi;
					points->at(i) = points->at(j);
					points->at(j) = pt;
				}
			}
		}
	}
}

std::vector<std::string> Utils::split(const std::string & s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

std::string & Utils::trim_right_inplace(std::string & s, const std::string & delimiters)
{
	return s.erase(s.find_last_not_of(delimiters) + 1);
}

std::string & Utils::trim_left_inplace(std::string & s, const std::string & delimiters)
{
	return s.erase(0, s.find_first_not_of(delimiters));
}

std::string & Utils::trim_inplace(std::string & s, const std::string & delimiters)
{
	return trim_left_inplace(trim_right_inplace(s, delimiters), delimiters);
}

template<typename Out>
void Utils::split(const std::string &s, char delim, Out result) 
{
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}
