#ifndef MESH_H_sdfjlasdfjfsdfjljfasdf
#define MESH_H_sdfjlasdfjfsdfjljfasdf

#include "Vertex.h"
#include <vector>
#include <map>
#include <string>
#include "material.h"
#include "Vec3D.h"

/************************************************************
 * Triangle Class
 ************************************************************/
//A triangle contains 3 indices to refer to vertex positions 
//and 3 indices to refer to texture coordinates (optional)
class Triangle {
public:
    inline Triangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline Triangle (const Triangle & t2) {
        v[0] = t2.v[0];
        v[1] = t2.v[1];
        v[2] = t2.v[2];

        t[0] = t2.t[0];
        t[1] = t2.t[1];
        t[2] = t2.t[2];

    }
    inline Triangle (unsigned int v0, unsigned int t0, unsigned int v1, unsigned int t1, unsigned int v2, unsigned int t2) {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;

        t[0] = t0;
        t[1] = t1;
        t[2] = t2;
    }
    inline virtual ~Triangle () {}
    inline Triangle & operator= (const Triangle & t2) {
        v[0] = t2.v[0];
        v[1] = t2.v[1];
        v[2] = t2.v[2];
        t[0] = t2.v[0];
        t[1] = t2.v[1];
        t[2] = t2.v[2];
        return (*this);
    }

	// Vertex position 
	// Index of the vertices vector.
    unsigned int v[3];

	// Texture coordinate
	// Index of the texcoords vector.
    unsigned int t[3];
};

/************************************************************
 * Basic Mesh class
 ************************************************************/
class Mesh {
public:
	Mesh(){}
    inline Mesh (const std::vector<Vertex> & v, const std::vector<Triangle> & t) : vertices (v), triangles (t)  {}
    bool loadMesh(const char * filename, bool randomizeTriangulation);
	bool loadMtl(const char * filename, std::map<std::string, unsigned int> & materialIndex);
    void computeVertexNormals ();
    void draw();
	void draw(Vec3Df);
    void drawSmooth();

	// Vertices are the vertex positions, and normals of the mesh.
	std::vector<Vertex> vertices;

	// TexCoords are the texture coordinates, these are DIFFERENT indices in triangles.
	// in the current version, if you use textures, then you have to use texture coords everywhere
	// for convenience, Vec3Df is used, although only 2D tex coordinates are stored (x,y entry of the Vec3Df).
	std::vector<Vec3Df> texcoords;
	
	// Triangles are the indices of the vertices involved in a triangle.
	// A triangle, thus, contains a triplet of values corresponding to the 3 vertices of a triangle. 
    std::vector<Triangle> triangles;
	
	// These are the material properties
	// each triangle (!), NOT (!) each vertex, has a material. 
	// Use the triangle index to receive a material INDEX
	std::vector<unsigned int> triangleMaterials;
	
	// using the material index, you can then recover the material from this vector
	// the class material is defined just above
	std::vector<Material> materials;
	
	// As an example:
	// triangle triangles[i] has material index triangleMaterials[i]
	// and uses Material materials[triangleMaterials[i]].
};

#endif // MESH_H
