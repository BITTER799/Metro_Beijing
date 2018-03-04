//�����ĵ�����ʣ�

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

//����ȫ�����ӿ������ٶ�
int Path[MaxNum][MaxNum];//F�㷨��������֮����ת���
int FDistance[MaxNum][MaxNum];//����F�㷨�õ��ĸ������С����


//****************�������ṹ�Ľ�����********************//
struct MetroLine//������·
{
	int num;//��·��
	int staNum;//վ�����
	int isRing;//�Ƿ�Ϊ��·
	char StationName[50][20];//վ������
	MetroLine()
	{
		num = -1;
		staNum = EMPTY;
		isRing = 0;
		StationName[0][0] = '\0';
	}
};

struct Vertex//���㣨վ�㣩
{
	char vexName[20];
	int LineNum[3] = { 0 };
	Vertex()
	{
		vexName[0] = '\0';
	}

};

namespace metro {
	struct Arc//��->�ڽӾ���Ԫ��
	{
		int weight;//Ȩֵ
		int LineNum;//��������·
	};//Arc��Windows.h�е�һ������������
}
//****************���ӱ��ػ�ȡ��·��Ϣ��*****************//

//****************��Metro��Ľ�����********************//
class Metro
{
public:
	//****************����ʼ��������********************//
	void GettingData(MetroLine & Line, char *fileName);
	void InitLine();
	void Detect();//����ı��ļ��Ƿ���ڣ���Ϊ����ʼ��ʼ��
	void InitVex();//��ʼ�������
	void InitArcMatrix();//��ʼ���ڽӾ���
	void MainScene();//������
	void ShowSta();//��ѯվ����Ϣ
	void ShowLine();//��ѯ��·��Ϣ

	//****************�������ṩ�Ľӿڡ�*****************//
	int Locate(char *staName);//�ҵ�վ���ڶ������±�
	int FindArc(int i, int j);//�ҵ��߶�Ӧ��Ȩֵ
	int FindLine(int i, int j);//���ر�������·
	char* FindName(int i);//����վ������
	int StationSum();//����վ������
	bool isTransSta(int i);//�����Ƿ�Ϊ����վ

	//****************�����·�߹滮��********************//
	void getOri(char*& Ori);//�������
	void getDes(char*& Des);//�����յ�

	void Floyd();//���������㷨����Path��FDistance����
	void PrintRouteF(char* ori, char* des);//��ӡ���·��

	void ShortPath(char*ori, char* des, int mode);//dijkstra�㷨
	int FindMin(int n, int m);
	void PrintRoute(char*ori, char*des, int mode );

	char* search(char *&ori);//����·�߹滮

	void ListHistory(char*& ori,char*& des);

	//****************���ļ���ȡ�봢�桿********************//
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
	MetroLine Line[MaxLine];//�洢��·��Ϣ
	Vertex vexList[MaxNum];//�����
	metro::Arc ArcList[MaxNum][MaxNum];//�߾���
	int staSum;//վ������
	bool isTransfer[MaxNum];//�����Ƿ�Ϊ����վ

	bool visited[MaxNum] = { false };//��¼���ö�������·���Ƿ��Ѿ��ҵ�
	int Disk[MaxNum];//��¼ԭʼ���㵽ͼ�ϸ������·������
	int FormerVex[MaxNum];//��¼�������·���µ�ǰ��
	int interchange[MaxNum];//��¼���������軻�˴���
	int forbidden[10] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };//��վ��Ϣ
	int History[10] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };//��ʷ��¼��������
	int Hrear=-1;//��ʷ��¼��������βָ��
};

//****************����Ա������ʵ�֡�********************//
void Metro::GettingData(MetroLine & Line, char *fileName)
{
	int i;
	FILE *fp;
	int no;//�����ж�fscanf�Ƿ�����˿���
	if ((fp = fopen(fileName, "r")) == NULL)
	{
		throw FILEERROR;
	}
	no = fscanf(fp, "%d %d %d", &(Line.num), &(Line.staNum), &(Line.isRing));
	for (i = 0; no != EOF; i++)
	{
		no = fscanf(fp, "%s", Line.StationName[i]);
	}
	for (i = i - 1; i < 49; i++)//���հ���ȫ�����'\0'�����ڳ�ʼ�������ʱ�ﵽif������
	{
		Line.StationName[i][0] = '\0';
	}
	fclose(fp);
}

