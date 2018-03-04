//程序文档请访问：

#pragma once
#include<stdio.h>
#include<string>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<Windows.h>
#include<cmath>
#include<cstdlib>


#define FILEERROR -1
#define MaxLine 20
#define MaxNum 400
#define NOTLINKED 1000
#define LINKED 1
#define EMPTY 0

//放在全局区加快启动速度
int Path[MaxNum][MaxNum];//F算法储存点与点之间中转结点
int FDistance[MaxNum][MaxNum];//储存F算法得到的各点间最小距离


//****************【辅助结构的建立】********************//
struct MetroLine//地铁线路
{
	int num;//线路号
	int staNum;//站点个数
	int isRing;//是否为环路
	char StationName[50][20];//站点名称
	MetroLine()
	{
		num = -1;
		staNum = EMPTY;
		isRing = 0;
		StationName[0][0] = '\0';
	}
};

struct Vertex//顶点（站点）
{
	char vexName[20];
	int LineNum[3] = { 0 };
	Vertex()
	{
		vexName[0] = '\0';
	}

};

namespace metro {
	struct Arc//边->邻接矩阵元素
	{
		int weight;//权值
		int LineNum;//边所属线路
	};//Arc与Windows.h中的一个函数重名了
}
//****************【从本地获取线路信息】*****************//

//****************【Metro类的建立】********************//
class Metro
{
public:
	//****************【初始化工作】********************//
	void GettingData(MetroLine & Line, char *fileName);
	void InitLine();
	void Detect();//检测文本文件是否存在，若为否则开始初始化
	void InitVex();//初始化顶点表
	void InitArcMatrix();//初始化邻接矩阵
	void MainScene();//主界面
	void ShowSta();//查询站点信息
	void ShowLine();//查询线路信息

	//****************【对外提供的接口】*****************//
	int Locate(char *staName);//找到站点在顶点表的下标
	int FindArc(int i, int j);//找到边对应的权值
	int FindLine(int i, int j);//返回边所属线路
	char* FindName(int i);//返回站点名称
	int StationSum();//返回站点总数
	bool isTransSta(int i);//返回是否为换乘站

	//****************【最短路线规划】********************//
	void getOri(char*& Ori);//输入起点
	void getDes(char*& Des);//输入终点

	void Floyd();//弗洛伊德算法更新Path和FDistance数组
	void PrintRouteF(char* ori, char* des);//打印最短路线

	void ShortPath(char*ori, char* des, int mode);//dijkstra算法
	int FindMin(int n, int m);
	void PrintRoute(char*ori, char*des, int mode );

	char* search(char *&ori);//景点路线规划

	void ListHistory(char*& ori,char*& des);

	//****************【文件读取与储存】********************//
	void SaveVex();
	void SaveArc();
	void SavePath();
	void SaveFDistance();
	void SaveHistory();

	void ReadPath();
	void ReadVex();
	void ReadArc();
	void ReadFDistance();
	void ReadHistory();

private:
	MetroLine Line[MaxLine];//存储线路信息
	Vertex vexList[MaxNum];//顶点表
	metro::Arc ArcList[MaxNum][MaxNum];//边矩阵
	int staSum;//站点总数
	bool isTransfer[MaxNum];//储存是否为换乘站

	bool visited[MaxNum] = { false };//记录到该顶点的最短路径是否已经找到
	int Disk[MaxNum];//记录原始顶点到图上各顶点的路径长度
	int FormerVex[MaxNum];//记录顶点最短路径下的前驱
	int interchange[MaxNum];//记录各顶点所需换乘次数
	int forbidden[10] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };//封站信息
	int History[10] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };//历史记录辅助矩阵
	int Hrear=-1;//历史记录辅助矩阵尾指针
};

//****************【成员函数的实现】********************//
void Metro::GettingData(MetroLine & Line, char *fileName)
{
	int i;
	FILE *fp;
	int no;//用来判断fscanf是否读到了空行
	if ((fp = fopen(fileName, "r")) == NULL)
	{
		throw FILEERROR;
	}
	no = fscanf(fp, "%d %d %d", &(Line.num), &(Line.staNum), &(Line.isRing));
	for (i = 0; no != EOF; i++)
	{
		no = fscanf(fp, "%s", Line.StationName[i]);
	}
	for (i = i - 1; i < 49; i++)//将空白行全部变成'\0'，用于初始化顶点表时达到if的条件
	{
		Line.StationName[i][0] = '\0';
	}
	fclose(fp);
}

