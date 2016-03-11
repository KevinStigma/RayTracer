#include "raytracer.h"
#include "GlobalSys.h"
#include "zyk/Objects.h"
#include "zyk/mathematics.h"
#include <tchar.h>
#include <typeinfo>
#include <windows.h>
#include <WinBase.h>
#include <QFileDialog>
#include <QString>

RayTracer::RayTracer(QWidget *parent)
	: QMainWindow(parent),render_buffer(NULL),viewport_image(g_pGlobalSys->viewport_width,g_pGlobalSys->viewport_height,QImage::Format_RGB888),
	draw_shadow(false),draw_reflect(true),mMax_depth(2)
{
	ui.setupUi(this);
	setFixedSize(1026,703);
	connect(ui.RenderButton,SIGNAL(clicked()),this,SLOT(renderScene()));
	connect(ui.drawShadowCheck,SIGNAL(clicked()),this,SLOT(drawShadowSet()));
	connect(ui.drawReflectCheck,SIGNAL(clicked()),this,SLOT(drawReflectSet()));
	connect(ui.actionLoad_Scene,SIGNAL(triggered()),this,SLOT(loadScene()));

	ui.render_label->setFixedSize(g_pGlobalSys->viewport_width,g_pGlobalSys->viewport_height);
	render_buffer=new zyk::UCHAR3[g_pGlobalSys->pixel_num];
	initObjects();
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
	initRenderBuffer(render_buffer);
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
	rayTracing(render_buffer);
	renderViewport(render_buffer);
	ui.render_label->setPixmap(QPixmap::fromImage(viewport_image));
	ui.render_label->show();
#ifdef RECORD_TIME
	DWORD end_time=GetTickCount();
	std::cout<<end_time-start_time<<"ms"<<std::endl;
#endif
}

void RayTracer::initRenderBuffer(zyk::UCHAR3*buffer)
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

#define REFRAC_TEST
void RayTracer::initObjects()
{
#ifndef REFRAC_TEST
	m_objects.resize(3);
	m_objects[0]=new zyk::Sphere(Vec3(0,0,-5),2);
	m_objects[0]->setMaterial(&g_pGlobalSys->m_materials[1]);

	m_objects[1]=new zyk::Sphere(Vec3(0,5,-5),1.5);
	m_objects[1]->setMaterial(&g_pGlobalSys->m_materials[2]);

	m_objects[2]=new zyk::Sphere(Vec3(5.5,0,-5),1.5);
	m_objects[2]->setMaterial(&g_pGlobalSys->m_materials[0]);
#else
	m_objects.resize(2);

	//Vec3 triangle_pt[3]={Vec3(1.5,4,-100),Vec3(-0.5,-1,-100),Vec3(5.5,-1,-100)};
	//m_objects[0]=new zyk::Triangle(triangle_pt);
	m_objects[0]=new zyk::Sphere(Vec3(-2.0,0,-5),2);
	m_objects[0]->setMaterial(&g_pGlobalSys->m_materials[2]);
	
	m_objects[1]=new zyk::Sphere(Vec3(0,0,1),1.5);
	m_objects[1]->setMaterial(&g_pGlobalSys->m_materials[6]);
	
	//m_objects[1]=new zyk::Sphere(Vec3(2,-1,-2),2);
	//m_objects[1]->setMaterial(&g_pGlobalSys->m_materials[6]);

#endif
}

void RayTracer::renderViewport(zyk::UCHAR3*buffer)
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

void RayTracer::calPhongShading_oneLight(const zyk::Material& pMaterial,const zyk::Light& pLight,const Vec3& cam_pos,const Vec3& shad_pos,const Vec3& pNormal,Vec4& pColor)
{
	using zyk::dot_multV4;
	Vec3 view_dir=(cam_pos-shad_pos).normalized();
	Vec3 r_vec=-pLight.dir+2*pLight.dir.dot(pNormal)*pNormal;
	float diff_val=pNormal.dot(pLight.dir);
	float spec_val=pow(max(view_dir.dot(r_vec),0.0f),pMaterial.power);
	if(diff_val<0)
		spec_val=0.0;
	//Phong Shading
	pColor=dot_multV4(max(diff_val,0.0f)*pLight.c_diffuse,pMaterial.rd)
		+dot_multV4(spec_val*pLight.c_specular,pMaterial.rs);
}

Vec4 RayTracer::calPhongShading_manyLights(const zyk::Material& pMaterial,const std::vector<bool>& is_lighting,const Vec3& cam_pos,const Vec3& shad_pos,const Vec3& pNormal)
{
	assert(is_lighting.size()==g_pGlobalSys->mLightNum);
	Vec4 shade_color=pMaterial.ra;
	for(int i=0;i<g_pGlobalSys->mLightNum;i++)
	{
		if(!is_lighting[i])
			continue;
		Vec4 lighting_color;
		calPhongShading_oneLight(pMaterial,g_pGlobalSys->mLights[i],cam_pos,shad_pos,pNormal,lighting_color);
		shade_color+=lighting_color;
	}
	zyk::clip_0_to_1(shade_color);
	return shade_color;
}

