#include <stdio.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include "raytracing.h"
#include "main.h"
#include "Shapes\shape.h"

/**
 * VARIABLES
 */
std::vector<Shape*> shapes;

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
 * - 
 */
void init()
{
	//load the mesh file
	//please realize that not all OBJ files will successfully load.
	//Nonetheless, if they come from Blender, they should, if they 
	//are exported as WavefrontOBJ.
	//PLEASE ADAPT THE LINE BELOW TO THE FULL PATH OF THE dodgeColorTest.obj
	//model, e.g., "C:/temp/myData/GraphicsIsFun/dodgeColorTest.obj", 
	//otherwise the application will not load properly
    //MyMesh.loadMesh("H:/Development/graphics/Project2015/git/cube.obj", true);
	//MyMesh.computeVertexNormals();


	/**
	 * Shapes
	 */
	// Draw a single red plane.
	shapes.push_back(new Plane(Vec3Df(1, 0, 0), Vec3Df(0, -1, 0), Vec3Df(0, 1, 0)));

	/**
	 * Lights
	 */
	// One light at the starting camera position.
	MyLightPositions.push_back(MyCameraPosition);
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
	// Usefull when we're going to do refraction and reflection.
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
	// If we are out of bounces, return black.
	if (level == max)
		return Vec3Df(0, 0, 0);

	// Color of the point.
	Vec3Df color;

	// The maximum depth.
	float current_depth = FLT_MAX;

	// True if a collision occured.
	bool intersected = false;

	// The new origin at the intersected point
	Vec3Df new_origin;

	// The normal at the intersected point
	Vec3Df new_direction;

	// Loop over all objects in the scene.
	for (int i = 0; i < shapes.size(); i++) {

		// Temp variables for the intersected function.
		Vec3Df tmp_origin;
		Vec3Df tmp_direction;		

		if (shapes[i]->intersection(origin, direction, tmp_origin, tmp_direction, color)) {
			// Check if the new depth is closer to the camera.
			float depth = (new_origin - origin).getLength();
			if (depth < current_depth) {

				// We have intersected and are closer to the origin. This is the new next object to raytrace.
				intersected = true;
				current_depth = depth;
				new_origin = tmp_origin;
				new_direction = tmp_direction;				
			}
		}
	}

	// If no intersection happend, return black. (Color at infinity)
	if (!intersected)
		return Vec3Df(0.f, 0.f, 0.f);

	// Return the color of the intersected point.
	return color;
}




/**
 * Debug function to draw things in real time
 */
void yourDebugDraw()
{
	//draw open gl debug stuff
	//this function is called every frame

	//let's draw the mesh
	//MyMesh.draw();

	// Draw all the shapes for the viewport window.
	for (int i = 0; i < shapes.size(); i++) {
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