void Metro::InitLine()
{
	GettingData(Line[1], "地铁1号线.txt");
	GettingData(Line[2], "地铁2号线.txt");
	GettingData(Line[3], "地铁昌平线.txt");
	GettingData(Line[4], "地铁4号线.txt");
	GettingData(Line[5], "地铁5号线.txt");
	GettingData(Line[6], "地铁6号线.txt");
	GettingData(Line[7], "地铁7号线.txt");
	GettingData(Line[8], "地铁8号线.txt");
	GettingData(Line[9], "地铁9号线.txt");
	GettingData(Line[10], "地铁10号线.txt");
	GettingData(Line[13], "地铁13号线.txt");
	//GettingData(Line[15], "地铁15号线.txt");
}

void Metro::Detect()
{
	HANDLE hFile;
	LPCTSTR lpFileName = L".\\Vex.txt";
	WIN32_FIND_DATA pNextInfo;  //搜索得到的文件信息将储存在pNextInfo中;  
	hFile = FindFirstFile(lpFileName, &pNextInfo);//请注意是 &pNextInfo , 不是 pNextInfo;  	
	if (hFile != INVALID_HANDLE_VALUE)
	{
		//std::cout << 1;//调试用
		ReadVex();//搜索成功则开始读取文件
		std::cout << "站点信息读取完毕！" << std::endl;
	}
	else
	{
		InitVex();
		std::cout << "站点信息初始化且向本地存储完毕！" << std::endl;
	}

	lpFileName = L".\\Arc.txt";
	hFile = FindFirstFile(lpFileName, &pNextInfo);//请注意是 &pNextInfo , 不是 pNextInfo;  	
	if (hFile != INVALID_HANDLE_VALUE)
	{
		ReadArc();//搜索成功则开始读取文件
		std::cout << "线路连接信息读取完毕！" << std::endl;
	}
	else
	{
		InitArcMatrix();
		std::cout << "线路连接信息初始化且向本地存储完毕！" << std::endl;
	}

	lpFileName = L".\\Path.txt";
	hFile = FindFirstFile(lpFileName, &pNextInfo);//请注意是 &pNextInfo , 不是 pNextInfo;  	
	if (hFile != INVALID_HANDLE_VALUE)
	{
		ReadPath();//搜索成功则开始读取文件
	}
	else
	{
		Floyd();
	}

	lpFileName = L".\\FDistance.txt";
	hFile = FindFirstFile(lpFileName, &pNextInfo);//请注意是 &pNextInfo , 不是 pNextInfo;  	
	if (hFile != INVALID_HANDLE_VALUE)
	{
		ReadFDistance();//搜索成功则开始读取文件
	}
	else
	{
		Floyd();
	}

	lpFileName = L".\\History.txt";
	hFile = FindFirstFile(lpFileName, &pNextInfo);//请注意是 &pNextInfo , 不是 pNextInfo;  	
	if (hFile != INVALID_HANDLE_VALUE)
	{
		ReadHistory();//搜索成功则开始读取文件
		std::cout << "历史查询记录读取完毕！" << std::endl;
	}

	system("pause");
}

void Metro::InitVex()
{
	int k = 0;
	staSum = 0;
	for (int i = 0; i < MaxLine; i++)
	{
		for (int j = 0; j < Line[i].staNum; j++)//中间判断已经杜绝了继续扫描线路表的空余部分的情况
		{
			if (strcmp(vexList[k].vexName, Line[i].StationName[j]))//判断Line的站点名称表是否到头
			{
				strcpy(vexList[k].vexName, Line[i].StationName[j]);
				vexList[k++].LineNum[0] = i;
			}
		}
		staSum += Line[i].staNum;
	}


	Vertex temp;
	for (int i = 0; i < staSum; i++)
	{
		k = 1;
		temp = vexList[i];
		isTransfer[i] = false;
		for (int j = i + 1; j < staSum; j++)
		{
			if (strcmp(vexList[j].vexName, temp.vexName) == 0)
			{
				isTransfer[i] = true;
				vexList[i].LineNum[k++] = vexList[j].LineNum[0];
			}
		}
	}
	SaveVex();
}

