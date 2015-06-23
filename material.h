#ifndef MATERIAL_H_sdfjlasdfjfsdfjljfasdf
#define MATERIAL_H_sdfjlasdfjfsdfjljfasdf

// Includes
#include<string>
#include"Vec3D.h"

/**
 * Material class;
 */
class Material {
	public:
		// Constructors
		Material();
		Material(const Material & m);

		// Methods
		Material & operator=(const Material & m);
		void cleanup();
		bool is_valid() const;

		// Has methods
		bool has_Kd();
		bool has_Ka();
		bool has_Ks();
		bool has_Ns();
		bool has_Ni();
		bool has_illum();
		bool has_Tr();
		bool has_Tf();
		bool has_tex() const;

		// Set methods
		void set_Kd(float r, float g, float b);
		void set_Ka(float r, float g, float b);
		void set_Ks(float r, float g, float b);
		void set_Ns(float r);
		void set_Ni(float r);
		void set_illum(int r);
		void set_Tr(float t);
		void set_Tf(float r, float g, float b);
		void set_textureName(const std::string& s);
		void set_normal_mapName(const std::string& s);
		void set_name(const std::string & s);

		// Get methods
		const Vec3Df& Kd() const;
		const Vec3Df& Ka() const;
		const Vec3Df& Ks() const;
		float Ni() const;
		float Ns() const;
		int illum()const;
		float Tr() const;
		const Vec3Df& Tf() const;
		const std::string & textureName();
		const std::string & normal_mapName();
		const std::string & name();

	private:
		// Variables
		Vec3Df Kd_;						// Color diffuse
		Vec3Df Ka_;						// Color ambient
		Vec3Df Ks_;						// Color specular
		float Ns_;						// Shininess
		float Ni_;						// Refraction something...
		int illum_;						// Illumination
		float Tr_;						// Transparancy
		Vec3Df Tf_;						// Transmission filter
		std::string name_;				// Name of the material
		std::string textureName_;		// Name of the textureMap (filePath + fileName)
		std::string normal_mapName_;	// Name of the normalMap (filePath + fileName)

		// Booleans
		bool Kd_is_set_;
		bool Ka_is_set_;
		bool Ks_is_set_;
		bool Ns_is_set_;
		bool Ni_is_set_;
		bool illum_is_set_;
		bool Tr_is_set_;
		bool Tf_is_set_;
		bool tex_is_set;
		bool normal_is_set;
};

#endif