#include "raytracer.h"
#include "GlobalSys.h"
#include "zyk/Objects.h"
#include "zyk/mathematics.h"
#include "zyk/RandomGenerator.h"
#include <tchar.h>
#include <typeinfo>
#include <windows.h>
#include <WinBase.h>
#include <QFileDialog>
#include <QString>

RayTracer::RayTracer(QWidget *parent)
	: QMainWindow(parent),render_buffer(NULL),mRandGen(NULL),viewport_image(g_pGlobalSys->viewport_width,g_pGlobalSys->viewport_height,QImage::Format_RGB888),
	draw_shadow(false),draw_reflect(true),mMax_depth(2),m_render_type(MC_PATH_TRACING),mSampleNum(64)
{
	ui.setupUi(this);
	setFixedSize(1026,703);
	connect(ui.RenderButton,SIGNAL(clicked()),this,SLOT(renderScene()));
	connect(ui.drawShadowCheck,SIGNAL(clicked()),this,SLOT(drawShadowSet()));
	connect(ui.drawReflectCheck,SIGNAL(clicked()),this,SLOT(drawReflectSet()));
	connect(ui.actionLoad_Scene,SIGNAL(triggered()),this,SLOT(loadScene()));
	connect(ui.actionGeneral_RayTracing,SIGNAL(triggered()),this,SLOT(setGeneRayTracing()));
	connect(ui.actionMC_PathTracing,SIGNAL(triggered()),this,SLOT(setMCPathTracing()));

	ui.render_label->setFixedSize(g_pGlobalSys->viewport_width,g_pGlobalSys->viewport_height);
	render_buffer=new zyk::UCHAR3[g_pGlobalSys->pixel_num];
	mRandGen=new zyk::RandomGenerator;
	initObjects();
	
	initRenderBuffer(render_buffer);
	renderViewport(render_buffer);
	ui.render_label->setPixmap(QPixmap::fromImage(viewport_image));
	ui.render_label->show();
	//renderScene();
}

RayTracer::~RayTracer()
{
	 SAFE_DELETE(g_pGlobalSys);
	 SAFE_DELETE_ARRAY(render_buffer);
	 SAFE_DELETE(mRandGen);
	 for(int i=0;i<(int)m_objects.size();i++)
	 {
		 SAFE_DELETE(m_objects[i]);
	 }
}

void RayTracer::setGeneRayTracing()
{
	ui.actionGeneral_RayTracing->setChecked(true);
	ui.actionMC_PathTracing->setChecked(false);
	m_render_type=GENERAL_RAY_TRACING;
	std::cout<<"Render Type:General Ray Tracing"<<std::endl;
}

void RayTracer::setMCPathTracing()
{
	ui.actionGeneral_RayTracing->setChecked(false);
	ui.actionMC_PathTracing->setChecked(true);
	m_render_type=MC_PATH_TRACING;
	std::cout<<"Render Type:Monte Carlo Path Tracing"<<std::endl;
}

void RayTracer::loadParaFromUI()
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

	mMax_depth=ui.depthEdit->text().toInt();
	mSampleNum=ui.sampleEdit->text().toInt();
	light_intensity=ui.lightIntenEdit->text().toFloat();
}

