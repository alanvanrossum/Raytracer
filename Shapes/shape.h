#ifndef SHAPES_header
#define SHAPES_header

#include "../Vec3D.h"
#include "../mesh.h"

// EPSILON -> Used for rounding errors. (Margin)
static const float EPSILON = 1e-4f;

/**
 * Shape class. All shapes inherit from this class.
 */
class Shape {
	public:
		// Constructor
		Shape(Vec3Df origin);

		/**
		 * Method to check if a ray intersects with this shape.
		 * 1st param:	Origin of the ray
		 * 2nd param:	Direction of the ray
		 * Return:		Wheter the ray has intersected with this object.
		 */
		virtual bool intersection(const Vec3Df&, const Vec3Df&) = 0;

		// Variables
		const Vec3Df _origin;
};

/**
* Plane
*/
class Plane : public Shape {
	public:
		// Constructor
		Plane(Vec3Df origin, float radius);

		// Inherited methods.
		virtual bool intersection(const Vec3Df&, const Vec3Df&) = 0;
};

/**
 * Sphere
 */
class Sphere : public Shape {
	public:
		// Constructor
		Sphere(Vec3Df origin, float coefficient);

		// Inherited methods.
		virtual bool intersection(const Vec3Df&, const Vec3Df&) = 0;
};

#endif // SHAPES_header