void Metro::InitArcMatrix()
{
	int i, j, m, n, start, end;//start,end用于记录环路的起始站和终点站
	for (i = 0; i < MaxNum; i++)//初始化权值
	{
		for (j = 0; j < MaxNum; j++)
		{
			ArcList[i][j].weight = NOTLINKED;
			ArcList[i][j].LineNum = -1;
		}
	}
	for (i = 0; i < MaxLine; i++)
	{
		for (j = 0; j < Line[i].staNum - 1; j++)
		{
			m = Locate(Line[i].StationName[j]);
			n = Locate(Line[i].StationName[j + 1]);
			if (j == 0 && Line[i].isRing == 1) { start = m; }//环路的起始站
			if (j == Line[i].staNum - 2 && Line[i].isRing == 1) { end = n; }//环路的终点站
			ArcList[m][n].weight = LINKED;
			ArcList[n][m].weight = LINKED;
			ArcList[m][n].LineNum = i;
			ArcList[n][m].LineNum = i;
		}
		if (Line[i].isRing == 1)//将环路的起点和终点连在一起
		{
			ArcList[start][end].weight = LINKED;
			ArcList[end][start].weight = LINKED;
			ArcList[start][end].LineNum = i;
			ArcList[end][start].LineNum = i;
		}
	}
	SaveArc();
}

int Metro::Locate(char * staName)//找到站点在顶点表中下标
{
	int i = 0;
	do
	{
		if (!strcmp(staName, vexList[i].vexName))
		{
			return i;
		}
		i++;
	} while (i < staSum);
	return -1;
}

int Metro::FindArc(int i, int j)//找边，返回权值
{
	return ArcList[i][j].weight;
}

int Metro::FindLine(int i, int j)
{
	return ArcList[i][j].LineNum;
}

char* Metro::FindName(int i)//返回站点名称
{
	return vexList[i].vexName;
}

int Metro::StationSum()//返回站点总数
{
	return staSum;
}

bool Metro::isTransSta(int i)
{
	return isTransfer[i];
}

void Metro::getOri(char*& Ori)
{
	std::cout << "请输入起点：";
	char ori[20] = { '\0' };
	std::cin >> ori;
	strcpy(Ori, ori);
}

void Metro::getDes(char*& Des)
{
	
	std::cout << "请输入终点：";
	char des[20] = { '\0' };
	std::cin >> des;
	strcpy(Des, des);
}

void Metro::Floyd()
{
	int v, w, k;
	for (v = 0; v < StationSum(); v++)
	{
		for (w = 0; w < StationSum(); w++)
		{
			FDistance[v][w] = FindArc(v, w);
			Path[v][w] = w;
		}
	}
	for (k = 0; k < StationSum(); k++)
	{
		for (v = 0; v < StationSum(); v++)
		{
			for (w = 0; w < StationSum(); w++)
			{
				if (FDistance[v][w] > FDistance[v][k] + FDistance[k][w])
				{
					FDistance[v][w] = FDistance[v][k] + FDistance[k][w];
					Path[v][w] = Path[v][k];
				}
			}
		}
	}
	SavePath();
	SaveFDistance();
}

