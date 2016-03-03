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
}