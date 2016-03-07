#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QtWidgets/QMainWindow>
#include <QKeyEvent>
#include "ui_raytracer.h"
#include "zyk/Data_structure.h"

//class's forward declaration in a namespace
namespace zyk
{
	class Object;
};

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

protected:
	void keyPressEvent(QKeyEvent *e);
	void initRenderBuffer(zyk::UCHAR3*buffer);
	void initObjects();
	void renderViewport(zyk::UCHAR3*buffer);
	void rayTracing(zyk::UCHAR3*buffer);
	Vec3 castRay(const zyk::Camera&pCam,const float p_radio_height,int x,int y);
	void renderTest();
	Vec4 reflectLighting(const Vec3&origin,const Vec3&ray_dir,int depth);
	void shadowCheck(int lightsNum,const zyk::Light* lights,const Vec3&intersect_pt,std::vector<bool>& is_lighting);
	void intersectionCheck(const std::vector<zyk::Object*>& objects,const Vec3& origin,const Vec3& ray_dir,
		int& near_obj_id,Vec3& inter_pt_nor,Vec3&inter_pt);
	void calPhongShading_oneLight(const zyk::Material& pMaterial,const zyk::Light& pLight,const Vec3& cam_pos,
		const Vec3& shad_pos,const Vec3& pNormal,Vec4& pColor);
	Vec4 calPhongShading_manyLights(const zyk::Material& pMaterial,const std::vector<bool>& is_lighting,
		const Vec3& cam_pos,const Vec3& shad_pos,const Vec3& pNormal);
	void savePic();//save current picture in the data folder

private:
	Ui::RayTracerClass ui;
	zyk::UCHAR3* render_buffer;  
	QImage viewport_image;
	std::vector<zyk::Object*> m_objects;
	bool draw_shadow,draw_reflect;
	int mMax_depth;
};

#endif // RAYTRACER_H