void Metro::PrintRouteF(char* ori,char* des)//打印最短路线
{
	
	int m = 0;
	for (m = 0; m < 10; m += 2)
	{
		if (History[m] == Locate(ori) && History[m + 1] == Locate(des))break;
	}//比较历史记录中是否已有要查询的路线，防止重复
	if (m == 10) {
		History[(++Hrear) % 10] = Locate(ori);
		History[(++Hrear) % 10] = Locate(des);
	}

	int first = Locate(ori);
	int last = Locate(des);

	int count = 0;
	int i = first;//中转结点
	int *temp = new int[StationSum()];
	temp[0] = -1;
	//if (Path[first][last] == last)temp[++count]=last;
	while (i != last)
	{
		temp[++count] = Path[i][last];
		i = Path[i][last];
	}
	std::cout << "总共需要坐" << count + 1 << "个站,路线为：" << std::endl;
	std::cout << std::setw(12) << ori << std::endl;

	for (int i = 1; i <= count; i++)
	{
		//name = G.FindName(p[i]);
		if (temp[i] != last)
		{
			if (i == 1 && isTransSta(temp[i]) && FindLine(first, temp[i]) != FindLine(temp[i], temp[i + 1]))
			{
				if (FindLine(temp[i], temp[i + 1]) == 3)
				{
					std::cout << std::setw(12) << FindName(temp[i]) << "   换乘昌平线" << std::endl;
				}
				else
				{
					std::cout << std::setw(12) << FindName(temp[i]) << "   换乘" << FindLine(temp[i], temp[i + 1]) << "号线" << std::endl;
				}

			}

			else if (i != 1 && isTransSta(temp[i]) && FindLine(temp[i - 1], temp[i]) != FindLine(temp[i], temp[i + 1]))
			{
				if (FindLine(temp[i], temp[i + 1]) == 3)
				{
					std::cout << std::setw(12) << FindName(temp[i]) << "   换乘昌平线" << std::endl;
				}
				else
				{
					std::cout << std::setw(12) << FindName(temp[i]) << "   换乘" << FindLine(temp[i], temp[i + 1]) << "号线" << std::endl;
				}
			}
			else
			{
				std::cout << std::setw(12) << FindName(temp[i]) << std::endl;
			}
		}
		else std::cout << std::setw(12) << FindName(temp[i]) << std::endl;
	}
	delete[]temp;
}

void Metro::ShortPath(char*ori,char* des,int mode)//寻找最短路径
{
	int v = Locate(ori);//找到起点对应下标
	int count(0), judge(0);
	for (int i = 0; i < MaxNum; i++)
	{
		Disk[i] = FindArc(v, i);
		visited[i] = false;
		if (Disk[i] != NOTLINKED) {
			FormerVex[i] = v;     //初始化辅助数组
			interchange[i] = 0;
		}
		else {
			FormerVex[i] = -1;
			interchange[i] = NOTLINKED;
		}
	}
	visited[v] = true;
	Disk[v] = 0;

	for (int i = 0; i < MaxNum; i++)
	{
		if ((v = FindMin(MaxNum, mode)) == -1)//若v点为孤立结点，没有邻接点，则返回
			break;
		visited[v] = true;//每确定一个结点后再以它为中转点更新路径长度
		judge = 0;
		for (int k = 0; k < 10; k++) {
			if (forbidden[k] == v) {
				judge = 1;
			}
		}
		for (int j = 0; j < MaxNum; j++)//找到转发节点后更新辅助数组
		{
			if (mode == 1) {
				if (!visited[j] && (Disk[j] > FindArc(v, j) + Disk[v]))//按乘坐站点数多少决定是否更新
				{
					if (judge == 0 || FindLine(v, j) == FindLine(FormerVex[v], v)) {
						Disk[j] = FindArc(v, j) + Disk[v];
						FormerVex[j] = v;
					}
				}
			}
			if (mode == 2) {
				if (!visited[j] && FindLine(v, j) != -1) {
					if (FindLine(v, j) != FindLine(FormerVex[v], v)) {
						count = 1;
					}
					if (judge == 0 || FindLine(v, j) == FindLine(FormerVex[v], v)) {
						if ((count + interchange[v]) < interchange[j])//按换乘次数多少决定是否更新
						{
							Disk[j] = FindArc(v, j) + Disk[v];
							FormerVex[j] = v;
							interchange[j] = count + interchange[v];
						}
						else if ((count + interchange[v]) == interchange[j])//换乘次数一样时，选择站点数最少的
						{
							if (Disk[j] > FindArc(v, j) + Disk[v])
							{
								Disk[j] = FindArc(v, j) + Disk[v];
								FormerVex[j] = v;
							}
						}
						count = 0;
					}
				}
			}
		}
	}
	PrintRoute(ori, des,mode);
}

