#include "bezier_curve.h"
#include "utils.h"

BezierCurve::BezierCurve() : 
	_controlPoints(new std::vector<Point *>()), 
	_degrees(new std::vector<std::vector<Point *> *>()), 
	_drawableCurveLines(new DrawableLines()), 
	_drawablePolygonLines(new DrawableLines()), 
	_drawableControlPoints(new DrawablePoints()), 
	_drawableDeCasteljauLines(new DrawableLines()), 
	_drawableDeCasteljauPoints(new DrawablePoints()), 
	_deCasteljauT(-1.0f),
	_drawableCurveTPoint(new DrawablePoints()), 
	_drawableCurveSPoint(new DrawablePoints()),
	_drawableCurveTicks(new DrawablePoints()), 
	_drawableConvexHull(new DrawablePolygon()), 
	_lastX(-1.0), 
	_lastY(-1.0), 
	_lastS(-1.0), 
	_lastSLeft(-1.0), 
	_lastSRight(-1.0)
{
}

BezierCurve::~BezierCurve()
{
	// Delete the control points vector
	Utils::DeletePointVector(_controlPoints);

	// Get the number of degrees stored
	int nDegrees = _degrees->size();

	// Traverse through the degrees and delete them
	for (int i = 0; i < nDegrees; i += 1) 
	{
		Utils::DeletePointVector(_degrees->at(i));
	}

	// Delete the degrees vector
	delete _degrees;
}

void BezierCurve::AddControlPoint(double x, double y)
{
	// Add the new point to the control points vector and indicate the geometries has been updated
	_controlPoints->push_back(new Point(x, y));
}

void BezierCurve::Clear()
{
	// Delete the control points
	Utils::DeletePointVector(_controlPoints);

	// Get the number of stored degrees
	int nDegrees = _degrees->size();
	
	// Traverse the degrees and delete them
	for (int i = 0; i < nDegrees; i += 1) 
	{
		Utils::DeletePointVector(_degrees->at(i));
	}

	// Delete the degrees vector
	delete _degrees;

	// Initialize the vectors
	_controlPoints = new std::vector<Point *>();
	_degrees = new std::vector<std::vector<Point *> *>();
}

std::vector<Point*>* BezierCurve::CopyControlPoints()
{
	// Initialize the vector for storing the copy of the control points
	std::vector<Point *> * copiedPoints = new std::vector<Point *>();

	// Get the number of control points
	int nControlPoints = _controlPoints->size();

	// Traverse through the control points
	for (int i = 0; i < nControlPoints; i += 1)
	{
		// Get the reference to the current points and store a copy of it into the copy vector
		Point * p = _controlPoints->at(i);
		copiedPoints->push_back(p->copy());
	}

	// Return the pointer to the vector with the copy of the control points
	return copiedPoints;
}

std::vector<Point*>* BezierCurve::CurvePoints(double x, double y, int segments)
{
	// Initialize the vector where the curve points will be stored
	std::vector<Point *> * curvePoints = new std::vector<Point *>();

	// Find the parameter step
	double tStep = (y - x) / (double)segments;

	// Get the points and store them in the curve points vector
	for (double t = x; t <= y; t += tStep) 
	{
		// Get the points at the given t value and copy it into the curve points vector
		Point * p = PointAt(t);
		curvePoints->push_back(p);
	}

	// Return the pointer to the vector with the curve points
	return curvePoints;
}

void BezierCurve::DeleteControlPoint(int index)
{
	// Delete the control point at the given location
	delete _controlPoints->at(index);

	// Erase the given location from the control points vector
	_controlPoints->erase(_controlPoints->begin() + index);
}

void BezierCurve::Draw(const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection, bool drawControlPoints)
{
	// If there are more than 1 control points then draw the elements of the curve
	if (_controlPoints->size() > 1) 
	{
		// Draw the convex hull
		_drawableConvexHull->Draw(model, view, projection);

		// Draw the polygon lines
		_drawablePolygonLines->Draw(model, view, projection);

		// Draw the DeCasteljau lines
		_drawableDeCasteljauLines->Draw(model, view, projection);

		// Draw the DeCasteljau points
		_drawableDeCasteljauPoints->Draw(model, view, projection);

		// Draw the curve lines
		_drawableCurveLines->Draw(model, view, projection);

		// Draw the curve ticks
		_drawableCurveTicks->Draw(model, view, projection);

		// Draw the curve t point
		_drawableCurveTPoint->Draw(model, view, projection);

		// Draw the curve s point
		_drawableCurveSPoint->Draw(model, view, projection);
	}

	// Draw if there is at least one control point and it is indicated
	if (_controlPoints->size() > 0 && drawControlPoints)
	{
		// Draw the control points
		_drawableControlPoints->Draw(model, view, projection);
	}
}

