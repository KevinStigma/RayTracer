#include "GlobalSys.h"
#include "zyk/Objects.h"
#include <algorithm>

CGlobalSys *g_pGlobalSys = NULL;
CGlobalSys::CGlobalSys():mLights(NULL),mLightNum(0)
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
	/*
	mLightNum=2;
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
	mLights[1].dir=Vec3( 0.57735027,-0.57735027,-0.57735027).normalized();*/
}

void set_material_from_file(std::ifstream&in,zyk::Material&material)
{
	assert(in.is_open());
	float tmp_array[3];
	float tmp_value;
	std::string tmp_str;
	
	in>>tmp_str;
	material.name=tmp_str;

	in>>tmp_value;
	if(tmp_value==0)
		material.type=zyk::SOLID;
	else if(tmp_value==1)
		material.type=zyk::DIELECTRIC;
	else
		material.type=zyk::LIGHTSOURCE;

	in>>tmp_value;
	in>>tmp_array[0]>>tmp_array[1]>>tmp_array[2];
	material.ka=tmp_value;
	material.ra=Vec4(tmp_array[0],tmp_array[1],tmp_array[2],1.0f);

	in>>tmp_value;
	in>>tmp_array[0]>>tmp_array[1]>>tmp_array[2];
	material.kd=tmp_value;
	material.rd=Vec4(tmp_array[0],tmp_array[1],tmp_array[2],1.0f);

	in>>tmp_value;
	in>>tmp_array[0]>>tmp_array[1]>>tmp_array[2];
	material.ks=tmp_value;
	material.rs=Vec4(tmp_array[0],tmp_array[1],tmp_array[2],1.0f);

	in>>tmp_value;
	material.power=tmp_value;

	in>>tmp_value;
	material.kr=tmp_value;

	in>>tmp_value;
	material.rei=tmp_value;
}

void CGlobalSys::load_Material(const char* txtName)
{
	std::ifstream in(txtName);
	if(!in.is_open())
	{
		std::cout<<"Can't open the file!"<<std::endl;
		return;
	}
	in>>mMatNum;
	if(mMatNum>MAX_MATERIAL)
	{
		std::cout<<"Exceed the number of max material num!"<<std::endl;
		return;
	}

	for(int i=0;i<mMatNum;i++)
		set_material_from_file(in,m_materials[i]);

	in.close();
}

const zyk::Material* CGlobalSys::getMaterialByName(const std::string& name)const
{
	for(int i=0;i<mMatNum;i++)
	{
		if(name==m_materials[i].name)
			return &m_materials[i];
	}
	return NULL;
}

zyk::Material* CGlobalSys::getMaterialByName(const std::string& name)
{
	return const_cast<zyk::Material*>(static_cast<const CGlobalSys&>(*this).getMaterialByName(name));
}

#define LOAD_MAT
void CGlobalSys::init_Material()
{

#ifndef LOAD_MAT
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
	m_materials[index].type=zyk::SOLID;
	
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
	m_materials[index].type=zyk::SOLID;

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
	m_materials[index].type=zyk::SOLID;

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
	m_materials[index].type=zyk::SOLID;

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
	m_materials[index].type=zyk::SOLID;

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
	m_materials[index].type=zyk::SOLID;

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
	m_materials[index].type=zyk::DIELECTRIC;
	m_materials[index].rei=1.157f;

	index=7;
	m_materials[index].name="light_source";
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
	m_materials[index].type=zyk::LIGHTSOURCE;
	m_materials[index].rei=1.157f;

	index=8;
	m_materials[index].name="red";
	m_materials[index].attr=2; 
	m_materials[index].color.x=255;
	m_materials[index].color.y=0;
	m_materials[index].color.z=0;

	m_materials[index].ks=m_materials[index].kd=m_materials[index].ka=1.0f;
	m_materials[index].kr=tmp_kr;
	m_materials[index].power=50.0f;
	m_materials[index].ra=Vec4(m_materials[index].color.x*0.3f*inv_255,m_materials[index].color.y*0.3f*inv_255,m_materials[index].color.z*0.3f*inv_255,1.0f);
	m_materials[index].rd=Vec4(m_materials[index].color.x*0.7f*inv_255,m_materials[index].color.y*0.7f*inv_255,m_materials[index].color.z*0.7f*inv_255,1.0f);
	//m_materials[4].rs=Vec4(1.0f,1.0f,1.0f,1.0f);
	m_materials[index].rs=Vec4(0.0f,0.0f,0.0f,1.0f);
	m_materials[index].type=zyk::SOLID;
	m_materials[index].rei=1.157f;

	index=9;
	m_materials[index].name="blue";
	m_materials[index].attr=2; 
	m_materials[index].color.x=0;
	m_materials[index].color.y=0;
	m_materials[index].color.z=100;

	m_materials[index].ks=m_materials[index].kd=m_materials[index].ka=1.0f;
	m_materials[index].kr=tmp_kr;
	m_materials[index].power=50.0f;
	m_materials[index].ra=Vec4(m_materials[index].color.x*0.3f*inv_255,m_materials[index].color.y*0.3f*inv_255,m_materials[index].color.z*0.3f*inv_255,1.0f);
	m_materials[index].rd=Vec4(m_materials[index].color.x*0.7f*inv_255,m_materials[index].color.y*0.7f*inv_255,m_materials[index].color.z*0.7f*inv_255,1.0f);
	//m_materials[4].rs=Vec4(1.0f,1.0f,1.0f,1.0f);
	m_materials[index].rs=Vec4(0.0f,0.0f,0.0f,1.0f);
	m_materials[index].type=zyk::SOLID;
	m_materials[index].rei=1.157f;
#else
	load_Material("material_script.txt");
#endif
}

