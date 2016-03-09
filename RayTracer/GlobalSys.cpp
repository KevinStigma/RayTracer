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
	float tmp_kr=0.4;
	int index=0;
	m_materials[index].name="gray";
	m_materials[index].attr=2; 
	m_materials[index].color.x=154;
	m_materials[index].color.y=169;
	m_materials[index].color.z=192;

	m_materials[index].ks=m_materials[index].kd=m_materials[index].ka=1.0f;
	m_materials[index].kr=tmp_kr;
	m_materials[index].power=50.0f;
	m_materials[index].ra=Vec4(m_materials[index].color.x*0.2f*inv_255,m_materials[index].color.y*0.2f*inv_255,m_materials[index].color.z*0.2f*inv_255,1.0f);
	m_materials[index].rd=Vec4(m_materials[index].color.x*0.8f*inv_255,m_materials[index].color.y*0.8f*inv_255,m_materials[index].color.z*0.8f*inv_255,1.0f);
	//m_materials[0].rs=Vec4(1.0f,1.0f,1.0f,1.0f);
	m_materials[index].rs=Vec4(0.1f,0.1f,0.1f,1.0f);
	m_materials[index].is_solid=true;
	
	index=1;
	m_materials[index].name="yellow";
	m_materials[index].attr=2; 
	m_materials[index].color.x=245;
	m_materials[index].color.y=241;
	m_materials[index].color.z=134;

	m_materials[index].ks=m_materials[index].kd=m_materials[index].ka=1.0f;
	m_materials[index].kr=tmp_kr;
	m_materials[index].power=50.0f;
	m_materials[index].ra=Vec4(m_materials[index].color.x*0.3f*inv_255,m_materials[index].color.y*0.3f*inv_255,m_materials[index].color.z*0.3f*inv_255,1.0f);
	m_materials[index].rd=Vec4(m_materials[index].color.x*0.7f*inv_255,m_materials[index].color.y*0.7f*inv_255,m_materials[index].color.z*0.7f*inv_255,1.0f);
	//m_materials[1].rs=Vec4(1.0f,1.0f,1.0f,1.0f);
	m_materials[index].rs=Vec4(0.1f,0.1f,0.1f,1.0f);
	m_materials[index].is_solid=true;

	index=2;
	m_materials[index].name="purple";
	m_materials[index].attr=2; 
	m_materials[index].color.x=147;
	m_materials[index].color.y=82;
	m_materials[index].color.z=124;

	m_materials[index].ks=m_materials[index].kd=m_materials[index].ka=1.0f;
	m_materials[index].kr=tmp_kr;
	m_materials[index].power=50.0f;
	m_materials[index].ra=Vec4(m_materials[index].color.x*0.3f*inv_255,m_materials[index].color.y*0.3f*inv_255,m_materials[index].color.z*0.3f*inv_255,1.0f);
	m_materials[index].rd=Vec4(m_materials[index].color.x*0.7f*inv_255,m_materials[index].color.y*0.7f*inv_255,m_materials[index].color.z*0.7f*inv_255,1.0f);
	//m_materials[2].rs=Vec4(1.0f,1.0f,1.0f,1.0f);
	m_materials[index].rs=Vec4(0.1f,0.1f,0.1f,1.0f);
	m_materials[index].is_solid=true;

	index=3;
	m_materials[index].name="cyan";
	m_materials[index].attr=2; 
	m_materials[index].color.x=200;
	m_materials[index].color.y=255;
	m_materials[index].color.z=255;

	m_materials[index].ks=m_materials[index].kd=m_materials[index].ka=1.0f;
	m_materials[index].kr=tmp_kr;
	m_materials[index].power=50.0f;
	m_materials[index].ra=Vec4(m_materials[index].color.x*0.3f*inv_255,m_materials[index].color.y*0.3f*inv_255,m_materials[index].color.z*0.3f*inv_255,1.0f);
	m_materials[index].rd=Vec4(m_materials[index].color.x*0.7f*inv_255,m_materials[index].color.y*0.7f*inv_255,m_materials[index].color.z*0.7f*inv_255,1.0f);
	//m_materials[3].rs=Vec4(1.0f,1.0f,1.0f,1.0f);
	m_materials[index].rs=Vec4(0.1f,0.1f,0.1f,1.0f);
	m_materials[index].is_solid=true;

	index=4;
	m_materials[index].name="red";
	m_materials[index].attr=2; 
	m_materials[index].color.x=215;
	m_materials[index].color.y=93;
	m_materials[index].color.z=93;

	m_materials[index].ks=m_materials[index].kd=m_materials[index].ka=1.0f;
	m_materials[index].kr=tmp_kr;
	m_materials[index].power=50.0f;
	m_materials[index].ra=Vec4(m_materials[index].color.x*0.3f*inv_255,m_materials[index].color.y*0.3f*inv_255,m_materials[index].color.z*0.3f*inv_255,1.0f);
	m_materials[index].rd=Vec4(m_materials[index].color.x*0.7f*inv_255,m_materials[index].color.y*0.7f*inv_255,m_materials[index].color.z*0.7f*inv_255,1.0f);
	//m_materials[4].rs=Vec4(1.0f,1.0f,1.0f,1.0f);
	m_materials[index].rs=Vec4(0.1f,0.1f,0.1f,1.0f);
	m_materials[index].is_solid=true;

	index=5;
	m_materials[index].name="red";
	m_materials[index].attr=2; 
	m_materials[index].color.x=215;
	m_materials[index].color.y=93;
	m_materials[index].color.z=93;

	m_materials[index].ks=m_materials[index].kd=m_materials[index].ka=1.0f;
	m_materials[index].kr=tmp_kr;
	m_materials[index].power=50.0f;
	m_materials[index].ra=Vec4(m_materials[index].color.x*0.3f*inv_255,m_materials[index].color.y*0.3f*inv_255,m_materials[index].color.z*0.3f*inv_255,1.0f);
	m_materials[index].rd=Vec4(m_materials[index].color.x*0.7f*inv_255,m_materials[index].color.y*0.7f*inv_255,m_materials[index].color.z*0.7f*inv_255,1.0f);
	//m_materials[4].rs=Vec4(1.0f,1.0f,1.0f,1.0f);
	m_materials[index].rs=Vec4(0.1f,0.1f,0.1f,1.0f);
	m_materials[index].is_solid=true;

	index=6;
	m_materials[index].name="glass";
	m_materials[index].attr=2; 
	m_materials[index].color.x=0;
	m_materials[index].color.y=0;
	m_materials[index].color.z=0;

	m_materials[index].ks=m_materials[index].kd=m_materials[index].ka=1.0f;
	m_materials[index].kr=tmp_kr;
	m_materials[index].power=50.0f;
	m_materials[index].ra=Vec4(m_materials[index].color.x*0.3f*inv_255,m_materials[index].color.y*0.3f*inv_255,m_materials[index].color.z*0.3f*inv_255,1.0f);
	m_materials[index].rd=Vec4(m_materials[index].color.x*0.7f*inv_255,m_materials[index].color.y*0.7f*inv_255,m_materials[index].color.z*0.7f*inv_255,1.0f);
	//m_materials[4].rs=Vec4(1.0f,1.0f,1.0f,1.0f);
	m_materials[index].rs=Vec4(0.0f,0.0f,0.0f,1.0f);
	m_materials[index].is_solid=false;
	m_materials[index].rei=1.157f;
}