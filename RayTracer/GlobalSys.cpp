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
	m_light.pf=10.0f;
}

void CGlobalSys::init_Material()
{
	m_materials[0].name="gray";
	m_materials[0].attr=2; //flat
	m_materials[0].color.x=200;
	m_materials[0].color.y=200;
	m_materials[0].color.z=200;

	m_materials[0].power=m_materials[0].ks=m_materials[0].kd=m_materials[0].ka=1.0f;
	m_materials[0].ra=Vec4(0.12108f,0.13282f,0.15078f,1.0f);
	m_materials[0].rd=Vec4(0.48432f,0.53128f,0.60312f,1.0f);
	m_materials[0].rs=Vec4(1.0f,1.0f,1.0f,1.0f);
}