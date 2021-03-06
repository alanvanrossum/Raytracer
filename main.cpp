#ifdef WIN32
#include <windows.h>
#endif
#include "main.h"
#include "traqueboule.h"
#include <GL/glut.h>
#include <iomanip>
#include <ostream>

/**
 * VARIABLE DEFINITION
 */
// The current position of the camera.
Vec3Df MyCameraPosition;

// All the light positions. Only used for raytracing.
std::vector<Vec3Df> MyLightPositions;

// Windows size
unsigned int WindowSize_X = 2000;
unsigned int WindowSize_Y = 2000;

// Raytraced image size
unsigned int ImageSize_X = WindowSize_X;
unsigned int ImageSize_Y = WindowSize_Y;

// Number of samples.
unsigned int ns = 4;

/**
 * Main function, which is drawing an image (frame) on the screen.
 *
 * Fires every cycle.
 */
void drawFrame()
{
	yourDebugDraw();
}

/**
 * Animation is called for every image on the screen once
 */
void animate()
{
	MyCameraPosition=getCameraPosition();
	glutPostRedisplay();
}

/**
 * Main Programme
 */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    // Framebuffer setup
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

    // Positioning and size of window
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(WindowSize_X,WindowSize_Y);
    glutCreateWindow(argv[0]);	

    // Initialize viewpoint
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-4);
    tbInitTransform();     // This is for the trackball, please ignore
    tbHelp();             // idem
	MyCameraPosition=getCameraPosition();

	// Activate the light following the camera
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    int LightPos[4] = {0,0,2,0};
    int MatSpec [4] = {1,1,1,1};
    glLightiv(GL_LIGHT0,GL_POSITION,LightPos);

	// Normals will be normalized in the graphics pipeline
	glEnable(GL_NORMALIZE);
    // Clear color of the background is black.
	glClearColor (0.0, 0.0, 0.0, 0.0);

	
	// Activate rendering modes
    // Activate depth test
	glEnable( GL_DEPTH_TEST ); 
    // Araw front-facing triangles filled
	// and back-facing triangles as wires
    glPolygonMode(GL_FRONT,GL_FILL);
    glPolygonMode(GL_BACK,GL_LINE);
    // Interpolate vertex colors over the triangles
	glShadeModel(GL_SMOOTH);


	// Glut setup... to ignore
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMouseFunc(tbMouseFunc);    // trackball
    glutMotionFunc(tbMotionFunc);  // uses mouse
    glutIdleFunc( animate);


	init();

    
	// Main loop for glut... this just runs your application
    glutMainLoop();
        
    return 0;  // execution never reaches this point
}

/**
 * OpenGL setup - functions do not need to be changed! 
 * you can SKIP AHEAD TO THE KEYBOARD FUNCTION
 */
// What to do before drawing an image
 void display(void)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);//store GL state
    // Effacer tout
    glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT); // clear image
    
    glLoadIdentity();  

    tbVisuTransform(); // init trackball

    drawFrame( );    //actually draw

    glutSwapBuffers();//glut internal switch
	glPopAttrib();//return to old GL state
}

// Window changes size
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho (-1.1, 1.1, -1.1,1.1, -1000.0, 1000.0);
    gluPerspective (50, (float)w/h, 0.01, 10);
    glMatrixMode(GL_MODELVIEW);
}

