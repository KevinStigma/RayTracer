#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QtWidgets/QMainWindow>
#include <QKeyEvent>
#include "ui_raytracer.h"
#include "zyk/Data_structure.h"
#include "Calculagraph.h"

//class's forward declaration in a namespace
namespace zyk
{
	class Object;
	class RandomGenerator;
};
enum RenderType{GENERAL_RAY_TRACING,MC_PATH_TRACING};

struct Intersection_Info;
class RayTracer : public QMainWindow
{
	Q_OBJECT

public:
	RayTracer(QWidget *parent = 0);
	~RayTracer();
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

public slots:
	void renderScene(); 
	void drawShadowSet();
	void drawReflectSet();
	void loadScene();
	void loadScene(QString name);
	void generateScene();
	void setGeneRayTracing();
	void setMCPathTracing();
	void setScene1();
	void setScene2();

protected:
	void keyPressEvent(QKeyEvent *e);
	void initRenderBuffer(zyk::UCHAR3*buffer);
	void loadParaFromUI();
	void initObjects();
	void outputTimeRecordInfo(int depth,const std::string& str);
	void renderViewport(zyk::UCHAR3*buffer);
	void rayTracing(zyk::UCHAR3*buffer);
	Vec3 getRayDirection(const zyk::Camera&pCam,float x,float y);
	Vec3 getRayDirection_randSampling(const zyk::Camera&pCam,float x,float y);
	void renderTest();
	
	Vec4 shadeSinglePxiel_Genral(int x,int y);
	Vec4 shadeSinglePxiel_Normal(int x,int y);
	Vec4 shadeSinglePixel_MC_Sampling(int x,int y);

	Vec4 castRayShading_RayTracing(const Vec3& origin,const Vec3& ray_dir,int depth,float input_rei=1.0f);
	//Vec4 castRayShading_McSampling(const Vec3& origin,const Vec3& ray_dir,int depth,float input_rei=1.0f);
	//Vec4 raySahding_basedType(const Intersection_Info& inter_info,
	//	const Vec3& ray_dir, int depth, float input_rei);

	void shadowCheck(int lightsNum,const zyk::Light* lights,const Vec3&intersect_pt,std::vector<bool>& is_lighting);
	//void intersectionCheck(const Vec3& origin, const Vec3& ray_dir,Intersection_Info& inter_info);
	void calPhongShading_oneLight(const zyk::Material& pMaterial,const zyk::Light& pLight,const Vec3& cam_pos,
		const Vec3& shad_pos,const Vec3& pNormal,Vec4& pColor,bool calSpecular=true);
	Vec4 calPhongShading_manyLights(const zyk::Material& pMaterial,const std::vector<bool>& is_lighting,
		const Vec3& cam_pos,const Vec3& shad_pos,const Vec3& pNormal,bool calSpecular=true);
	void savePic()const;//save current picture in the data folder

private:
	zyk::RandomGenerator* mRandGen;
	Ui::RayTracerClass ui;
	zyk::UCHAR3* render_buffer;
	QImage viewport_image;
	std::vector<zyk::Object*> m_objects;
	RenderType m_render_type;
	bool draw_shadow,draw_reflect;
	int mSampleNum;
	Calculagraph mCalculagraph;
};

#endif // RAYTRACER_H
