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
extern Vec3Df MyCameraPosition;

// All the light positions. Only used for raytracing.
extern std::vector<Vec3Df> MyLightPositions;

// Windows size
extern unsigned int WindowSize_X;
extern unsigned int WindowSize_Y;

// Raytraced image size
extern unsigned int ImageSize_X;
extern unsigned int ImageSize_Y;

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