#include "GlobalSys.h"

CGlobalSys *g_pGlobalSys = NULL;
CGlobalSys::CGlobalSys()
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

void CGlobalSys::init_Light()
{
	m_light.attr=1; //direction light
	m_light.c_ambient=Vec4::Ones();
	m_light.c_diffuse=Vec4::Ones();
	m_light.c_specular=Vec4::Ones();
	m_light.pos=Vec3::Zero();
	m_light.dir=Vec3(0,1,1).normalized();
}

void CGlobalSys::init_Material()
{
	float inv_255=1/255.0f;
	m_materials[0].name="gray";
	m_materials[0].attr=2; //flat
	m_materials[0].color.x=200;
	m_materials[0].color.y=200;
	m_materials[0].color.z=200;

	m_materials[0].ks=m_materials[0].kd=m_materials[0].ka=1.0f;
	m_materials[0].power=12.0f;
	m_materials[0].ra=Vec4(0.12108f,0.13282f,0.15078f,1.0f);
	m_materials[0].rd=Vec4(0.48432f,0.53128f,0.60312f,1.0f);
	m_materials[0].rs=Vec4(1.0f,1.0f,1.0f,1.0f);

	m_materials[1].name="yellow";
	m_materials[1].attr=2; //flat
	m_materials[1].color.x=245;
	m_materials[1].color.y=241;
	m_materials[1].color.z=134;

	m_materials[1].ks=m_materials[1].kd=m_materials[1].ka=1.0f;
	m_materials[1].power=12.0f;
	m_materials[1].ra=Vec4(m_materials[1].color.x*0.3f*inv_255,m_materials[1].color.y*0.3f*inv_255,m_materials[1].color.z*0.3f*inv_255,1.0f);
	m_materials[1].rd=Vec4(m_materials[1].color.x*0.7f*inv_255,m_materials[1].color.y*0.7f*inv_255,m_materials[1].color.z*0.7f*inv_255,1.0f);
	m_materials[1].rs=Vec4(1.0f,1.0f,1.0f,1.0f);
}