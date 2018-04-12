#pragma once

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#ifdef Linux
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#else
#include <GL\gl3w.h>
#include <GLFW\glfw3.h>
#endif
#include "imgui.h"
#include "bezier_curve.h"
#include "square.h"
#include <memory>

const int MOUSE_RELEASED = 0;
const int MOUSE_PRESSED = 1;

/*
The class representing the Bezier Curve application.
*/
class Application 
{

public:

	// The X coordinate of the mouse
	double _mouseX;

	// The Y coordinate of the mouse
	double _mouseY;

	// The index of the curve where the control focus is at the moment
	int _focusOnCurve;

	// The index of the control point where the mouse is at the moment
	int _mouseOnControlPoint;

	// Indicates whether the mouse is dragging a control point
	bool _mouseDragging;

	// Indicates whether to draw the control points
	bool _drawControlPoints;

	// Indicates whether to draw the convex hulls
	bool _drawConvexHull;

	// Indicates whether to draw the curves
	bool _drawCurve;

	// Indicate whether to draw the curve t point (last point in the DeCasteljau)
	bool _drawCurveTPoint;

	// Indicate whether to draw the curve s point (last point in the reparametrized DeCasteljau)
	bool _drawCurveSPoint;

	// Indicate whether to draw the tick marks on the curve
	bool _drawCurveTicks;

	// Indicate whether to draw the DeCasteljaus
	bool _drawDeCasteljau;

	// Indicate whether to draw the polygons
	bool _drawPolygon;

	// The hand cursor shape
	GLFWcursor * _mouseHandCursor;

	// The width of the window
	double _windowWidth;

	// The height of the window
	double _windowHeight;

	// Half of the window width
	double _halfWindowWidth;

	// Half of the window height
	double _halfWindowHeight;

	// The left value for the orthogonal projection
	float _orthoLeft;

	// The right value for the orthogonal projection
	float _orthoRight;

	// The top value for the orthogonal projection
	float _orthoTop;

	// The bottom value for the orthogonal projection
	float _orthoBottom;

	// The reference to the window
	GLFWwindow * _window;

	// The background color
	ImVec4 _backgroundColor;

	// The color for the control points
	ImVec4 _controlPointsColor;

	// The color for the polygon lines
	ImVec4 _polygonLinesColor;

	// The color for the DeCasteljau lines and points
	ImVec4 _deCasteljauColor;

	// The color for the convex hull polygon
	ImVec4 _convexHullColor;

	// The color of the curve
	ImVec4 _curveColor;

	// The color for the curve t point
	ImVec4 _curveTPointColor;

	// The color for the curve s point
	ImVec4 _curveSPointColor;

	// The color for the curve ticks
	ImVec4 _curveTicksColor;

	// The square for drawing the points
	Square * _square;

	// The value for the left parametric domain
	float _tLeftDomain;

	// The value for the right parametric domain
	float _tRightDomain;

	// The value for the left reparametrized domain
	float _sLeftDomain;

	// The value for the right reparametrized domain
	float _sRightDomain;

	// The t parameter for drawing the DeCasteljau
	float _deCasteljauT;

	// The s paramter for drawing the reparametrized DeCasteljau
	float _sParameter;

	// The number of ticks to be drawn over the curves
	int _nTicks;

	// The number of segments for drawing the curves
	int _nSegments;

	// The vector containing the Bezier curves
	std::vector<BezierCurve *> * _bezierCurves;

	// The indez of the Bezier curve to be updated
	int _updateCurve;

	// A static reference to the instance of the application
	static std::shared_ptr<Application> _staticInstance;

	/*
	Constructor of the class.
	*/
	Application();

	/*
	Destructor of the class.
	*/
	~Application();

	/*
	Removes everything from the screen.
	*/
	void ClearAll();

	/*
	Renders the content of the application.
	*/
	void Draw();

	/*
	Renders the user interface.
	*/
	void DrawUI();

	/*
	Returns the index of the control point where the mouse is with respect of the current focused Bezier curve
	@return int The index of the control point where the mouse is with respect of the current focused Bezier curve
	*/
	int MouseOnControlPoint();

	/*
	Initialize the objects of the application.
	*/
	void Init();

	/*
	Opens and processess the given file. Bezier curves are stored directly in the Bezier curves vector
	*/
	void OpenFile();

	/*
	Saves the current screenshot as a tif image. It is used DevIL SDK for Windows (http://openil.sourceforge.net/) 
	for generating the images. Check http://downloads.sourceforge.net/openil/DevIL-Windows-SDK-1.8.0.zip for the SDK.
	@return std::string The name of the file
	*/
	void SaveImage();

	/*
	Subdivides the current focused curve at the given t value
	@param double t
	*/
	void Subdivide(double t);

	/*
	Performs any required update on the curves.
	*/
	void Update();

	/*
	Updates the buffers that store the elements for the shaders
	*/
	void UpdateCurve();

	/*
	Writes the control points of the current curves in screen on a text file.
	*/
	void WriteFile();

	/*
	Returns a static instance of the application.
	*/
	static std::shared_ptr<Application> GetInstance();

	/*
	The callback function for mouse position events.
	@param GLFWwindow * window
	@param double xpos
	@param double ypos
	*/
	static void CursorPosCallback(GLFWwindow * window, double xpos, double ypos);

	/*
	The callback function for error events.
	@param int error
	@param const char * description
	*/
	static void ErrorCallback(int error, const char * description);

	/*
	The callback function for mouse button events.
	@param GLFWwindow * window
	@param int button
	@param int actions
	@param int mods
	*/
	static void MouseButtonCallback(GLFWwindow * window, int button, int actions, int mods);

	/*
	The callback function for window size events.
	@param GLFWwindow * window
	@param int width
	@param int height
	*/
	static void WindowSizeCallback(GLFWwindow * window, int width, int height);

	/*
	Runs the application.
	@param int argc
	@param char ** argv
	*/
	static int Run(int argc, char ** argv);

};

#endif