// Transform the x, y position on the screen into the corresponding 3D world position
void produceRay(int x_I, int y_I, Vec3Df * origin, Vec3Df * dest)
{
		int viewport[4];
		double modelview[16];
		double projection[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //recuperer matrices
		glGetDoublev(GL_PROJECTION_MATRIX, projection); //recuperer matrices
		glGetIntegerv(GL_VIEWPORT, viewport);//viewport
		int y_new = viewport[3] - y_I;

		double x, y, z;
		
		gluUnProject(x_I, y_new, 0, modelview, projection, viewport, &x, &y, &z);
		origin->p[0]=float(x);
		origin->p[1]=float(y);
		origin->p[2]=float(z);
		gluUnProject(x_I, y_new, 1, modelview, projection, viewport, &x, &y, &z);
		dest->p[0]=float(x);
		dest->p[1]=float(y);
		dest->p[2]=float(z);
}

// React to keyboard input
void keyboard(unsigned char key, int x, int y)
{
    printf("key %d pressed at %d,%d\n",key,x,y);
    fflush(stdout);
	switch (key)
	{
		// Add a light based on the camera position.
		case 'L':
			MyLightPositions.push_back(getCameraPosition());
			break;

		// Update a light based on the camera position.
		case 'l':
			MyLightPositions[MyLightPositions.size() - 1] = getCameraPosition();
			break;

		// Pressing r will launch the raytracing.
		case 'r':
		{
			cout << "Raytracing" << endl;

			// Setup an image with the size of the current image.
			Image result(ImageSize_X, ImageSize_Y);

			// Produce the rays for each pixel, by first computing
			// the rays for the corners of the frustum.
			Vec3Df origin00, dest00;
			Vec3Df origin01, dest01;
			Vec3Df origin10, dest10;
			Vec3Df origin11, dest11;
			Vec3Df origin, dest;

			produceRay(0, 0, &origin00, &dest00);
			produceRay(0, ImageSize_Y - 1, &origin01, &dest01);
			produceRay(ImageSize_X - 1, 0, &origin10, &dest10);
			produceRay(ImageSize_X - 1, ImageSize_Y - 1, &origin11, &dest11);

			for (unsigned int y = 0; y < ImageSize_Y; ++y) {
				for (unsigned int x = 0; x < ImageSize_X; ++x)
				{
					// Produce the rays for each pixel, by interpolating 
					// the four rays of the frustum corners.
					float xscale = 1.0f - float(x) / (ImageSize_X - 1);
					float yscale = 1.0f - float(y) / (ImageSize_Y - 1);

					origin = yscale*(xscale*origin00 + (1 - xscale)*origin10) +
						(1 - yscale)*(xscale*origin01 + (1 - xscale)*origin11);
					dest = yscale*(xscale*dest00 + (1 - xscale)*dest10) +
						(1 - yscale)*(xscale*dest01 + (1 - xscale)*dest11);

					// Launch raytracing for the given ray.
					Vec3Df rgb = performRayTracing(origin, dest);
					// Store the result in an image 
					result.setPixel(x, y, RGBValue(rgb[0], rgb[1], rgb[2]));
				}

				loadbar(y, ImageSize_Y, 50);
			}

			loadbar(ImageSize_Y, ImageSize_Y, 50);
			cout << endl;
			cout << endl;

			result.writeImage("result.ppm");

			cout << endl;
			break;
		}

		// Pressing s will launch the raytracing with sampling.
		case 's':
		{
			cout << "Raytracing with sampling" << endl;

			// Setup an image with the size of the current image.
			Image result(ImageSize_X, ImageSize_Y);

			// Produce the rays for each pixel, by first computing
			// the rays for the corners of the frustum.
			Vec3Df origin00, dest00;
			Vec3Df origin01, dest01;
			Vec3Df origin10, dest10;
			Vec3Df origin11, dest11;
			Vec3Df origin, dest;

			produceRay(0, 0, &origin00, &dest00);
			produceRay(0, ImageSize_Y - 1, &origin01, &dest01);
			produceRay(ImageSize_X - 1, 0, &origin10, &dest10);
			produceRay(ImageSize_X - 1, ImageSize_Y - 1, &origin11, &dest11);
			
			Vec3Df rgb;

			unsigned int fromY = 0;
			unsigned int toY = 100;

			for (unsigned int y = fromY; y < toY; ++y) {
				for (unsigned int x = 0; x < ImageSize_X; ++x)
				{
					// Produce the rays for each pixel, by interpolating 
					// the four rays of the frustum corners.
					float xscale; // = 1.0f - float(x) / (ImageSize_X - 1);
					float yscale; // = 1.0f - float(y) / (ImageSize_Y - 1);

					for (unsigned int sx = 0; sx < ns; sx++) {
						for (unsigned int sy = 0; sy < ns; sy++) {
							xscale = 1.0f - (x + (sx + 0.5f) / ns) / (ImageSize_X - 1);
							yscale = 1.0f - (y + (sy + 0.5f) / ns) / (ImageSize_Y - 1);

							origin = yscale*(xscale*origin00 + (1 - xscale)*origin10) + (1 - yscale)*(xscale*origin01 + (1 - xscale)*origin11);
							dest = yscale*(xscale*dest00 + (1 - xscale)*dest10) + (1 - yscale)*(xscale*dest01 + (1 - xscale)*dest11);

							// Launch raytracing for the given ray.
							rgb += performRayTracing(origin, dest);
						}
					}
					
					rgb /= float(ns*ns);

					// Store the result in an image 
					result.setPixel(x, y, RGBValue(rgb[0], rgb[1], rgb[2]));
				}

				loadbar(y-fromY, toY-fromY, 50);
			}

			loadbar(toY - fromY, toY - fromY, 50);
			cout << endl;
			cout << endl;

			result.writeImage("result.ppm");

			cout << endl;
			break;
		}

		// ESC pressed
		case 27:
		{
			exit(0);
		}

		// Space pressed, trace our debug line.
		case 32:
		{
			cout << "Raytracing single ray" << endl;

			// Produce the ray for the current mouse position
			Vec3Df testRayOrigin, testRayDestination;
			produceRay(x, y, &testRayOrigin, &testRayDestination);

			yourKeyboardFunc(key, x, y, testRayOrigin, testRayDestination);
		}
	}
}

static inline void loadbar(unsigned int x, unsigned int n, unsigned int w = 50) {
	if ((x != n) && (x % (n / 100 + 1) != 0)) return;

	float ratio = x / (float)n;
	unsigned int c = unsigned int(ratio * w);

	cout << std::setw(3) << (int)(ratio * 100) << "% [";
	for (unsigned int x = 0; x < c; x++) cout << "=";
	for (unsigned int x = c; x < w; x++) cout << " ";
	cout << "]\r" << std::flush;
}