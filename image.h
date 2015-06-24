#ifndef IMAGE_JFDJKDFSLJFDFKSDFDJFDFJSDKSFJSDLF
#define IMAGE_JFDJKDFSLJFDFKSDFDJFDFJSDKSFJSDLF
#include <vector>
#include <iostream>

/**
* RGBValue class. Stores RGB Values per pixel.
*/
class RGBValue {
public:
	// Constructor
	RGBValue(float rI, float gI, float bI);

	// Methods
	float operator[](int i) const;
	float & operator[](int i);

	// Variables
	float r, b, g;
};

/**
 * Image class
 *
 * This class can be used to read or write an image.
 * It reads and writes it in a PPM format.
 */
class Image
{
public:
	// Constructors
	Image(int width, int height);
	explicit Image(const char * filename);

	// Methods
	void setPixel(int i, int j, const RGBValue & rgb);
	bool writeImage(const char * filename);

	// Variabless
	std::vector<float> _image;
	int _width;
	int _height;

private:
	// Private methods
	bool readImage(const char * filename);
};

#endif