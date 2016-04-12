#ifndef  RAYTRACER_GLOBALSYS_H
#define RAYTRACER_GLOBALSYS_H
#include "zyk/Data_structure.h"
#include<Eigen/StdVector>

#define MAX_MATERIAL 10

namespace zyk
{
	class TriMesh; 
};

class CGlobalSys
{
public:
	CGlobalSys();
	~CGlobalSys();
	void generateAreaLights(zyk::TriMesh*pTri_mesh);
	void init_Light();
	void init_Material();
	void load_Material(const char* txtName);//parse the txt file to get the material information
	const zyk::Material* getMaterialByName(const std::string& name)const;
	zyk::Material* getMaterialByName(const std::string& name);

	float viewport_width,viewport_height,pixel_num;
	zyk::Camera m_cam;
	zyk::Light* mLights;
	zyk::Material m_materials[MAX_MATERIAL];
	int mLightNum;
	int mMatNum;
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
extern CGlobalSys *g_pGlobalSys;
#endif