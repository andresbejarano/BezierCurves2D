<div style="text-align:center;">
  <img src="https://github.com/andresbejarano/beziercurves2d/blob/master/images/img0.png" width="200" />
</div>

# 2D Bezier Curves
A simple project for implementing 2D Bezier Curves and related concepts.
* It is built using C++.
* Rendering is performed using OpenGL, shaders, gl3w and glfw3.
* User interface is implemented using [imgui](https://github.com/ocornut/imgui).

## Implemented Operations
Related concepts implemented in the project include:
* Raising the degree of the curve.
* Remapping the parameterization range from 0-1 to any x-y.
* Extending the curve for t<0 and t>1.
* Curve subdivision at given parameter t (generating two independent Bezier Curves).