void CGlobalSys::generateAreaLights(zyk::TriMesh*pTri_mesh)
{
	if(!pTri_mesh||!pTri_mesh->getMesh())
		return;
	pTri_mesh->calOBB();
	const zyk::OBB* obb=pTri_mesh->getOBB();
	if(!obb)
	{
		std::cout<<"You must calculate the mesh's bounding box first!"<<std::endl;
		return;
	}
	zyk::Light_Ptr light_backup=mLights;
	mLights=new zyk::Light[mLightNum+1];
	for(int i=0;i<mLightNum;i++)
		mLights[i]=light_backup[i];
	SAFE_DELETE_ARRAY(light_backup);
	mLights[mLightNum].type=zyk::AREA_LIGHT;
	mLights[mLightNum].light_source_mesh=pTri_mesh;
	mLightNum++;

	//float coord_length[3]={obb->XL,obb->YL,obb->ZL};
	//int index_list[3]={0,1,2};
	////I don't know why this is false when I cancel the symbom '&' in the lambda function
	//std::sort(index_list,index_list+3,[&coord_length](const int&a,const int&b)->int{return coord_length[a]>coord_length[b];});

	//int division=5,grid_num=division*division;
	//float grid_span[3];
	//for(int i=0;i<3;i++)
	//{
	//	if(i!=2)
	//		grid_span[i]=coord_length[index_list[i]]/division;
	//	else
	//		grid_span[i]=coord_length[index_list[i]];
	//}
	//
	////copy the backup lights info into new array
	//zyk::Light_Ptr light_backup=mLights;
	//mLights=new zyk::Light[mLightNum+grid_num];
	//for(int i=0;i<mLightNum;i++)
	//	mLights[i]=light_backup[i];
	//SAFE_DELETE_ARRAY(light_backup);

	//int start_ind=mLightNum;
	//for(int i=0;i<division;i++)
	//	for(int j=0;j<division;j++)
	//	{
	//		Vec3 p1=obb->bot_pos;
	//		Vec3 vz=obb->local_coord[index_list[2]]*grid_span[2];
	//		Vec3 vx=(i*2+1)*obb->local_coord[index_list[0]]*grid_span[0];
	//		Vec3 vy=(j*2+1)*obb->local_coord[index_list[1]]*grid_span[1];
	//		Vec3 position=p1+vx+vy+vz;
	//		mLights[start_ind+i*division+j].c_ambient=Vec4::Ones();
	//		mLights[start_ind+i*division+j].c_diffuse=Vec4::Ones();
	//		mLights[start_ind+i*division+j].c_specular=Vec4::Ones();
	//		mLights[start_ind+i*division+j].pos=position;
	//		mLights[start_ind+i*division+j].type=zyk::SPOT_LIGHT;
	//		mLights[start_ind+i*division+j].kc=1.0f;
	//		mLights[start_ind+i*division+j].kl=0.1f;
	//		mLights[start_ind+i*division+j].kq=0.01f;
	//	}
	//mLightNum+=grid_num;
}