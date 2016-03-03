#include "raytracer.h"
#include "GlobalSys.h"
#include <windows.h>
#include <WinBase.h>

RayTracer::RayTracer(QWidget *parent)
	: QMainWindow(parent),render_buffer(NULL),viewport_image(g_pGlobalSys->viewport_width,g_pGlobalSys->viewport_height,QImage::Format_RGB888),sphere(Vec3(0,0,0),2)
{
	ui.setupUi(this);
	connect(ui.RenderButton,SIGNAL(clicked()),this,SLOT(renderScene()));

	ui.render_label->setFixedSize(g_pGlobalSys->viewport_width,g_pGlobalSys->viewport_height);
	render_buffer=new zyk::UCHAR3[g_pGlobalSys->pixel_num];
	init_render_buffer(render_buffer);
	renderScene();
}

RayTracer::~RayTracer()
{
	 SAFE_DELETE(g_pGlobalSys);
	 SAFE_DELETE_ARRAY(render_buffer);
}

#define RECORD_TIME
void RayTracer::renderScene()
{
	QString num[3];
	num[0]=ui.posXEdit->text();
	num[1]=ui.posYEdit->text();
	num[2]=ui.posZEdit->text();
	zyk::Camera& pCam=g_pGlobalSys->m_cam;
	
	for(int i=0;i<3;i++)
		pCam.pos[i]=num[i].toFloat();
	pCam.backup_pos=pCam.pos;
	pCam.reset_camera_matrix();


#ifdef RECORD_TIME
	DWORD start_time=GetTickCount();
#endif
	ray_tracing(render_buffer);
	render_2_viewport(render_buffer);
	ui.render_label->setPixmap(QPixmap::fromImage(viewport_image));
	ui.render_label->show();
#ifdef RECORD_TIME
	DWORD end_time=GetTickCount();
	std::cout<<end_time-start_time<<"ms"<<std::endl;
#endif
}

void RayTracer::init_render_buffer(zyk::UCHAR3*buffer)
{
	if(!buffer)
		return;
	for(int i=0;i<g_pGlobalSys->pixel_num;i++)
	{
		buffer[i].m[0]=0;
		buffer[i].m[1]=0;
		buffer[i].m[2]=0;
	}
}
void RayTracer::render_2_viewport(zyk::UCHAR3*buffer)
{
	for(int i=0;i<g_pGlobalSys->viewport_height;i++)
	{
		int row_ind=i*g_pGlobalSys->viewport_width;
		for(int j=0;j<g_pGlobalSys->viewport_width;j++)
		{
			int index=row_ind+j;
			uint color=0;
			zyk::UCHAR3 tmp_color=buffer[index];
			viewport_image.setPixel(j,i,qRgb((int)tmp_color.x,(int)tmp_color.y,(int)tmp_color.z));
		}
	}
}

void RayTracer::ray_tracing(zyk::UCHAR3*buffer)
{
	const zyk::Camera& v_cam=g_pGlobalSys->m_cam;
	const Vec4 &view_plane=v_cam.view_plane;
	float inv_width=1/v_cam.viewport_width;
	float inv_height=1/v_cam.viewport_height;

	for(int i=0;i<v_cam.viewport_height;i++)
	{
		float ratio_height=(i+0.5f)*inv_height;
		int row_ind=i*v_cam.viewport_width;
		for(int j=0;j<v_cam.viewport_width;j++)
		{
			Vec3 pixel_pos(view_plane(0)+(j+0.5f)*inv_width*v_cam.view_width,
				view_plane(2)+ratio_height*v_cam.view_height,
				v_cam.pos(2)-v_cam.near_clip_z);
			Vec3 ray_dir=(pixel_pos-v_cam.pos).normalized();
			float t;
			bool is_intersect=sphere.intersect(v_cam.pos,ray_dir,t);
			if(!is_intersect)
				continue;

			int index=row_ind+j;
			buffer[index].x=255;
			buffer[index].y=0;
			buffer[index].z=0;
		}
	}
}
