#ifndef CALCULAGRAPG_H
#define CALCULAGRAPG_H
#include <iostream>
#include <windows.h>
#include <WinBase.h>
class Calculagraph
{
public:
	Calculagraph();
	void startTiming(){start_time=GetTickCount();}
	DWORD getTimeElapsed()
	{
		end_time=GetTickCount();
		return end_time-start_time;
	}
	void recordTimeElapsed(){std::cout<<getTimeElapsed()<<" ms"<<std::endl;}
private:
	DWORD start_time;
	DWORD end_time;
};
#endif