int Metro::FindMin(int n, int m)//寻找距离原始点最近的顶点
{
	int k = 0, min = MaxNum;
	for (int i = 0; i < n; i++)
	{
		if (m == 1) {
			if (!visited[i] && min > Disk[i])
			{
				min = Disk[i];
				k = i;
			}
		}
		if (m == 2) {
			if (!visited[i] && min > interchange[i])
			{
				min = interchange[i];
				k = i;
			}
		}
	}
	if (min == MaxNum) return -1;
	return k;
}

void Metro::PrintRoute(char*ori, char*des, int mode)//打印最短路线
{

	int first = Locate(ori);
	int last = Locate(des);
	int sta = last, length = Disk[last];
	int *p = new int[length];
	for (int k = 0; k < 10; k++) {
		if (first == forbidden[k] || last == forbidden[k]) {
			std::cout << "站点因特殊原因封站，无法乘坐地铁到达" << std::endl;
			return;
		}
	}
	for (int i = 0; sta != first; i++)//从终点回溯至起点获得路线
	{
		int judge = 0;
		for (int j = 0; j < 10; j++) {
			if (sta == forbidden[j]) {
				judge = 1;
				break;
			}
		}

		if (!judge) {
			p[i] = sta;
		}
		else length--;
		sta = FormerVex[sta];
	}
	if (mode == 2)std::cout << "总共换乘" << interchange[last] << "次" << std::endl;
	std::cout << "总共需要坐" << length + 1 << "个站，路线为：" << std::endl;
	std::cout << ori << std::endl;

	//char *name = NULL;
	for (int i = length - 1; i >= 0; i--)
	{
		//name = G.FindName(p[i]);
		if (strcmp(des, FindName(p[i])))
		{
			std::cout << FindName(p[i]);
			if (i == length - 1 && FindLine(first, p[i]) != FindLine(p[i], p[i - 1])) {
				if (FindLine(p[i], p[i - 1]) != 3)
					std::cout << "   换乘" << FindLine(p[i], p[i - 1]) << "号线";
				else std::cout << "   换乘昌平线";
			}
			if (i != length - 1 && FindLine(p[i + 1], p[i]) != FindLine(p[i], p[i - 1])) {
				if (FindLine(p[i], p[i - 1]) != 3)
					std::cout << "   换乘" << FindLine(p[i], p[i - 1]) << "号线";
				else std::cout << "   换乘昌平线";
			}
			std::cout << std::endl;
		}
		else std::cout << FindName(p[i]) << std::endl;
	}
	delete[]p;
}

