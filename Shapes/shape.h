#ifndef SHAPES_header
#define SHAPES_header

#include "../Vec3D.h"
#include "../mesh.h"
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
		 * 1st param:	Origin of the ray
		 * 2nd param:	Direction of the ray
		 * 3rd param:	New origin of the intersected point
		 * 4th param:	New direction at the intersected point
		 * Return:		Wheter the ray has intersected with this object.
		 */
		virtual bool intersection(const Vec3Df&, const Vec3Df&, Vec3Df&, Vec3Df&) = 0;

		/**
		* Shade the shape using specular, diffuse and ambient terms of the Material.
		* 1st param:	The camera position
		* 2nd param:	The point of intersection with this object and the ray.
		* 3rd param:	The position of the light.
		* 4th param:	The normal at the point of intersection.
		* Return		The color of this intersection point.
		*/
		virtual Vec3Df shade(const Vec3Df&, const Vec3Df&, const Vec3Df&, const Vec3Df&) = 0;

		/**
		* Calculate the refraction vector. For simplicity, all vectors must be normalized.
		* 1st param:	The normal at the point of intersection.
		* 2nd param:	The direction of the view vector.
		* 3rd param:	The other refraction index.
		* 4th param:	The return address for the fresnel value.
		* Return		The color of this intersection point.
		*/
		virtual Vec3Df refract(const Vec3Df&, const Vec3Df&, const float&, float&) = 0;

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
		Plane(Material material, Vec3Df origin, Vec3Df coefficient);

		// Inherited methods.
		virtual bool intersection(const Vec3Df&, const Vec3Df&, Vec3Df&, Vec3Df&);
		virtual Vec3Df shade(const Vec3Df&, const Vec3Df&, const Vec3Df&, const Vec3Df&);
		virtual Vec3Df refract(const Vec3Df&, const Vec3Df&, const float&, float&);

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
		Sphere(Material material, Vec3Df origin, float radius);

		// Inherited methods.
		virtual bool intersection(const Vec3Df&, const Vec3Df&, Vec3Df&, Vec3Df&);
		virtual Vec3Df shade(const Vec3Df&, const Vec3Df&, const Vec3Df&, const Vec3Df&);
		virtual Vec3Df refract(const Vec3Df&, const Vec3Df&, const float&, float&);

		virtual Shape* getIntersectedShape() { return  this; }

		// Draw method
		virtual void draw();

		// Variables
		const float _radius;
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
	virtual Vec3Df refract(const Vec3Df&, const Vec3Df&, const float&, float&);

	virtual Shape* getIntersectedShape() { return  this; }

	// Methods special to this class
	void barycentric(const Triangle &triangle, const Vec3Df &p, float &a, float &b);

	// Draw method
	virtual void draw();

	// Variables
	
	// Pointer to the mesh.
	Mesh _mesh;
};

#endif // SHAPES_header