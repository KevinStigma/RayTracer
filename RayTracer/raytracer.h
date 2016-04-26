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

class RayTracer : public QMainWindow
{
	Q_OBJECT

public:
	RayTracer(QWidget *parent = 0);
	~RayTracer();

public slots:
	void renderScene(); 
	void drawShadowSet();
	void drawReflectSet();
	void loadScene();
	void setGeneRayTracing();
	void setMCPathTracing();

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
	void fresnel(float cos1,float cos2,const float rei[],float &kr)const; 
	bool refractRay(const Vec3& origin,const Vec3&incident_dir,const Vec3& normal,const float rei[],
		Vec3& refract_dir,float& ref_weight)const;
	
	Vec4 shadeSinglePxiel_Genral(int x,int y);
	Vec4 shadeSinglePixel_MC_Sampling(int x,int y);

	Vec4 castRayShading_RayTracing(const Vec3& origin,const Vec3& ray_dir,int depth,float input_rei=1.0f);
	Vec4 castRayShading_McSampling(const Vec3& origin,const Vec3& ray_dir,int depth,float input_rei=1.0f);
	Vec4 raySahding_basedType(const zyk::Object* inter_object,const Vec3& inter_pt,const Vec3& inter_nor,
		const Vec3& ray_dir,int depth,float input_rei);
	void shadowCheck(int lightsNum,const zyk::Light* lights,const Vec3&intersect_pt,std::vector<bool>& is_lighting);
	void intersectionCheck(const std::vector<zyk::Object*>& objects,const Vec3& origin,const Vec3& ray_dir,
		int& near_obj_id,Vec3& inter_pt_nor,Vec3&inter_pt);
	void calPhongShading_oneLight(const zyk::Material& pMaterial,const zyk::Light& pLight,const Vec3& cam_pos,
		const Vec3& shad_pos,const Vec3& pNormal,Vec4& pColor,bool calSpecular=true);
	Vec4 calPhongShading_manyLights(const zyk::Material& pMaterial,const std::vector<bool>& is_lighting,
		const Vec3& cam_pos,const Vec3& shad_pos,const Vec3& pNormal,bool calSpecular=true);
	Vec3 uniformSampleHemisphere(const float &r1, const float &r2);//generate a random direction in a hemisphere
	void savePic()const;//save current picture in the data folder

private:
	Ui::RayTracerClass ui;
	zyk::UCHAR3* render_buffer;
	zyk::RandomGenerator* mRandGen;
	QImage viewport_image;
	std::vector<zyk::Object*> m_objects;
	RenderType m_render_type;
	bool draw_shadow,draw_reflect;
	int mMax_depth,mSampleNum;
	Calculagraph mCalculagraph;
	float light_intensity;
};

#endif // RAYTRACER_H
