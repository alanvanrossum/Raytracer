#include <stdio.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include "raytracing.h"
#include "main.h"
#include "Shapes\shape.h"
#include "image.h"

/**
 * VARIABLES
 */
std::vector<Shape*> shapes;
std::vector<Material> materials;

// Global variables to draw a debug ray trace.
Vec3Df testRayOrigin;
Vec3Df testRayDestination;

/**
 * INIT
 *
 * Initialize the scene here.
 * 
 * - Load meshes/triangles/cubes/spheres or whatever.
 * - Load lightsources
 * - Load materials
 */
void init()
{
	/**
	 * Meshes
	 */
	// The mesh
	Mesh testMesh;
		
	// Load the cornellbox.
	testMesh.loadMesh("git/Meshes/cornellBox/cornellBoxMirrorTriangulated.obj", true);
	testMesh.computeVertexNormals();

	Shape* cube = new MyMesh(testMesh, Vec3Df(0.f, -1.f, 1.f));
	shapes.push_back(cube);

	/**
	 * Materials
	 */
	Material moonmat;
	moonmat.set_Kd(0.2f, 0.f, 0.f);		// Diffuse
	moonmat.set_Ks(0.2f, 0.2f, 0.2f);	// Specular
	moonmat.set_textureName("git/Meshes/Textures/moon.ppm");
	Image moon_img("git/Meshes/Textures/moon.ppm");
	Texture* moon_tex = new Texture(moon_img);
	materials.push_back(moonmat);

	Material plane_mat;
	plane_mat.set_Ka(0.0f,0.0f,0.0f);
	plane_mat.set_Kd(0.f, 0.f, 0.f);
	plane_mat.set_Ks(1.f, 1.f, 1.f);
	materials.push_back(plane_mat);

	/**
	 * Shapes
	 */
	// The moon
	Shape* moon = new Sphere(materials[0], Vec3Df(0.35f, -0.15f, 1.35f), .25f);
	moon->setTexture(moon_tex);
	shapes.push_back(moon);

	// The mirroring sphere back
	Shape* mirrorSphere = new Sphere(materials[1], Vec3Df(-0.3f, 0.45f, 0.7f), .25f);
	shapes.push_back(mirrorSphere);

	// The mirroring sphere front
	Shape* mirrorSphere1 = new Sphere(materials[1], Vec3Df(-0.5f, -.75f, 1.4f), .25f);
	shapes.push_back(mirrorSphere1);

	// Test plane.
	//shapes.push_back(new Plane(materials[1], Vec3Df(0.f, -.5f, 0.f), Vec3Df(0.f, 1.f, 0.f)));


	/**
	 * Lights
	 */
	// One light at the starting camera position and one at the top of the cornellbox.
	MyLightPositions.push_back(MyCameraPosition);
	MyLightPositions.push_back(Vec3Df(0.f, .90f, .9f));
}

/**
* Ray Tracing
*
* This function uses an origin and destination.
*
* It will return a ray tracing function which uses origin and direction.
*/
Vec3Df performRayTracing(const Vec3Df & origin, const Vec3Df & destination)
{
	// Perform ray tracing with a origin and a direction instead of a origin and a destination.
	// Usefull when we're going to do reflections.
	Vec3Df direction = destination - origin;

	// Normalize the direction.
	direction.normalize();

	// Return the ray tracing function which uses origin and direction.
	// Level start at 0, max 5.
	return performRayTracing(origin, direction, 0, 5);
}

/**
 * Ray Tracing
 * 
 * This function uses an origin and direction to calculate collisions.
 * 
 * It will return the color of the pixel.
 */
