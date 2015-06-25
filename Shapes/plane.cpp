#include "shape.h"
#include <GL/glut.h>

/**
 * SHAPE: Plane
 *
 * Each instance of this class is one plane. It is constructed via an origin and a coefficient.
 *
 *
 * Constructor
 */
Plane::Plane(Material &material, Vec3Df origin, Vec3Df coefficient) : Shape(material, origin), _coefficient(coefficient) {}

/**
* Intersection method, returns if collided, and which color.
*/
bool Plane::intersection(const Vec3Df& origin, const Vec3Df& direction, Vec3Df& newOrigin, Vec3Df& newDirection){
	//
	// See this for explanation: http://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
	//
	
	// Normalize the coefficient
	newDirection = _coefficient;
	newDirection.normalize();

	// Calculate the denominator
	float denominator = Vec3Df::dotProduct(direction, newDirection);
	if (denominator > -EPSILON && denominator < EPSILON)
		return false;

	// Solving for t.
	float t = Vec3Df::dotProduct(_origin - origin, newDirection) / denominator;

	// Check if t falls within the rounding margin.
	if (t < EPSILON)
		return false;
	
	// Calculate the new origin.
	newOrigin = origin + t * direction;

	return true;
}

/**
* Shading method specific for plane.
*/
Vec3Df Plane::shade(const Vec3Df& camPos, const Vec3Df& intersect, const Vec3Df& lightPos, const Vec3Df& normal){
	return Shape::shade(camPos, intersect, lightPos, normal);
}

/**
 * Draw function to view the plane in the viewport.
 */
void Plane::draw() {
	glPushMatrix();

	glTranslatef(this->_origin[0], this->_origin[1], this->_origin[2]);

	// A plane will be rendered green.
	glColor3f(0.f, 1.f, 0.f);
	glScalef(100.f, 0.001f, 100.f);
	glutSolidCube(1);

	glPopMatrix();
}