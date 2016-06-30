#ifndef RAYTRACER_GLOBALSYS_H
#define RAYTRACER_GLOBALSYS_H
#include "zyk/Data_structure.h"
#include<Eigen/StdVector>

#define MAX_MATERIAL 20

namespace zyk
{
	class TriMesh; 
	class Object;
	class RandomGenerator;
};

struct Intersection_Info;
struct InputRay;
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

	void intersectionCheck(const std::vector<zyk::Object*>&p_objects, const InputRay&ray, Intersection_Info& inter_info);
	Vec4 castRayShading_McSampling(const std::vector<zyk::Object*>&p_objects, const InputRay&ray, int depth, float input_rei=1.0f);

	float viewport_width,viewport_height,pixel_num;
	zyk::Camera m_cam;
	zyk::Light* mLights;
	zyk::Material** m_materials;
	int m_width_global,m_height_global;
	int mLightNum;
	int mMatNum;
	int mMax_depth;
	Vec4 environment_color;
	float light_intensity;
	zyk::RandomGenerator* mRandGen;
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
extern CGlobalSys *g_pGlobalSys;
#endif