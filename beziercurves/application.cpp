#include "application.h"
#include "square.h"
#include "utils.h"
#include "imgui_impl_glfw_gl3.h"
#include <glm/gtc/matrix_transform.hpp>
#ifdef Linux
#include <IL/il.h>
#include <IL/ilut.h>
#else
#include <IL\il.h>
#include <IL\ilut.h>
#endif
#include <chrono>
#include <ctime>
#include <sstream>
#include <fstream>
#include <iostream>
#include "tinyfiledialogs.h"

// Initialize the static pointer to the application
std::shared_ptr<Application> Application::_staticInstance = nullptr;

Application::Application() :
	_mouseX(0.0),
	_mouseY(0.0), 
	_focusOnCurve(-1), 
	_mouseOnControlPoint(-1),
	_mouseDragging(false),
	_drawControlPoints(true),
	_drawConvexHull(true),
	_drawCurve(true), 
	_drawCurveTPoint(true), 
	_drawCurveSPoint(true), 
	_drawCurveTicks(true), 
	_drawDeCasteljau(true), 
	_drawPolygon(true), 
	_mouseHandCursor(glfwCreateStandardCursor(GLFW_HAND_CURSOR)), 
	_windowWidth(900.0), 
	_windowHeight(900.0), 
	_halfWindowWidth(_windowWidth / 2.0), 
	_halfWindowHeight(_windowHeight / 2.0), 
	_orthoLeft(-1.0f), 
	_orthoRight(1.0f),
	_orthoTop(1.0f),
	_orthoBottom(-1.0f),
	_backgroundColor(ImVec4(0.298f, 0.298f, 0.498f, 1.0f)), 
	_controlPointsColor(ImVec4(1.0f, 1.0f, 0.0f, 1.0f)), 
	_polygonLinesColor(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)), 
	_deCasteljauColor(ImVec4(0.0f, 1.0f, 1.0f, 1.0f)), 
	_convexHullColor(ImVec4(1.0f, 1.0f, 1.0f, 0.3f)), 
	_curveColor(ImVec4(1.0f, 0.0f, 1.0f, 1.0f)), 
	_curveTPointColor(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)), 
	_curveSPointColor(ImVec4(0.0f, 0.0f, 1.0f, 1.0f)),
	_curveTicksColor(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), 
	_tLeftDomain(0.0f), 
	_tRightDomain(1.0f), 
	_sLeftDomain(0.0f), 
	_sRightDomain(1.0f), 
	_deCasteljauT(0.5f), 
	_sParameter(0.5f), 
	_nTicks(1), 
	_nSegments(200), 
	_bezierCurves(new std::vector<BezierCurve *>()), 
	_updateCurve(-1)
{
}

Application::~Application()
{
}

void Application::ClearAll()
{
	// Get the number of stored Bezier curves
	int nBezierCurves = _bezierCurves->size();

	// Traverse through the stored Bezier curves and delete them
	for (int i = 0; i < nBezierCurves; i += 1) 
	{
		delete _bezierCurves->at(i);
	}

	// Delete the Bezier curves vector
	delete _bezierCurves;

	// Initialize the Bezier curves vector and push a new Bezier curve
	_bezierCurves = new std::vector<BezierCurve *>();
	_bezierCurves->push_back(new BezierCurve());

	// Indicate the focused Bezier curve is at index 0
	_focusOnCurve = 0;

	// Indicate no Bezier curve has to be updated
	_updateCurve = -1;

	// Indicate the mouse is not over any control point
	_mouseOnControlPoint = -1;
}

