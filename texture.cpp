#include "texture.h"

Texture::Texture(Image img) : _image_data(img) {}

/**
 * method: convertBarycentricCoordToTexCoord()
 *
 * Calculates the Texture cooredinates:
 *
 * See: https://en.wikipedia.org/wiki/UV_mapping
 */
void Texture::convertBarycentricCoordToTexCoord(float a, float b, Vec3Df* texCoords, float& texU, float& texV) {

	// Calculate third barycentric coordinate
	float c = 1 - a - b;
	Vec3Df uv = c * texCoords[0] + a * texCoords[1] + b * texCoords[2];
	texU = uv[0];
	texV = uv[1];
}

/**
* method: getColor()
*
* Returns the color at a given UV point.
*
* See: https://en.wikipedia.org/wiki/UV_mapping
*/
Vec3Df Texture::getColor(float texU, float texV) {
	
	// Start without color.
	Vec3Df RGB(0.f, 0.f, 0.f);

	// Get U & V.
	int u = int(_image_data._width * texU) - 1;
	int v = int(_image_data._height * texV) - 1;

	// U || V !< 0
	if (u < 0)
		u = 0;
	if (v < 0)
		v = 0;

	// Find the RGB values of the UV point.
	for (int i = 0; i < 3; i++) {
		RGB[i] = _image_data._image[unsigned int(u * 3 + v * 3 * _image_data._width + i)];
	}
	return RGB;
}