void BezierCurve::LowerDegree()
{
	int nDegrees = _degrees->size();

	if (nDegrees > 0) 
	{
		Utils::DeletePointVector(_controlPoints);

		_controlPoints = _degrees->at(nDegrees - 1);

		_degrees->erase(_degrees->begin() + (nDegrees - 1));
	}
}

Point * BezierCurve::PointAt(double t)
{
	// If no control points yet return null
	if (_controlPoints->size() == 0) 
	{
		return NULL;
	}

	// Make a copy of the control points
	std::vector<Point *> * deCasteljauPoints = CopyControlPoints();

	// Get the number of DeCasteljau points
	int nDeCasteljauPoints = deCasteljauPoints->size();

	// Repeat while there is more than one point in the DeCasteljau points vector
	while (nDeCasteljauPoints > 1)
	{
		// Initialize the vector for storing linear interpolation points
		std::vector<Point *> * lerps = new std::vector<Point *>();

		// Traverse through the DeCasteljau points
		for (int i = 0; i < nDeCasteljauPoints - 1; i += 1)
		{
			// Find the linear interpolation point between the current DeCasteljau point and the next one
			Point * l = Utils::lerp(deCasteljauPoints->at(i), deCasteljauPoints->at(i + 1), t);

			// Store the new point in the lerps vector
			lerps->push_back(l);
		}

		// Delete the DeCasteljau vector
		Utils::DeletePointVector(deCasteljauPoints);

		// Make the lerps vector the new DeCasteljau vector
		deCasteljauPoints = lerps;

		// Update the number of DeCasteljau points
		nDeCasteljauPoints = deCasteljauPoints->size();
	}

	// Keep the reference to the remaining point in the DeCasteljau points vector
	Point * at = deCasteljauPoints->at(0)->copy();

	// Delete the DeCasteljau vector
	Utils::DeletePointVector(deCasteljauPoints);

	// Return the pointer of the remaining point
	return at;
}

void BezierCurve::RaiseDegree()
{
	// If there are less than two control points then exit the function
	if (_controlPoints->size() < 2) 
	{
		return;
	}

	// Initialize the vector for storing the next degree points
	std::vector<Point *> * nextDegreePoints = new std::vector<Point *>();

	// Push a copy of the first control point into the next degree points vector
	nextDegreePoints->push_back(_controlPoints->at(0)->copy());

	// Get the number of control points
	int nPoints = _controlPoints->size();

	// Generate the control points for the new degree
	for (int i = 1; i < nPoints; i += 1) 
	{
		// Get the pointer to the required points for calculating the new current one
		Point * pi_1 = _controlPoints->at(i - 1)->copy();
		Point * pi = _controlPoints->at(i)->copy();

		// Calculate the common value for both points
		// NOTE: The actual value is (i / (n + 1)), since nPoints is already (n + 1) then let's use it
		double val = (double)i / ((double)nPoints);

		// 
		pi_1->multiply(val);

		// 
		pi->multiply(1.0 - val);

		// 
		pi_1->add(pi);

		// Push a copy of the first control point into the next degree points vector
		nextDegreePoints->push_back(pi_1);

		delete pi;
	}

	// Push a copy of the last control point into the next degree points vector
	nextDegreePoints->push_back(_controlPoints->at(nPoints - 1)->copy());

	// Store the current control points in the degrees vector
	_degrees->push_back(_controlPoints);

	// Set the new degree points as the control points of the curve
	_controlPoints = nextDegreePoints;

	// NOTE: It is assumed a curve update is performed after this function
}

