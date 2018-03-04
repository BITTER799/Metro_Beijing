#include"Metro.h"

using namespace std;

int main()
{	
	Metro TestMetro;
	TestMetro.InitLine();
	TestMetro.Detect();
	//TestMetro.InitVex();
	//TestMetro.InitArcMatrix();

	//TestMetro.SaveArc();
	//TestMetro.SaveVex();

	//TestMetro.ReadVex();
	//TestMetro.ReadArc();

	//ShortPath(Metro, Ori, Des);
	//PrintRoute(Metro, Ori, Des);
	
	//TestMetro.Floyd();
	//SavePath();
	//TestMetro.ReadPath();
	//TestMetro.PrintRouteF();
	TestMetro.MainScene();

	return 0;
}
