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
	void init_render_buffer(zyk::UCHAR3*buffer);
	void init_objects();
	void render_2_viewport(zyk::UCHAR3*buffer);
	void ray_tracing(zyk::UCHAR3*buffer);
	void renderTest();
	Vec4 reflectLighting(const Vec3&origin,const Vec3&ray_dir,int depth);

private:
	Ui::RayTracerClass ui;
	zyk::UCHAR3* render_buffer;  
	QImage viewport_image;
	std::vector<zyk::Object*> m_objects;
	bool draw_shadow,draw_reflect;
	int mMax_depth;
};

#endif // RAYTRACER_H
