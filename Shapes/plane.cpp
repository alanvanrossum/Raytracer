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
Plane::Plane(Vec3Df color, Vec3Df origin, Vec3Df coefficient) : Shape(color, origin), _coefficient(coefficient) {}

/**
* Intersection method, returns if collided, and which color.
*/
bool Plane::intersection(const Vec3Df& origin, const Vec3Df& direction, Vec3Df& new_origin, Vec3Df& new_direction, Vec3Df& color){
	//
	// See this for explanation: https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
	//

	// Set the color.
	color = _color;
	
	// Normalize the coefficient
	new_direction = _coefficient;
	new_direction.normalize();

	// 
	float denom = Vec3Df::dotProduct(direction, new_direction);
	if (denom > -EPSILON && denom < EPSILON)
		return false;

	// Calculate term t in the expressen 'p = o + tD'
	float t = Vec3Df::dotProduct(_origin - origin, new_direction) / denom;
	if (t < EPSILON)
		return false;
	
	// Calculate the new origin.
	new_origin = origin + t * direction;

	return true;
}

/**
 * Draw function to view the plane in the viewport.
 */
void Plane::draw() {
	glPushMatrix();

	glTranslatef(this->_origin[0], this->_origin[1], this->_origin[2]);
	glColor3f(this->_color[0], this->_color[1], this->_color[2]);
	glScalef(10, 0.4, 10);
	glutSolidCube(1);

	glPopMatrix();
}