char* Metro::search(char *&ori) {
	std::cout << std::endl << "当前为景点及地标查询模式，可供查询的景点有： " << std::endl;
	std::cout << std::setiosflags(std::ios::left)
		<< std::setw(20) << "1.天安门 故宫" << std::setw(20) << "2.国家博物馆" << std::setw(20) << "3.毛主席纪念堂" << std::endl
		<< std::setw(20) << "4.首都博物馆" << std::setw(20) << "5.军事博物馆" << std::setw(20) << "6.奥林匹克森林公园" << std::endl
		<< std::setw(20) << "7.鸟巢 水立方" << std::setw(20) << "8.颐和园" << std::setw(20) << "9.圆明园" << std::endl
		<< std::setw(20) << "10.前门 大栅栏" << std::setw(20) << "11.王府井" << std::setw(20) << "12.天坛" << std::endl
		<< std::setw(20) << "13.北京动物园" << std::setw(20) << "14.雍和宫" << std::setw(20) << "15.孔庙 国子监" << std::endl
		<< std::setw(20) << "16.什刹海" << std::setw(20) << "17.南锣鼓巷" << std::setw(20) << "18.景山公园" << std::endl
		<< std::setw(20) << "19.北海" << std::setw(20) << "20.玉渊潭公园" << std::setw(20) << "21.西单" << std::endl
		<< std::setw(20) << "22.钟鼓楼" << std::setw(20) << "23.中国地质博物馆" << std::setw(20) << "24.清华北大" << std::endl
		<< std::setw(20) << "25.中国美术馆" << std::setw(20) << "26.三里屯" << std::setw(20) << "27.世贸天阶" << std::endl
		<< std::setw(20) << "28.大钟寺" << std::setw(20) << "29.古观象台" << std::setw(20) << "30.中山公园" << std::endl
		<< std::setw(20) << "31.北京西站" << std::setw(20) << "32.北京站" << std::setw(20) << "33.北京南站" << std::endl;
	std::cout << std::endl << "请输入您当前所在站点： ";
	char tempOri[20] = {'\0'};
	std::cin >> tempOri;
	int i=Locate(tempOri);
	ori = FindName(i);
	std::cout << "请输入您想要去的景点序号，放弃搜索请输入0： ";
	int number;
	char *judge = NULL;
	std::cin >> number;
	switch (number)
	{
	case 1:
	case 2:
	case 3:
		judge = "天安门东"; break;
	case 4:
		judge = "木樨地"; break;
	case 5:
	case 20:
		judge = "军事博物馆"; break;
	case 6:
		judge = "森林公园南门"; break;
	case 7:
		judge = "奥体中心"; break;
	case 8:
		judge = "北宫门"; break;
	case 9:
		judge = "圆明园"; break;
	case 10:
		judge = "前门"; break;
	case 11:
		judge = "王府井"; break;
	case 12:
		judge = "天坛东门"; break;
	case 13:
		judge = "动物园"; break;
	case 14:
	case 15:
		judge = "雍和宫"; break;
	case 16:
		judge = "什刹海"; break;
	case 17:
	case 18:
		judge = "南锣鼓巷"; break;
	case 19:
		judge = "北海北"; break;
	case 21:
		judge = "西单"; break;
	case 22:
		judge = "鼓楼大街"; break;
	case 23:
		judge = "西四"; break;
	case 24:
		judge = "北京大学东门"; break;
	case 25:
		judge = "东四"; break;
	case 26:
		judge = "团结湖"; break;
	case 27:
		judge = "金台夕照"; break;
	case 28:
		judge = "大钟寺"; break;
	case 29:
		judge = "建国门"; break;
	case 30:
		judge = "天安门西"; break;
	case 31:
		judge = "北京西站"; break;
	case 32:
		judge = "北京站"; break;
	case 33:
		judge = "北京南站"; break;
	case 0:
	default:
		judge = NULL;	break;
	}
	return judge;
}

void Metro::ListHistory(char*& ori, char*& des)
{
	int m = 0;
	char choice;
	bool HasHistory = false;
	for (m = 0; m < 10 && History[m] != -1; m += 2)
	{
		HasHistory = true;
		break;
	}
	if (HasHistory)
	{
		std::cout << "以下是您之前的查询记录：" << std::endl;
		for (m = 0; m < 10 && History[m] != -1; m += 2)
		{
			std::cout << (m / 2) + 1 << '.'
				<< std::setw(18) << FindName(History[m])
				<< "——————>"
				<< std::setw(18) << FindName(History[m + 1]) << std::endl;
		}
		std::cout << "请问查询记录中是否有您要查询的线路，有请输入序号，无则输入任意字母以进入下一步：";
		std::cin >> choice;
		if (choice >= '1' && choice <= '5')//asc2码的1-5
		{
			ori = FindName(History[static_cast<int>((choice-'0') * 2-2)]);
			des = FindName(History[static_cast<int>((choice-'0') * 2-1)]);
		}
		else
		{
			ori = NULL;
			des = NULL;
		}
	}
	else return;
}

void Metro::SavePath()
{
	std::ofstream pathFile;
	pathFile.open("Path.txt");
	for (int i = 0; i < MaxNum; i++)
	{
		for (int j = 0; j < MaxNum; j++)
		{
			pathFile << Path[i][j] << ' ';
		}
		pathFile << std::endl;
	}
	pathFile.close();
}

void Metro::SaveFDistance()
{
	std::ofstream distanceFile;
	distanceFile.open("FDistance.txt");
	for (int i = 0; i < MaxNum; i++)
	{
		for (int j = 0; j < MaxNum; j++)
		{
			distanceFile << FDistance[i][j] << ' ';
		}
		distanceFile << std::endl;
	}
	distanceFile.close();

}

void Metro::SaveHistory()
{
	std::ofstream HistoryFile;
	HistoryFile.open("History.txt");
	HistoryFile << Hrear << ' ';
	for (int i = 0; i < 10; i++)
	{
		HistoryFile << History[i] << ' ';
	}
	HistoryFile.close();

}

