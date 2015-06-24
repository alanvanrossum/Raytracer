// Includes
#include "image.h"

/**
* Image class
*
* This class can be used to read or write an image.
* It reads and writes it in a PPM format.
*/

/**
 * Constructor which creates an image with a defined width.
 */
Image::Image(int width, int height) : _width(width), _height(height) {
	_image.resize(3 * _width*_height);
}

/**
 * Constructor which reads an image.
 */
Image::Image(const char * filename) {
	readImage(filename);
}

/**
 * setPixel; Sets the desired pixel with the given RGBValue.
 */
void Image::setPixel(int i, int j, const RGBValue & rgb) {
	_image[3 * (_width*j + i)] = rgb[0];
	_image[3 * (_width*j + i) + 1] = rgb[1];
	_image[3 * (_width*j + i) + 2] = rgb[2];
}

/**
 * writeImage; Writes the stored image to the given file name.
 */
bool Image::writeImage(const char * filename) {
	FILE* file;
	errno_t file_result = fopen_s(&file, filename, "wb");
	if (!file) {
		printf("Dump file problem... file\n");
		return false;
	}

	fprintf(file, "P6\n%i %i\n255\n", _width, _height);


	std::vector<unsigned char> imageC(_image.size());

	for (unsigned int i = 0; i<_image.size(); ++i)
		imageC[i] = (unsigned char)(_image[i] * 255.0f);

	int t = fwrite(&(imageC[0]), _width * _height * 3, 1, file);
	if (t != 1) {
		printf("Dump file problem... fwrite\n");
		return false;
	}

	std::cout << "Image succesfully written to: " << filename << std::endl;

	fclose(file);
	return true;
}

/**
 * readImage; Reads the image from the given file name.
 */
bool Image::readImage(const char * filename) {
	FILE* file;
	errno_t file_result = fopen_s(&file, filename, "rb");
	if (!file) {
		printf("ERROR: No file called %s!\n", filename);
		return false;
	}

	int width, height;
	fscanf_s(file, "%*s\n");
	char buf[256];
	while (fgets(buf, 256, file) && buf[0] == '#') {
		printf(buf);
	}

	sscanf_s(buf, "%i %i\n", &width, &height);
	fscanf_s(file, "255\n");
	this->_width = width;
	this->_height = height;
	std::vector<unsigned char> imageC(width * height * 3);
	for (size_t i = 0; i < imageC.size(); i++) {
		_image.push_back((float)imageC[i] / 255.0f);
	}
	printf("Loaded texture %s\n", filename);
	return true;
}

/**
 * RGBValue class. Stores RGB Values per pixel.
 */
RGBValue::RGBValue(float rI = 0, float gI = 0, float bI = 0) : r(rI), g(gI), b(bI) {
	if (r>1)
		r = 1.0;
	if (g>1)
		g = 1.0;
	if (b>1)
		b = 1.0;

	if (r<0)
		r = 0.0;
	if (g<0)
		g = 0.0;
	if (b<0)
		b = 0.0;
};

/**
 * Retrieves a r,g or b value based on the index 0, 1, 2.
 */
float RGBValue::operator[](int i) const {
	switch (i) {
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		default:
			return r;
	}
}

/**
* Retrieves a reference to a r,g or b value based on the index 0, 1, 2.
*/
float& RGBValue::operator[](int i) {
	switch (i) {
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		default:
			return r;
	}
}