Vec3Df performRayTracing(const Vec3Df & origin, const Vec3Df & direction, unsigned char level, unsigned char max)
{
	// If we are out of bounces, return no color (black).
	if (level == max)
		return Vec3Df(0, 0, 0);

	// The maximum depth.
	float currentDepth = FLT_MAX;

	// True if a collision occured.
	bool hasIntersected = false;

	// The new origin at the intersected point.
	Vec3Df newOrigin;

	// The normal direction at the intersected point.
	Vec3Df newDirection;

	// The object which is intersected.
	Shape* intersectedShape = nullptr;

	// Loop over all objects in the scene.
	for (size_t i = 0; i < shapes.size(); i++) {

		// Temp variables for the intersected function.
		Vec3Df tmpOrigin;
		Vec3Df tmpDirection;

		if (shapes[i]->intersection(origin, direction, tmpOrigin, tmpDirection)) {
			// Check if the new depth is closer to the camera.
			float depth = (tmpOrigin - origin).getLength();
			if (depth < currentDepth) {

				// We have intersected and are closer to the origin. This is the new next object to raytrace.
				hasIntersected = true;
				currentDepth = depth;
				newOrigin = tmpOrigin;
				newDirection = tmpDirection;
				intersectedShape = shapes[i]->getIntersectedShape();
			}
		}
	}

	// If no intersection happend, return black. (Color at infinity)
	if (!hasIntersected)
		return Vec3Df(0.f, 0.f, 0.f);
	
	// The color of the intersected object for all lightsources.
	Vec3Df directColor = Vec3Df(0.f, 0.f, 0.f);

	// Start without a reflected color.
	Vec3Df reflectedColor = Vec3Df(0.f, 0.f, 0.f);


	// If the object has a mateiral we can try to find reflections.
	if (intersectedShape->hasMaterial()) {

		// The object can reflect if it has a specular component. Ks.
		if (intersectedShape->getMaterial().has_Ks()) {
			Vec3Df reflect = direction - 2.f * Vec3Df::dotProduct(direction, newDirection) * newDirection;

			// Raytrace the reflected color.
			reflectedColor = performRayTracing(newOrigin, reflect, level + 1, max) * intersectedShape->getMaterial().Ks();
		}
	}

	// Shadows. Loop over all light sources and check if an intersected point is in visible from a light source.
	for (unsigned int i = 0; i < MyLightPositions.size(); i++) {
		
		// Direction from the light source to the intersected point
		Vec3Df lightDir = MyLightPositions[i] - newOrigin;

		// Distance between intersected point and light source
		float lightDist = lightDir.getLength();

		// Bool which is true if it intersects and is the closest object to the light source.
		bool intersection = false;

		// Loop over shapes. Check if there is an object between the intersected point and the light source.
		for (unsigned int j = 0; j < shapes.size(); j++) {

			Vec3Df intPoint, unused;
			
			// Check if there's an intersection between the intersected point and the light source
			if (shapes[j]->intersection(newOrigin, lightDir, intPoint, unused) && (intPoint - newOrigin).getLength() < lightDist) {
				intersection = true;
			}
		}

		// There was no intersection. Shade directly.
		if (!intersection)
			directColor += intersectedShape->shade(origin, newOrigin, MyLightPositions[i], newDirection);
	}

	// Divide the color by the number of lightsources.
	directColor /= float(MyLightPositions.size());

	// Return the direct color and the reflectedcolor.
	return directColor + reflectedColor;
}

/**
 * Debug function to draw things in real time
 */
void yourDebugDraw()
{
	//draw open gl debug stuff
	//this function is called every frame

	//let's draw the mesh
	//testMesh.draw();

	// Draw all the shapes for the viewport window.
	for (size_t i = 0; i < shapes.size(); i++) {
		shapes[i]->draw();
	}
	
	//let's draw the lights in the scene as points
	glPushAttrib(GL_ALL_ATTRIB_BITS); //store all GL attributes
	glDisable(GL_LIGHTING);
	glColor3f(1,1,1);
	glPointSize(10);
	glBegin(GL_POINTS);
	for (unsigned int i=0;i<MyLightPositions.size();++i)
		glVertex3fv(MyLightPositions[i].pointer());
	glEnd();
	glPopAttrib();//restore all GL attributes
	//The Attrib commands maintain the state. 
	//e.g., even though inside the two calls, we set
	//the color to white, it will be reset to the previous 
	//state after the pop.


	//as an example: we draw the test ray, which is set by the keyboard function
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(0,1,1);
	glVertex3f(testRayOrigin[0], testRayOrigin[1], testRayOrigin[2]);
	glColor3f(0,0,1);
	glVertex3f(testRayDestination[0], testRayDestination[1], testRayDestination[2]);
	glEnd();
	glPointSize(10);
	glBegin(GL_POINTS);
	glVertex3fv(MyLightPositions[0].pointer());
	glEnd();
	glPopAttrib();
	
	//draw whatever else you want...
	////glutSolidSphere(1,10,10);
	////allows you to draw a sphere at the origin.
	////using a glTranslate, it can be shifted to whereever you want
	////if you produce a sphere renderer, this 
	////triangulated sphere is nice for the preview
}


//yourKeyboardFunc is used to deal with keyboard input.
//t is the character that was pressed
//x,y is the mouse position in pixels
//rayOrigin, rayDestination is the ray that is going in the view direction UNDERNEATH your mouse position.
//
//A few keys are already reserved: 
//'L' adds a light positioned at the camera location to the MyLightPositions vector
//'l' modifies the last added ligh t to the current 
//    camera position (by default, there is only one light, so move it with l)
//    ATTENTION These lights do NOT affect the real-time rendering. 
//    You should use them for the raytracing.
//'r' calls the function performRaytracing on EVERY pixel, using the correct associated ray. 
//    It then stores the result in an image "result.ppm".
//    Initially, this function is fast (performRaytracing simply returns 
//    the target of the ray - see the code above), but once you replaced 
//    this function and raytracing is in place, it might take a 
//    while to complete...
void yourKeyboardFunc(char t, int x, int y, const Vec3Df & rayOrigin, const Vec3Df & rayDestination)
{

	//here, as an example, I use the ray to fill in the values for my upper global ray variable
	//I use these variables in the debugDraw function to draw the corresponding ray.
	//try it: Press a key, move the camera, see the ray that was launched as a line.
	testRayOrigin=rayOrigin;	
	testRayDestination=rayDestination;
	
	// do here, whatever you want with the keyboard input t.
	
	//...
	
	
	std::cout<<t<<" pressed! The mouse was in location "<<x<<","<<y<<"!"<<std::endl;	
}