BezierCurve * BezierCurve::Subdivide(double t)
{
	// If no two points in the curve then return null
	if (_controlPoints->size() < 2)
	{
		return NULL;
	}

	// The vector where the control points for the first half curve will be stored
	std::vector<Point *> * firstHalf = new std::vector<Point *>();

	// Insert a copy of the first control point into the first half points vector
	firstHalf->push_back(_controlPoints->at(0)->copy());

	// The vector where the control points for the second half curve will be stored
	std::vector<Point *> * secondHalf = new std::vector<Point *>();

	// Insert a copy of the last control point into the second half points vector
	secondHalf->push_back(_controlPoints->at(_controlPoints->size() - 1)->copy());

	// Make a copy of the control points
	std::vector<Point *> * deCasteljauPoints = CopyControlPoints();

	// Get the number of DeCasteljau points
	int nDeCasteljauPoints = deCasteljauPoints->size();

	// Repeat while there is more than one point in the DeCasteljau points vector
	while (nDeCasteljauPoints > 1)
	{
		// Initialize the vector for storing linear interpolation points
		std::vector<Point *> * lerps = new std::vector<Point *>();

		// Traverse through the DeCasteljau points
		for (int i = 0; i < nDeCasteljauPoints - 1; i += 1)
		{
			// Find the linear interpolation point between the current DeCasteljau point and the next one
			Point * l = Utils::lerp(deCasteljauPoints->at(i), deCasteljauPoints->at(i + 1), t);

			// Store the new point in the lerps vector
			lerps->push_back(l);
		}

		// Push a copy of the first point in lerps to the first half vector
		firstHalf->push_back(lerps->at(0)->copy());

		// Push a copy of the last point in lerps to the second half vector
		secondHalf->push_back(lerps->at(lerps->size() - 1)->copy());

		// Delete the DeCasteljau vector
		Utils::DeletePointVector(deCasteljauPoints);

		// Make the lerps vector the new DeCasteljau vector
		deCasteljauPoints = lerps;

		// Update the number of DeCasteljau points
		nDeCasteljauPoints = deCasteljauPoints->size();
	}

	// Delete the DeCasteljau vector
	Utils::DeletePointVector(deCasteljauPoints);

	// Delete the control points
	Utils::DeletePointVector(_controlPoints);

	// Get the number of stored degrees
	int nDegrees = _degrees->size();

	// Traverse the degrees and delete them
	for (int i = 0; i < nDegrees; i += 1)
	{
		Utils::DeletePointVector(_degrees->at(i));
	}

	// Delete the degrees vector
	delete _degrees;

	// Set the first half as the new control points
	_controlPoints = firstHalf;

	// Initialize the degrees vector
	_degrees = new std::vector<std::vector<Point *> *>();

	// Initialize a new Bezier curve
	BezierCurve * newHalfCurve = new BezierCurve();

	// Delete the control points of the new bezier curve and set the second half points as control points
	delete newHalfCurve->_controlPoints;
	newHalfCurve->_controlPoints = secondHalf;

	// Return the new half bezier curve
	return newHalfCurve;
}

void BezierCurve::Update(double x, double y, int segments)
{
	// If there is at least one control point then redefine the control points geometry
	if (_controlPoints->size() > 0) 
	{
		// Redefine the geometry of the control points
		_drawableControlPoints->DefineGeometry(_controlPoints);
	}

	// If there are less than two control points then exit the function
	if (_controlPoints->size() < 2) 
	{
		return;
	}

	// Store the used x and y values for defining the curve
	_lastX = (double)x;
	_lastY = (double)y;

	// Redefine the geometry of the control polygon
	_drawablePolygonLines->DefineSequenceGeometry(_controlPoints);

	// Get the curve points
	std::vector<Point *> * curvePoints = CurvePoints(x, y, segments);

	// Redefine the geometry of the curve lines
	_drawableCurveLines->DefineSequenceGeometry(curvePoints);

	// Sort the control points and calculate the convex hull
	std::vector<Point *> * controlPointsCopy = CopyControlPoints();
	std::vector<Point *> * convexHullVertices = Utils::ConvexHull(controlPointsCopy);

	// Define the convex hull geometry
	_drawableConvexHull->DefineGeometry(convexHullVertices);

	// Delete vectors
	Utils::DeletePointVector(curvePoints);
	Utils::DeletePointVector(controlPointsCopy);
	Utils::DeletePointVector(convexHullVertices);
}

