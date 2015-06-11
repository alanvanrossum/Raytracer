#ifndef SHAPES_header
#define SHAPES_header

#include "../Vec3D.h"
#include "../mesh.h"

/**
 * Shape class. All shapes inherit from this class.
 */
class Shape {
	public:
		/**
		 * Method to check if a ray intersects with this shape.
		 * 1st param:	Origin of the ray
		 * 2nd param:	Direction of the ray
		 * 3rd param:	Point of intersection
		 * 4th param:	Normal at the point of intersection.
		 * Return:		Wheter the ray has intersected with this object.
		 */
		virtual bool intersection(const Vec3Df&, const Vec3Df&, Vec3Df&, Vec3Df&) = 0;
};

/**
* Triangle
* 
* Uses Triangle from mesh.h
*/
class MyTriangle : public Shape {
public:
	// Constructor
	MyTriangle(Mesh *mesh, Triangle *triangle);

	// Inherited methods.
	virtual bool intersection(const Vec3Df&, const Vec3Df&, Vec3Df&, Vec3Df&) = 0;
};

/**
* Plane
*/
class Plane : public Shape {
public:
	// Constructor
	Plane(Vec3Df origin, float radius);

	// Inherited methods.
	virtual bool intersection(const Vec3Df&, const Vec3Df&, Vec3Df&, Vec3Df&) = 0;
};

/**
 * Sphere
 */
class Sphere : public Shape {
	public:
		// Constructor
		Sphere(Vec3Df origin, float coefficient);

		// Inherited methods.
		virtual bool intersection(const Vec3Df&, const Vec3Df&, Vec3Df&, Vec3Df&) = 0;
};

#endif // SHAPES_header