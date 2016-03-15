#include "GlobalSys.h"
#include "zyk/Objects.h"
#include <algorithm>

CGlobalSys *g_pGlobalSys = NULL;
CGlobalSys::CGlobalSys():mLights(NULL),mLightNum(2)
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
	m_light.attr=1; //direction light
	m_light.c_ambient=Vec4::Ones();
	m_light.c_diffuse=Vec4::Ones();
	m_light.c_specular=Vec4::Ones();
	m_light.pos=Vec3::Zero();
	//m_light.dir=Vec3(0,1,1).normalized();
	m_light.dir=Vec3(-0.57735027,0.57735027,0.57735027).normalized();

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
	mLights[1].dir=Vec3( 0.57735027,-0.57735027,-0.57735027).normalized();
}

void CGlobalSys::init_Material()
{
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
	m_materials[index].is_solid=true;
	
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
	m_materials[index].is_solid=true;

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
	m_materials[index].is_solid=true;

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
	m_materials[index].is_solid=true;

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
	m_materials[index].is_solid=true;

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
	m_materials[index].is_solid=true;

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
	m_materials[index].is_solid=false;
	m_materials[index].rei=1.157f;
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

	float coord_length[3]={obb->XL,obb->YL,obb->ZL};
	int index_list[3]={0,1,2};
	//I don't know why this is false when I cancel the symbom '&' in the lambda function
	std::sort(index_list,index_list+3,[&coord_length](const int&a,const int&b)->int{return coord_length[a]>coord_length[b];});

	int division=5,grid_num=division*division;
	float grid_span[3];
	for(int i=0;i<3;i++)
	{
		if(i!=2)
			grid_span[i]=coord_length[index_list[i]]*2.0f/division;
		else
			grid_span[i]=coord_length[index_list[i]];
	}
	
	//copy the backup lights info into new array
	zyk::Light_Ptr light_backup=mLights;
	mLights=new zyk::Light[mLightNum+grid_num];
	for(int i=0;i<mLightNum;i++)
		mLights[i]=light_backup[i];
	SAFE_DELETE_ARRAY(light_backup);

	mLightNum+=grid_num;
	int start_ind=mLightNum;

	for(int i=0;i<division;i++)
		for(int j=0;j<division;j++)
		{
			Vec3 position=obb->bot_pos+obb->local_coord[index_list[2]]*grid_span[2]+
				(i*2+1)*obb->local_coord[index_list[0]]*grid_span[0]+
				(j*2+1)*obb->local_coord[index_list[1]]*grid_span[1];
			mLights[start_ind+i*division+j].c_ambient=Vec4::Ones();
			mLights[start_ind+i*division+j].c_diffuse=Vec4::Ones();
			mLights[start_ind+i*division+j].c_specular=Vec4::Ones();
			mLights[start_ind+i*division+j].pos=position;
		}
}