void Application::Draw()
{
	// Define the viewport
	glViewport(0, 0, (int)_windowWidth, (int)_windowHeight);

	// Draw the background
	glClearColor(_backgroundColor.x, _backgroundColor.y, _backgroundColor.z, _backgroundColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Define the view and projection transformation matrices
	// NOTE: They are the same for all elements
	const glm::mat4 modelMatrix = glm::mat4();
	const glm::mat4 viewMatrix = glm::mat4();
	const glm::mat4 projMatrix = glm::ortho(_orthoLeft, _orthoRight, _orthoBottom, _orthoTop, -1.0f, 1.0f);

	// Get the number of curves
	int nBezierCurves = _bezierCurves->size();

	// Traverse through the bezier curves and draw them
	for (int i = 0; i < nBezierCurves; i += 1) 
	{
		// Get the reference to the current bezier curve
		BezierCurve * bezierCurve = _bezierCurves->at(i);

		// Set the color values
		bezierCurve->_drawableCurveLines->SetColor(_curveColor.x, _curveColor.y, _curveColor.z, _curveColor.w);
		bezierCurve->_drawablePolygonLines->SetColor(_polygonLinesColor.x, _polygonLinesColor.y, _polygonLinesColor.z, _polygonLinesColor.w);
		bezierCurve->_drawableDeCasteljauLines->SetColor(_deCasteljauColor.x, _deCasteljauColor.y, _deCasteljauColor.z, _deCasteljauColor.w);
		bezierCurve->_drawableDeCasteljauPoints->SetColor(_deCasteljauColor.x, _deCasteljauColor.y, _deCasteljauColor.z, _deCasteljauColor.w);
		bezierCurve->_drawableControlPoints->SetColor(_controlPointsColor.x, _controlPointsColor.y, _controlPointsColor.z, _controlPointsColor.w);
		bezierCurve->_drawableConvexHull->SetColor(_convexHullColor.x, _convexHullColor.y, _convexHullColor.z, _convexHullColor.w);
		bezierCurve->_drawableCurveTPoint->SetColor(_curveTPointColor.x, _curveTPointColor.y, _curveTPointColor.z, _curveTPointColor.w);
		bezierCurve->_drawableCurveSPoint->SetColor(_curveSPointColor.x, _curveSPointColor.y, _curveSPointColor.z, _curveSPointColor.w);
		bezierCurve->_drawableCurveTicks->SetColor(_curveTicksColor.x, _curveTicksColor.y, _curveTicksColor.z, _curveTicksColor.w);

		// Set the drawing options
		bezierCurve->_drawableControlPoints->SetDrawing(_drawControlPoints);
		bezierCurve->_drawableConvexHull->SetDrawing(_drawConvexHull);
		bezierCurve->_drawableCurveLines->SetDrawing(_drawCurve);
		bezierCurve->_drawableDeCasteljauLines->SetDrawing(_drawDeCasteljau);
		bezierCurve->_drawableDeCasteljauPoints->SetDrawing(_drawDeCasteljau);
		bezierCurve->_drawablePolygonLines->SetDrawing(_drawPolygon);
		bezierCurve->_drawableCurveTPoint->SetDrawing(_drawCurveTPoint);
		bezierCurve->_drawableCurveSPoint->SetDrawing(_drawCurveSPoint);
		bezierCurve->_drawableCurveTicks->SetDrawing(_drawCurveTicks);

		// Draw the bezier curve and its components
		bezierCurve->Draw(modelMatrix, viewMatrix, projMatrix, i != _focusOnCurve);

		// Draw the control points if required
		if (_drawControlPoints && i == _focusOnCurve)
		{
			// Set the control points color to the square
			_square->SetColor(_controlPointsColor.x, _controlPointsColor.y, _controlPointsColor.z, _controlPointsColor.w);

			// Get the number of control points
			int nControlPoints = bezierCurve->_controlPoints->size();

			// Traverse through the control points and draw a square for each one of them
			for (int j = 0; j < nControlPoints; j += 1)
			{
				// If the current control point is being dragged then ommit it
				// NOTE: This is done for drawing such point above everything else
				if (j == _mouseOnControlPoint && _mouseDragging)
				{
					continue;
				}

				// Get the reference to the current control point
				Point * p = bezierCurve->_controlPoints->at(j);

				// Define the model matrix for the current control point
				const glm::mat4 pointModelMatrix = glm::translate(glm::mat4(), glm::vec3(p->x, p->y, 0.0f));

				// Draw a square representing the current control point
				_square->Draw(pointModelMatrix, viewMatrix, projMatrix);
			}

			// Draw the square for the dragged point
			if (_mouseOnControlPoint > -1 && _mouseDragging)
			{
				// Get the reference to the dragged control point
				Point * p = bezierCurve->_controlPoints->at(_mouseOnControlPoint);

				// Define the model matrix for the dragged control point
				const glm::mat4 modelMatrix = glm::translate(glm::mat4(), glm::vec3(p->x, p->y, 0.0f));

				// Draw a square representing the dragged control point
				_square->Draw(modelMatrix, viewMatrix, projMatrix);
			}
		}
	}
}

void Application::DrawUI()
{
	// Get a new ImGui frame
	ImGui_ImplGlfwGL3_NewFrame();

	// Define the visualization panel components
	ImGui::Begin("Visualization Panel");
	ImGui::ColorEdit4("t Point", (float*)&_curveTPointColor);
	ImGui::ColorEdit4("s Point", (float*)&_curveSPointColor);
	ImGui::ColorEdit4("Ticks", (float*)&_curveTicksColor);
	ImGui::ColorEdit4("Curve", (float*)&_curveColor);
	ImGui::ColorEdit4("Control Points", (float*)&_controlPointsColor);
	ImGui::ColorEdit4("Control Polygon", (float*)&_polygonLinesColor);
	ImGui::ColorEdit4("De Casteljau", (float*)&_deCasteljauColor);
	ImGui::ColorEdit4("Convex Hull", (float*)&_convexHullColor);
	ImGui::ColorEdit3("Background", (float*)&_backgroundColor);
	ImGui::Separator();
	ImGui::Checkbox("Curve", &_drawCurve); ImGui::SameLine(150);
	ImGui::Checkbox("Control Polygon", &_drawPolygon); ImGui::SameLine(300);
	ImGui::Checkbox("Ticks", &_drawCurveTicks);
	ImGui::Checkbox("t Point", &_drawCurveTPoint); ImGui::SameLine(150);
	ImGui::Checkbox("De Casteljau", &_drawDeCasteljau); ImGui::SameLine(300);
	ImGui::Checkbox("s Point", &_drawCurveSPoint);
	ImGui::Checkbox("Control Points", &_drawControlPoints); ImGui::SameLine(150);
	ImGui::Checkbox("Convex Hull", &_drawConvexHull);
	ImGui::End();

	// Build the char * with the index options
	std::stringstream ss;
	int nBezierCurves = _bezierCurves->size();
	for (int i = 0; i < nBezierCurves; i += 1)
	{
		ss << i << '\0';
	}
	ss << '\0';
	std::string curveIndices = ss.str();
	const char * indexOptions = curveIndices.c_str();

	// Build the char * with the mouse coordinates
	ss.str("");
	ss.clear();
	ss << "Mouse at (" << _mouseX << ", " << _mouseY << ")";
	std::string mouseAt = ss.str();
	const char * mouseAtChar = mouseAt.c_str();

	// Define the control panel components
	ImGui::Begin("Control Panel");
	ImGui::Combo("Working on curve", &_focusOnCurve, indexOptions);
	ImGui::Separator();
	ImGui::Text("Parametric Domains & User Values (For selected curve)");
	ImGui::DragFloatRange2("[X, Y] range for t", &_tLeftDomain, &_tRightDomain, 0.01f, 0.0f, 0.0f, "X: %.1f", "Y: %.1f");
	ImGui::SliderFloat("t in [0, 1]", &_deCasteljauT, 0.0f, 1.0f);
	ImGui::DragFloatRange2("[X, Y] range for s", &_sLeftDomain, &_sRightDomain, 0.01f, 0.0f, 0.0f, "X: %.1f", "Y: %.1f");
	ss.str("");
	ss.clear();
	ss << "s in [" << _sLeftDomain << ", " << _sRightDomain << "]";
	std::string sRange = ss.str();
	const char * sRangeC = sRange.c_str();
	if (_sParameter < _sLeftDomain) 
	{
		_sParameter = _sLeftDomain;
	}
	if (_sParameter > _sRightDomain)
	{
		_sParameter = _sRightDomain;
	}
	ImGui::SliderFloat(sRangeC, &_sParameter, _sLeftDomain, _sRightDomain);
	ImGui::SliderInt("Tick marks", &_nTicks, 1, 100);
	if (ImGui::Button("Restore Defaults")) 
	{
		_tLeftDomain = 0.0f;
		_tRightDomain = 1.0f;
		_sLeftDomain = 0.0f;
		_sRightDomain = 1.0f;
		_deCasteljauT = 0.5f;
		_sParameter = 0.5f;
		_nTicks = 1;
	}
	ImGui::Separator();
	ImGui::Text("Curve Operations (For selected curve)");
	if (ImGui::Button("Raise Degree"))
	{
		// Raise the degree of the focused Bezier curve and indicate it has to be updated
		_bezierCurves->at(_focusOnCurve)->RaiseDegree();
		_updateCurve = _focusOnCurve;
	}
	ImGui::SameLine();
	if (ImGui::Button("Lower Degree"))
	{
		// Lower the degree of the focused Bezier curve and indicate it has to be updated
		_bezierCurves->at(_focusOnCurve)->LowerDegree();
		_updateCurve = _focusOnCurve;
	}
	ImGui::SameLine();
	if (ImGui::Button("Subdivide at t")) 
	{
		// Subdivide the curve using the current t value
		Subdivide(_deCasteljauT);
	}
	ImGui::Separator();
	ImGui::Text("Display Options");
	if (ImGui::Button("Clear All"))
	{
		// Clears everything
		ClearAll();
	}
	ImGui::SameLine();
	if (ImGui::Button("Save Screenshot"))
	{
		// Save the screenshot
		SaveImage();
	}


	if (ImGui::Button("Open File")) 
	{
		// Open a file and process it
		OpenFile();
	}
	ImGui::SameLine();
	if (ImGui::Button("Save File")) 
	{
		// Save the curves in a text file
		WriteFile();
	}
	ImGui::Separator();
	ImGui::TextUnformatted(mouseAtChar);
	ImGui::End();

	// Render the control panel 
	// NOTE: We need to indicate to use no shader program for drawing the panel
	glUseProgram(0);
	ImGui::Render();

	// Sanity check for overlapping range values
	if (_tLeftDomain == _tRightDomain)
	{
		_tRightDomain += 0.1f;
	}
}

int Application::MouseOnControlPoint()
{
	// Get the reference to the focused Bezier curve
	BezierCurve * bezierCurve = _bezierCurves->at(_focusOnCurve);

	// Get the number of control points
	int nControlPoints = bezierCurve->_controlPoints->size();

	// Traverse through the control points
	for (int i = 0; i < nControlPoints; i += 1)
	{
		// Get the reference to the current control point
		Point * p = bezierCurve->_controlPoints->at(i);

		// Return the current index if the mouse coordinates are within the current control point rect
		if (
			_mouseX >= p->x - _square->_halfSize &&
			_mouseX <= p->x + _square->_halfSize &&
			_mouseY >= p->y - _square->_halfSize &&
			_mouseY <= p->y + _square->_halfSize)
		{
			// Return the index of the control point
			return i;
		}
	}

	// The mouse was not found over any control point, then return -1
	return -1;
}

void Application::Init()
{
	// Add a new Bezier curve into the Bezier curves array
	_bezierCurves->push_back(new BezierCurve());

	// Indicate the focus is on the very first (and only for now) Bezier curve
	_focusOnCurve = 0;

	// Generate a new square
	_square = new Square(0.04);
}

void Application::OpenFile()
{
	// The filter for txt files
	const char * filePatterns[1] = { "*.txt" };

	// Display the open file dialog
	const char * filename = tinyfd_openFileDialog("Open a Bezier File", "", 1, filePatterns, NULL, 0);

	// If it was given a filename then open it
	if (filename) 
	{
		// Open the file as an input stream file (makes life easier)
		std::ifstream in(filename);

		// The string where individual lines will be stored
		std::string line;

		// The number of bezier curves in the file
		//int nArcs = -1;

		// The vector for storing the generated bezier curves
		std::vector<BezierCurve *> curves;

		// The current Bezier curve
		BezierCurve * currentCurve = new BezierCurve();

		// Read the file line by line
		while (std::getline(in, line)) 
		{
			// Split the line with the comment char
			std::vector<std::string> tokens = Utils::split(line, '#');

			// Discard the comments, split by blanks
			tokens = Utils::split(tokens.at(0), ' ');

			// Take the command of the line and trim it
			std::string first = tokens.at(0);
			Utils::trim_inplace(first);

			// Compare the command and check which one is
			if (first.compare("arcs") == 0) 
			{
				// DO NOTHING!
				// This field is irrelevant since curves are allocated dinamycally

				std::cout << "arcs = " << tokens.at(1) << std::endl;
			}
			else if (first.compare("arc") == 0)
			{
				// DO NOTHING!
				// This field is irrelevant since curve points are allocated dinamycally

				std::cout << "arc = " << tokens.at(1) << std::endl;
			}
			else if (first.compare("endarc") == 0) 
			{
				std::cout << "endarc" << std::endl;

				// If the current curve has points then store it in the curves vector; otherwise, delete the current new curve
				if (currentCurve->_controlPoints->size() > 0) 
				{
					curves.push_back(currentCurve);
				}
				else 
				{
					delete currentCurve;
				}

				// Initialize a new curve
				currentCurve = new BezierCurve();
			}
			else 
			{
				// Get the X value of the point (trim blanks and <>)
				std::string sx = tokens.at(0);
				Utils::trim_inplace(sx);
				Utils::trim_inplace(sx, "<>");
				double x = std::stod(sx);

				// Get the Y value of the point (trim blanks and <>)
				std::string sy = tokens.at(1);
				Utils::trim_inplace(sy);
				Utils::trim_inplace(sy, "<>");
				double y = std::stod(sy);

				std::cout << "double (" << x << ", " << y << ")" << std::endl;

				// Generate a new point ans push it into the control points for the new current curve
				currentCurve->_controlPoints->push_back(new Point(x, y));
			}
		}

		// Close the file
		in.close();

		// Get the number of generated curves
		int nCurves = curves.size();

		std::cout << "Generated " << nCurves << " curves" << std::endl;

		// Get tne index for the new curve (it is the size of the number of stored curves)
		int newCurveIndex = _bezierCurves->size();

		// Update and add the new curves
		for (int i = 0; i < nCurves; i += 1) 
		{
			// Store the current new curve
			_bezierCurves->push_back(curves.at(i));

			// Delete the reference to the new curve from the curves vector (we don't want to loose it)
			curves.at(i) = NULL;

			// Update the visualization of the new Bezier curve
			_bezierCurves->at(newCurveIndex)->Update((float)_tLeftDomain, (float)_tRightDomain, _nSegments);
			_bezierCurves->at(newCurveIndex)->UpdateDeCasteljau(_deCasteljauT);

			// Increment the index for the next new curve
			newCurveIndex += 1;
		}

		// Delete the current curve
		delete currentCurve;
	}
}

void Application::SaveImage()
{
	// The filter for tiff files
	const char * filePatterns[1] = { "*.tiff" };

	// Get the file name
	const char * filename = tinyfd_saveFileDialog("Save Screenshot", "screenshot.tiff", 1, filePatterns, NULL);

	// Save the screenshot if a file name is given
	if (filename) 
	{
		// Init IL
		ilInit();

		// Get the image id for the screenshot and bind it
		ILuint imageID = ilGenImage();
		ilBindImage(imageID);

#ifndef Linux
                // FIXME : plain wrong on Linux, and save file does NOT work (but does not crash at least ...)
		// Get the screen
		ilutGLScreen();
#endif

		// Enable file overwrite
		ilEnable(IL_FILE_OVERWRITE);

		// Save the image to disk using the given file name
		ilSaveImage(filename);

		// Delete the image from memory
		ilDeleteImage(imageID);
	}
}

void Application::Subdivide(double t)
{
	// Get the pointer to the subdivided bezier curve
	BezierCurve * half = _bezierCurves->at(_focusOnCurve)->Subdivide(t);

        _bezierCurves->at(_focusOnCurve)->UpdateTicks(_nTicks);

	// If NULL is returned then exit the function (nothing happened in the original curve)
	if (half == NULL) 
	{
		return;
	}

	// Push the new Bezier curve into the Bezier curves vector
	_bezierCurves->push_back(half);

	// Update the visualization of the current focused Bezier curve
	_bezierCurves->at(_focusOnCurve)->Update((float)_tLeftDomain, (float)_tRightDomain, _nSegments);
	_bezierCurves->at(_focusOnCurve)->UpdateDeCasteljau(_deCasteljauT);
	_bezierCurves->at(_focusOnCurve)->UpdateSPoint(_sParameter, _sLeftDomain, _sRightDomain);

	// Get the index of the new Bezier curve
	int nNewCurve = _bezierCurves->size() - 1;

	// Update the visualization of the new Bezier curve
	_bezierCurves->at(nNewCurve)->Update((float)_tLeftDomain, (float)_tRightDomain, _nSegments);
	_bezierCurves->at(nNewCurve)->UpdateDeCasteljau(_deCasteljauT);
	_bezierCurves->at(nNewCurve)->UpdateSPoint(_sParameter, _sLeftDomain, _sRightDomain);

	// See : https://github.com/andresbejarano/BezierCurves2D/issues/1
	_bezierCurves->at(nNewCurve)->UpdateTicks(_nTicks);

}

void Application::Update()
{
	// If the focused Bezier curve has less than two points then exit the function
	// NOTE: There is nothing we can do
	if (_bezierCurves->at(_focusOnCurve)->_controlPoints->size() < 2) 
	{
		return;
	}

	// If the focused curve has stored a different parametric domain then update it
	if ((float)_bezierCurves->at(_focusOnCurve)->_lastX != _tLeftDomain || (float)_bezierCurves->at(_focusOnCurve)->_lastY != _tRightDomain) 
	{
		_bezierCurves->at(_focusOnCurve)->Update((float)_tLeftDomain, (float)_tRightDomain, _nSegments);
	}

	// If the focused curve has stored a different t parameter then update it
	if ((float)_bezierCurves->at(_focusOnCurve)->_deCasteljauT != _deCasteljauT) 
	{
		// Update the DeCasteljau with the current paramenter
		_bezierCurves->at(_focusOnCurve)->UpdateDeCasteljau(_deCasteljauT);
	}

	// If the focused curve has stored a different tick value then update it
	if (_bezierCurves->at(_focusOnCurve)->_nTicks != _nTicks) 
	{
		// Update the ticks on the curve
		_bezierCurves->at(_focusOnCurve)->UpdateTicks(_nTicks);
	}

	// If the focused curve has stored a different s paramter then update the point
	if ((float)_bezierCurves->at(_focusOnCurve)->_lastS != _sParameter || 
		(float)_bezierCurves->at(_focusOnCurve)->_lastSLeft != _sLeftDomain ||
		(float)_bezierCurves->at(_focusOnCurve)->_lastSRight != _sRightDomain)
	{
		_bezierCurves->at(_focusOnCurve)->UpdateSPoint(_sParameter, _sLeftDomain, _sRightDomain);
	}
}

void Application::UpdateCurve()
{
	// If there is a curve to be updated
	if (_updateCurve > -1) 
	{
		// Update the bezier curve elements
		_bezierCurves->at(_updateCurve)->Update((float)_tLeftDomain, (float)_tRightDomain, _nSegments);
		_bezierCurves->at(_updateCurve)->UpdateDeCasteljau(_deCasteljauT);
		_bezierCurves->at(_updateCurve)->UpdateSPoint(_sParameter, _sLeftDomain, _sRightDomain);
		_bezierCurves->at(_updateCurve)->UpdateTicks(_nTicks);

		// Indicate the curve no longer requires to be updated
		_updateCurve = -1;
	}
}

void Application::WriteFile()
{
	// The filter for txt files
	const char * filePatterns[1] = { "*.txt" };

	// Get the file name
	const char * filename = tinyfd_saveFileDialog("Save Bezier curves", "beziercurves.txt", 1, filePatterns, NULL);

	// If a filename is given then store the curves
	if (filename) 
	{
		// The file object
		std::ofstream file;

		// Open the file
		file.open(filename);

		// Get the number of Bezier curves
		int nBezierCurves = _bezierCurves->size();

		// Write the number of bezier curves found
		file << "arcs <" << nBezierCurves << ">" << '\t' << "# number of arcs following" << '\n';

		// Traverse through the bezier curves and write down their control points
		for (int i = 0; i < nBezierCurves; i += 1)
		{
			// Get the reference to the current Bezier curve
			BezierCurve * bezierCurve = _bezierCurves->at(i);

			// Get the number of control points for the current Bezier curve
			int nControlPoints = bezierCurve->_controlPoints->size();

			// Write the degree of the current curve
			file << "arc <" << (nControlPoints - 1) << ">" << '\t' << "# degree = number of control points - 1" << '\n';

			// Traverse through the control points of the curve
			for (int j = 0; j < nControlPoints; j += 1)
			{
				// Get the reference to the current control point
				Point * p = bezierCurve->_controlPoints->at(j);

				// Write the current control point (only x and y)
				file << "<" << p->x << " " << p->y << ">" << '\t' << "# control point as two floats separated by blank" << '\n';
			}

			// Write the end of the arc
			file << "endarc" << '\t' << "# terminates arc description" << '\n';
		}

		// Close the file
		file.close();
	}
}

std::shared_ptr<Application> Application::GetInstance()
{
	// Instantiate the application if it hasn't
	if (_staticInstance == nullptr) 
	{
		_staticInstance = std::shared_ptr<Application>(new Application());
	}
	
	// Return the static pointer to the instance of the application
	return _staticInstance;
}

void Application::CursorPosCallback(GLFWwindow * window, double xpos, double ypos)
{
	// If the mouse is hovering any ImGui window then exit the function
	// NOTE: This is done for avoiding clicks in the UI to be considered as valid clicks for intereacting with the curve
	if (ImGui::IsMouseHoveringAnyWindow())
	{
		return;
	}

	// Get the static pointer to the instance of the application
	std::shared_ptr<Application> app = Application::GetInstance();

	// Update the mouse coordinates and set them to be within range [-1, 1] x [-1, 1]
	app->_mouseX = Utils::map(xpos, 0.0, app->_windowWidth, -1.0, 1.0);
	app->_mouseY = Utils::map(app->_windowHeight - ypos, 0.0, app->_windowHeight, -1.0, 1.0);

	// If the mouse is dragging then we know it is over a control point
	if (app->_mouseDragging)
	{
		// Get the reference to the dragged point
		Point * p = app->_bezierCurves->at(app->_focusOnCurve)->_controlPoints->at(app->_mouseOnControlPoint);

		// Update the point coordinates
		p->x = app->_mouseX;
		p->y = app->_mouseY;

		// Indicate the curve points have to be updated
		// NOTE: This is required since a control point has been dragged
		app->_updateCurve = app->_focusOnCurve;
	}
	else 
	{
		// Since the mouse is not dragging then check the control point on the focused Bezier curve where the mouse is at the moment
		app->_mouseOnControlPoint = app->MouseOnControlPoint();

		// If the mouse is over a control point then change the cursor shape
		if (app->_mouseOnControlPoint > -1)
		{
			// Change the mouse cursor to a hand shape
			glfwSetCursor(app->_window, app->_mouseHandCursor);
		}
		else
		{
			// Change the mouse cursor to the normal shape
			glfwSetCursor(app->_window, NULL);
		}
	}
}

void Application::ErrorCallback(int error, const char * description)
{
	std::cout << "Error: (" << error << ", " << description << ")" << std::endl;
}

void Application::MouseButtonCallback(GLFWwindow * window, int button, int actions, int mods)
{
	// If the mouse is hovering any ImGui window then exit the function
	// NOTE: This is done for avoiding clicks in the UI to be considered as valid clicks for intereacting with the curve
	if (ImGui::IsMouseHoveringAnyWindow()) 
	{
		return;
	}

	// Get the static pointer to the instance of the application
	std::shared_ptr<Application> app = Application::GetInstance();

	// If it is a left button press over a control point then indicate a dragging is taking effect
	if (button == GLFW_MOUSE_BUTTON_LEFT && actions == GLFW_PRESS) 
	{
		// If the mouse is on a control point then start indicating the mouse is dragging
		if (app->_mouseOnControlPoint > -1) 
		{
			app->_mouseDragging = true;
		}
	}

	// If it is a left button click then add a new control point
	else if (button == GLFW_MOUSE_BUTTON_LEFT && actions == GLFW_RELEASE)
	{
		// Check if the mouse is dragging
		if (app->_mouseDragging) 
		{
			// Indicate the mouse is no longer dragging
			app->_mouseDragging = false;
		}

		// If the mouse coordinates are within the orthogonal viewing domain then add a point
		else if(app->_mouseX >= app->_orthoLeft && app->_mouseX <= app->_orthoRight && app->_mouseY >= app->_orthoBottom && app->_mouseY <= app->_orthoTop)
		{
			// Since it was not dragging then add a new control point with the current mouse coordinates
			app->_bezierCurves->at(app->_focusOnCurve)->AddControlPoint(app->_mouseX, app->_mouseY);

			// Change the mouse cursor to a hand shape
			glfwSetCursor(app->_window, app->_mouseHandCursor);

			// Indicate the curve points have to be updated
			// NOTE: This is required since a new control point has been added
			app->_updateCurve = app->_focusOnCurve;
		}
	}

	// If it is the right button release click and it is within a control point then delete it
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && actions == GLFW_RELEASE)
	{
		// If the mouse is on any control point then delete it
		if (app->_mouseOnControlPoint > -1) 
		{
			// Delete the control point
			app->_bezierCurves->at(app->_focusOnCurve)->DeleteControlPoint(app->_mouseOnControlPoint);

			// Indicate the mouse is not over a control point
			app->_mouseOnControlPoint = -1;

			// Change the mouse cursor to the normal shape
			glfwSetCursor(app->_window, NULL);

			// Indicate the curve points have to be updated
			// NOTE: This is required since a control point has been deleted
			app->_updateCurve = app->_focusOnCurve;
		}
	}
}

