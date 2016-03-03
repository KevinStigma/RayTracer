#include "raytracer.h"
#include "GlobalSys.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)|
		_CRTDBG_LEAK_CHECK_DF);
	g_pGlobalSys=new CGlobalSys;
	QApplication a(argc, argv);
	RayTracer w;
	w.show();
	return a.exec();
}