void Metro::InitLine()
{
	GettingData(Line[1], "����1����.txt");
	GettingData(Line[2], "����2����.txt");
	GettingData(Line[3], "������ƽ��.txt");
	GettingData(Line[4], "����4����.txt");
	GettingData(Line[5], "����5����.txt");
	GettingData(Line[6], "����6����.txt");
	GettingData(Line[7], "����7����.txt");
	GettingData(Line[8], "����8����.txt");
	GettingData(Line[9], "����9����.txt");
	GettingData(Line[10], "����10����.txt");
	GettingData(Line[13], "����13����.txt");
	//GettingData(Line[15], "����15����.txt");
}

void Metro::Detect()
{
	HANDLE hFile;
	LPCTSTR lpFileName = L".\\Vex.txt";
	WIN32_FIND_DATA pNextInfo;  //�����õ����ļ���Ϣ��������pNextInfo��;  
	hFile = FindFirstFile(lpFileName, &pNextInfo);//��ע���� &pNextInfo , ���� pNextInfo;  	
	if (hFile != INVALID_HANDLE_VALUE)
	{
		//std::cout << 1;//������
		ReadVex();//�����ɹ���ʼ��ȡ�ļ�
		std::cout << "վ����Ϣ��ȡ��ϣ�" << std::endl;
	}
	else
	{
		InitVex();
		std::cout << "վ����Ϣ��ʼ�����򱾵ش洢��ϣ�" << std::endl;
	}

	lpFileName = L".\\Arc.txt";
	hFile = FindFirstFile(lpFileName, &pNextInfo);//��ע���� &pNextInfo , ���� pNextInfo;  	
	if (hFile != INVALID_HANDLE_VALUE)
	{
		ReadArc();//�����ɹ���ʼ��ȡ�ļ�
		std::cout << "��·������Ϣ��ȡ��ϣ�" << std::endl;
	}
	else
	{
		InitArcMatrix();
		std::cout << "��·������Ϣ��ʼ�����򱾵ش洢��ϣ�" << std::endl;
	}

	lpFileName = L".\\Path.txt";
	hFile = FindFirstFile(lpFileName, &pNextInfo);//��ע���� &pNextInfo , ���� pNextInfo;  	
	if (hFile != INVALID_HANDLE_VALUE)
	{
		ReadPath();//�����ɹ���ʼ��ȡ�ļ�
	}
	else
	{
		Floyd();
	}

	lpFileName = L".\\FDistance.txt";
	hFile = FindFirstFile(lpFileName, &pNextInfo);//��ע���� &pNextInfo , ���� pNextInfo;  	
	if (hFile != INVALID_HANDLE_VALUE)
	{
		ReadFDistance();//�����ɹ���ʼ��ȡ�ļ�
	}
	else
	{
		Floyd();
	}

	lpFileName = L".\\History.txt";
	hFile = FindFirstFile(lpFileName, &pNextInfo);//��ע���� &pNextInfo , ���� pNextInfo;  	
	if (hFile != INVALID_HANDLE_VALUE)
	{
		ReadHistory();//�����ɹ���ʼ��ȡ�ļ�
		std::cout << "��ʷ��ѯ��¼��ȡ��ϣ�" << std::endl;
	}

	system("pause");
}

void Metro::InitVex()
{
	int k = 0;
	staSum = 0;
	for (int i = 0; i < MaxLine; i++)
	{
		for (int j = 0; j < Line[i].staNum; j++)//�м��ж��Ѿ��ž��˼���ɨ����·��Ŀ��ಿ�ֵ����
		{
			if (strcmp(vexList[k].vexName, Line[i].StationName[j]))//�ж�Line��վ�����Ʊ��Ƿ�ͷ
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
	int i, j, m, n, start, end;//start,end���ڼ�¼��·����ʼվ���յ�վ
	for (i = 0; i < MaxNum; i++)//��ʼ��Ȩֵ
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
			if (j == 0 && Line[i].isRing == 1) { start = m; }//��·����ʼվ
			if (j == Line[i].staNum - 2 && Line[i].isRing == 1) { end = n; }//��·���յ�վ
			ArcList[m][n].weight = LINKED;
			ArcList[n][m].weight = LINKED;
			ArcList[m][n].LineNum = i;
			ArcList[n][m].LineNum = i;
		}
		if (Line[i].isRing == 1)//����·�������յ�����һ��
		{
			ArcList[start][end].weight = LINKED;
			ArcList[end][start].weight = LINKED;
			ArcList[start][end].LineNum = i;
			ArcList[end][start].LineNum = i;
		}
	}
	SaveArc();
}

