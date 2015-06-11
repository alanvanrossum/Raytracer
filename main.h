#ifndef MAIN_header
#define MAIN_header
#include <vector>
#include "mesh.h"

/////////////////
// MAIN HEADER //
/////////////////

/**
 * GLOBAL VARIABLES
 */

// The current position of the camera.
Vec3Df MyCameraPosition;

// All the light positions. Only used for raytracing.
std::vector<Vec3Df> MyLightPositions;

// A single mesh
Mesh MyMesh;

// Windows size
unsigned int WindowSize_X = 800;
unsigned int WindowSize_Y = 800;

// Raytraced image size
unsigned int ImageSize_X = WindowSize_X;
unsigned int ImageSize_Y = WindowSize_Y;

/**
 * Function declarations
 */
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void produceRay(int x_I, int y_I, Vec3Df * origin, Vec3Df * dest);
void reshape(int w, int h);
void display(void);
void animate();
void drawFrame();

#endif // MAIN_header