void BezierCurve::UpdateDeCasteljau(double t)
{
	// If no control points yet then exit the function
	if (_controlPoints->size() < 2)
	{
		return;
	}

	// Update the DeCasteljau parameter
	_deCasteljauT = t;

	// Make a copy of the control points
	std::vector<Point *> * copyPoints = CopyControlPoints();

	// Get the number of copy points
	int nCopyPoints = copyPoints->size();

	// Initialize the vector where the DeCasteljau pair points are stored
	// NOTE: These are the points representing the DeCasteljau lines
	std::vector<Point *> * deCasteljauPairPoints = new std::vector<Point *>();

	// Initialize the vector where the DeCasteljau points are stored
	// NOTE: These are the points in the DeCasteljau lines, where the last one is the point in the curve
	std::vector<Point *> * deCasteljauPoints = new std::vector<Point *>();

	// Indicate it is working with the first copy of points
	// NOTE: This is a copy of the control points, not useful for representing the DeCasteljau lines
	bool firstCopy = true;

	// Repeat while there is more than one point in the copy points vector
	while (nCopyPoints > 1)
	{
		// Initialize the vector for storing linear interpolation points
		std::vector<Point *> * lerps = new std::vector<Point *>();

		// Traverse through the copy points
		for (int i = 0; i < nCopyPoints - 1; i += 1)
		{
			// Find the linear interpolation point between the current copy point and the next one
			Point * l = Utils::lerp(copyPoints->at(i), copyPoints->at(i + 1), t);

			// Store the new point in the lerps vector
			lerps->push_back(l);
		}

		// If it is the first copy of points then delete it
		// NOTE: It is a copy of the control points, they are not useful for drawing the DeCasteljau
		if (firstCopy) 
		{
			// Indicate it is no longer the first copy
			firstCopy = false;
		}
		else 
		{
			// Traverse through the copy points
			for (int i = 0; i < nCopyPoints; i += 1)
			{
				// Store the pointer of the current point into the DeCasteljau pair points
				deCasteljauPairPoints->push_back(copyPoints->at(i)->copy());

				// Store the pointer of the current point into the DeCasteljau points
				deCasteljauPoints->push_back(copyPoints->at(i)->copy());

				// If it is not the first or the last point then store the point again
				// NOTE: This is done since we are building pairs of lines, not a sequence of lines
				if (i > 0 && i < nCopyPoints - 1) 
				{
					// Store the pointer of the current point into the DeCasteljau pair points
					deCasteljauPairPoints->push_back(copyPoints->at(i)->copy());
				}
			}
		}

		// Delete the copy points vector
		Utils::DeletePointVector(copyPoints);

		// Make the lerps vector the new copy vector
		copyPoints = lerps;

		// Update the number of copy points by decreasing one
		nCopyPoints = copyPoints->size();
	}

	// Define the geometry of the DeCasteljau lines using the stored points so far
	_drawableDeCasteljauLines->DefinePairsGeometry(deCasteljauPairPoints);

	// Define the geometry of the DeCasteljau points
	_drawableDeCasteljauPoints->DefineGeometry(deCasteljauPoints);

	// Initialize a vector for storing the last point in the DeCasteljau
	std::vector<Point *> * tCurvePoint = new std::vector<Point *>();

	// Store the last DeCasteljau point. This is the curve point
	tCurvePoint->push_back(copyPoints->at(0)->copy());

	// Define the geometry of the curve point
	_drawableCurveTPoint->DefineGeometry(tCurvePoint);

	// Delete the vectors
	Utils::DeletePointVector(copyPoints);
	Utils::DeletePointVector(deCasteljauPairPoints);
	Utils::DeletePointVector(deCasteljauPoints);
	Utils::DeletePointVector(tCurvePoint);
}

void BezierCurve::UpdateSPoint(double s, double l, double r)
{
	// Update the last s reparametrized parameter
	_lastS = s;

	// Update the last left reparametrized domain
	_lastSLeft = l;

	// Update the last right reparametrized domain
	_lastSRight = r;

	// Calculate the equivalent t value for the repatametrized domain
	double t = ((-l) / (r - l)) + (((1.0) / (r - l)) * s);

	// Get the point for the calculated t
	Point * p = PointAt(t);

	// Initialize a vector for storing the point
	std::vector<Point *> * sPoint = new std::vector<Point *>();

	// Insert the point into the vector
	sPoint->push_back(p->copy());

	// Redefine the geometry of the reparametrized point
	_drawableCurveSPoint->DefineGeometry(sPoint);

	// Delete the calculated point
	delete p;

	// Delete the vector
	Utils::DeletePointVector(sPoint);
}

void BezierCurve::UpdateTicks(int n)
{
	// If no ticks are to be drawn then exit the function
	if (_controlPoints->size() < 1 || n < 1) 
	{
		return;
	}

	// Store the number of generated ticks
	_nTicks = n;

	// Initialize the vector where the tick points will be stored
	std::vector<Point *> * ticks = new std::vector<Point *>();

	// Traverse through the ticks and generate them
	for (int i = 0; i <= n; i += 1) 
	{
		// Calculate the parameter value for the current tick
		double t = (double)i / (double)n;

		// Get the point for the current tick
		Point * p = PointAt(t);

		// Push the point into the ticks vector
		ticks->push_back(p);
	}

	// Define the geometry for the curve ticks
	_drawableCurveTicks->DefineGeometry(ticks);

	// Delete the ticks vector
	Utils::DeletePointVector(ticks);
}
