#include "shape.h"
#include <GL/glut.h>

/**
* SHAPE: Sphere
*
* Each instance of this class is one sphere. It is constructed via an origin and a radius.
*
*
* Constructor
*/
Sphere::Sphere(Vec3Df color, Vec3Df origin, float radius) : Shape(color, origin), _radius(radius) {}

/**
* Intersection method, returns if collided, and which color.
*/
bool Sphere::intersection(const Vec3Df& origin, const Vec3Df& direction, Vec3Df& new_origin, Vec3Df& new_direction, Vec3Df& color){
	//
	// See this for explantion of the formula: https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
	//

	// Set the color.
	color = _color;

	Vec3Df trans_origin = origin - this->_origin;
	float a = Vec3Df::dotProduct(direction, direction);
	float b = 2 * Vec3Df::dotProduct(trans_origin, direction);
	float c = Vec3Df::dotProduct(trans_origin, trans_origin) - this->_radius * this->_radius;

	float disc = b * b - 4 * a * c;
	if (disc < 0)	return false;

	// Quadratic formula.
	float q = (b > 0) ? -0.5 * (b + sqrtf(disc)) : -0.5 * (b - sqrtf(disc));
	float t0 = q / a;
	float t1 = c / q;
	if (t0 < t1) std::swap(t0, t1);

	float t;
	if (t0 < EPSILON)	return false;
	if (t1 < 0)		t = t0;
	else			t = t1;

	new_direction = trans_origin + t * direction;
	new_direction.normalize();
	new_origin = origin + t * direction;

	return true;
}

/**
* Draw function to view the plane in the viewport.
*/
void Sphere::draw() {
	glPushMatrix();

	glTranslatef(this->_origin[0], this->_origin[1], this->_origin[2]);
	glColor3f(this->_color[0], this->_color[1], this->_color[2]);
	glutSolidSphere(this->_radius, 20, 20);

	glPopMatrix();
}