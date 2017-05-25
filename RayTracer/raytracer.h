#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QtWidgets/QMainWindow>
#include <QKeyEvent>
#include <atomic>
#include <thread>
#include "ui_raytracer.h"
#include "zyk/Data_structure.h"
#include "Calculagraph.h"

namespace zyk
{
	class Object;
	class RandomGenerator;
};
enum RenderType{RENDER_NORMAL,MC_PATH_TRACING};

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
	void loadScene();
	void loadScene(QString name);
	void generateScene();
	void setNormalRendering();
	void setMCPathTracing();
	void setScene1();
	void setScene2();

protected:
	void keyPressEvent(QKeyEvent *e);
	void initRenderBuffer(zyk::UCHAR3*buffer);
	void loadParaFromUI();
	void updateThread();
	void renderViewport(zyk::UCHAR3*buffer);
	void rayTracing(zyk::UCHAR3*buffer, int start_col, int end_col);
	void statistical_work();
	
	Vec4 shadeSinglePixel_MC_Sampling(int x,int y);

	void savePic()const;//save current picture in the data folder

private:
	std::vector<std::thread> m_thread_list;
	zyk::RandomGenerator* mRandGen;
	Ui::RayTracerClass ui;
	zyk::UCHAR3* render_buffer;
	QImage viewport_image;
	std::vector<zyk::Object*> m_objects;
	RenderType m_render_type;
	Calculagraph mCalculagraph;
	static std::atomic_int computed_col;
};

#endif // RAYTRACER_H
