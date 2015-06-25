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
		void convertBarycentricCoordToTexCoord(float, float, Vec3Df*, float&, float&);
		Vec3Df getColor(float, float);

	private:
		Image _image_data;
};

#endif