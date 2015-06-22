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
bool Plane::intersection(const Vec3Df& origin, const Vec3Df& direction, Vec3Df& new_origin, Vec3Df& new_direction){
	//
	// See this for explanation: https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
	//
	
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
* Shading method specific for plane.
*/
Vec3Df Plane::shade(const Vec3Df& camPos, const Vec3Df& intersect, const Vec3Df& lightPos, const Vec3Df& normal){
	return Shape::shade(camPos, intersect, lightPos, normal);
}

/**
* Refraction method specific for plane.
*/
Vec3Df Plane::refract(const Vec3Df &normal, const Vec3Df &direction, const float &ni, float &fresnel){
	return Shape::refract(normal, direction, ni, fresnel);
}

/**
 * Draw function to view the plane in the viewport.
 */
void Plane::draw() {
	glPushMatrix();

	glTranslatef(this->_origin[0], this->_origin[1], this->_origin[2]);
	//glColor3f(this->_material.Kd()[0], this->_material.Kd()[1], this->_material.Kd()[2]);
	glColor3f(0.f, 1.f, 0.f);
	glScalef(100.f, 0.001f, 100.f);
	glutSolidCube(1);

	glPopMatrix();
}