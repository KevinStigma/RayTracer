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

	num[0]=ui.expEdit->text();
	g_pGlobalSys->m_light.pf=num[0].toFloat();

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

void calPhongShading(zyk::Material& pMaterial,zyk::Light& pLight,const Vec3& cam_pos,const Vec3& shad_pos,const Vec3& pNormal,zyk::UCHAR3& pColor)
{
	using zyk::dot_multV4;
	Vec3 view_dir=(cam_pos-shad_pos).normalized();
	Vec3 r_vec=-pLight.dir+2*pLight.dir.dot(pNormal)*pNormal;
	Vec4 color_pt;
	float val=pNormal.dot(pLight.dir);
	float spec_val=pow(max(view_dir.dot(r_vec),0.0f),pLight.pf);
	color_pt=dot_multV4(pMaterial.ka*pMaterial.ra,pLight.c_ambient)
		+dot_multV4(max(val,0.0f)*pLight.c_diffuse,pMaterial.kd*pMaterial.rd)
		+dot_multV4(spec_val*pLight.c_specular,pMaterial.ks*pMaterial.rs);
	
	zyk::clip_0_to_1(color_pt);
	pColor.x=color_pt[0]*255;
	pColor.y=color_pt[1]*255;
	pColor.z=color_pt[2]*255;
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
			//cast a ray to test if it intersects an object.
			Vec3 pixel_pos(view_plane(0)+(j+0.5f)*inv_width*v_cam.view_width,
				view_plane(2)+ratio_height*v_cam.view_height,
				v_cam.pos(2)-v_cam.near_clip_z);
			Vec3 ray_dir=(pixel_pos-v_cam.pos).normalized();
			float t;
			bool is_intersect=sphere.intersect(v_cam.pos,ray_dir,t);
			if(!is_intersect)
				continue;

			Vec3 inters_pt=v_cam.pos+t*ray_dir;
			Vec3 v_normal=(inters_pt-sphere.center).normalized();
			int index=row_ind+j;
			calPhongShading(g_pGlobalSys->m_materials[0],g_pGlobalSys->m_light,v_cam.pos,inters_pt,v_normal,buffer[index]);
		}
	}
}