void RayTracer::shadowCheck(int lightsNum,const zyk::Light* lights,const Vec3&intersect_pt,std::vector<bool>& is_lighting)
{
	assert(is_lighting.size()==lightsNum);
	float t;
	for(int l_ind=0;l_ind<lightsNum;l_ind++)
	{
		bool in_shadow=false;
		for(int obj_id=0;obj_id<(int)m_objects.size();obj_id++)
		{
			bool is_intersect=m_objects[obj_id]->intersect(intersect_pt+lights[l_ind].dir*0.01f,lights[l_ind].dir,t);
			if(is_intersect)
			{
				in_shadow=true;
				break;
			}
		}
		if(in_shadow)
			is_lighting[l_ind]=false;
	}
}

/**
	get a 3D ray based on a given point
*/
Vec3 RayTracer::rayCasting(const zyk::Camera&pCam,int x,int y)
{
	static float inv_width=1.0f/pCam.viewport_width;
	static float inv_height=1.0f/pCam.viewport_height;

	Vec3 pixel_pos(pCam.view_plane(0)+(x+0.5f)*inv_width*pCam.view_width,
		pCam.view_plane(2)-(y+0.5f)*inv_height*pCam.view_height,
		pCam.pos(2)-pCam.near_clip_z);
	return (pixel_pos-pCam.pos).normalized();
}

void RayTracer::intersectionCheck(const std::vector<zyk::Object*>& pObjects,const Vec3& origin,const Vec3& ray_dir,
	int& near_obj_id,Vec3& inter_pt_nor,Vec3&inter_pt)
{
	float t,min_t;
	Vec3 tmp_nor;
	Vec3 tmp_inter_pt;
	for(int obj_id=0;obj_id<(int)pObjects.size();obj_id++)
	{
		bool is_intersect=pObjects[obj_id]->intersect(origin,ray_dir,t,tmp_nor,tmp_inter_pt);
		if(!is_intersect)
			continue;
		if(near_obj_id==-1||t<min_t)
		{
			min_t=t;
			near_obj_id=obj_id;
			inter_pt_nor=tmp_nor;
			inter_pt=tmp_inter_pt;
		}
	}
}
inline void combineColor(float ref_para,const Vec4&pRefColor,Vec4& pShadeColor)
{
	pShadeColor=pShadeColor+ref_para*pRefColor;
	zyk::clip_0_to_1(pShadeColor);
	/*
	if(pRefColor!=Vec4(0,0,0,1))
	{
		pShadeColor=(1-ref_para)*pShadeColor+ref_para*pRefColor;
		zyk::clip_0_to_1(pShadeColor);
	}*/
}

void RayTracer::fresnel(float cos1,float cos2,const float rei[],float &kr)const
{
	//in fact, this function should check if there exists total internal reflection,
	//but now that its father function refractRay has this detection, I don't consider
	//this situation in the function
	float fr1,fr2;
	fr1=(rei[1]*cos1-rei[0]*cos2)/(rei[1]*cos1+rei[0]*cos2);
	fr2=(rei[0]*cos2-rei[1]*cos1)/(rei[0]*cos2+rei[1]*cos1);
	kr=(fr1*fr1+fr2*fr2)*0.5f;
}

bool RayTracer::refractRay(const Vec3& origin,const Vec3&incident_dir,const Vec3& normal,const float rei[],
	Vec3& refract_dir,float& ref_weight)const
{
	float eta=rei[0]/rei[1];
	float c1=normal.dot(incident_dir),c2=1-eta*eta*(1-c1*c1);
	if(c2<0)
	{
		ref_weight=1.0f;
		return false;
	}

	Vec3 v_nor=normal;
	if(c1<0.0f)
		c1=-c1;
	else
		v_nor*=-1;
	refract_dir=eta*incident_dir+(eta*c1-sqrt(c2))*v_nor;
	fresnel(c1,fabs(refract_dir.dot(normal)),rei,ref_weight);
	return true;
}