#define RECORD_TIME
void RayTracer::renderScene()
{
	initRenderBuffer(render_buffer);
	loadParaFromUI();	

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
	m_objects.resize(2);

	m_objects[0]=new zyk::Sphere(Vec3(-2.2,0,-5),2);
	m_objects[0]->setMaterial(&g_pGlobalSys->m_materials[2]);

	m_objects[1]=new zyk::Sphere(Vec3(0,0,1),1.5);
	m_objects[1]->setMaterial(&g_pGlobalSys->m_materials[6]);
#else
	m_objects.resize(8);
	std::vector<zyk::TriMesh*> meshes;
	meshes.resize(6);

	zyk::TriMesh* tri_mesh=new zyk::TriMesh("../data/scene/1/lightsource.obj");
	tri_mesh->setMaterial(g_pGlobalSys->getMaterialByName("light_source"));
	m_objects[0]=tri_mesh;
	meshes[0]=tri_mesh;

	tri_mesh=new zyk::TriMesh("../data/scene/1/wall1.obj");
	tri_mesh->setNormal(1,0,0);
	tri_mesh->setMaterial(g_pGlobalSys->getMaterialByName("red"));
	m_objects[1]=tri_mesh;
	meshes[1]=tri_mesh;

	tri_mesh=new zyk::TriMesh("../data/scene/1/wall2.obj");
	tri_mesh->setNormal(0,-1,0);
	tri_mesh->setMaterial(g_pGlobalSys->getMaterialByName("gray"));
	m_objects[2]=tri_mesh;
	meshes[2]=tri_mesh;

	tri_mesh=new zyk::TriMesh("../data/scene/1/wall3.obj");
	tri_mesh->setNormal(0,0,1);
	tri_mesh->setMaterial(g_pGlobalSys->getMaterialByName("gray"));
	m_objects[3]=tri_mesh;
	meshes[3]=tri_mesh;

	tri_mesh=new zyk::TriMesh("../data/scene/1/wall5.obj");
	tri_mesh->setNormal(0,1,0);
	tri_mesh->setMaterial(g_pGlobalSys->getMaterialByName("gray"));
	m_objects[4]=tri_mesh;
	meshes[4]=tri_mesh;

	tri_mesh=new zyk::TriMesh("../data/scene/1/wall4.obj");
	tri_mesh->setNormal(-1,0,0);
	tri_mesh->setMaterial(g_pGlobalSys->getMaterialByName("blue"));
	m_objects[5]=tri_mesh;
	meshes[5]=tri_mesh;

	m_objects[6]=new zyk::Sphere(Vec3(-2.3852720f,-3.4f,10.43973f),1.6f);
	m_objects[6]->setMaterial(g_pGlobalSys->getMaterialByName("mirrow"));

	m_objects[7]=new zyk::Sphere(Vec3(2.24f,-3.4f,13.9258f),1.6f);
	m_objects[7]->setMaterial(g_pGlobalSys->getMaterialByName("glass"));

	Vec3 trans_vec(0.0f,-5.0f,12.0f);
	for(int i=0;i<(int)meshes.size();i++)
		meshes[i]->translate(trans_vec);

	g_pGlobalSys->generateAreaLights(meshes[0]);
#endif
}

inline Vec4 getBlack()
{
	return Vec4(0,0,0,1);
}

inline Vec4 getWhite()
{
	return Vec4(1,1,1,1);
}

inline bool isBlack(const Vec4& color)
{
	return (FCMP(color(0),0)&&FCMP(color(1),0)&&FCMP(color(2),0));
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
	final_color.x=pColor(0)*255.f+0.5f;
	final_color.y=pColor(1)*255.f+0.5f;
	final_color.z=pColor(2)*255.f+0.5f;
}

inline void fillColor_for_MC_Sampling(const Vec4&pColor,zyk::UCHAR3& final_color)
{
	for(int i=0;i<3;i++)
	{
		int c=int(pow(pColor[i],0.4545f)*255.0f+0.5f);
		c=(c>255)?255:c;
		final_color.m[i]=c;
	}
}


void RayTracer::calPhongShading_oneLight(const zyk::Material& pMaterial,const zyk::Light& pLight,const Vec3& cam_pos,
	const Vec3& shad_pos,const Vec3& pNormal,Vec4& pColor,bool calSpecular)
{
	using zyk::dot_multV4;
	Vec3 view_dir=(cam_pos-shad_pos).normalized(),light_dir,light_pos;
	Vec4 light_intensity;
	pLight.getIlluminatinInfo(shad_pos,light_dir,light_intensity,light_pos);
	Vec3 r_vec;
	float diff_val=pNormal.dot(light_dir);
	float spec_val;
	if(diff_val<=0||!calSpecular)
		spec_val=0;
	else
	{
		r_vec=-light_dir+2*light_dir.dot(pNormal)*pNormal;
		spec_val=pow(max(view_dir.dot(r_vec),0.0f),pMaterial.power);
	}
	//Phong Shading
	pColor=dot_multV4(max(diff_val,0.0f)*light_intensity,pMaterial.rd*pMaterial.kd)
		+dot_multV4(spec_val*light_intensity,pMaterial.rs*pMaterial.ks);
}