void Metro::ReadPath()
{
	std::ifstream pathFile;
	pathFile.open("Path.txt");
	if (!pathFile.is_open())return;

	for (int i = 0; i < MaxNum; i++)
	{
		for (int j = 0; j < MaxNum; j++)
		{
			pathFile >> Path[i][j];
		}
	}
	pathFile.close();
}

void Metro::ReadVex()
{
	std::ifstream vexFile;
	vexFile.open("Vex.txt");
	if (!vexFile.is_open())return;

	vexFile >> staSum;

	for (int i = 0; i < staSum; i++)
	{
		vexFile >> vexList[i].vexName;
		for (int k = 0; k < 3; k++)
		{
			vexFile >> vexList[i].LineNum[k];
		}
	}
	vexFile.close();

	Vertex temp;
	for (int i = 0; i < staSum; i++)
	{
		temp = vexList[i];
		isTransfer[i] = false;
		for (int j = i + 1; j < staSum; j++)
		{
			if (strcmp(vexList[j].vexName, temp.vexName) == 0)
				isTransfer[i] = true;
		}
	}
	//std::cout << 1;//调试用

}

void Metro::ReadArc()
{
	std::ifstream arcFile;
	arcFile.open("Arc.txt");
	if (!arcFile.is_open())return;

	for (int i = 0; i < MaxNum; i++)
	{
		for (int j = 0; j < MaxNum; j++)
		{
			arcFile >> ArcList[i][j].LineNum;
			arcFile >> ArcList[i][j].weight;
		}
	}
	arcFile.close();
}

void Metro::ReadFDistance()
{
	std::ifstream distanceFile;
	distanceFile.open("FDistance.txt");
	if (!distanceFile.is_open())return;

	for (int i = 0; i < MaxNum; i++)
	{
		for (int j = 0; j < MaxNum; j++)
		{
			distanceFile >> FDistance[i][j];
		}
	}
	distanceFile.close();

}

void Metro::ReadHistory()
{
	std::ifstream HistoryFile;
	HistoryFile.open("History.txt");
	if (!HistoryFile.is_open())return;

	HistoryFile >> Hrear;
	for (int i = 0; i < 10; i++)
	{
		HistoryFile >> History[i];
	}
	HistoryFile.close();
}

void Metro::SaveVex()
{
	std::ofstream vexFile;
	vexFile.open("Vex.txt");
	vexFile << staSum << std::endl;
	for (int i = 0; i < MaxNum; i++)
	{
		vexFile << vexList[i].vexName << " ";
		for (int k = 0; k < 3; k++)
		{
			vexFile << vexList[i].LineNum[k] << " ";
		}
		vexFile << std::endl;
	}
	vexFile.close();

}

void Metro::SaveArc()
{
	std::ofstream arcFile;
	arcFile.open("Arc.txt");
	for (int i = 0; i < MaxNum; i++)
	{
		for (int j = 0; j < MaxNum; j++)
		{
			arcFile << ArcList[i][j].LineNum << ' ';
			arcFile << ArcList[i][j].weight << ' ';
		}
		arcFile << std::endl;
	}
	arcFile.close();

}

