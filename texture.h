#ifndef TEXTURE_JFDJKDFSLJFDFKSDFDJFDFJSDKSFJSDLF
#define TEXTURE_JFDJKDFSLJFDFKSDFDJFDFJSDKSFJSDLF

#include "image.h"
#include "Vec3D.h"

/**
 * Texture class.
 */
class Texture {
	public:
		// Constructor
		Texture(Image img);

		// Methods
		void convertBarycentricToTexCoord(float a, float b, Vec3Df* texcoords, float& tex_u, float& tex_v);
		Vec3Df getColor(float u, float v);

	private:
		Image _image_data;
};

#endif