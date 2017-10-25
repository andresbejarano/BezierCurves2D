#pragma once

#ifndef _SHADER_SOURCES_H_
#define _SHADER_SOURCES_H_

namespace Shaders
{
	// Basic vertex shader code
	static const char * basicVertexShaderSource = R"glsl(
		#version 330

		in vec3 point;

		uniform mat4 modelMatrix;
		uniform mat4 viewMatrix;
		uniform mat4 projMatrix;

		void main()
		{
			gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(point, 1.0);
		}

	)glsl";


	// Basic fragment shader code
	static const char * basicFragmentShaderSource = R"glsl(
		#version 330
		
		uniform vec4 color;
		
		out vec4 outColor;

		void main()
		{
			outColor = color;
		}

	)glsl";

	// Vertex shader code for drawing the points
	static const char * pointsVertexShaderSource = R"glsl(
		#version 330

		in vec3 point;

		uniform mat4 modelMatrix;
		uniform mat4 viewMatrix;
		uniform mat4 projMatrix;

		void main()
		{
			gl_PointSize = 5;
			gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(point, 1.0);
		}

	)glsl";


	// Fragment shader code for drawing the points
	static const char * pointsFragmentShaderSource = R"glsl(
		#version 330

		uniform vec4 color;
		
		out vec4 outColor;

		void main()
		{
			outColor = color;
		}

	)glsl";

}

#endif 
