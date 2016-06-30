#include "GlobalSys.h"
#include "zyk/Objects.h"
#include "zyk/RandomGenerator.h"
#include "Material\Solid.h"
#include "Material\Anisotropy.h"
#include "Material\Dielectric.h"
#include "Material\LightSource.h"
#include "Material\Mirror.h"
#include "Material\Solid.h"
#include "Material\Solid.h"
#include "Material\Solid.h"
#include "IntersectionInfo.h"
#include "InputRay.h"
#include <windows.h>
#include <WinBase.h>
#include <algorithm>

CGlobalSys *g_pGlobalSys = NULL;
CGlobalSys::CGlobalSys() :mLights(NULL), mLightNum(0), m_width_global(0), m_height_global(0), mRandGen(NULL)
{
	viewport_height=600;
	viewport_width=800;
	pixel_num=viewport_width*viewport_height;
	m_cam.init(viewport_width,viewport_height);
	m_cam.pos=Vec3(0,0,10);
	m_cam.reset_camera_matrix();
	init_Light();
	init_Material();
	mRandGen = new zyk::RandomGenerator;
}

CGlobalSys::~CGlobalSys()
{
	SAFE_DELETE_ARRAY(mLights);
	SAFE_DELETE(mRandGen);
	for (int i = 0; i < mMatNum; i++)
	{
		SAFE_DELETE(m_materials[i]);
	}
	SAFE_DELETE_ARRAY(m_materials);
}


void CGlobalSys::init_Light()
{
	/*
	mLightNum=2;
	mLights=new zyk::Light[mLightNum];
	mLights[0].c_ambient=Vec4::Ones();
	mLights[0].c_diffuse=Vec4::Ones();
	mLights[0].c_specular=Vec4::Ones();
	mLights[0].pos=Vec3::Zero();
	mLights[0].dir=Vec3(-0.57735027,0.57735027,0.57735027).normalized();

	mLights[1].c_ambient=Vec4::Ones();
	mLights[1].c_diffuse=Vec4::Ones();
	mLights[1].c_specular=Vec4::Ones();
	mLights[1].pos=Vec3::Zero();
	mLights[1].dir=Vec3( 0.57735027,-0.57735027,-0.57735027).normalized();*/
}

zyk::Material* createMaterial_baseType(int type_id)
{
	zyk::Material* material;
	switch (type_id)
	{
	case 0:
	{
		material = new Solid();
		material->type = zyk::SOLID;
	}break;
	case 1:
	{
		material = new Dielectric();
		material->type = zyk::DIELECTRIC;
	}break;
	case 2:
	{
		material = new LightSource();
		material->type = zyk::LIGHTSOURCE;
	}break;
	case 3:
	{
		material = new Mirror();
		material->type = zyk::MIRROW;
	}break;
	case 4:
	{
		material = new Anistropy();
		material->type = zyk::ANISOTROPY;
	}break;
	default:
		material = NULL;
	}
	return material;
}

zyk::Material* set_material_from_file(std::ifstream&in)
{
	assert(in.is_open());
	float tmp_array[3];
	float tmp_value;
	std::string tmp_str;
	
	in>>tmp_str;

	in>>tmp_value;
	zyk::Material* material = createMaterial_baseType(tmp_value);
	if (!material)
	{
		std::cout << "Error material type!" << std::endl;
		return NULL;
	}

	material->name = tmp_str;
	in>>tmp_value;
	in>>tmp_array[0]>>tmp_array[1]>>tmp_array[2];
	material->ka=tmp_value;
	material->ra=Vec4(tmp_array[0],tmp_array[1],tmp_array[2],1.0f);

	in>>tmp_value;
	in>>tmp_array[0]>>tmp_array[1]>>tmp_array[2];
	material->kd=tmp_value;
	material->rd=Vec4(tmp_array[0],tmp_array[1],tmp_array[2],1.0f);

	in>>tmp_value;
	in>>tmp_array[0]>>tmp_array[1]>>tmp_array[2];
	material->ks=tmp_value;
	material->rs=Vec4(tmp_array[0],tmp_array[1],tmp_array[2],1.0f);

	in>>tmp_value;
	material->power=tmp_value;

	in>>tmp_value;
	material->kr=tmp_value;

	in>>tmp_value;
	material->rei=tmp_value;

	if(material->type==zyk::ANISOTROPY)
	{
		in>>tmp_value;
		material->rough_exponent=tmp_value;
		in>>tmp_value;
		material->specular_ratio=tmp_value;
	}
	return material;
}

void CGlobalSys::load_Material(const char* txtName)
{
	std::ifstream in(txtName);
	if(!in.is_open())
	{
		std::cout<<"Can't open the file!"<<std::endl;
		return;
	}
	in>>mMatNum;
	if(mMatNum>MAX_MATERIAL)
	{
		std::cout<<"Exceed the number of max material num!"<<std::endl;
		return;
	}
	m_materials = new zyk::Material*[mMatNum];
	for(int i=0;i<mMatNum;i++)
		m_materials[i] = set_material_from_file(in);

	in.close();
}

const zyk::Material* CGlobalSys::getMaterialByName(const std::string& name)const
{
	for(int i=0;i<mMatNum;i++)
	{
		if(name==m_materials[i]->name)
			return m_materials[i];
	}
	return NULL;
}

