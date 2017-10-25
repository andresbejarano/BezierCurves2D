#pragma once

#ifndef _BEZIER_CURVE_H_
#define _BEZIER_CURVE_H_

#include "drawable_lines.h"
#include "drawable_points.h"
#include "drawable_polygon.h"

/*
The class representing the Bezier Curve
*/
class BezierCurve 
{

public:

	// The vector where control points are stored
	std::vector<Point *> * _controlPoints;

	// The vector containing the increased point degrees of the curve
	std::vector<std::vector<Point *> *> * _degrees;

	// The drawable lines representing the curve
	DrawableLines * _drawableCurveLines;

	// The drawable lines representing the curve polygon
	DrawableLines * _drawablePolygonLines;

	// The drawable lines representing the DeCasteljau
	DrawableLines * _drawableDeCasteljauLines;

	// The drawable points representing the control points of the curve
	DrawablePoints * _drawableControlPoints;

	// The drawable points representing the DeCasteljau
	DrawablePoints * _drawableDeCasteljauPoints;

	// The drawable point representing the last DeCasteljau point with parameter t
	DrawablePoints * _drawableCurveTPoint;

	// The drawable point representing the last DeCasteljau point with parameter s (reparametrized domain)
	DrawablePoints * _drawableCurveSPoint;

	// The drawable points representing the ticks on the curve
	DrawablePoints * _drawableCurveTicks;

	// The drawable polygon representing the convex hull of the curve
	DrawablePolygon * _drawableConvexHull;

	// The t parameters used for defining the current DeCasteljau elements
	double _deCasteljauT;

	// The last left parametric domain used for defining the curve
	double _lastX;

	// The last right parametric domain used for defining the curve
	double _lastY;

	// The last s for calculating the reparametrized point
	double _lastS;

	// The last left reparametrized domain
	double _lastSLeft;

	// The last right reparametrized domain
	double _lastSRight;

	// The number of ticks defined for the curve
	int _nTicks;

	// Indicate the las
	int _updatedControlPoints;


	/*
	Constructor of the class
	*/
	BezierCurve();

	/*
	Destructor of the class
	*/
	~BezierCurve();

	/*
	Adds a new control point with the given coordinates to the control points vector
	@param double x The X coordinate of the point
	@param double y The Y coordinate of the point
	*/
	void AddControlPoint(double x, double y);

	/*
	Clears all of the content of the Bezier curve
	*/
	void Clear();

	/*
	Returns a pointer to the vector with the copy of the control points.
	@return std::vector<Point *> * A pointer to the vector with the copy of the control points
	*/
	std::vector<Point *> * CopyControlPoints();

	/*
	Returns a vector with the points of the Bezier Curve in the domain [x, y]
	@param double x The left value of the parameter domain
	@param double y The right value of the parameter domain
	@param int segments The number of segments of the curve
	@return std::vector<Point *> * A pointer to the vector with the points of the curve
	*/
	std::vector<Point *> * CurvePoints(double x, double y, int segments);

	/*
	Deletes the control points stored at the given index.
	@param int index The index of the point to be deleted from the control points vector.
	*/
	void DeleteControlPoint(int index);

	/*
	Draws the elements of the curve
	@param const glm::mat4 model
	@param const glm::mat4 view
	@param const glm::mat4 projection
	@param bool drawControlPoints
	*/
	void Draw(const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection, bool drawControlPoints);

	/*
	Lowers the degree of the curve to the latest stored.
	*/
	void LowerDegree();

	/*
	Returns the curve point at parameter t. Point is obtained using DeCasteljau's algorithm
	@param double t The parameter for the point
	@return Point * The pointer to the point at the given parameter
	*/
	Point * PointAt(double t);

	/*
	Raises the curve degree by one. Algorithm detailed in 
	http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/Bezier/bezier-elev.html
	*/
	void RaiseDegree();

	/*
	Subdivides the current curve into two curves. The current curve keeps the first half (from the first control point to the point at t).
	The second half (from point at t to the original final control point) is returned.
	NOTE: It is required for the original curve to have at least two points, otherwise it is not subdivided and the returned curve is NULL.
	@param double t The cutting parameter
	@return BezierCurve * The second half of the curve
	*/
	BezierCurve * Subdivide(double t);

	/*
	Updates the geometry of the drawable elements representing the curve (curve and polygon lines).
	@param double x The left value of the parameter domain
	@param double y The right value of the parameter domain
	@param int segments The number of segments of the curve
	*/
	void Update(double x, double y, int segments);

	/*
	Updates the DeCasteljau lines using the given parameter.
	@param double t
	*/
	void UpdateDeCasteljau(double t);

	/*
	Updates the curve point with reparametrized domain [l, r]
	@param double s The reparametrized parameter
	@param double l The left boundary of the reparametrized domain
	@param double r The right boundary for the reparametrized domain
	*/
	void UpdateSPoint(double s, double l, double r);

	/*
	Update the number of ticks to be displayed on the curve.
	@param int n The number of ticks
	*/
	void UpdateTicks(int n);

};

#endif
