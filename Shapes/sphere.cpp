#include "shape.h"
#include <GL/glut.h>

#define M_PI 3.14159265358979323846

/**
* SHAPE: Sphere
*
* Each instance of this class is one sphere. It is constructed via an origin and a radius.
*
*
* Constructor
*/
Sphere::Sphere(Material &material, Vec3Df origin, float radius) : Shape(material, origin), _radius(radius) {}

/**
* Intersection method, returns if collided, and which color.
*/
bool Sphere::intersection(const Vec3Df& origin, const Vec3Df& direction, Vec3Df& newOrigin, Vec3Df& newDirection){
	//
	// See this for explantion of the formula: https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
	//

	Vec3Df transOrigin = origin - this->_origin;
	float a = Vec3Df::dotProduct(direction, direction);
	float b = 2 * Vec3Df::dotProduct(transOrigin, direction);
	float c = Vec3Df::dotProduct(transOrigin, transOrigin) - this->_radius * this->_radius;

	float disc = b * b - 4 * a * c;
	if (disc < 0)	return false;

	// Quadratic formula.
	float q = (b > 0.f) ? -0.5f * (b + sqrtf(disc)) : -0.5f * (b - sqrtf(disc));
	float t0 = q / a;
	float t1 = c / q;
	if (t0 < t1)
		std::swap(t0, t1);

	float t;
	if (t0 < EPSILON)
		return false;
	if (t1 < 0)
		t = t0;
	else
		t = t1;

	newDirection = transOrigin + t * direction;
	newDirection.normalize();
	newOrigin = origin + t * direction;

	return true;
}

/**
 * Shading method specific for sphere.
 */
Vec3Df Sphere::shade(const Vec3Df& camPos, const Vec3Df& intersect, const Vec3Df& lightPos, const Vec3Df& normal){

	// If it has a texture, map the current color.
	if (_material.has_tex()) {

		Vec3Df direction = intersect - this->_origin;
		direction.normalize();

		// Calculate the position of the UV point.
		float u = 0.5f + (atan2(direction[2], direction[0])) / (2.f * float(M_PI));
		float v = 0.5f - asin(direction[1]) / float(M_PI);

		// U || V can't be less than 0.
		if (u < 0)
			u = 0;
		if (v < 0)
			v = 0;

		// Set the diffuse color.
		Vec3Df diffuse = this->_textureMap->getColor(u, v);
		this->_material.set_Kd(diffuse[0], diffuse[1], diffuse[2]);

		return Shape::shade(camPos, intersect, lightPos, normal);
	}
	
	// If no texture, then go directly to the Phong shading function.
	else {
		return Shape::shade(camPos, intersect, lightPos, normal);
	}
}

/**
* Draw function to view the plane in the viewport.
*/
void Sphere::draw() {
	glPushMatrix();

	glTranslatef(this->_origin[0], this->_origin[1], this->_origin[2]);
	
	// Sphere's are drawn red.
	glColor3f(1, 0, 0);
	glutSolidSphere(this->_radius, 20, 20);

	glPopMatrix();
}