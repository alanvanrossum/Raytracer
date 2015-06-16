#include "material.h"

Material::Material() {
	cleanup();
}

Material::Material(const Material & m) {
	*this = m;
}

Material& Material::operator=(const Material & m) {
	Kd_ = m.Kd_;
	Kd_is_set_ = m.Kd_is_set_; // diffuse
	Ka_ = m.Ka_;
	Ka_is_set_ = m.Ka_is_set_; // ambient
	Ks_ = m.Ks_;
	Ks_is_set_ = m.Ks_is_set_; // specular

	Ns_ = m.Ns_;
	Ns_is_set_ = m.Ns_is_set_; // specular
	Ni_ = m.Ni_;
	Ni_is_set_ = m.Ni_is_set_; // specular

	Tr_ = m.Tr_;
	Tr_is_set_ = m.Tr_is_set_; // transparency (use this value to trade off reflection/refraction
	illum_ = m.illum_;
	name_ = m.name_;
	textureName_ = m.textureName_;
	tex_is_set = m.tex_is_set;
	return (*this);
}

void Material::cleanup() {
	Kd_is_set_ = false;
	Ka_is_set_ = false;
	Ks_is_set_ = false;
	Ns_is_set_ = false;
	Ni_is_set_ = false;
	Tr_is_set_ = false;
	illum_is_set_ = false;
	tex_is_set = false;
	name_ = "empty";
	textureName_ = "empty";
	normal_mapName_ = "empty";
}

bool Material::is_valid() const {
	return tex_is_set || Kd_is_set_ || Ka_is_set_ || Ks_is_set_ || Tr_is_set_;
}

bool Material::has_Kd() {
	return Kd_is_set_;
}

bool Material::has_Ka() {
	return Ka_is_set_;
}

bool Material::has_Ks() {
	return Ks_is_set_;
}

bool Material::has_Ns() {
	return Ns_is_set_;
}

bool Material::has_Ni() {
	return Ni_is_set_;
}

bool Material::has_illum() {
	return illum_is_set_;
}

bool Material::has_Tr() {
	return Tr_is_set_;
}

bool Material::has_tex() const {
	return tex_is_set;
}

void Material::set_Kd(float r, float g, float b) {
	Kd_ = Vec3Df(r, g, b); Kd_is_set_ = true;
}

void Material::set_Ka(float r, float g, float b) {
	Ka_ = Vec3Df(r, g, b); Ka_is_set_ = true;
}

void Material::set_Ks(float r, float g, float b) {
	Ks_ = Vec3Df(r, g, b); Ks_is_set_ = true;
}

void Material::set_Ns(float r) {
	Ns_ = r;
	Ns_is_set_ = true;
}

void Material::set_Ni(float r) {
	Ni_ = r;
	Ni_is_set_ = true;
}

void Material::set_illum(int r)	{
	illum_ = r;
	illum_is_set_ = true;
}

void Material::set_Tr(float t) {
	Tr_ = t;
	Tr_is_set_ = true;
}

void Material::set_textureName(const std::string& s) {
	textureName_ = s;
	tex_is_set = true;
}

void Material::set_normal_mapName(const std::string& s) {
	normal_mapName_ = s;
	normal_is_set = true;
}

void Material::set_name(const std::string & s) {
	name_ = s;
}

const Vec3Df& Material::Kd() const {
	return Kd_;
} //diffuse

const Vec3Df& Material::Ka() const {
	return Ka_;
} //ambiant

const Vec3Df& Material::Ks() const {
	return Ks_;
} //specular

float  Material::Ni() const {
	return Ni_;
}

float  Material::Ns() const {
	return Ns_;
} //shininess

int Material::illum()const {
	return illum_;
}

float Material::Tr() const {
	return Tr_;
}//can be hijacked, e.g., for transparency

const std::string& Material::textureName()	{
	return textureName_;
}

const std::string & Material::normal_mapName() {
	return normal_mapName_;
}

const std::string & Material::name() {
	return name_;
}