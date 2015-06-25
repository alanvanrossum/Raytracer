#include "shape.h"
#include <GL/glut.h>

/**
 * SHAPE: Mesh
 *
 * Each instance of this class is one plane. It is constructed via an origin and a coefficient.
 *
 *
 * Constructor
 */
MyMesh::MyMesh(Mesh mesh, Vec3Df origin) : Shape(mesh.materials[0], origin), _mesh(mesh) {}

/**
 * Intersection method for the whole mesh.
 */
bool MyMesh::intersection(const Vec3Df& origin, const Vec3Df& direction, Vec3Df& new_origin, Vec3Df& new_direction){
	//
	// See this for explanation: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
	//

	// Closest triangle
	float ClosestTriangle = FLT_MAX;

	// Tmp values
	Vec3Df tmp_new_origin, tmp_new_direction;

	// Has intersected
	bool hasIntersected = false;

	for (size_t i = 0; i < _mesh.triangles.size(); i++) {
		if (intersection(_mesh.triangles[i], origin, direction, tmp_new_origin, tmp_new_direction)) {
			if ((tmp_new_origin - origin).getLength() < ClosestTriangle) {
				ClosestTriangle = (tmp_new_origin - origin).getLength();
				new_origin = tmp_new_origin;
				new_direction = tmp_new_direction;
				hasIntersected = true;

				TriangleShape* triangleShape = new TriangleShape(_mesh.materials[_mesh.triangleMaterials[i]], _mesh.triangles[i]);
				// New last intersected triangle.
				_lastIntersectedTriangle = triangleShape;
			}
		}
	}

	return hasIntersected;
}

/**
* Intersection method for a single triangle
*/
bool MyMesh::intersection(const Triangle& triangle, const Vec3Df& origin, const Vec3Df& direction, Vec3Df& new_origin, Vec3Df& new_direction){
	//
	// See this for explanation: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
	//

	Vec3Df u = (_mesh.vertices[triangle.v[1]].p + _origin) - (_mesh.vertices[triangle.v[0]].p + _origin);
	Vec3Df v = (_mesh.vertices[triangle.v[2]].p + _origin) - (_mesh.vertices[triangle.v[0]].p + _origin);

	/**
	 * First calculate where the ray intersects the plane in which the triangle lies
	 */
	// Calculate the normal of the plane
	Vec3Df planeNormal = Vec3Df::crossProduct(u, v);

	// Calculate the angle of the ray relative to the plane
	float denom = Vec3Df::dotProduct(direction, planeNormal);
	if (denom > -EPSILON && denom < EPSILON) return false;

	// Calculate term t in the expressen 'p = o + tD'
	float t = Vec3Df::dotProduct((_mesh.vertices[triangle.v[0]].p + _origin) - origin, planeNormal) / denom;
	if (t < EPSILON)
		return false;

	Vec3Df p = origin + t * direction;

	/**
	 * Second determine barycentric coordinates using Cramer's rule
	 */
	float a, b;
	barycentric(triangle, p, a, b);
	if (a < -EPSILON || b < -EPSILON || a + b > 1)
		return false;

	/**
	 * Third interpolate the vertex normals using barycentric coordinates
	 */
	new_direction = (1 - a - b) * (_mesh.vertices[triangle.v[0]].n + _origin) +
		a * (_mesh.vertices[triangle.v[1]].n + _origin) +
		b * (_mesh.vertices[triangle.v[2]].n + _origin);
	new_direction.normalize();
	new_origin = p;

	return true;
}

/**
 * Calculate the barycentric coordinates.
 */
void MyMesh::barycentric(const Triangle &triangle, const Vec3Df &p, float &a, float &b) {
	Vec3Df u = (_mesh.vertices[triangle.v[1]].p + _origin) - (_mesh.vertices[triangle.v[0]].p + _origin);
	Vec3Df v = (_mesh.vertices[triangle.v[2]].p + _origin) - (_mesh.vertices[triangle.v[0]].p + _origin);

	float d00 = Vec3Df::dotProduct(u, u);
	float d01 = Vec3Df::dotProduct(u, v);
	float d11 = Vec3Df::dotProduct(v, v);
	float d20 = Vec3Df::dotProduct(p - (_mesh.vertices[triangle.v[0]].p + _origin), u);
	float d21 = Vec3Df::dotProduct(p - (_mesh.vertices[triangle.v[0]].p + _origin), v);
	float invDenom = 1.f / (d00 * d11 - d01 * d01);

	a = (d11 * d20 - d01 * d21) * invDenom;
	b = (d00 * d21 - d01 * d20) * invDenom;
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