void Metro::MainScene()
{
	int choice;
	bool state = true;
	char* ori = NULL;
	char* des = NULL;
	char* Destination = NULL;
	char* Origin = NULL;
	int mode;
	int m;

	while (state)
	{
		system("cls");
		std::cout << "\n\n\t\t\t" << "******************************" << std::endl;
		std::cout << "\t\t\t" << "*                            *" << std::endl;
		std::cout << "\t\t\t" << "*  欢迎来到北京地铁查询系统  *" << std::endl;
		std::cout << "\t\t\t" << "*                            *" << std::endl;
		std::cout << "\t\t\t" << "******************************" << std::endl;
		std::cout << "\t\t\t" << "     本系统提供以下功能：	 " << std::endl;
		std::cout << "\t\t\t" << "******************************" << std::endl;
		std::cout << "\t\t\t" << "     1.查询站点信息" << std::endl;
		std::cout << "\t\t\t" << "     2.查询线路信息" << std::endl;
		std::cout << "\t\t\t" << "******************************" << std::endl;
		std::cout << "\t\t\t" << "     3.景点路线规划" << std::endl;
		std::cout << "\t\t\t" << "     4.最短线路规划" << std::endl;
		std::cout << "\t\t\t" << "     5.最少换乘路径规划" << std::endl;
		std::cout << "\t\t\t" << "******************************" << std::endl;
		std::cout << "\t\t\t" << "     6.退出系统" << std::endl;
		std::cout << std::endl;
		std::cout << "请在此输入您的选择：";
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			ShowSta();
			system("pause");
			break;
		case 2:
			ShowLine();
			system("pause");
			break;
		case 3:
			ListHistory(ori, des);

			if (ori == NULL)
			{
				Destination = search(ori);
				Origin = ori;
				if (Destination == NULL)
				{
					system("pause");
					break;
				}//取消操作，返回主界面

				for (m = 0; m < 10; m += 2)
				{
					if (History[m] == Locate(Origin) && History[m + 1] == Locate(Destination))break;
				}//比较历史记录中是否已有要查询的路线，防止重复
				if (m == 10) {
					History[(++Hrear) % 10] = Locate(Origin);
					History[(++Hrear) % 10] = Locate(Destination);
				}
			}
			else
			{
				Origin = ori;
				Destination = des;
			}
			SaveHistory();
			std::cout << std::endl << "查询结果：" << std::endl;
			PrintRouteF(Origin, Destination);
			system("pause");
			break;
		case 4:
			ListHistory(ori, des);
			if (ori == NULL) 
			{ 
				ori = new char[20];
				des = new char[20];

				getOri(ori);
				getDes(des);
				std::cout << "最短路径为：" << std::endl;
				PrintRouteF(ori,des); 

				for (m = 0; m < 10; m += 2)
				{
					if (History[m] == Locate(ori) && History[m + 1] == Locate(des))break;
				}//比较历史记录中是否已有要查询的路线，防止重复
				if (m == 10) {
					History[(++Hrear) % 10] = Locate(ori);
					History[(++Hrear) % 10] = Locate(des);
				}

				delete[]ori;
				ori = NULL;
				delete[]des;
				des = NULL;
			}
			else
			{
				Origin = ori;
				Destination = des;
				PrintRouteF(Origin, Destination);
			}
			SaveHistory();
			system("pause");
			break;
		case 5:
			mode = 2;

			ListHistory(ori, des);
			if (ori == NULL) 
			{
				ori = new char[20];
				des = new char[20];

				getOri(ori);
				getDes(des);
				std::cout << "最短路径为：" << std::endl;

				for (m = 0; m < 10; m += 2)
				{
					if (History[m] == Locate(ori) && History[m + 1] == Locate(des))break;
				}//比较历史记录中是否已有要查询的路线，防止重复
				if (m == 10) {
					History[(++Hrear) % 10] = Locate(ori);
					History[(++Hrear) % 10] = Locate(des);
				}

				ShortPath(ori, des, mode);
				
				delete[]ori;
				ori = NULL;
				delete[]des;
				des = NULL;
			}
			else
			{
				Origin = ori;
				Destination = des;
				ShortPath(Origin, Destination, mode);
			}
			SaveHistory();
			system("pause");
			break;
		case 6:
			state = false;
			break;
		}
	}
}

void Metro::ShowSta()
{
	std::cout << "请输入您想查询的站点：";
	char name[20];
	std::cin >> name;
	int loc = Locate(name);
	std::cout << name << std::endl;
	for (int i = 0; i < 3; i++)
	{
		if (vexList[loc].LineNum[i] != 0)
		{
			if (vexList[loc].LineNum[i] == 3)std::cout << "昌平线" << std::endl;
			else std::cout << vexList[loc].LineNum[i] << "号线" << std::endl;
		}

	}
}

void Metro::ShowLine()
{
	int num;
	std::cout << "请输入您想要查询的线路：(请输入数字，昌平线代号为3)";
	std::cin >> num;
	int i = 0;
	while (Line[num].StationName[i][0] != '\0')
	{
		std::cout << Line[num].StationName[i++] << std::endl;
	}
}

