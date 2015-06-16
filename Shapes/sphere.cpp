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
Sphere::Sphere(Material material, Vec3Df origin, float radius) : Shape(material, origin), _radius(radius) {}

/**
* Intersection method, returns if collided, and which color.
*/
bool Sphere::intersection(const Vec3Df& origin, const Vec3Df& direction, Vec3Df& new_origin, Vec3Df& new_direction){
	//
	// See this for explantion of the formula: https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
	//

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
 * Shading method specific for sphere.
 */
Vec3Df Sphere::shade(const Vec3Df& camPos, const Vec3Df& intersect, const Vec3Df& lightPos, const Vec3Df& normal){
	if (!_material.has_tex())
		return Shape::shade(camPos, intersect, lightPos, normal);
	float u, v;
	Vec3Df mid = this->_origin;
	Vec3Df dir = intersect - mid;
	dir.normalize();
	u = 0.5 + (atan2(dir[2], dir[0])) / (2 * M_PI);
	v = 0.5 - asin(dir[1]) / M_PI;
	Vec3Df diffuse = this->_textureMap->getColor(u, v);
	this->_material.set_Kd(diffuse[0], diffuse[1], diffuse[2]);
	return Shape::shade(camPos, intersect, lightPos, normal);
}

/**
* Refraction method specific for sphere.
*/
Vec3Df Sphere::refract(const Vec3Df &normal, const Vec3Df &direction, const float &ni, float &fresnel) {
	return Shape::refract(normal, direction, ni, fresnel);
}

/**
* Draw function to view the plane in the viewport.
*/
void Sphere::draw() {
	glPushMatrix();

	glTranslatef(this->_origin[0], this->_origin[1], this->_origin[2]);
	//glColor3f(this->_material.Kd()[0], this->_material.Kd()[1], this->_material.Kd()[2]);
	glColor3f(1, 0, 0);
	glutSolidSphere(this->_radius, 20, 20);

	glPopMatrix();
}