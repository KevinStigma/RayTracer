#include "raytracer.h"
#include "GlobalSys.h"
#include "zyk/Objects.h"
#include <windows.h>
#include <WinBase.h>

RayTracer::RayTracer(QWidget *parent)
	: QMainWindow(parent),render_buffer(NULL),viewport_image(g_pGlobalSys->viewport_width,g_pGlobalSys->viewport_height,QImage::Format_RGB888),
	draw_shadow(true)
{
	ui.setupUi(this);
	connect(ui.RenderButton,SIGNAL(clicked()),this,SLOT(renderScene()));
	connect(ui.drawShadowCheck,SIGNAL(clicked()),this,SLOT(drawShadowSet()));

	ui.render_label->setFixedSize(g_pGlobalSys->viewport_width,g_pGlobalSys->viewport_height);
	render_buffer=new zyk::UCHAR3[g_pGlobalSys->pixel_num];
	init_objects();
	renderScene();
}

RayTracer::~RayTracer()
{
	 SAFE_DELETE(g_pGlobalSys);
	 SAFE_DELETE_ARRAY(render_buffer);
	 for(int i=0;i<(int)m_objects.size();i++)
	 {
		 SAFE_DELETE(m_objects[i]);
	 }
}

#define RECORD_TIME
void RayTracer::renderScene()
{
	init_render_buffer(render_buffer);
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

void RayTracer::init_objects()
{
	m_objects.resize(2);
	m_objects[0]=new zyk::Sphere(Vec3(0,0,0),2);
	m_objects[0]->setMat(&g_pGlobalSys->m_materials[1]);

	m_objects[1]=new zyk::Sphere(Vec3(-1.8,1,2),0.5f);
	m_objects[1]->setMat(&g_pGlobalSys->m_materials[2]);
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

void calPhongShading(const zyk::Material& pMaterial,zyk::Light& pLight,const Vec3& cam_pos,const Vec3& shad_pos,const Vec3& pNormal,zyk::UCHAR3& pColor)
{
	using zyk::dot_multV4;
	Vec3 view_dir=(cam_pos-shad_pos).normalized();
	Vec3 r_vec=-pLight.dir+2*pLight.dir.dot(pNormal)*pNormal;
	Vec4 color_pt;
	float diff_val=pNormal.dot(pLight.dir);
	float spec_val=pow(max(view_dir.dot(r_vec),0.0f),pMaterial.power);
	//Phong Shading
	color_pt=dot_multV4(pMaterial.ka*pMaterial.ra,pLight.c_ambient)
		+dot_multV4(max(diff_val,0.0f)*pLight.c_diffuse,pMaterial.kd*pMaterial.rd)
		+dot_multV4(spec_val*pLight.c_specular,pMaterial.ks*pMaterial.rs);
	
	zyk::clip_0_to_1(color_pt);
	pColor.x=color_pt[0]*255;
	pColor.y=color_pt[1]*255;
	pColor.z=color_pt[2]*255;
}

#define DRAW_SHADOW
void RayTracer::ray_tracing(zyk::UCHAR3*buffer)
{
	//check if the objects are valid
	if(!m_objects.size())
		return;
	for(int i=0;i<(int)m_objects.size();i++)
		assert(m_objects[i]&&m_objects[i]->getMat());

	const zyk::Camera& v_cam=g_pGlobalSys->m_cam;
	const Vec4 &view_plane=v_cam.view_plane;
	float inv_width=1/v_cam.viewport_width;
	float inv_height=1/v_cam.viewport_height;
	Vec3 tmp_nor,v_normal;
	Vec3 tmp_inter_pt,v_inter_pt;

	for(int i=0;i<v_cam.viewport_height;i++)
	{
		float ratio_height=(i+0.5f)*inv_height;
		int row_ind=i*v_cam.viewport_width;
		for(int j=0;j<v_cam.viewport_width;j++)
		{
			//cast a ray to test if it intersects an object.
			Vec3 pixel_pos(view_plane(0)+(j+0.5f)*inv_width*v_cam.view_width,
				view_plane(2)-ratio_height*v_cam.view_height,
				v_cam.pos(2)-v_cam.near_clip_z);
			Vec3 ray_dir=(pixel_pos-v_cam.pos).normalized();
			
			float t,min_t;
			int near_obj_id=-1;
			for(int obj_id=0;obj_id<(int)m_objects.size();obj_id++)
			{
				bool is_intersect=m_objects[obj_id]->intersect(v_cam.pos,ray_dir,t,tmp_nor,tmp_inter_pt);
				if(!is_intersect)
					continue;
				if(near_obj_id==-1||t<min_t)
				{
					min_t=t;
					near_obj_id=obj_id;
					v_normal=tmp_nor;
					v_inter_pt=tmp_inter_pt;
				}
			}

			bool in_shadow=false;
			if(draw_shadow)
			{
				for(int obj_id=0;obj_id<(int)m_objects.size();obj_id++)
				{
					if(near_obj_id==obj_id)
						continue;
					bool is_intersect=m_objects[obj_id]->intersect(v_inter_pt,g_pGlobalSys->m_light.dir,t);
					if(is_intersect)
					{
						in_shadow=true;
						break;
					}
				}
			}

			if(near_obj_id==-1)
				continue;	
			int index=row_ind+j;
			if(!in_shadow)
				calPhongShading(*m_objects[near_obj_id]->getMat(),g_pGlobalSys->m_light,v_cam.pos,v_inter_pt,v_normal,buffer[index]);
			else
			{
				buffer[index].x=0;
				buffer[index].y=0;
				buffer[index].z=0;
			}
		}
	}
}

void RayTracer::drawShadowSet()
{
	draw_shadow=ui.drawShadowCheck->isChecked();
}

//here we write some code to test effects of rendering
void RayTracer::renderTest()
{
	zyk::Sphere* small_sphere=dynamic_cast<zyk::Sphere*>(m_objects[1]);
	assert(small_sphere);
	const float radius=3.0f;
	QString filename("../data/test/test_pic");

	for(int i=0;i<36;i++)
	{
		float radian=10.0f*i*DEG_TO_RAD;
		small_sphere->center=Vec3(radius*cos(radian),0.5f,radius*sin(radian));
		init_render_buffer(render_buffer);
		ray_tracing(render_buffer);
		render_2_viewport(render_buffer);
		if(viewport_image.save(filename+QString::number(i,10)+QString(".bmp")))
			std::cout<<"save "<<i<<"'th test picture successfully!"<<std::endl;
	}
}

void RayTracer::keyPressEvent(QKeyEvent *e)
{
	switch(e->key())
	{
	case Qt::Key_A:
		{
			renderTest();			
			break;
		}
	}
}