#include "raytracer.h"
#include "GlobalSys.h"
#include "zyk/Objects.h"
#include "zyk/mathematics.h"
#include <windows.h>
#include <WinBase.h>

RayTracer::RayTracer(QWidget *parent)
	: QMainWindow(parent),render_buffer(NULL),viewport_image(g_pGlobalSys->viewport_width,g_pGlobalSys->viewport_height,QImage::Format_RGB888),
	draw_shadow(true),draw_reflect(true),mMax_depth(3)
{
	ui.setupUi(this);
	connect(ui.RenderButton,SIGNAL(clicked()),this,SLOT(renderScene()));
	connect(ui.drawShadowCheck,SIGNAL(clicked()),this,SLOT(drawShadowSet()));
	connect(ui.drawReflectCheck,SIGNAL(clicked()),this,SLOT(drawReflectSet()));

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

	mMax_depth=ui.depthEdit->text().toFloat();

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
	//m_objects.resize(1);
	//zyk::TriMesh* tri_mesh=new zyk::TriMesh;
	//if(!tri_mesh->importMesh(std::string("../data/Mesh/cube.obj")))
	//{
	//	std::cout<<"Can't read mesh successfully!"<<std::endl;
	//	exit(0);
	//}
	////tri_mesh->scaleMesh(Vec3(0.015,0.015,0.015));
	//Vec3 center=tri_mesh->getCenter();
	//tri_mesh->translate(-center);
	//tri_mesh->setMat(&g_pGlobalSys->m_materials[0]);
	//m_objects[0]=tri_mesh;


	m_objects.resize(3);
	m_objects[0]=new zyk::Sphere(Vec3(0,0,-5),2);
	m_objects[0]->setMaterial(&g_pGlobalSys->m_materials[1]);

	m_objects[1]=new zyk::Sphere(Vec3(0,4.5,-5),1.5);
	m_objects[1]->setMaterial(&g_pGlobalSys->m_materials[2]);

	m_objects[2]=new zyk::Sphere(Vec3(5.5,0,-5),1.5);
	m_objects[2]->setMaterial(&g_pGlobalSys->m_materials[0]);

	/*float radian=130.0f*DEG_TO_RAD,radius=3;
	Vec3 vCenter(radius*cos(radian),0.5f,radius*sin(radian));
	m_objects[1]=new zyk::Sphere(vCenter,0.5f);
	m_objects[1]->setMaterial(&g_pGlobalSys->m_materials[2]);

	radian=20.0f*DEG_TO_RAD;
	radius=4;
	vCenter=Vec3(radius*cos(radian),0.7f,radius*sin(radian));
	m_objects[2]=new zyk::Sphere(vCenter,0.8f);
	m_objects[2]->setMaterial(&g_pGlobalSys->m_materials[0]);*/

	/*m_objects[4]=new zyk::Plane3D(Vec3(1,0,0),Vec3(-5,0,0));
	m_objects[4]->setMaterial(&g_pGlobalSys->m_materials[4]);*/

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

inline void fillColor(const Vec4&pColor,zyk::UCHAR3& final_color)
{
	final_color.x=pColor(0)*255;
	final_color.y=pColor(1)*255;
	final_color.z=pColor(2)*255;
}

void calPhongShading(const zyk::Material& pMaterial,zyk::Light& pLight,const Vec3& cam_pos,const Vec3& shad_pos,const Vec3& pNormal,Vec4& pColor)
{
	using zyk::dot_multV4;
	Vec3 view_dir=(cam_pos-shad_pos).normalized();
	Vec3 r_vec=-pLight.dir+2*pLight.dir.dot(pNormal)*pNormal;
	float diff_val=pNormal.dot(pLight.dir);
	float spec_val=pow(max(view_dir.dot(r_vec),0.0f),pMaterial.power);
	if(diff_val<0)
		spec_val=0.0;
	//Phong Shading
	pColor=dot_multV4(pMaterial.ka*pMaterial.ra,pLight.c_ambient)
		+dot_multV4(max(diff_val,0.0f)*pLight.c_diffuse,pMaterial.kd*pMaterial.rd)
		+dot_multV4(spec_val*pLight.c_specular,pMaterial.ks*pMaterial.rs);
	zyk::clip_0_to_1(pColor);
}

Vec4 RayTracer::reflectLighting(const Vec3&origin,const Vec3&ray_dir,int depth)
{
	if(depth==mMax_depth)
		return Vec4(0,0,0,1);

	int near_reflect_id=-1;
	float t,min_t;
	Vec3 tmp_nor,tmp_inter_pt,ref_nor,ref_pt;
	for(int i=0;i<(int)m_objects.size();i++)
	{
		bool is_intersect=m_objects[i]->intersect(origin+ray_dir*0.01f,ray_dir,t,tmp_nor,tmp_inter_pt);
		if(!is_intersect)
			continue;
		if(near_reflect_id==-1||t<min_t)
		{
			min_t=t;
			near_reflect_id=i;
			ref_nor=tmp_nor;
			ref_pt=tmp_inter_pt;
		}
	}

	if(near_reflect_id==-1)
		return Vec4(0,0,0,1);

	Vec3 reflect_dir=(ray_dir-2*ray_dir.dot(ref_nor)*ref_nor).normalized();
	Vec4 shade_color;
	float ref_para=m_objects[near_reflect_id]->getMaterial()->kr;
	calPhongShading(*m_objects[near_reflect_id]->getMaterial(),g_pGlobalSys->m_light,g_pGlobalSys->m_cam.pos,ref_pt,ref_nor,shade_color);
	Vec4 ref_color=reflectLighting(ref_pt+reflect_dir*0.01,reflect_dir,depth+1);
	if(ref_color==Vec4(0,0,0,1)||depth==mMax_depth-1)
		return shade_color;
	else
		return ref_para*shade_color+(1-ref_para)*ref_color;
}

#define ZDEBUG
void RayTracer::ray_tracing(zyk::UCHAR3*buffer)
{
	//check if the objects are valid
	if(!m_objects.size())
		return;
	for(int i=0;i<(int)m_objects.size();i++)
		assert(m_objects[i]&&m_objects[i]->getMaterial());

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
#ifdef ZDEBUG
			int test_x=400,test_y=216;
			if(j==test_x&&i==test_y)
				int z=0;
#endif
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

			if(near_obj_id!=3)
				int z=0;
			bool in_shadow=false;
			//shadow
			if(draw_shadow)
			{
				for(int obj_id=0;obj_id<(int)m_objects.size();obj_id++)
				{
					if(near_obj_id==obj_id)
						continue;
					bool is_intersect=m_objects[obj_id]->intersect(v_inter_pt+g_pGlobalSys->m_light.dir*0.01f,g_pGlobalSys->m_light.dir,t);
					if(is_intersect)
					{
						in_shadow=true;
						break;
					}
				}
			}

			//reflection
			//int near_reflect_id=-1;
			Vec4 ref_color(0,0,0,1);
			if(!in_shadow&&draw_reflect)
			{
				Vec3 reflect_dir=(ray_dir-2*ray_dir.dot(v_normal)*v_normal).normalized();
				ref_color=reflectLighting(v_inter_pt+reflect_dir*0.01f,reflect_dir,0);
			}

			if(near_obj_id==-1)
				continue;	
			int index=row_ind+j;
			if(!in_shadow)
			{
				Vec4 shade_color;
				calPhongShading(*m_objects[near_obj_id]->getMaterial(),g_pGlobalSys->m_light,v_cam.pos,v_inter_pt,v_normal,shade_color);
				if(ref_color!=Vec4(0,0,0,1))
					shade_color=shade_color*m_objects[near_obj_id]->getMaterial()->kr+((1-m_objects[near_obj_id]->getMaterial()->kr)*ref_color);
				zyk::clip_0_to_1(shade_color);
				fillColor(shade_color,buffer[index]);
			}
			else
				fillColor(Vec4::Zero(),buffer[index]);
		}
	}
}

void RayTracer::drawShadowSet()
{
	draw_shadow=ui.drawShadowCheck->isChecked();
}

void RayTracer::drawReflectSet()
{
	draw_reflect=ui.drawReflectCheck->isChecked();
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

//save current picture in the data folder
void RayTracer::savePic()
{
	std::string filename;
	std::cout<<"file name:";
	std::cin>>filename;

	if(viewport_image.save(QString("../data/screenshot/")+QString::fromStdString(filename)+QString(".bmp")))
		std::cout<<"Save image successfully!"<<std::endl;
	else
		std::cout<<"Save image fail!"<<std::endl;
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
	case Qt::Key_S:
		{
			savePic();
			break;
		}
	}
}