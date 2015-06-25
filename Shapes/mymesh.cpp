#include "shape.h"
#include <GL/glut.h>

/**
 * SHAPE: Mesh
 *
 * Each instance of this class is one mesh.
 *
 * Constructor
 */
MyMesh::MyMesh(Mesh mesh, Vec3Df origin) : Shape(mesh.materials[0], origin), _mesh(mesh) {}

/**
 * Intersection method for the whole mesh.
 */
bool MyMesh::intersection(const Vec3Df& origin, const Vec3Df& direction, Vec3Df& newOrigin, Vec3Df& newDirection){
	
	// Closest triangle
	float closestTriangle = FLT_MAX;

	// Tmp values
	Vec3Df tmpNewOrigin, tmpNewDirection;

	// Has intersected, true if it did.
	bool hasIntersected = false;

	int lastTriangle = -1;

	// Loop over all the triangles in the mesh.
	for (size_t i = 0; i < _mesh.triangles.size(); i++) {

		// Check if the ray intersected with this triangle.
		if (intersection(_mesh.triangles[i], origin, direction, tmpNewOrigin, tmpNewDirection)) {

			// Check if this triangle is closer to the origin than the other triangles (if exists).
			if ((tmpNewOrigin - origin).getLength() < closestTriangle) {

				// New closest triangle found!
				// Set the return values.
				closestTriangle = (tmpNewOrigin - origin).getLength();
				newOrigin = tmpNewOrigin;
				newDirection = tmpNewDirection;
				hasIntersected = true;

				lastTriangle = i;
			}
		}
	}

	if (lastTriangle != -1) {
		// Create a new last intersected triangle.
		_lastIntersectedTriangle = new TriangleShape(_mesh.materials[_mesh.triangleMaterials[lastTriangle]], _mesh.triangles[lastTriangle]);
	}

	return hasIntersected;
}

/**
* Intersection method for a single triangle
*/
bool MyMesh::intersection(const Triangle& triangle, const Vec3Df& origin, const Vec3Df& direction, Vec3Df& newOrigin, Vec3Df& newDirection){
	//
	// See this for explanation: http://geomalgorithms.com/a06-_intersect-2.html
	//

	Vec3Df u = (_mesh.vertices[triangle.v[1]].p + _origin) - (_mesh.vertices[triangle.v[0]].p + _origin);
	Vec3Df v = (_mesh.vertices[triangle.v[2]].p + _origin) - (_mesh.vertices[triangle.v[0]].p + _origin);

	/**
	 * Calculate where the ray hits the plane in which the triangle lies.
	 */
	// Calculate the normal of the plane.
	Vec3Df planeNormal = Vec3Df::crossProduct(u, v);

	// Calculate the angle of the ray relative to the plane.
	float denominator = Vec3Df::dotProduct(direction, planeNormal);

	// Check if the denominator falls within the rounding margin.
	if (denominator > -EPSILON && denominator < EPSILON)
		return false;

	// Calculate the distance from the origin.
	float t = Vec3Df::dotProduct((_mesh.vertices[triangle.v[0]].p + _origin) - origin, planeNormal) / denominator;

	// Check if t falls within the rounding margin.
	if (t < EPSILON)
		return false;

	Vec3Df p = origin + t * direction;

	/**
	 * Calculate the barycentric coordinates.
	 */
	float a, b;
	barycentric(triangle, p, a, b);
	if (a < -EPSILON || b < -EPSILON || a + b > 1)
		return false;

	/**
	 * Interpolate the vertex normals using barycentric coordinates
	 */
	newDirection = (1 - a - b) * (_mesh.vertices[triangle.v[0]].n + _origin) + a * (_mesh.vertices[triangle.v[1]].n + _origin) + b * (_mesh.vertices[triangle.v[2]].n + _origin);
	newDirection.normalize();
	newOrigin = p;

	return true;
}

/**
 * Calculate the barycentric coordinates.
 * 
 * See: http://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
 *		-> Second post.
 */
void MyMesh::barycentric(const Triangle &triangle, const Vec3Df &p, float &u, float &v) {
	Vec3Df v0 = (_mesh.vertices[triangle.v[1]].p + _origin) - (_mesh.vertices[triangle.v[0]].p + _origin);
	Vec3Df v1 = (_mesh.vertices[triangle.v[2]].p + _origin) - (_mesh.vertices[triangle.v[0]].p + _origin);
	Vec3Df v2 = p - (_mesh.vertices[triangle.v[0]].p + _origin);


	float d00 = Vec3Df::dotProduct(v0, v0);
	float d01 = Vec3Df::dotProduct(v0, v1);
	float d11 = Vec3Df::dotProduct(v1, v1);
	float d20 = Vec3Df::dotProduct(v2, v0);
	float d21 = Vec3Df::dotProduct(v2, v1);
	float denominator = (d00 * d11 - d01 * d01);

	u = (d11 * d20 - d01 * d21) / denominator;
	v = (d00 * d21 - d01 * d20) / denominator;
}

/**
 * Shading method specific for MyMesh.
 */
Vec3Df MyMesh::shade(const Vec3Df& camPos, const Vec3Df& intersect, const Vec3Df& lightPos, const Vec3Df& normal){
	return Shape::shade(camPos, intersect, lightPos, normal);
}

/**
 * Draw function to view the plane in the viewport.
 */
void MyMesh::draw() {
	_mesh.draw(_origin);
}