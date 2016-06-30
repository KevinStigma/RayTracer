#include "raytracer.h"
#include "GlobalSys.h"
#include "zyk/Objects.h"
#include "zyk/mathematics.h"
#include "zyk/RandomGenerator.h"
#include "IntersectionInfo.h"
#include "InputRay.h"
#include <tchar.h>
#include <typeinfo>
#include <windows.h>
#include <WinBase.h>
#include <QFileDialog>
#include <QString>


RayTracer::RayTracer(QWidget *parent)
	: QMainWindow(parent),render_buffer(NULL),mRandGen(NULL),viewport_image(g_pGlobalSys->viewport_width,
	g_pGlobalSys->viewport_height,QImage::Format_RGB888),m_render_type(MC_PATH_TRACING)
{
	ui.setupUi(this);
	setFixedSize(1026,703);
	connect(ui.RenderButton,SIGNAL(clicked()),this,SLOT(renderScene()));
	connect(ui.actionLoad_Scene,SIGNAL(triggered()),this,SLOT(loadScene()));
	connect(ui.actionNormal_Rendering, SIGNAL(triggered()), this, SLOT(setNormalRendering()));
	connect(ui.actionMC_PathTracing,SIGNAL(triggered()),this,SLOT(setMCPathTracing()));
	connect(ui.actionScene1,SIGNAL(triggered()),this,SLOT(setScene1()));
	connect(ui.actionScene2,SIGNAL(triggered()),this,SLOT(setScene2()));


	ui.render_label->setFixedSize(g_pGlobalSys->viewport_width,g_pGlobalSys->viewport_height);
	render_buffer=new zyk::UCHAR3[g_pGlobalSys->pixel_num];
	mRandGen=new zyk::RandomGenerator;
	//initObjects();
	
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

void RayTracer::setNormalRendering()
{
	ui.actionNormal_Rendering->setChecked(true);
	ui.actionMC_PathTracing->setChecked(false);
	m_render_type=RENDER_NORMAL;
	std::cout<<"Render Type:Normal Rendering"<<std::endl;
}

void RayTracer::setMCPathTracing()
{
	ui.actionNormal_Rendering->setChecked(false);
	ui.actionMC_PathTracing->setChecked(true);
	m_render_type=MC_PATH_TRACING;
	std::cout<<"Render Type:Monte Carlo Path Tracing"<<std::endl;
}

void RayTracer::setScene1()
{
	ui.actionScene1->setChecked(true);
	ui.actionScene2->setChecked(false);
	std::cout<<"Scene1"<<std::endl;

	ui.posXEdit->setText("0");ui.posYEdit->setText("0");ui.posZEdit->setText("30");
	ui.viewXEdit->setText("0");ui.viewYEdit->setText("0");ui.viewZEdit->setText("-1");
	ui.upXEdit->setText("0");ui.upYEdit->setText("1");ui.upZEdit->setText("0");
	ui.envREdit->setText("0");ui.envGEdit->setText("0");ui.envBEdit->setText("0");
}

void RayTracer::setScene2()
{
	ui.actionScene1->setChecked(false);
	ui.actionScene2->setChecked(true);
	std::cout<<"Scene2"<<std::endl;

	ui.posXEdit->setText("1.6");ui.posYEdit->setText("10");ui.posZEdit->setText("18");
	ui.viewXEdit->setText("0");ui.viewYEdit->setText("-0.5");ui.viewZEdit->setText("-1");
	ui.upXEdit->setText("0");ui.upYEdit->setText("1");ui.upZEdit->setText("0");
	ui.envREdit->setText("0");ui.envGEdit->setText("0");ui.envBEdit->setText("0");
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

	num[0]=ui.envREdit->text();
	num[1]=ui.envGEdit->text();
	num[2]=ui.envBEdit->text();
	for(int i=0;i<3;i++)
		g_pGlobalSys->environment_color(i)=num[i].toFloat();

	num[0]=ui.viewXEdit->text();
	num[1]=ui.viewYEdit->text();
	num[2]=ui.viewZEdit->text();

	for(int i=0;i<3;i++)
		pCam.n[i]=num[i].toFloat();

	num[0]=ui.upXEdit->text();
	num[1]=ui.upYEdit->text();
	num[2]=ui.upZEdit->text();

	//up vector
	for(int i=0;i<3;i++)
		pCam.v[i]=num[i].toFloat();

	pCam.n.normalize();
	pCam.v.normalize();
	pCam.u=pCam.n.cross(pCam.v);
	pCam.v=pCam.u.cross(pCam.n);

	g_pGlobalSys->mMax_depth=ui.depthEdit->text().toInt();
	g_pGlobalSys->mSampleNum=ui.sampleEdit->text().toInt();
	g_pGlobalSys->light_intensity = ui.lightIntenEdit->text().toFloat();
}

#define RECORD_TIME
void RayTracer::renderScene()
{
	initRenderBuffer(render_buffer);
	generateScene();
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

//#define SCENE1
void RayTracer::initObjects()
{
#ifdef SCENE1
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
#else
	m_objects.resize(10);
	std::vector<zyk::TriMesh*> meshes;
	meshes.resize(6);

	zyk::TriMesh* tri_mesh=new zyk::TriMesh("../data/scene/2/wall1.obj");
	tri_mesh->setMaterial(g_pGlobalSys->getMaterialByName("gray2"));
	m_objects[0]=tri_mesh;
	meshes[0]=tri_mesh;

	tri_mesh=new zyk::TriMesh("../data/scene/2/wall2.obj");
	tri_mesh->setMaterial(g_pGlobalSys->getMaterialByName("gray2"));
	m_objects[1]=tri_mesh;
	meshes[1]=tri_mesh;

	tri_mesh=new zyk::TriMesh("../data/scene/2/plate1.obj");
	tri_mesh->setMaterial(g_pGlobalSys->getMaterialByName("mirrow"));
	tri_mesh->normalizeNormals();
	m_objects[2]=tri_mesh;
	meshes[2]=tri_mesh;

	tri_mesh=new zyk::TriMesh("../data/scene/2/plate2.obj");
	tri_mesh->setMaterial(g_pGlobalSys->getMaterialByName("mirrow"));
	tri_mesh->normalizeNormals();
	m_objects[3]=tri_mesh;
	meshes[3]=tri_mesh;

	tri_mesh=new zyk::TriMesh("../data/scene/2/plate3.obj");
	tri_mesh->setMaterial(g_pGlobalSys->getMaterialByName("mirrow"));
	tri_mesh->normalizeNormals();
	m_objects[4]=tri_mesh;
	meshes[4]=tri_mesh;

	tri_mesh=new zyk::TriMesh("../data/scene/2/plate4.obj");
	tri_mesh->setMaterial(g_pGlobalSys->getMaterialByName("mirrow"));
	tri_mesh->normalizeNormals();
	m_objects[5]=tri_mesh;
	meshes[5]=tri_mesh;

	m_objects[6]=new zyk::Sphere(Vec3(7.10389,6.43046,3.03575),0.897079);
	m_objects[6]->setMaterial(g_pGlobalSys->getMaterialByName("light_source"));

	m_objects[7]=new zyk::Sphere(Vec3(3.6344,6.43045,3.03575),0.60687);
	m_objects[7]->setMaterial(g_pGlobalSys->getMaterialByName("light_source"));

	m_objects[8]=new zyk::Sphere(Vec3(-0.0104812,6.43046,3.03574),0.334434);
	m_objects[8]->setMaterial(g_pGlobalSys->getMaterialByName("light_source"));

	m_objects[9]=new zyk::Sphere(Vec3(-3.36951,6.43046,3.03575),0.114867);
	m_objects[9]->setMaterial(g_pGlobalSys->getMaterialByName("light_source"));
		
	//Vec3 trans_vec(0.0f,-5.0f,-15.0f);//-5 -12
	//for(int i=0;i<(int)meshes.size();i++)
	//	meshes[i]->translate(trans_vec);

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


inline void combineColor(float ref_para,const Vec4&pRefColor,Vec4& pShadeColor)
{
	pShadeColor=pShadeColor+ref_para*pRefColor;
	zyk::clip_0_to_1(pShadeColor);
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
			/*int max_x=670,max_y=416,min_x=219,min_y=227;
			if(!(min_x<=j&&j<=max_x&&min_y<=i&&i<=max_y))
				continue;*/
			int test_x=318,test_y=202;
			if(!(j==test_x&&i==test_y))
				continue;
#endif		
			Vec4 shade_color;
			if (m_render_type == RENDER_NORMAL)
			{
				shade_color = g_pGlobalSys->shadeSinglePxiel_Normal(m_objects, j, i);
				zyk::clip_0_to_1(shade_color);
				fillColor(shade_color, buffer[row_ind + j]);
			}
			else if (m_render_type == MC_PATH_TRACING)
			{
				shade_color = g_pGlobalSys->shadeSinglePixel_MC_Sampling(m_objects,j, i);
				zyk::clip_0_to_1(shade_color);
				fillColor_for_MC_Sampling(shade_color, buffer[row_ind + j]);
			}
		}
		printf("%d ",i);
	}
	printf("\n");
}
//TODO:this function is too hard code, we should make it more general
void RayTracer::generateScene()
{
	extern int zyk::sceneId;
	if(ui.actionScene1->isChecked())
	{
		zyk::sceneId=1;
		loadScene("scene1.txt");
		std::vector<zyk::TriMesh*> mesh_data(6);
		Vec3 trans_vec(0.0f,-5.0f,12.0f);
		for(int i=0;i<6;i++)
		{
			mesh_data[i]=dynamic_cast<zyk::TriMesh*>(m_objects[i]);
			assert(mesh_data[i]);
			mesh_data[i]->translate(trans_vec);
		}
		mesh_data[1]->setNormal(1,0,0);
		mesh_data[2]->setNormal(0,-1,0);
		mesh_data[3]->setNormal(0,0,1);
		mesh_data[4]->setNormal(0,1,0);
		mesh_data[5]->setNormal(-1,0,0);
	}
	else
	{
		zyk::sceneId=2;
		loadScene("scene2.txt");
	}
}

void RayTracer::loadScene(QString filename)
{
	std::wstring fname = filename.toStdWString();
	std::ifstream in(fname);
	if(!in.is_open())
	{
		std::cout<<"Can't open the file!"<<std::endl;
		return;
	}

	int n;
	in>>n;
	std::string object_name;

	int mat_id,i;
	std::string mat_name;
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
		in>>mat_name;
		m_objects[i]->setMaterial(g_pGlobalSys->getMaterialByName(mat_name));
	}
	if(i==n)
		std::cout<<"Load scene successfully!"<<std::endl;
	in.close();
}

void RayTracer::loadScene()
{
	QString filename=QFileDialog::getOpenFileName(this,tr("Load Scene"),".",
		"scene scripts(*.txt)", 0);
	if(!filename.size())
		return; 
	loadScene(filename);
}

void RayTracer::renderTest()
{
	//here we write some code to test effects of rendering
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