Vec4 RayTracer::castRayShading(const Vec3& origin,const Vec3& ray_dir,int depth,float input_rei)
{
	if(depth==mMax_depth+1)
		return Vec4(0,0,0,1);
	int near_obj_id=-1;
	Vec3 inter_nor,inter_pt;
	intersectionCheck(m_objects,origin,ray_dir,near_obj_id,inter_nor,inter_pt);

	if(near_obj_id==-1)
		return Vec4(0,0,0,1);
	
	Vec3 reflect_dir=(ray_dir-2*ray_dir.dot(inter_nor)*inter_nor).normalized();
	Vec4 ref_color=castRayShading(inter_pt+reflect_dir*0.01,reflect_dir,depth+1,input_rei);
	Vec4 shade_color;
	
	if(m_objects[near_obj_id]->getMaterial()->is_solid)
	{
		std::vector<bool> is_lighting(g_pGlobalSys->mLightNum,true);
		if(draw_shadow)
			shadowCheck(g_pGlobalSys->mLightNum,g_pGlobalSys->mLights,inter_pt,is_lighting);

		shade_color=calPhongShading_manyLights(*m_objects[near_obj_id]->getMaterial(),is_lighting,g_pGlobalSys->m_cam.pos,
			inter_pt,inter_nor);

		float ref_para=m_objects[near_obj_id]->getMaterial()->kr;
		combineColor(ref_para,ref_color,shade_color);	
	}
	else
	{
		float rei[2]={input_rei,m_objects[near_obj_id]->getMaterial()->rei};
		if(FCMP(rei[0],rei[1])&&rei[0]>1.0f)// the ray is leaving out the dielectric
			rei[1]=1.0f;

		Vec3 refra_dir;
		float ref_weight,refra_weight;
		bool is_refract=refractRay(inter_pt,ray_dir,inter_nor,rei,refra_dir,ref_weight);
		if(is_refract)
		{
			refra_weight=1-ref_weight;
			float out_rei;
			(typeid(*m_objects[near_obj_id])==typeid(zyk::Triangle))?out_rei=rei[0]:out_rei=rei[1];
			Vec4 refra_color=castRayShading(inter_pt+refra_dir*0.01,refra_dir,depth+1,out_rei);
			shade_color=ref_color*ref_weight+refra_color*refra_weight;
		}
		else
			shade_color=ref_color;
		zyk::clip_0_to_1(shade_color);
	}
	return shade_color;
}

#define ZDEBUG
void RayTracer::rayTracing(zyk::UCHAR3*buffer)
{
	//check if the objects are valid
	if(!m_objects.size())
		return;
	for(int i=0;i<(int)m_objects.size();i++)
		assert(m_objects[i]&&m_objects[i]->getMaterial());

	const zyk::Camera& v_cam=g_pGlobalSys->m_cam;
	const zyk::Light* lights=g_pGlobalSys->mLights;
	int lightsNum=g_pGlobalSys->mLightNum;
	Vec3 v_normal,v_inter_pt;

	for(int i=0;i<v_cam.viewport_height;i++)
	{
		int row_ind=i*v_cam.viewport_width;
		for(int j=0;j<v_cam.viewport_width;j++)
		{	
#ifdef ZDEBUG
			int test_x=432,test_y=304;
			if(j==test_x&&i==test_y)
				int z=0;
#endif
			Vec3 ray_dir=rayCasting(v_cam,j,i);
			Vec4 shade_color=castRayShading(v_cam.pos,ray_dir,0);
			fillColor(shade_color,buffer[row_ind+j]);
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

void RayTracer::loadScene()
{
	QString filename=QFileDialog::getOpenFileName(this,tr("Load Scene"),".",
		"scene scripts(*.txt)", 0);
	if(!filename.size())
		return; 
	std::ifstream in(filename.toStdWString());
	if(!in.is_open())
	{
		std::cout<<"Can't open the file!"<<std::endl;
		return;
	}

	int n;
	in>>n;
	std::string object_name;
	
	int mat_id,i;
	for(i=0;i<(int)m_objects.size();i++)
		delete m_objects[i];
	m_objects.resize(n);
	for(i=0;i<n;i++)
	{
		in>>object_name;
		if(object_name=="Sphere")
		{
			float x,y,z,r;
			in>>x>>y>>z>>r;
			m_objects[i]=new zyk::Sphere(Vec3(x,y,z),r);
		}
		else if(object_name=="Triangle")
		{
			Vec3 pt[3];
			for(int j=0;j<3;j++)
				in>>pt[j][0]>>pt[j][1]>>pt[j][2];
			m_objects[i]=new zyk::Triangle(pt);
		}
		else if(object_name=="Mesh")
		{
			std::string mesh_name;
			in>>mesh_name;
			m_objects[i]=new zyk::TriMesh(mesh_name);
		}
		else
		{
			std::cout<<"The Scene script has error!"<<std::endl;
			break;
		}
		in>>mat_id;
		m_objects[i]->setMaterial(&g_pGlobalSys->m_materials[mat_id]);
	}
	if(i==n)
		std::cout<<"Load scene successfully!"<<std::endl;

}

void RayTracer::renderTest()
{
	//here we write some code to test effects of rendering
	zyk::Sphere* small_sphere=dynamic_cast<zyk::Sphere*>(m_objects[1]);
	assert(small_sphere);
	const float radius=4.5f;
	QString filename("../data/test/test_pic");

	for(int i=0;i<36;i++)
	{
		float radian=10.0f*i*DEG_TO_RAD;
		small_sphere->center=Vec3(radius*cos(radian),0.0f,radius*sin(radian)-5);
		initRenderBuffer(render_buffer);
		rayTracing(render_buffer);
		renderViewport(render_buffer);
		if(viewport_image.save(filename+QString::number(i,10)+QString(".bmp")))
			std::cout<<"save "<<i<<"'th test picture successfully!"<<std::endl;
	}
}

void RayTracer::savePic()const
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