#ifndef SHAPES_header
#define SHAPES_header

#include "../Vec3D.h"
#include "../mesh.h"
#include "../material.h"
#include "../texture.h"
#include "../image.h"

// EPSILON -> Used for rounding errors. (Margin)
static const float EPSILON = 1e-4f;

/**
 * Shape class. All shapes inherit from this class.
 */
class Shape {
	public:
		// Constructor
		Shape(Material& material, Vec3Df origin);

		/**
		 * Method to check if a ray intersects with this shape.
		 *
		 * 1st param:	Origin of the ray
		 * 2nd param:	Direction of the ray
		 * 3rd param:	New origin of the intersected point
		 * 4th param:	New direction at the intersected point
		 * Return:		True if the ray has intersected with this object. False otherwise
		 */
		virtual bool intersection(const Vec3Df&, const Vec3Df&, Vec3Df&, Vec3Df&) = 0;

		/**
		* Shade the shape using specular, diffuse and ambient components of the Material.
		*
		* 1st param:	The camera position
		* 2nd param:	The point of intersection with this object and the ray.
		* 3rd param:	The position of the light.
		* 4th param:	The normal at the point of intersection.
		* Return		The color of this intersection point.
		*/
		virtual Vec3Df shade(const Vec3Df&, const Vec3Df&, const Vec3Df&, const Vec3Df&) = 0;

		/**
		 * Return reference to this.
		 */
		virtual Shape* getIntersectedShape() = 0;

		/**
		 * Getters and Setters for materials.
		 */
		virtual bool hasMaterial() { return true; }
		virtual Material& getMaterial() { return _material; }
		bool hasTexture() { return textureMapSet; }
		void setTexture(Texture* textureMap) { textureMapSet = true; _textureMap = textureMap; }
		void setNormalMap(Texture* normalMap) { normalMapSet = true; _normalMap = normalMap; }

		// Draw function
		// Used so we can see our shape in the viewport. -> Not used for raytracing
		virtual void draw() = 0;

		// Variables
		const Vec3Df _origin;
		Material &_material;
		bool textureMapSet;
		Texture* _textureMap;
		bool normalMapSet;
		Texture* _normalMap;
};

/**
* Plane
*/
class Plane : public Shape {
	public:
		// Constructor
		Plane(Material& material, Vec3Df origin, Vec3Df coefficient);

		// Inherited methods.
		virtual bool intersection(const Vec3Df&, const Vec3Df&, Vec3Df&, Vec3Df&);
		virtual Vec3Df shade(const Vec3Df&, const Vec3Df&, const Vec3Df&, const Vec3Df&);

		virtual Shape* getIntersectedShape() { return  this; }

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
		Sphere(Material& material, Vec3Df origin, float radius);

		// Inherited methods.
		virtual bool intersection(const Vec3Df&, const Vec3Df&, Vec3Df&, Vec3Df&);
		virtual Vec3Df shade(const Vec3Df&, const Vec3Df&, const Vec3Df&, const Vec3Df&);

		virtual Shape* getIntersectedShape() { return  this; }

		// Draw method
		virtual void draw();

		// Variables
		const float _radius;
};

/**
 * Triangle
 */
class TriangleShape : public Shape {
	public:
		// Constructor
		TriangleShape(Material& material, Triangle &triangle);

		// Inherited methods.
		virtual bool intersection(const Vec3Df&, const Vec3Df&, Vec3Df&, Vec3Df&);
		virtual Vec3Df shade(const Vec3Df&, const Vec3Df&, const Vec3Df&, const Vec3Df&);

		virtual Shape* getIntersectedShape() { return  this; }

		// Draw method
		virtual void draw();

		// Triangle
		Triangle &_triangle;
};

/**
 * Mesh
 */
class MyMesh : public Shape {
public:
	// Contructor
	MyMesh(Mesh mesh, Vec3Df origin);

	// Inherited methods.
	virtual bool intersection(const Vec3Df&, const Vec3Df&, Vec3Df&, Vec3Df&);
	virtual bool intersection(const Triangle &triangle, const Vec3Df&, const Vec3Df&, Vec3Df&, Vec3Df&);
	virtual Vec3Df shade(const Vec3Df&, const Vec3Df&, const Vec3Df&, const Vec3Df&);

	virtual Shape* getIntersectedShape() { return  _lastIntersectedTriangle; }

	// Methods special to this class
	void barycentric(const Triangle &, const Vec3Df &, float &, float &);

	// Draw method
	virtual void draw();

	// Variables

	// Last intersected triangle
	TriangleShape *_lastIntersectedTriangle;
	
	// Pointer to the mesh.
	Mesh _mesh;
};

#endif // SHAPES_header