int Metro::Locate(char * staName)//�ҵ�վ���ڶ�������±�
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

int Metro::FindArc(int i, int j)//�ұߣ�����Ȩֵ
{
	return ArcList[i][j].weight;
}

int Metro::FindLine(int i, int j)
{
	return ArcList[i][j].LineNum;
}

char* Metro::FindName(int i)//����վ������
{
	return vexList[i].vexName;
}

int Metro::StationSum()//����վ������
{
	return staSum;
}

bool Metro::isTransSta(int i)
{
	return isTransfer[i];
}

void Metro::getOri(char*& Ori)
{
	std::cout << "��������㣺";
	char ori[20] = { '\0' };
	std::cin >> ori;
	strcpy(Ori, ori);
}

void Metro::getDes(char*& Des)
{
	
	std::cout << "�������յ㣺";
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

void Metro::PrintRouteF(char* ori,char* des)//��ӡ���·��
{
	
	int m = 0;
	for (m = 0; m < 10; m += 2)
	{
		if (History[m] == Locate(ori) && History[m + 1] == Locate(des))break;
	}//�Ƚ���ʷ��¼���Ƿ�����Ҫ��ѯ��·�ߣ���ֹ�ظ�
	if (m == 10) {
		History[(++Hrear) % 10] = Locate(ori);
		History[(++Hrear) % 10] = Locate(des);
	}

	int first = Locate(ori);
	int last = Locate(des);

	int count = 0;
	int i = first;//��ת���
	int *temp = new int[StationSum()];
	temp[0] = -1;
	//if (Path[first][last] == last)temp[++count]=last;
	while (i != last)
	{
		temp[++count] = Path[i][last];
		i = Path[i][last];
	}
	std::cout << "�ܹ���Ҫ��" << count + 1 << "��վ,·��Ϊ��" << std::endl;
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
					std::cout << std::setw(12) << FindName(temp[i]) << "   ���˲�ƽ��" << std::endl;
				}
				else
				{
					std::cout << std::setw(12) << FindName(temp[i]) << "   ����" << FindLine(temp[i], temp[i + 1]) << "����" << std::endl;
				}

			}

			else if (i != 1 && isTransSta(temp[i]) && FindLine(temp[i - 1], temp[i]) != FindLine(temp[i], temp[i + 1]))
			{
				if (FindLine(temp[i], temp[i + 1]) == 3)
				{
					std::cout << std::setw(12) << FindName(temp[i]) << "   ���˲�ƽ��" << std::endl;
				}
				else
				{
					std::cout << std::setw(12) << FindName(temp[i]) << "   ����" << FindLine(temp[i], temp[i + 1]) << "����" << std::endl;
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

void Metro::ShortPath(char*ori,char* des,int mode)//Ѱ�����·��
{
	int v = Locate(ori);//�ҵ�����Ӧ�±�
	int count(0), judge(0);
	for (int i = 0; i < MaxNum; i++)
	{
		Disk[i] = FindArc(v, i);
		visited[i] = false;
		if (Disk[i] != NOTLINKED) {
			FormerVex[i] = v;     //��ʼ����������
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
		if ((v = FindMin(MaxNum, mode)) == -1)//��v��Ϊ������㣬û���ڽӵ㣬�򷵻�
			break;
		visited[v] = true;//ÿȷ��һ������������Ϊ��ת�����·������
		judge = 0;
		for (int k = 0; k < 10; k++) {
			if (forbidden[k] == v) {
				judge = 1;
			}
		}
		for (int j = 0; j < MaxNum; j++)//�ҵ�ת���ڵ����¸�������
		{
			if (mode == 1) {
				if (!visited[j] && (Disk[j] > FindArc(v, j) + Disk[v]))//������վ�������پ����Ƿ����
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
						if ((count + interchange[v]) < interchange[j])//�����˴������پ����Ƿ����
						{
							Disk[j] = FindArc(v, j) + Disk[v];
							FormerVex[j] = v;
							interchange[j] = count + interchange[v];
						}
						else if ((count + interchange[v]) == interchange[j])//���˴���һ��ʱ��ѡ��վ�������ٵ�
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

int Metro::FindMin(int n, int m)//Ѱ�Ҿ���ԭʼ������Ķ���
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

void Metro::PrintRoute(char*ori, char*des, int mode)//��ӡ���·��
{

	int first = Locate(ori);
	int last = Locate(des);
	int sta = last, length = Disk[last];
	int *p = new int[length];
	for (int k = 0; k < 10; k++) {
		if (first == forbidden[k] || last == forbidden[k]) {
			std::cout << "վ��������ԭ���վ���޷�������������" << std::endl;
			return;
		}
	}
	for (int i = 0; sta != first; i++)//���յ�����������·��
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
	if (mode == 2)std::cout << "�ܹ�����" << interchange[last] << "��" << std::endl;
	std::cout << "�ܹ���Ҫ��" << length + 1 << "��վ��·��Ϊ��" << std::endl;
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
					std::cout << "   ����" << FindLine(p[i], p[i - 1]) << "����";
				else std::cout << "   ���˲�ƽ��";
			}
			if (i != length - 1 && FindLine(p[i + 1], p[i]) != FindLine(p[i], p[i - 1])) {
				if (FindLine(p[i], p[i - 1]) != 3)
					std::cout << "   ����" << FindLine(p[i], p[i - 1]) << "����";
				else std::cout << "   ���˲�ƽ��";
			}
			std::cout << std::endl;
		}
		else std::cout << FindName(p[i]) << std::endl;
	}
	delete[]p;
}

char* Metro::search(char *&ori) {
	std::cout << std::endl << "��ǰΪ���㼰�ر��ѯģʽ���ɹ���ѯ�ľ����У� " << std::endl;
	std::cout << std::setiosflags(std::ios::left)
		<< std::setw(20) << "1.�찲�� �ʹ�" << std::setw(20) << "2.���Ҳ����" << std::setw(20) << "3.ë��ϯ������" << std::endl
		<< std::setw(20) << "4.�׶������" << std::setw(20) << "5.���²����" << std::setw(20) << "6.����ƥ��ɭ�ֹ�԰" << std::endl
		<< std::setw(20) << "7.�� ˮ����" << std::setw(20) << "8.�ú�԰" << std::setw(20) << "9.Բ��԰" << std::endl
		<< std::setw(20) << "10.ǰ�� ��դ��" << std::setw(20) << "11.������" << std::setw(20) << "12.��̳" << std::endl
		<< std::setw(20) << "13.��������԰" << std::setw(20) << "14.Ӻ�͹�" << std::setw(20) << "15.���� ���Ӽ�" << std::endl
		<< std::setw(20) << "16.ʲɲ��" << std::setw(20) << "17.�������" << std::setw(20) << "18.��ɽ��԰" << std::endl
		<< std::setw(20) << "19.����" << std::setw(20) << "20.��Ԩ̶��԰" << std::setw(20) << "21.����" << std::endl
		<< std::setw(20) << "22.�ӹ�¥" << std::setw(20) << "23.�й����ʲ����" << std::setw(20) << "24.�廪����" << std::endl
		<< std::setw(20) << "25.�й�������" << std::setw(20) << "26.������" << std::setw(20) << "27.��ó���" << std::endl
		<< std::setw(20) << "28.������" << std::setw(20) << "29.�Ź���̨" << std::setw(20) << "30.��ɽ��԰" << std::endl
		<< std::setw(20) << "31.������վ" << std::setw(20) << "32.����վ" << std::setw(20) << "33.������վ" << std::endl;
	std::cout << std::endl << "����������ǰ����վ�㣺 ";
	char tempOri[20] = {'\0'};
	std::cin >> tempOri;
	int i=Locate(tempOri);
	ori = FindName(i);
	std::cout << "����������Ҫȥ�ľ�����ţ���������������0�� ";
	int number;
	char *judge = NULL;
	std::cin >> number;
	switch (number)
	{
	case 1:
	case 2:
	case 3:
		judge = "�찲�Ŷ�"; break;
	case 4:
		judge = "ľ�ص�"; break;
	case 5:
	case 20:
		judge = "���²����"; break;
	case 6:
		judge = "ɭ�ֹ�԰����"; break;
	case 7:
		judge = "��������"; break;
	case 8:
		judge = "������"; break;
	case 9:
		judge = "Բ��԰"; break;
	case 10:
		judge = "ǰ��"; break;
	case 11:
		judge = "������"; break;
	case 12:
		judge = "��̳����"; break;
	case 13:
		judge = "����԰"; break;
	case 14:
	case 15:
		judge = "Ӻ�͹�"; break;
	case 16:
		judge = "ʲɲ��"; break;
	case 17:
	case 18:
		judge = "�������"; break;
	case 19:
		judge = "������"; break;
	case 21:
		judge = "����"; break;
	case 22:
		judge = "��¥���"; break;
	case 23:
		judge = "����"; break;
	case 24:
		judge = "������ѧ����"; break;
	case 25:
		judge = "����"; break;
	case 26:
		judge = "�Ž��"; break;
	case 27:
		judge = "��̨Ϧ��"; break;
	case 28:
		judge = "������"; break;
	case 29:
		judge = "������"; break;
	case 30:
		judge = "�찲����"; break;
	case 31:
		judge = "������վ"; break;
	case 32:
		judge = "����վ"; break;
	case 33:
		judge = "������վ"; break;
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
		std::cout << "��������֮ǰ�Ĳ�ѯ��¼��" << std::endl;
		for (m = 0; m < 10 && History[m] != -1; m += 2)
		{
			std::cout << (m / 2) + 1 << '.'
				<< std::setw(18) << FindName(History[m])
				<< "������������>"
				<< std::setw(18) << FindName(History[m + 1]) << std::endl;
		}
		std::cout << "���ʲ�ѯ��¼���Ƿ�����Ҫ��ѯ����·������������ţ���������������ĸ�Խ�����һ����";
		std::cin >> choice;
		if (choice >= '1' && choice <= '5')//asc2���1-5
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
	//std::cout << 1;//������

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
		std::cout << "\t\t\t" << "*  ��ӭ��������������ѯϵͳ  *" << std::endl;
		std::cout << "\t\t\t" << "*                            *" << std::endl;
		std::cout << "\t\t\t" << "******************************" << std::endl;
		std::cout << "\t\t\t" << "     ��ϵͳ�ṩ���¹��ܣ�	 " << std::endl;
		std::cout << "\t\t\t" << "******************************" << std::endl;
		std::cout << "\t\t\t" << "     1.��ѯվ����Ϣ" << std::endl;
		std::cout << "\t\t\t" << "     2.��ѯ��·��Ϣ" << std::endl;
		std::cout << "\t\t\t" << "******************************" << std::endl;
		std::cout << "\t\t\t" << "     3.����·�߹滮" << std::endl;
		std::cout << "\t\t\t" << "     4.�����·�滮" << std::endl;
		std::cout << "\t\t\t" << "     5.���ٻ���·���滮" << std::endl;
		std::cout << "\t\t\t" << "******************************" << std::endl;
		std::cout << "\t\t\t" << "     6.�˳�ϵͳ" << std::endl;
		std::cout << std::endl;
		std::cout << "���ڴ���������ѡ��";
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
				}//ȡ������������������

				for (m = 0; m < 10; m += 2)
				{
					if (History[m] == Locate(Origin) && History[m + 1] == Locate(Destination))break;
				}//�Ƚ���ʷ��¼���Ƿ�����Ҫ��ѯ��·�ߣ���ֹ�ظ�
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
			std::cout << std::endl << "��ѯ�����" << std::endl;
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
				std::cout << "���·��Ϊ��" << std::endl;
				PrintRouteF(ori,des); 

				for (m = 0; m < 10; m += 2)
				{
					if (History[m] == Locate(ori) && History[m + 1] == Locate(des))break;
				}//�Ƚ���ʷ��¼���Ƿ�����Ҫ��ѯ��·�ߣ���ֹ�ظ�
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
				std::cout << "���·��Ϊ��" << std::endl;

				for (m = 0; m < 10; m += 2)
				{
					if (History[m] == Locate(ori) && History[m + 1] == Locate(des))break;
				}//�Ƚ���ʷ��¼���Ƿ�����Ҫ��ѯ��·�ߣ���ֹ�ظ�
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
	std::cout << "�����������ѯ��վ�㣺";
	char name[20];
	std::cin >> name;
	int loc = Locate(name);
	std::cout << name << std::endl;
	for (int i = 0; i < 3; i++)
	{
		if (vexList[loc].LineNum[i] != 0)
		{
			if (vexList[loc].LineNum[i] == 3)std::cout << "��ƽ��" << std::endl;
			else std::cout << vexList[loc].LineNum[i] << "����" << std::endl;
		}

	}
}

void Metro::ShowLine()
{
	int num;
	std::cout << "����������Ҫ��ѯ����·��(���������֣���ƽ�ߴ���Ϊ3)";
	std::cin >> num;
	int i = 0;
	while (Line[num].StationName[i][0] != '\0')
	{
		std::cout << Line[num].StationName[i++] << std::endl;
	}
}

