#include "shape.h"

Shape::Shape(Material& material, Vec3Df origin) : _material(material), _origin(origin) {}


/**
 * Basic shading method using Phong.
 *
 * See: http://dicklyon.com/tech/Graphics/Phong_TR-Lyon.pdf
 */
Vec3Df Shape::shade(const Vec3Df& camPos, const Vec3Df& intersection, const Vec3Df& lightPos, const Vec3Df& normal) {
	
	// Start without color on all compnents.
	Vec3Df ambient(0.f, 0.f, 0.f);
	Vec3Df diffuse(0.f, 0.f, 0.f);
	Vec3Df specular(0.f, 0.f, 0.f);

	// Calculate the lightVector. Direction whoch the light is comming from.
	Vec3Df lightVec = lightPos - intersection;
	lightVec.normalize();

	// If it has an ambient color, set it.
	if (this->_material.has_Ka())
		ambient = _material.Ka();

	// If it has a diffuse color, set it.
	if (this->_material.has_Kd()) {

		float dot = Vec3Df::dotProduct(normal, lightVec);
		if (dot < 0)
			dot = Vec3Df::dotProduct(-normal, lightVec);
		diffuse = dot * _material.Kd();
	}

	// If it has a specular color, set it.
	if (this->_material.has_Ks()){

		Vec3Df reflect = 2 * Vec3Df::dotProduct(lightVec, normal) * normal - lightVec;
		Vec3Df view = camPos - intersection;
		view.normalize();

		if (Vec3Df::dotProduct(view, reflect) < 0) {
			specular = Vec3Df(0, 0, 0);
		}
		else {
			float shininess;
			if (this->_material.has_Ns())
				shininess = _material.Ns();
			else
				shininess = 20;

			specular = pow(Vec3Df::dotProduct(reflect, view), shininess) * _material.Ks();
		}
	}

	return ambient + diffuse + specular;
}