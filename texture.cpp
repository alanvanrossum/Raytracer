#include "texture.h"

Texture::Texture(Image img) : _image_data(img) {}

void Texture::convertBarycentricToTexCoord(float a, float b, Vec3Df* texcoords, float& tex_u, float& tex_v) {
	// Calculate third barycentric coordinate
	float c = 1 - a - b;
	Vec3Df uv = c * texcoords[0] + a * texcoords[1] + b * texcoords[2];
	tex_u = uv[0];
	tex_v = uv[1];
}

Vec3Df Texture::getColor(float tex_u, float tex_v) {
	Vec3Df rgb(0.f, 0.f, 0.f);
	float u = _image_data._width * tex_u - 1;
	float v = _image_data._height * tex_v - 1;
	for (int i = 0; i < 3; i++) {
		rgb[i] = _image_data._image[u * 3 + v * 3 * _image_data._width + i];
	}
	return rgb;
}