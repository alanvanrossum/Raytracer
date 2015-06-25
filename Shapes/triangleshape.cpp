#include "shape.h"

/**
* SHAPE: TriangleShape
*
* Each instance of this class is one TriangleShape.
*
*
* Constructor
*/
TriangleShape::TriangleShape(Material &material, Triangle &triangle) : Shape(material, Vec3Df(0,0,0)), _triangle(triangle) {}

/**
* Intersection method, returns if collided, and which color.
*/
bool TriangleShape::intersection(const Vec3Df& origin, const Vec3Df& direction, Vec3Df& new_origin, Vec3Df& new_direction){
	return false;
}

/**
* Shading method specific for sphere.
*/
Vec3Df TriangleShape::shade(const Vec3Df& camPos, const Vec3Df& intersect, const Vec3Df& lightPos, const Vec3Df& normal){
	return Shape::shade(camPos, intersect, lightPos, normal);
}

/**
* Draw function to view the plane in the viewport.
*/
void TriangleShape::draw() {
	// No need to draw a single triangle.
}