Vec4 RayTracer::calPhongShading_manyLights(const zyk::Material& pMaterial,const std::vector<bool>& is_lighting,
	const Vec3& cam_pos,const Vec3& shad_pos,const Vec3& pNormal,bool calSpecular)
{
	assert(is_lighting.size()==g_pGlobalSys->mLightNum);
	Vec4 shade_color=pMaterial.ra*pMaterial.ka;
	for(int i=0;i<g_pGlobalSys->mLightNum;i++)
	{
		if(!is_lighting[i])
			continue;
		Vec4 lighting_color;
		calPhongShading_oneLight(pMaterial,g_pGlobalSys->mLights[i],cam_pos,shad_pos,pNormal,lighting_color,calSpecular);
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
		Vec3 light_pos;
		Vec3 light_dir=lights[l_ind].getLightingDirection(intersect_pt,light_pos);
		for(int obj_id=0;obj_id<(int)m_objects.size();obj_id++)
		{
			bool is_intersect=m_objects[obj_id]->intersect(intersect_pt+light_dir*0.01f,light_dir,t);
			if(is_intersect&&m_objects[obj_id]->getMaterial()->type!=zyk::AREA_LIGHT)
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
Vec3 RayTracer::getRayDirection(const zyk::Camera&pCam,float x,float y)
{
	static float inv_width=1.0f/pCam.viewport_width;
	static float inv_height=1.0f/pCam.viewport_height;

	Vec3 pixel_pos(pCam.view_plane(0)+x*inv_width*pCam.view_width,
		pCam.view_plane(2)-y*inv_height*pCam.view_height,
		pCam.pos(2)-pCam.near_clip_z);
	return (pixel_pos-pCam.pos).normalized();
}

Vec3 RayTracer::getRayDirection_randSampling(const zyk::Camera&pCam,float x,float y)
{
	//Here we select one point in the pixel randomly to cast a ray  
	float r1=mRandGen->getRand();
	float r2=mRandGen->getRand();

	return getRayDirection(pCam,x+r1,y+r2);
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
}

void RayTracer::fresnel(float cos1,float cos2,const float rei[],float &kr)const
{
	//in fact, this function should check if there exists total internal reflection,
	//but now that its father function refractRay has this detection, so I don't consider
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

Vec4 RayTracer::castRayShading_RayTracing(const Vec3& origin,const Vec3& ray_dir,int depth,float input_rei)
{
	if(depth==mMax_depth+1)
		return getBlack();
	int near_obj_id=-1;
	Vec3 inter_nor,inter_pt;
	intersectionCheck(m_objects,origin,ray_dir,near_obj_id,inter_nor,inter_pt);

	if(near_obj_id==-1)
		return Vec4(0,0,0,1);
	
	Vec3 reflect_dir=(ray_dir-2*ray_dir.dot(inter_nor)*inter_nor).normalized();
	Vec4 ref_color(0,0,0,1);
	ref_color=castRayShading_RayTracing(inter_pt+reflect_dir*0.01,reflect_dir,depth+1,input_rei);
	Vec4 shade_color;
	
	if(m_objects[near_obj_id]->getMaterial()->type==zyk::SOLID)
	{
		std::vector<bool> is_lighting(g_pGlobalSys->mLightNum,true);
		if(draw_shadow)
			shadowCheck(g_pGlobalSys->mLightNum,g_pGlobalSys->mLights,inter_pt,is_lighting);

		shade_color=calPhongShading_manyLights(*m_objects[near_obj_id]->getMaterial(),is_lighting,g_pGlobalSys->m_cam.pos,
			inter_pt,inter_nor);

		float ref_para=m_objects[near_obj_id]->getMaterial()->kr;
		combineColor(ref_para,ref_color,shade_color);	
	}
	else if(m_objects[near_obj_id]->getMaterial()->type==zyk::DIELECTRIC)
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
			Vec4 refra_color=castRayShading_RayTracing(inter_pt+refra_dir*0.01,refra_dir,depth+1,out_rei);
			shade_color=ref_color*ref_weight+refra_color*refra_weight;
		}
		else
			shade_color=ref_color;
		zyk::clip_0_to_1(shade_color);
	}
	else if(m_objects[near_obj_id]->getMaterial()->type==zyk::LIGHTSOURCE)
		shade_color=Vec4(1,1,1,1);
	return shade_color;
}

Vec3 RayTracer::uniformSampleHemisphere(const float &r1,const float &r2)
{
	float sinTheta = sqrtf(1 - r1*r1); 
	float phi = 2 * M_PI * r2; 
	float x = sinTheta * cosf(phi); 
	float z = sinTheta * sinf(phi); 
	return Vec3(x,r1,z); 
}

//transform the vector dir(in local coordinate) into world coordinate based on 
//a normal vector
Vec3 transCoordinate(const Vec3& normal,const Vec3&dir)
{
	Vec3 Nt,Nb;
	if (std::fabs(normal(0))>std::fabs(normal(1))) 
		Nt = Vec3(normal(2), 0, -normal(0)).normalized(); 
	else 
		Nt = Vec3(0, -normal(2), normal(1)).normalized(); 
	Nb = normal.cross(Nt); 	
	Mat3 trans_mat;
	trans_mat.col(0)=Nt;
	trans_mat.col(1)=normal;
	trans_mat.col(2)=Nb;
	return trans_mat*dir;
}

//TODO:make new struct IntersectionInfo to reduce the number of parameters of function 
Vec4 RayTracer::raySahding_basedType(const zyk::Object* inter_object,const Vec3& inter_pt,const Vec3& inter_nor,
	const Vec3& ray_dir,int depth,float input_rei)
{
	Vec4 shade_color;
	if(inter_object->getMaterial()->type==zyk::SOLID)
	{
		float r1=mRandGen->getRand();
		float r2=mRandGen->getRand();
		Vec3 sample_dir=transCoordinate(inter_nor,uniformSampleHemisphere(r1,r2));

		Vec4 c=castRayShading_McSampling(inter_pt+sample_dir*0.01,sample_dir,depth+1);
		shade_color=zyk::dot_multV4(r1*c,inter_object->getMaterial()->rd);
	}
	else if(inter_object->getMaterial()->type==zyk::LIGHTSOURCE)
		shade_color=Vec4(light_intensity,light_intensity,light_intensity,1);
	else if(inter_object->getMaterial()->type==zyk::MIRROW)
	{
		Vec3 reflect_dir=(ray_dir-2*ray_dir.dot(inter_nor)*inter_nor).normalized();
		shade_color=castRayShading_McSampling(inter_pt+reflect_dir*0.01,reflect_dir,depth+1);
	}
	else if(inter_object->getMaterial()->type==zyk::DIELECTRIC)
	{
		float rei[2]={input_rei,inter_object->getMaterial()->rei};
		if(FCMP(rei[0],rei[1])&&rei[0]>1.0f)// the ray is leaving out the dielectric
			rei[1]=1.0f;

		Vec3 refra_dir;
		float ref_weight,refra_weight;
		bool is_refract=refractRay(inter_pt,ray_dir,inter_nor,rei,refra_dir,ref_weight);
		if(is_refract)
		{
			float out_rei=rei[1];
			shade_color=castRayShading_McSampling(inter_pt+refra_dir*0.01,refra_dir,depth+1,out_rei);
		}
		else
			shade_color=getBlack();
	}
	return shade_color;
}

Vec4 RayTracer::castRayShading_McSampling(const Vec3& origin,const Vec3& ray_dir,int depth,float input_rei)
{
	if(depth==mMax_depth+1)
		return getBlack();
	
	int near_obj_id=-1;
	Vec3 inter_nor,inter_pt;
	intersectionCheck(m_objects,origin,ray_dir,near_obj_id,inter_nor,inter_pt);

	if(near_obj_id==-1)
		return getBlack();

	Vec4 shade_color=raySahding_basedType(m_objects[near_obj_id],inter_pt,inter_nor,ray_dir,depth,input_rei);
	return shade_color;
}

Vec4 RayTracer::shadeSinglePxiel_Genral(int x,int y)
{
	//for each pixel, we only cast one ray here, so we set the position
	//to cast in the center of the pixel
	Vec3 ray_dir=getRayDirection(g_pGlobalSys->m_cam,x+0.5f,y+0.5f);
	return castRayShading_RayTracing(g_pGlobalSys->m_cam.pos,ray_dir,0);
}

Vec4 RayTracer::shadeSinglePixel_MC_Sampling(int x,int y)
{
	Vec4 shade_color=getBlack();
	for(int i=0;i<mSampleNum;i++)
	{
		Vec3 ray_dir=getRayDirection_randSampling(g_pGlobalSys->m_cam,x,y);
		shade_color+=castRayShading_McSampling(g_pGlobalSys->m_cam.pos,ray_dir,0);
	}
	shade_color/=mSampleNum;
	return shade_color;
}

//#define ZDEBUG
void RayTracer::rayTracing(zyk::UCHAR3*buffer)
{
	if(!m_objects.size())
		return;
	for(int i=0;i<(int)m_objects.size();i++)
		assert(m_objects[i]&&m_objects[i]->getMaterial());

	const zyk::Camera& v_cam=g_pGlobalSys->m_cam;
	for(int i=0;i<v_cam.viewport_height;i++)
	{
		int row_ind=i*v_cam.viewport_width;
		for(int j=0;j<v_cam.viewport_width;j++)
		{	
#ifdef ZDEBUG
			int max_x=576,max_y=550,min_x=435,min_y=360;
			if(!(min_x<=j&&j<=max_x&&min_y<=i&&i<=max_y))
				continue;
			/*int test_x=387,test_y=99;
			if(!(j==test_x&&i==test_y))
				continue;*/
#endif		
			Vec4 shade_color;
			if(m_render_type==GENERAL_RAY_TRACING)
			{
				shade_color=shadeSinglePxiel_Genral(j,i);
				zyk::clip_0_to_1(shade_color);
				fillColor(shade_color,buffer[row_ind+j]);
			}
			else if(m_render_type==MC_PATH_TRACING)
			{
				shade_color=shadeSinglePixel_MC_Sampling(j,i);
				zyk::clip_0_to_1(shade_color);
				fillColor_for_MC_Sampling(shade_color,buffer[row_ind+j]);
			}
		}
		printf("%d ",i);
	}
	printf("\n");
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