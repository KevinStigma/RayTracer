#include "GlobalSys.h"

CGlobalSys *g_pGlobalSys = NULL;
CGlobalSys::CGlobalSys():mLights(NULL),mLightNum(2)
{
	viewport_height=600;
	viewport_width=800;
	pixel_num=viewport_width*viewport_height;
	m_cam.init(viewport_width,viewport_height);
	m_cam.pos=Vec3(0,0,10);
	m_cam.reset_camera_matrix();
	init_Light();
	init_Material();
}

CGlobalSys::~CGlobalSys()
{
	SAFE_DELETE_ARRAY(mLights);
}

void CGlobalSys::init_Light()
{
	m_light.attr=1; //direction light
	m_light.c_ambient=Vec4::Ones();
	m_light.c_diffuse=Vec4::Ones();
	m_light.c_specular=Vec4::Ones();
	m_light.pos=Vec3::Zero();
	//m_light.dir=Vec3(0,1,1).normalized();
	m_light.dir=Vec3(-0.57735027,0.57735027,0.57735027).normalized();

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
	mLights[1].dir=Vec3( 0.57735027,-0.57735027,-0.57735027).normalized();
}

void CGlobalSys::init_Material()
{
	float inv_255=1/255.0f;
	m_materials[0].name="gray";
	m_materials[0].attr=2; 
	m_materials[0].color.x=200;
	m_materials[0].color.y=200;
	m_materials[0].color.z=200;

	m_materials[0].ks=m_materials[0].kd=m_materials[0].ka=1.0f;
	m_materials[0].kr=0.7f;
	m_materials[0].power=50.0f;
	m_materials[0].ra=Vec4(0.12108f,0.13282f,0.15078f,1.0f);
	m_materials[0].rd=Vec4(0.48432f,0.53128f,0.60312f,1.0f);
	m_materials[0].rs=Vec4(1.0f,1.0f,1.0f,1.0f);
	
	m_materials[1].name="yellow";
	m_materials[1].attr=2; 
	m_materials[1].color.x=245;
	m_materials[1].color.y=241;
	m_materials[1].color.z=134;

	m_materials[1].ks=m_materials[1].kd=m_materials[1].ka=1.0f;
	m_materials[1].kr=0.7f;
	m_materials[1].power=50.0f;
	m_materials[1].ra=Vec4(m_materials[1].color.x*0.3f*inv_255,m_materials[1].color.y*0.3f*inv_255,m_materials[1].color.z*0.3f*inv_255,1.0f);
	m_materials[1].rd=Vec4(m_materials[1].color.x*0.7f*inv_255,m_materials[1].color.y*0.7f*inv_255,m_materials[1].color.z*0.7f*inv_255,1.0f);
	m_materials[1].rs=Vec4(1.0f,1.0f,1.0f,1.0f);

	m_materials[2].name="purple";
	m_materials[2].attr=2; 
	m_materials[2].color.x=147;
	m_materials[2].color.y=82;
	m_materials[2].color.z=124;

	m_materials[2].ks=m_materials[2].kd=m_materials[2].ka=1.0f;
	m_materials[2].kr=0.7f;
	m_materials[2].power=50.0f;
	m_materials[2].ra=Vec4(m_materials[2].color.x*0.3f*inv_255,m_materials[2].color.y*0.3f*inv_255,m_materials[2].color.z*0.3f*inv_255,1.0f);
	m_materials[2].rd=Vec4(m_materials[2].color.x*0.7f*inv_255,m_materials[2].color.y*0.7f*inv_255,m_materials[2].color.z*0.7f*inv_255,1.0f);
	m_materials[2].rs=Vec4(1.0f,1.0f,1.0f,1.0f);

	m_materials[3].name="cyan";
	m_materials[3].attr=2; 
	m_materials[3].color.x=200;
	m_materials[3].color.y=255;
	m_materials[3].color.z=255;

	m_materials[3].ks=m_materials[3].kd=m_materials[3].ka=1.0f;
	m_materials[3].kr=0.7f;
	m_materials[3].power=50.0f;
	m_materials[3].ra=Vec4(m_materials[3].color.x*0.3f*inv_255,m_materials[3].color.y*0.3f*inv_255,m_materials[3].color.z*0.3f*inv_255,1.0f);
	m_materials[3].rd=Vec4(m_materials[3].color.x*0.7f*inv_255,m_materials[3].color.y*0.7f*inv_255,m_materials[3].color.z*0.7f*inv_255,1.0f);
	m_materials[3].rs=Vec4(1.0f,1.0f,1.0f,1.0f);

	m_materials[4].name="red";
	m_materials[4].attr=2; 
	m_materials[4].color.x=215;
	m_materials[4].color.y=93;
	m_materials[4].color.z=93;

	m_materials[4].ks=m_materials[4].kd=m_materials[4].ka=1.0f;
	m_materials[4].kr=0.7f;
	m_materials[4].power=50.0f;
	m_materials[4].ra=Vec4(m_materials[4].color.x*0.3f*inv_255,m_materials[4].color.y*0.3f*inv_255,m_materials[4].color.z*0.3f*inv_255,1.0f);
	m_materials[4].rd=Vec4(m_materials[4].color.x*0.7f*inv_255,m_materials[4].color.y*0.7f*inv_255,m_materials[4].color.z*0.7f*inv_255,1.0f);
	m_materials[4].rs=Vec4(1.0f,1.0f,1.0f,1.0f);
}