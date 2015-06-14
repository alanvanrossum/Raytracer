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
		Shape(Vec3Df color, Vec3Df origin);

		/**
		 * Method to check if a ray intersects with this shape.
		 * 1st param:	Origin of the ray
		 * 2nd param:	Direction of the ray
		 * 3rd param:	New origin of the intersected point
		 * 4rd param:	New direction at the intersected point
		 * 5rd param:	Color of intersected point
		 * Return:		Wheter the ray has intersected with this object.
		 */
		virtual bool intersection(const Vec3Df&, const Vec3Df&, Vec3Df&, Vec3Df&, Vec3Df&) = 0;

		// Draw function
		// Used so we can see our shape in the viewport. -> Not used for raytracing
		virtual void draw() = 0;

		// Variables
		const Vec3Df _origin;
		const Vec3Df _color;
};

/**
* Plane
*/
class Plane : public Shape {
	public:
		// Constructor
		Plane(Vec3Df color, Vec3Df origin, Vec3Df coefficient);

		// Inherited methods.
		virtual bool intersection(const Vec3Df&, const Vec3Df&, Vec3Df&, Vec3Df&, Vec3Df&);

		// Draw method
		virtual void draw();

		// Variables
		const Vec3Df _coefficient;
};

/**
 * Sphere
 */
class Sphere : public Shape {
	public:
		// Constructor
		Sphere(Vec3Df color, Vec3Df origin, float radius);

		// Inherited methods.
		virtual bool intersection(const Vec3Df&, const Vec3Df&, Vec3Df&, Vec3Df&, Vec3Df&);

		// Draw method
		virtual void draw();

		// Variables
		const Vec3Df _radius;
};

#endif // SHAPES_header