void Application::WindowSizeCallback(GLFWwindow * window, int width, int height)
{
	// Get the static pointer to the instance of the application
	std::shared_ptr<Application> app = Application::GetInstance();

	// Update the window dimensions
	app->_windowWidth = width;
	app->_windowHeight = height;
	app->_halfWindowWidth = (double)width / 2.0;
	app->_halfWindowHeight = (double)height / 2.0;
}

int Application::Run(int argc, char ** argv)
{

	// Initialize glfw (exit if any error occur)
	if (!glfwInit()) 
	{
		return -1;
	}
#ifdef Linux
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif
	// Get the static pointer to the instance of the application
	std::shared_ptr<Application> app = Application::GetInstance();

	// Create a windowed mode window and its OpenGL context (exit if any error occur)
	app->_window = glfwCreateWindow((int)app->_windowWidth, (int)app->_windowHeight, "Bezier Curves", NULL, NULL);
	if (!app->_window)
	{
		glfwTerminate();
		return -1;
	}

	// Set the error callback
	glfwSetErrorCallback(ErrorCallback);

	// Make the window's context current
	glfwMakeContextCurrent(app->_window);

	// Set the window's callback functions
	glfwSetCursorPosCallback(app->_window, CursorPosCallback);
	glfwSetMouseButtonCallback(app->_window, MouseButtonCallback);
	glfwSetWindowSizeCallback(app->_window, WindowSizeCallback);
	glfwSetInputMode(app->_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Init gl3w
	gl3wInit();

	// Init ImGui for glfw and OpenGL 3
	// NOTE: Indicate not to install the callbacks; otherwise, the already defined glfw callbacks will be useless
	ImGui_ImplGlfwGL3_Init(app->_window, false);

	// Enable depth
	glEnable(GL_DEPTH);

	// Enable changing the point size
	glEnable(GL_PROGRAM_POINT_SIZE);

	// Enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Init the application objects
	app->Init();

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(app->_window))
	{
		// If the there is a curve to be updated then do it
		if (app->_updateCurve > -1) 
		{
			app->UpdateCurve();
		}

		// Update
		app->Update();

		// Draw the application content
		app->Draw();

		// Draw the user interface
		app->DrawUI();

		// Swap front and back buffers
		glfwSwapBuffers(app->_window);

		// Poll for and process events
		glfwPollEvents();
	}

	// Close the program
	ImGui_ImplGlfwGL3_Shutdown();
	glfwDestroyCursor(app->_mouseHandCursor);
	glfwTerminate();
	return 0;
}
