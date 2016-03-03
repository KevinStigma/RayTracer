#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QtWidgets/QMainWindow>
#include "ui_raytracer.h"
#include "zyk/Data_structure.h"

class RayTracer : public QMainWindow
{
	Q_OBJECT

public:
	RayTracer(QWidget *parent = 0);
	~RayTracer();

public slots:
	void renderScene(); 

protected:
	void init_render_buffer(zyk::UCHAR3*buffer);
	void render_2_viewport(zyk::UCHAR3*buffer);
	void ray_tracing(zyk::UCHAR3*buffer);

private:
	Ui::RayTracerClass ui;
	zyk::UCHAR3* render_buffer;  
	QImage viewport_image;
	zyk::Sphere sphere;
};

#endif // RAYTRACER_H
