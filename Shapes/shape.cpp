#include "shape.h"

Shape::Shape(Material& material, Vec3Df origin) : _material(material), _origin(origin) {}


/**
 * Basic shading method using Phong.
 */
Vec3Df Shape::shade(const Vec3Df& camPos, const Vec3Df& intersection, const Vec3Df& lightPos, const Vec3Df& normal) {
	Vec3Df ambient(0.f, 0.f, 0.f);
	Vec3Df diffuse(0.f, 0.f, 0.f);
	Vec3Df specular(0.f, 0.f, 0.f);

	Vec3Df lightVec = lightPos - intersection;
	lightVec.normalize();

	if (this->_material.has_Ka()) ambient = _material.Ka();
	if (this->_material.has_Kd()) {
		float dot = Vec3Df::dotProduct(normal, lightVec);
		if (dot < 0)
			dot = Vec3Df::dotProduct(-normal, lightVec);
		diffuse = dot * _material.Kd();
	}
	if (this->_material.has_Ks()){
		Vec3Df reflect = 2 * Vec3Df::dotProduct(lightVec, normal) * normal - lightVec;
		Vec3Df view = camPos - intersection;
		view.normalize();

		if (Vec3Df::dotProduct(view, reflect) < 0) {
			specular = Vec3Df(0, 0, 0);
		}
		else {
			float shininess;
			if (this->_material.has_Ns()) shininess = _material.Ns();
			else shininess = 21;
			specular = pow(Vec3Df::dotProduct(reflect, view), shininess) * _material.Ks();
		}
	}
	return ambient + diffuse + specular;
}

/**
* Basic refraction method.
*/
Vec3Df Shape::refract(const Vec3Df &normal, const Vec3Df &direction, const float &ni, float &fresnel) {
	if (this->_material.has_Ni()) {
		double dot = Vec3Df::dotProduct(normal, direction);
		double ni1, ni2;
		Vec3Df realNormal = normal;

		// If dot(N,D) > 0, then we're exiting the medium
		if (dot > 0) {
			ni1 = this->_material.Ni();
			ni2 = ni;
			realNormal = -normal;

			// While we're at it, calculate Fresnel with Schlick's approximation.
			// Do nothing in the else statement, as Fresnel is initialised to zero
			// in the calling function.
			float fzero = pow(((ni1 - ni2) / (ni1 + ni2)), 2);
			fresnel = (fzero + (1 - fzero) * pow((1 - dot), 5)) / 100;
		}
		else {
			// FIXME: fresnel here too?
			ni1 = ni;
			ni2 = this->_material.Ni();
			dot = Vec3Df::dotProduct(-normal, direction);


			float fzero = pow(((ni1 - ni2) / (ni1 + ni2)), 2);
			fresnel = (fzero + (1 - fzero) * pow((1 - dot), 5)) / 100;
		}

		// If root < 0, total internal reflection takes place.
		// In this case, the refraction vector should be black.
		// Maybe we could integrate reflection here
		double n = ni1 / ni2;
		double root = 1.0 - n * n * (1.0 - dot * dot);

		if (root < 0) return direction - 2 * dot * realNormal;

		return direction * n + (n * dot - sqrt(root)) * realNormal;
	}

	return Vec3Df(0, 0, 0);
}