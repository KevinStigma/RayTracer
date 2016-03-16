#include "raytracer.h"
#include "GlobalSys.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	MEMORY_LEAK_CHECK
	g_pGlobalSys=new CGlobalSys;
	QApplication a(argc, argv);
	RayTracer w;
	w.show();
	return a.exec();
}
