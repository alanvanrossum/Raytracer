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

Mesh testMesh;

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
	
	testMesh.loadMesh("git/Meshes/cornellBox/cornellBoxMirrorTriangulated.obj", true);
	testMesh.computeVertexNormals();

	Shape* cube = new MyMesh(testMesh, Vec3Df(0.f, -1.f, 1.f));
	shapes.push_back(cube);

	/**
	 * Materials
	 */
	Material earthmat;
	earthmat.set_Kd(0.2f, 0.f, 0.f);		// Diffuse
	earthmat.set_Ks(0.2f, 0.2f, 0.2f);		// Specular
	//earthmat.set_Ni(1.3f);					// Index of refraction
	//earthmat.set_Tr(0.5f);					// 
	earthmat.set_textureName("git/Meshes/Textures/moon.ppm");
	Image earth_img("git/Meshes/Textures/moon.ppm");
	Texture* earth_tex = new Texture(earth_img);
	materials.push_back(earthmat);

	Material plane_mat;
	plane_mat.set_Ka(0.0f,0.0f,0.0f);
	plane_mat.set_Kd(0.f, 0.f, 0.f);
	plane_mat.set_Ks(1.f, 1.f, 1.f);
	materials.push_back(plane_mat);

	// Glass material
	//Material glass;
	//glass.set_Ka(0.01f, 0.01f, 0.01f);
	//glass.set_Kd(0.01f, 0.01f, 0.01f);
	//glass.set_Ks(0.95f, 0.95f, 0.95f);
	//glass.set_illum(7);
	//glass.set_Ni(2.5f);
	//glass.set_Ns(200.f);
	//glass.set_Tf(0.1f, 0.1f, 0.1f);
	//glass.set_Tr(0.f);
	//materials.push_back(glass);


	/**
	 * Shapes
	 */
	Shape* earth = new Sphere(materials[0], Vec3Df(0.35f, -0.15f, 1.35f), .25f);
	//Shape* earth = new Sphere(materials[0], Vec3Df(0.f, 0.f, 0.f), .5f);
	earth->setTexture(earth_tex);
	shapes.push_back(earth);

	//Shape* glassSphere = new Sphere(materials[0], Vec3Df(0.35f, -0.15f, 1.35f), .25f);
	//shapes.push_back(glassSphere);

	Shape* mirrorSphere = new Sphere(materials[1], Vec3Df(-0.3f, 0.45f, 0.7f), .25f);
	shapes.push_back(mirrorSphere);

	Shape* mirrorSphere1 = new Sphere(materials[1], Vec3Df(-0.5f, -.75f, 1.4f), .25f);
	shapes.push_back(mirrorSphere1);


	//shapes.push_back(new Plane(materials[1], Vec3Df(0.f, -.5f, 0.f), Vec3Df(0.f, 1.f, 0.f)));


	/**
	 * Lights
	 */
	// One light at the starting camera position.
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
	// Usefull when we're going to do refraction and reflection.
	Vec3Df direction = destination - origin;

	// Normalize the direction.
	direction.normalize();

	// Return the ray tracing function which uses origin and direction.
	// Level start at 0, max 5.
	return performRayTracing(origin, direction, 0, 10);
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

	// The maximum depth.
	float current_depth = FLT_MAX;

	// True if a collision occured.
	bool hasIntersected = false;

	// The new origin at the intersected point
	Vec3Df new_origin;

	// The normal at the intersected point
	Vec3Df new_direction;

	// The object which is intersected.
	Shape* intersectedShape = nullptr;

	// Loop over all objects in the scene.
	for (size_t i = 0; i < shapes.size(); i++) {

		// Temp variables for the intersected function.
		Vec3Df tmp_origin;
		Vec3Df tmp_direction;

		if (shapes[i]->intersection(origin, direction, tmp_origin, tmp_direction)) {
			// Check if the new depth is closer to the camera.
			float depth = (tmp_origin - origin).getLength();
			if (depth < current_depth) {

				// We have intersected and are closer to the origin. This is the new next object to raytrace.
				hasIntersected = true;
				current_depth = depth;
				new_origin = tmp_origin;
				new_direction = tmp_direction;
				intersectedShape = shapes[i]->getIntersectedShape();
			}
		}
	}

	// If no intersection happend, return black. (Color at infinity)
	if (!hasIntersected)
		return Vec3Df(0.f, 0.f, 0.f);

	// Dot product of the direction and new direction
	float dotProduct = Vec3Df::dotProduct(direction, new_direction);

	// Start with reflected and refracted colors both black.
	Vec3Df reflectedColor = Vec3Df(0.f, 0.f, 0.f);
	Vec3Df refractedColor = Vec3Df(0.f, 0.f, 0.f);
	
	// Initial reflection and refraction values.
	float reflection = 1.0f;
	float transmission = 1.0f;

	// If it has a material we can do reflections and refractions.
	if (intersectedShape->hasMaterial()) {
		// Refraction
		if (intersectedShape->getMaterial().has_Ni()) {
			float niAir = 1.0f;
			float fresnel = 0.f;

			Vec3Df refract = intersectedShape->refract(new_direction, direction, niAir, fresnel);

			reflection = fresnel;
			transmission = 1 - fresnel;

			float translucency = 0.f;
			if (intersectedShape->getMaterial().has_Tr()) {
				translucency = 1 - intersectedShape->getMaterial().Tr();
				if (translucency > 0) {
					refractedColor = translucency * performRayTracing(new_origin + refract * EPSILON, refract, level + 1, max);

					if (intersectedShape->getMaterial().has_Tf())
						refractedColor *= intersectedShape->getMaterial().Tf();
				}
			}
		}

		// Reflection
		if (intersectedShape->getMaterial().has_Ks()) {
			Vec3Df reflect = direction - 2.f * dotProduct * new_direction;
			if (reflection > 0)
				reflectedColor = performRayTracing(new_origin, reflect, level + 1, max) * intersectedShape->getMaterial().Ks();
		}
	}

	// The color of the intersected object for all lightsources.
	Vec3Df directColor = Vec3Df(0.f, 0.f, 0.f);
	Shape* shadowInt = nullptr;

	// Calculate shadows. Multiple light sources. Transparant shadows.
	for (unsigned int j = 0; j < MyLightPositions.size(); j++) {
		Vec3Df lightDir = MyLightPositions[j] - new_origin;
		float lightDist = lightDir.getLength();
		bool intersection = false;

		for (unsigned int i = 0; i < shapes.size(); i++) {
			Vec3Df hit, stub2;
			// Check whether there's an intersection between the hit point and the light source
			if (shapes[i]->intersection(new_origin, lightDir, hit, stub2) && (hit - new_origin).getLength() < lightDist) {
				intersection = true;
				shadowInt = shapes[i]->getIntersectedShape();

				if (!shadowInt->_material.has_Tr() || shadowInt->_material.Tr() == 1.0) {
					// Intersected with an opaque object.
					break;
				}
				else {
					// Material is transparent
					directColor += (1 - shapes[i]->getMaterial().Tr()) * intersectedShape->shade(origin, new_origin, MyLightPositions[j], new_direction);
					// If it has an ambient color, it should let that color pass through.
					if (shapes[i]->getMaterial().has_Ka() && shapes[i]->getMaterial().Ka() != Vec3Df(0.f, 0.f, 0.f)) {
						directColor *= shapes[i]->getMaterial().Ka();
					}
				}
			}
		}
		if (!intersection) {
			// There was no intersection.
			directColor += intersectedShape->shade(origin, new_origin, MyLightPositions[j], new_direction);
		}
	}
	directColor /= float(MyLightPositions.size());

	return directColor + reflection * reflectedColor + transmission * refractedColor;
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