zyk::Material* CGlobalSys::getMaterialByName(const std::string& name)
{
	return const_cast<zyk::Material*>(static_cast<const CGlobalSys&>(*this).getMaterialByName(name));
}

void CGlobalSys::init_Material()
{
	load_Material("material_script.txt");
}

void CGlobalSys::generateAreaLights(zyk::TriMesh*pTri_mesh)
{
	if(!pTri_mesh||!pTri_mesh->getMesh())
		return;
	pTri_mesh->calOBB();
	const zyk::OBB* obb=pTri_mesh->getOBB();
	if(!obb)
	{
		std::cout<<"You must calculate the mesh's bounding box first!"<<std::endl;
		return;
	}
	zyk::Light_Ptr light_backup=mLights;
	mLights=new zyk::Light[mLightNum+1];
	for(int i=0;i<mLightNum;i++)
		mLights[i]=light_backup[i];
	SAFE_DELETE_ARRAY(light_backup);
	mLights[mLightNum].type=zyk::AREA_LIGHT;
	mLights[mLightNum].light_source_mesh=pTri_mesh;
	mLightNum++;
}


void CGlobalSys::intersectionCheck(const std::vector<zyk::Object*>&p_objects,const InputRay&ray, Intersection_Info& inter_info)
{
	float t, min_t;
	Vec3 tmp_nor;
	Vec3 tmp_inter_pt;
	for (int obj_id = 0; obj_id<(int)p_objects.size(); obj_id++)
	{
		bool is_intersect = p_objects[obj_id]->intersect(ray.origin, ray.dir, t, tmp_nor, tmp_inter_pt);
		if (!is_intersect)
			continue;
		if (inter_info.material == NULL || t<min_t)
		{
			min_t = t;
			inter_info.material = p_objects[obj_id]->getMaterial();
			inter_info.inter_nor = tmp_nor;
			inter_info.inter_pt = tmp_inter_pt;
		}
	}
}

/**
get a 3D ray based on a given point
*/
Vec3 CGlobalSys::getRayDirection(const zyk::Camera&pCam, float x, float y)
{
	static float inv_width = 1.0f / pCam.viewport_width;
	static float inv_height = 1.0f / pCam.viewport_height;

	Vec3 x_world = pCam.u, y_world = pCam.v;
	Vec3 view_plane_cent_w = pCam.pos + pCam.n*pCam.near_clip_z;
	Vec3 plane_corner_pt = view_plane_cent_w - x_world*pCam.view_plane(1) + y_world*pCam.view_plane(2);

	Vec3 pixel_pos = plane_corner_pt + x*inv_width*pCam.view_width*x_world -
		y*inv_height*pCam.view_height*y_world;

	return (pixel_pos - pCam.pos).normalized();
}

Vec3 CGlobalSys::getRayDirection_randSampling(const zyk::Camera&pCam, float x, float y)
{
	//Here we select one point in the pixel randomly to cast a ray  
	float r1 = mRandGen->getRand();
	float r2 = mRandGen->getRand();

	return getRayDirection(pCam, x + r1, y + r2);
}

Vec4 CGlobalSys::shadeSinglePxiel_Normal(const std::vector<zyk::Object*>&p_objects,int x, int y)
{
	Vec3 ray_dir = getRayDirection(g_pGlobalSys->m_cam, x + 0.5f, y + 0.5f);

	int near_obj_id = -1;
	Vec3 inter_nor, inter_pt;
	Intersection_Info inter_info;
	g_pGlobalSys->intersectionCheck(p_objects, InputRay(g_pGlobalSys->m_cam.pos, ray_dir), inter_info);

	if (inter_info.material == NULL)
		return Vec4(0,0,0,1);
	else
		return Vec4(inter_info.inter_nor(0), inter_info.inter_nor(1), inter_info.inter_nor(2), 1);
}

Vec4 CGlobalSys::shadeSinglePixel_MC_Sampling(const std::vector<zyk::Object*>&p_objects,int x, int y)
{
	Vec4 shade_color = Vec4(0,0,0,1);
	for (int i = 0; i<mSampleNum; i++)
	{
		Vec3 ray_dir = getRayDirection_randSampling(g_pGlobalSys->m_cam, x, y);
		shade_color += castRayShading_McSampling(p_objects, InputRay(g_pGlobalSys->m_cam.pos, ray_dir), 0);
	}
	shade_color /= mSampleNum;
	return shade_color;
}

Vec4 CGlobalSys::castRayShading_McSampling(const std::vector<zyk::Object*>&p_objects,const InputRay&ray, int depth, float input_rei)
{
	if (depth == mMax_depth + 1)
		return environment_color;

	//find the nearest object based on the casting ray and origin
	int near_obj_id = -1;
	Intersection_Info inter_info;
	intersectionCheck(p_objects,ray, inter_info);
	if (!inter_info.material)
		return environment_color;

	Vec4 shade_color = inter_info.material->render(p_objects, inter_info, ray.dir, depth, input_rei);
	if (zyk::sceneId == 2)
		zyk::clip_0_to_1(shade_color);
	return shade_color;
}
