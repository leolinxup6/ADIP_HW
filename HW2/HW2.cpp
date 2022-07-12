#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <queue>
#include <ctime>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;
void openRaw(const char filename[], unsigned char* tmp, int h, int w);
void storeRaw(const char filename[], unsigned char* tmp, int h, int w, int size);
void zoom1(unsigned char* ori, unsigned char* zoom, int h, int w);//放大眼睛
void zoom2(unsigned char* ori, unsigned char* zoom, int h, int w, double s);//放大圖片(倍數)Nearest Neighbor
void zoom3(unsigned char* ori, unsigned char* zoom, int h, int w, double s);//放大圖片(倍數)Bilinear Interpolation
void zoom4(unsigned char* ori, unsigned char* zoom, int h, int w, double s);//放大圖片(倍數)Bicubic  Interpolation
void shrink1(unsigned char* ori, unsigned char* shr, int h, int w);//縮小圖片2
int Edge(int x, int min, int max);
float CubicHermite(float A, float B, float C, float D, float t);
//gray-level
void graylevel(unsigned char* ori, unsigned char* gray, int h, int w,int t);
double MSE(unsigned char* image1, unsigned char* image2, int h, int w);
double PSNR(unsigned char* image1, unsigned char* image2, int h, int w);
//map
void WhichD(unsigned char* maze, unsigned char* path);
void FourD(unsigned char* maze,unsigned char* path, int GrayValue);
void EightD(unsigned char* maze, unsigned char* path, int GrayValue);
void mD(unsigned char* maze, unsigned char* path, int GrayValue);


int D4(int x, int y, int step, int& beststep, unsigned char* map, unsigned char* bestmap)
{
	char temp[1];

	if (x == 19 && y == 19)
	{
		if (step < beststep)
		{
			beststep = step;
			for (int i = 0; i < 20; i++)
			{
				for (int j = 0; j < 20; j++)
				{
					if (map[i * 20 + j] == 255)
						bestmap[i * 20 + j] = map[i * 20 + j];
					else
						bestmap[i * 20 + j] = 0;
				}
			}
		}
		return 1;
	}
	if (map[y * 20 + x] != 1)
	{
		return 1;
	}
	if (x < 0 || y < 0 || x > 19 || y > 19 || step == beststep)
	{
		return 1;
	}
	temp[0] = map[y * 20 + x];
	map[y * 20 + x] = 255;
	step++;
	D4(x + 1, y, step, beststep, map, bestmap);
	D4(x, y + 1, step, beststep, map, bestmap);
	D4(x - 1, y, step, beststep, map, bestmap);
	D4(x, y - 1, step, beststep, map, bestmap);
	map[y * 20 + x] = temp[0];
	step--;
}
int Dm(int x, int y, int step, int& beststep, unsigned char* map, unsigned char* bestmap/*,int dm*/)
{
	char temp[1];
	if (x == 19 && y == 19)
	{
		if (step < beststep)
		{
			beststep = step;

			for (int i = 0; i < 20; i++)
			{
				for (int j = 0; j < 20; j++)
				{
					if (map[i * 20 + j] == 255)
						bestmap[i * 20 + j] = map[i * 20 + j];
					else
						bestmap[i * 20 + j] = 0;
				}
			}

		}
		return 1;
	}
	if (map[y * 20 + x] != 1)
	{
		return 1;
	}
	if (x < 0 || y < 0 || x > 19 || y > 19 || step == beststep)
	{
		return 1;
	}
	temp[0] = map[y * 20 + x];
	map[y * 20 + x] = 255;
	step++;
	if (map[x + (y - 1) * 20] != 1 && map[x + (y + 1) * 20] != 1 && map[(x - 1) + y * 20] != 1 && map[(x + 1) + y * 20] != 1) {
		Dm(x - 1, y + 1, step, beststep, map, bestmap);
		Dm(x + 1, y - 1, step, beststep, map, bestmap);
		Dm(x - 1, y - 1, step, beststep, map, bestmap);
		Dm(x + 1, y + 1, step, beststep, map, bestmap);
	}
	Dm(x, y - 1, step, beststep, map, bestmap);
	Dm(x, y + 1, step, beststep, map, bestmap);
	Dm(x + 1, y, step, beststep, map, bestmap);
	Dm(x - 1, y, step, beststep, map, bestmap);
	map[y * 20 + x] = temp[0];
	step--;
}
int D8(int x, int y, int step, int& beststep, unsigned char* map, unsigned char* bestmap)
{
	char temp[1];
	if (x == 19 && y == 19)
	{
		if (step < beststep)
		{
			beststep = step;

			for (int i = 0; i < 20; i++)
			{
				for (int j = 0; j < 20; j++)
				{
					if (map[i * 20 + j] == 255)
						bestmap[i * 20 + j] = map[i * 20 + j];
					else
						bestmap[i * 20 + j] = 0;
				}
			}

		}
		return 1;
	}
	if (map[y * 20 + x] != 1)
	{
		return 1;
	}
	if (x < 0 || y < 0 || x > 19 || y > 19 || step == beststep)
	{
		return 1;
	}
	temp[0] = map[y * 20 + x];
	map[y * 20 + x] = 255;
	step++;
	D8(x + 1, y + 1, step, beststep, map, bestmap);
	D8(x, y + 1, step, beststep, map, bestmap);
	D8(x + 1, y, step, beststep, map, bestmap);
	D8(x - 1, y + 1, step, beststep, map, bestmap);
	D8(x - 1, y, step, beststep, map, bestmap);
	D8(x + 1, y - 1, step, beststep, map, bestmap);
	D8(x, y - 1, step, beststep, map, bestmap);
	D8(x - 1, y - 1, step, beststep, map, bestmap);
	map[y * 20 + x] = temp[0];
	step--;
}

void whichGV(unsigned char* maze, unsigned char* path, int GrayValue) {
	unsigned char* map = new unsigned char[400];
	int step = 0;
	int beststep1 = 400, beststep2 = 400, beststep3 = 400, beststep4 = 400, 
		beststep5 = 400, beststep6 = 400, beststep7 = 400, beststep8 = 400, beststep9 = 400;
	int dm = 0;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			path[i * 20 + j] = 0;
		}
	}

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			map[i * 20 + j] = 0;
		}
	}
	if (GrayValue == 1) {
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				if (maze[j + i * 20] == 80)
				{
					map[i * 20 + j] = 1;
				}
			}
		}
		D4(0, 0, step, beststep1, map, path);
		Mat pathmat = Mat(20, 20, CV_8UC1, path);
		imshow("5_D4_1", pathmat);
		waitKey();
		destroyWindow("5_D4_1");
		imwrite("5_D4_1.png", pathmat);
		D8(0, 0, step, beststep2, map, path);
		pathmat = Mat(20, 20, CV_8UC1, path);
		imshow("5_D8_1", pathmat);
		waitKey();
		destroyWindow("5_D8_1");
		imwrite("5_D8_1.png", pathmat);
		Dm(0, 0, step, beststep3, map, path);
		pathmat = Mat(20, 20, CV_8UC1, path);
		imshow("5_Dm_1", pathmat);
		waitKey();
		destroyWindow("5_Dm_1");
		imwrite("5_Dm_1.png", pathmat);
		cout << "Gray-value of Road{80} => 4-D sum = " << beststep1 << endl;
		cout << "Gray-value of Road{80, 160} => 4-D sum = " << beststep2 << endl;
		cout << "Gray-value of Road{80, 160, 255} => 4-D sum = " << beststep3 << endl;
		
	}
	else if (GrayValue == 2) {
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				if (maze[j + i * 20] == 80 || maze[j + i * 20] == 160)
				{
					map[i * 20 + j] = 1;
				}
			}
		}
		D4(0, 0, step, beststep4, map, path);
		Mat pathmat = Mat(20, 20, CV_8UC1, path);
		imshow("5_D4_2", pathmat);
		waitKey();
		destroyWindow("5_D4_2");
		imwrite("5_D4_2.png", pathmat);
		D8(0, 0, step, beststep5, map, path);
		pathmat = Mat(20, 20, CV_8UC1, path);
		imshow("5_D8_2", pathmat);
		waitKey();
		destroyWindow("5_D8_2");
		imwrite("5_D8_2.png", pathmat);
		Dm(0, 0, step, beststep6, map, path);
		pathmat = Mat(20, 20, CV_8UC1, path);
		imshow("5_Dm_2", pathmat);
		waitKey();
		destroyWindow("5_Dm_2");
		imwrite("5_Dm_2.png", pathmat);

		cout << "Gray-value of Road{80} => 8-D sum = " << beststep4 << endl;
		cout << "Gray-value of Road{80, 160} => 8-D sum = " << beststep5 << endl;
		cout << "Gray-value of Road{80, 160, 255} => 8-D sum = " << beststep6 << endl;
	}
	else if (GrayValue == 3) {
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				if (maze[j + i * 20] == 80 || maze[j + i * 20] == 160 || maze[j + i * 20] == 255)
				{
					map[i * 20 + j] = 1;
				}
			}
		}
		D4(0, 0, step, beststep7, map, path);
		Mat pathmat = Mat(20, 20, CV_8UC1, path);
		imshow("5_D4_3", pathmat);
		waitKey();
		destroyWindow("5_D4_3");
		imwrite("5_D4_3.png", pathmat);
		D8(0, 0, step, beststep8, map, path);
		pathmat = Mat(20, 20, CV_8UC1, path);
		imshow("5_D8_3", pathmat);
		waitKey();
		destroyWindow("5_D8_3");
		imwrite("5_D8_3.png", pathmat);
		Dm(0, 0, step, beststep9, map, path);
		pathmat = Mat(20, 20, CV_8UC1, path);
		imshow("5_Dm_3", pathmat);
		waitKey();
		destroyWindow("5_Dm_3");
		imwrite("5_Dm_3.png", pathmat);

		cout << "Gray-value of Road{80} => m-D sum = " << beststep7 << endl;
		cout << "Gray-value of Road{80, 160} => m-D sum = " << beststep8 << endl;
		cout << "Gray-value of Road{80, 160, 255} => m-D sum = " << beststep9 << endl;

	}

}
int main() {
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	LARGE_INTEGER nEndTime;
	int end = 0;
	int GrayValue = 0;
	int a = 0, b = 0;
	const int height = 256;
	const int width = 256;
	const int size = height * width;
	
	unsigned char* lena = new unsigned char[size];
	unsigned char* lena_blur = new unsigned char[size];
	unsigned char* zoomeyes = new unsigned char[size];
	unsigned char* zoomBig = new unsigned char[size * 6.25];//2.5
	unsigned char* zoomSmall = new unsigned char[size * 1.5625];//1.25
	unsigned char* shrink = new unsigned char[size / 4];//0.5
	unsigned char* shrink_blur = new unsigned char[size / 4];//0.5
	unsigned char* lenagray = new unsigned char[size];
	unsigned char* baboon = new unsigned char[size];
	unsigned char* baboongray = new unsigned char[size];
	unsigned char* shrinkgray = new unsigned char[size / 4];//0.5
	unsigned char* zoomgray = new unsigned char[size * 1.5625];//1.25

	unsigned char* map = new unsigned char[20 * 20];
	unsigned char* path = new unsigned char[20 * 20];


	char mapname[50] = "map.raw";
	openRaw(mapname, map, 20, 20);

	//Mat map_mat = Mat(20, 20, CV_8UC1, map);

	char filename[50] = "lena_256.raw";
	char filebaboon[50] = "baboon_256.raw";
	char blur_filename[50] = "lena_256_blur.raw";
	openRaw(filename, lena, height, width);
	openRaw(filebaboon, baboon, height, width);
	openRaw(filename, zoomeyes, height, width);
	openRaw(blur_filename, lena_blur, height, width);
	while (end == 0) {
		cout << "enter the question number : " << endl << "1.Zooming and Shrinking" << endl << "2.Gray-level resolution" << endl << "3.Isopreference test on gray-level resolution" << endl << "4.Distance and Path" << endl << "5.Exit" << endl;
		cin >> a;
		if (a == 1||a == 4) {
			cout << "enter the number" << endl << "(1)" << endl << "(2)" << endl << "(3)" << endl;
			cin >> b;
		}
		switch (a)
		{
		case 1:
			switch (b) {
			case 1://1.1
			{
				zoom1(lena, zoomeyes, height, width);
				Mat eyesMat = Mat(height, width, CV_8UC1, zoomeyes);
				imshow("1.1", eyesMat);
				waitKey();
				destroyWindow("1.1");
				imwrite("1_1.png", eyesMat);
			}
				break;
			case 2://1.2
			{
				shrink1(lena, shrink, height, width);
				Mat shrinkMat = Mat(height/2, width/2, CV_8UC1, shrink);
				imshow("1.2 original", shrinkMat);
				waitKey();
				shrink1(lena_blur, shrink_blur, height, width);
				Mat shrink_blurMat = Mat(height/2, width/2, CV_8UC1, shrink_blur);
				imshow("1.2 blur", shrink_blurMat);
				waitKey();
				destroyWindow("1.2 original");
				destroyWindow("1.2 blur");
				imwrite("1_2_ori.png", shrinkMat);
				imwrite("1_2_blur.png", shrink_blurMat);
			}
				break;
			case 3://1.3
			{
				//use "Nearest Neighbor"

				//up 2.5 down 2
				QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
				QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數
				zoom2(lena, zoomBig, height, width, 2.5);
				shrink1(zoomBig, zoomSmall, height * 2.5, width * 2.5);
				QueryPerformanceCounter(&nEndTime);
				cout << "1.3 (NN) (up 2.5 down 2) : " << (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << "  ms" << endl;
				Mat zoom_UD_NN_mat = Mat(height * 1.25, width * 1.25, CV_8UC1, zoomSmall);
				imshow("1.3 (NN) (up 2.5 down 2)", zoom_UD_NN_mat);
				waitKey();
				destroyWindow("1.3 (NN) (up 2.5 down 2)");

				imwrite("1_3_NN_1.png", zoom_UD_NN_mat);

				//down 2 up 2.5
				QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
				QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數
				shrink1(lena, shrink, height, width);
				zoom2(shrink, zoomSmall, height / 2, width / 2, 2.5);
				QueryPerformanceCounter(&nEndTime);
				cout << "1.3 (NN) (down 2 up 2.5) : " << (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << "  ms" << endl;
				Mat zoom_DU_NN_mat = Mat(height * 1.25, width * 1.25, CV_8UC1, zoomSmall);
				imshow("1.3 (NN) (down 2 up 2.5)", zoom_DU_NN_mat);
				waitKey();
				destroyWindow("1.3 (NN) (down 2 up 2.5)");

				imwrite("1_3_NN_2.png", zoom_DU_NN_mat);

				//up 1.25
				QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
				QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數
				zoom2(lena, zoomSmall, height, width, 1.25);
				QueryPerformanceCounter(&nEndTime);
				cout << "1.3 (NN) (up 1.25) : " << (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << "  ms" << endl;
				Mat zoom_U_NN_mat = Mat(height * 1.25, width * 1.25, CV_8UC1, zoomSmall);
				imshow("1.3 (NN) (up 1.25)", zoom_U_NN_mat);
				waitKey();
				destroyWindow("1.3 (NN) (up 1.25)");

				imwrite("1_3_NN_3.png", zoom_U_NN_mat);

				//use "Bilinear"

				//up 2.5 down 2
				QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
				QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數
				zoom3(lena, zoomBig, height, width, 2.5);
				shrink1(zoomBig, zoomSmall, height * 2.5, width * 2.5);
				QueryPerformanceCounter(&nEndTime);
				cout << "1.3 (Bilinear) (up 2.5 down 2) : " << (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << "  ms" << endl;
				Mat zoom_UD_Bilinear_mat = Mat(height * 1.25, width * 1.25, CV_8UC1, zoomSmall);
				imshow("1.3 (Bilinear) (up 2.5 down 2)", zoom_UD_Bilinear_mat);
				waitKey();
				destroyWindow("1.3 (Bilinear) (up 2.5 down 2)");

				imwrite("1_3_Bilinear_1.png", zoom_UD_Bilinear_mat);

				//down 2 up 2.5
				QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
				QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數
				shrink1(lena, shrink, height, width);
				zoom3(shrink, zoomSmall, height / 2, width / 2, 2.5);
				QueryPerformanceCounter(&nEndTime);
				cout << "1.3 (Bilinear) (down 2 up 2.5) : " << (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << "  ms" << endl;
				Mat zoom_DU_Bilinear_mat = Mat(height * 1.25, width * 1.25, CV_8UC1, zoomSmall);
				imshow("1.3 (Bilinear) (down 2 up 2.5)", zoom_DU_Bilinear_mat);
				waitKey();
				destroyWindow("1.3 (Bilinear) (down 2 up 2.5)");

				imwrite("1_3_Bilinear_2.png", zoom_DU_Bilinear_mat);

				//up 1.25
				QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
				QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數
				zoom3(lena, zoomSmall, height, width, 1.25);
				QueryPerformanceCounter(&nEndTime);
				cout << "1.3 (Bilinear) (up 1.25) : " << (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << "  ms" << endl;
				Mat zoom_U_Bilinear_mat = Mat(height * 1.25, width * 1.25, CV_8UC1, zoomSmall);
				imshow("1.3 (Bilinear) (up 1.25)", zoom_U_Bilinear_mat);
				waitKey();
				destroyWindow("1.3 (Bilinear) (up 1.25)");

				imwrite("1_3_Bilinear_3.png", zoom_U_Bilinear_mat);


				//use "Bicubic"

				//up 2.5 down 2
				QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
				QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數
				zoom4(lena, zoomBig, height, width, 2.5);
				shrink1(zoomBig, zoomSmall, height * 2.5, width * 2.5);
				QueryPerformanceCounter(&nEndTime);
				cout << "1.3 (Bicubic) (up 2.5 down 2) : " << (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << "  ms" << endl;
				Mat zoom_UD_Bicubic_mat = Mat(height * 1.25, width * 1.25, CV_8UC1, zoomSmall);
				imshow("1.3 (Bicubic) (up 2.5 down 2)", zoom_UD_Bicubic_mat);
				waitKey();
				destroyWindow("1.3 (Bicubic) (up 2.5 down 2)");

				imwrite("1_3_Bicubic_1.png", zoom_UD_Bicubic_mat);

				//down 2 up 2.5
				QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
				QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數
				shrink1(lena, shrink, height, width);
				zoom4(shrink, zoomSmall, height / 2, width / 2, 2.5);
				QueryPerformanceCounter(&nEndTime);
				cout << "1.3 (Bicubic) (down 2 up 2.5) : " << (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << "  ms" << endl;
				Mat zoom_DU_Bicubic_mat = Mat(height * 1.25, width * 1.25, CV_8UC1, zoomSmall);
				imshow("1.3 (Bicubic) (down 2 up 2.5)", zoom_DU_Bicubic_mat);
				waitKey();
				destroyWindow("1.3 (Bicubic) (down 2 up 2.5)");

				imwrite("1_3_Bicubic_2.png", zoom_DU_Bicubic_mat);

				//up 1.25
				QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
				QueryPerformanceCounter(&m_nBeginTime); // 獲取時鍾計數
				zoom4(lena, zoomSmall, height, width, 1.25);
				QueryPerformanceCounter(&nEndTime);
				cout << "1.3 (Bicubic) (up 1.25) : " << (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << "  ms" << endl;
				Mat zoom_U_Bicubic_mat = Mat(height * 1.25, width * 1.25, CV_8UC1, zoomSmall);
				imshow("1.3 (Bicubic) (up 1.25)", zoom_U_Bicubic_mat);
				waitKey();
				destroyWindow("1.3 (Bicubic) (up 1.25)");

				imwrite("1_3_Bicubic_3.png", zoom_U_Bicubic_mat);
			}
				break;
			default:
				break;
			}

			break;
		case 2://2
		{
			//gray level

			for (int i = 0; i < 8; i++) {
				graylevel(lena, lenagray, height, width, i);
				cout << "lena " << 8-i << "-bit with 8-bit MSE : " << MSE(lena, lenagray, height, width) << " PSNR : " << PSNR(lena, lenagray, height, width) << "  db" << endl;
				Mat graymat = Mat(height, width, CV_8UC1, lenagray);
				imshow("2_lena", graymat);
				waitKey();
				destroyWindow("2_lena");
				if (i == 0) {
					imwrite("2_8bit_lena.png", graymat);
				}
				else if (i == 1) {
					imwrite("2_7bit_lena.png", graymat);
				}
				else if (i == 2) {
					imwrite("2_6bit_lena.png", graymat);
				}
				else if (i == 3) {
					imwrite("2_5bit_lena.png", graymat);
				}
				else if (i == 4) {
					imwrite("2_4bit_lena.png", graymat);
				}
				else if (i == 5) {
					imwrite("2_3bit_lena.png", graymat);
				}
				else if (i == 6) {
					imwrite("2_2bit_lena.png", graymat);
				}
				else if (i == 7) {
					imwrite("2_1bit_lena.png", graymat);
				}
			}
			for (int i = 0; i < 8; i++) {
				graylevel(baboon, baboongray, height, width, i);
				cout << "baboon " << (8-i) << "-bit with 8-bit MSE : " << MSE(baboon, baboongray, height, width) <<" PSNR : " << PSNR(baboon, baboongray, height, width)<<"  db" << endl;
				Mat graymat = Mat(height, width, CV_8UC1, baboongray);
				imshow("2_baboon", graymat);
				waitKey();
				destroyWindow("2_baboon");
				if (i == 0) {
					imwrite("2_8bit_baboon.png", graymat);
				}
				else if (i == 1) {
					imwrite("2_7bit_baboon.png", graymat);
				}
				else if (i == 2) {
					imwrite("2_6bit_baboon.png", graymat);
				}
				else if (i == 3) {
					imwrite("2_5bit_baboon.png", graymat);
				}
				else if (i == 4) {
					imwrite("2_4bit_baboon.png", graymat);
				}
				else if (i == 5) {
					imwrite("2_3bit_baboon.png", graymat);
				}
				else if (i == 6) {
					imwrite("2_2bit_baboon.png", graymat);
				}
				else if (i == 7) {
					imwrite("2_1bit_baboon.png", graymat);
				}
			}
		}
			break;
		case 3://3
		{

			Mat lenamat = Mat(height, width, CV_8UC1, lena);
			imshow("lena", lenamat);
			waitKey();
			destroyWindow("lena");
			imwrite("3_lena.png", lenamat);
			shrink1(lena, shrink, height, width);

			for (int i = 0; i < 8; i++) {
				graylevel(shrink, shrinkgray, height / 2, width / 2, i);
				Mat graymat = Mat(height / 2, width / 2, CV_8UC1, shrinkgray);
				imshow("3_shrink", graymat);
				waitKey();
				destroyWindow("3_shrink");
				if (i == 0) {
					imwrite("3_lena_8bit_shrink.png", graymat);
				}
				else if (i == 1) {
					imwrite("3_lena_7bit_shrink.png", graymat);
				}
				else if (i == 2) {
					imwrite("3_lena_6bit_shrink.png", graymat);
				}
				else if (i == 3) {
					imwrite("3_lena_5bit_shrink.png", graymat);
				}
				else if (i == 4) {
					imwrite("3_lena_4bit_shrink.png", graymat);
				}
				else if (i == 5) {
					imwrite("3_lena_3bit_shrink.png", graymat);
				}
				else if (i == 6) {
					imwrite("3_lena_2bit_shrink.png", graymat);
				}
				else if (i == 7) {
					imwrite("3_lena_1bit_shrink.png", graymat);
				}
			}

			//use "Nearest Neighbor"
			//down 2 up 2.5
			shrink1(lena, shrink, height, width);
			zoom2(shrink, zoomSmall, height / 2, width / 2, 2.5);

			for (int i = 0; i < 8; i++) {
				graylevel(zoomSmall, zoomgray, height * 1.25, width * 1.25, i);
				Mat graymat = Mat(height * 1.25, width * 1.25, CV_8UC1, zoomgray);
				imshow("3_lena_zoom", graymat);
				waitKey();
				destroyWindow("3_lena_zoom");
				if (i == 0) {
					imwrite("3_lena_8bit_zoom.png", graymat);
				}
				else if (i == 1) {
					imwrite("3_lena_7bit_zoom.png", graymat);
				}
				else if (i == 2) {
					imwrite("3_lena_6bit_zoom.png", graymat);
				}
				else if (i == 3) {
					imwrite("3_lena_5bit_zoom.png", graymat);
				}
				else if (i == 4) {
					imwrite("3_lena_4bit_zoom.png", graymat);
				}
				else if (i == 5) {
					imwrite("3_lena_3bit_zoom.png", graymat);
				}
				else if (i == 6) {
					imwrite("3_lena_2bit_zoom.png", graymat);
				}
				else if (i == 7) {
					imwrite("3_lena_1bit_zoom.png", graymat);
				}
			}


			Mat baboonmat = Mat(height, width, CV_8UC1, baboon);
			imshow("baboon", baboonmat);
			waitKey();
			destroyWindow("baboon");
			imwrite("3_baboon.png", baboonmat);

			shrink1(baboon, shrink, height, width);
			Mat shrinkMat = Mat(height / 2, width / 2, CV_8UC1, shrink);
			imshow("3 original", shrinkMat);
			waitKey();
			destroyWindow("3 original");
			imwrite("3_baboon_ori_shr.png", shrinkMat);

			for (int i = 0; i < 8; i++) {
				graylevel(shrink, shrinkgray, height / 2, width / 2, i);
				Mat graymat = Mat(height / 2, width / 2, CV_8UC1, shrinkgray);
				imshow("3_baboon_shrink", graymat);
				waitKey();
				destroyWindow("3_baboon_shrink");
				if (i == 0) {
					imwrite("3_baboon_8bit_shrink.png", graymat);
				}
				else if (i == 1) {
					imwrite("3_baboon_7bit_shrink.png", graymat);
				}
				else if (i == 2) {
					imwrite("3_baboon_6bit_shrink.png", graymat);
				}
				else if (i == 3) {
					imwrite("3_baboon_5bit_shrink.png", graymat);
				}
				else if (i == 4) {
					imwrite("3_baboon_4bit_shrink.png", graymat);
				}
				else if (i == 5) {
					imwrite("3_baboon_3bit_shrink.png", graymat);
				}
				else if (i == 6) {
					imwrite("3_baboon_2bit_shrink.png", graymat);
				}
				else if (i == 7) {
					imwrite("3_baboon_1bit_shrink.png", graymat);
				}
			}

			//use "Nearest Neighbor"
			//down 2 up 2.5
			shrink1(baboon, shrink, height, width);
			zoom2(shrink, zoomSmall, height / 2, width / 2, 2.5);
			Mat zoom_DU_NN_mat = Mat(height * 1.25, width * 1.25, CV_8UC1, zoomSmall);
			imshow("3 (NN) (down 2 up 2.5)", zoom_DU_NN_mat);
			waitKey();
			destroyWindow("3 (NN) (down 2 up 2.5)");

			imwrite("3_baboon_NN_2.png", zoom_DU_NN_mat);

			for (int i = 0; i < 8; i++) {
				graylevel(zoomSmall, zoomgray, height * 1.25, width * 1.25, i);
				Mat graymat = Mat(height * 1.25, width * 1.25, CV_8UC1, zoomgray);
				imshow("3_baboon_zoom", graymat);
				waitKey();
				destroyWindow("3_baboon_zoom");
				if (i == 0) {
					imwrite("3_baboon_8bit_zoom.png", graymat);
				}
				else if (i == 1) {
					imwrite("3_baboon_7bit_zoom.png", graymat);
				}
				else if (i == 2) {
					imwrite("3_baboon_6bit_zoom.png", graymat);
				}
				else if (i == 3) {
					imwrite("3_baboon_5bit_zoom.png", graymat);
				}
				else if (i == 4) {
					imwrite("3_baboon_4bit_zoom.png", graymat);
				}
				else if (i == 5) {
					imwrite("3_baboon_3bit_zoom.png", graymat);
				}
				else if (i == 6) {
					imwrite("3_baboon_2bit_zoom.png", graymat);
				}
				else if (i == 7) {
					imwrite("3_baboon_1bit_zoom.png", graymat);
				}
			}


		}
			break;
		case 4: {

			switch (b) {
			case 1://5.1
			{
				GrayValue = 1;//gray value {80}
				whichGV(map, path, GrayValue);
				cout << endl;

			}
			break;
			case 2://5.2
			{
				GrayValue = 2;//gray value {80 160}
				whichGV(map, path, GrayValue);
				cout << endl;

			}
			break;
			case 3://5.3
			{
				GrayValue = 3;//gray value {80 160 255}
				whichGV(map, path, GrayValue);
				cout << endl;

			}
			break;
			default:
				break;
			}
		}

			break;
		case 5:
			end = 1;
			break;
		case 6:
			//GrayValue = 1;
			for(GrayValue=1;GrayValue<4;GrayValue++)
				whichGV(map, path, GrayValue);
			break;
		default:
			break;
		}
	}

	return 0;
}


double MSE(unsigned char* image1,unsigned char* image2,int h,int w) {
	int sum_sq = 0;
	double mse;

	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			int p1 = image1[i*w+j];
			int p2 = image2[i*w+j];
			int err = p2 - p1;
			sum_sq += (err * err);
		}
	}
	return mse = (double)sum_sq / (h * w);
}
double PSNR(unsigned char* image1,unsigned char* image2,int h,int w) {
	double mse = 0;
	mse = MSE(image1, image2, h, w);
	return (10 * log10((255 * 255) / mse));
}

void WhichD(unsigned char* maze, unsigned char* path) {
	int GrayValue = 0;
	GrayValue = 1;//gray value {80}
	FourD(maze, path, GrayValue);
	Mat pathmat = Mat(20, 20, CV_8UC1, path);
	EightD(maze, path, GrayValue);
	pathmat = Mat(20, 20, CV_8UC1, path);
	mD(maze, path, GrayValue);
	pathmat = Mat(20, 20, CV_8UC1, path);
	GrayValue = 2;//gray value {80 160}
	cout << endl;
	FourD(maze, path, GrayValue);
	pathmat = Mat(20, 20, CV_8UC1, path);
	EightD(maze, path, GrayValue);
	pathmat = Mat(20, 20, CV_8UC1, path);
	mD(maze, path, GrayValue);
	pathmat = Mat(20, 20, CV_8UC1, path);
	GrayValue = 3;//gray value {80 160 255}
	cout << endl;
	FourD(maze, path, GrayValue);
	pathmat = Mat(20, 20, CV_8UC1, path);
	EightD(maze, path, GrayValue);
	pathmat = Mat(20, 20, CV_8UC1, path);
	mD(maze, path, GrayValue);
	pathmat = Mat(20, 20, CV_8UC1, path);
}
void mD(unsigned char* maze, unsigned char* path,int GrayValue) {
	int maze_Dm[22][22];
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			path[i * 20 + j] = 0;
		}
	}

	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 22; j++)
		{
			maze_Dm[i][j] = 0;
		}
	}
	if (GrayValue == 1) {
		for (int i = 0; i < 21; i++)
		{
			for (int j = 0; j < 21; j++)
			{
				if (maze[j + i * 20] == 80)
				{
					maze_Dm[i + 1][j + 1] = 1;
				}
			}
		}
	}
	else if (GrayValue == 2) {
		for (int i = 0; i < 21; i++)
		{
			for (int j = 0; j < 21; j++)
			{
				if (maze[j + i * 20] == 80 || maze[j + i * 20] == 160)
				{
					maze_Dm[i + 1][j + 1] = 1;
				}
			}
		}
	}
	else if (GrayValue == 3) {
		for (int i = 0; i < 21; i++)
		{
			for (int j = 0; j < 21; j++)
			{
				if (maze[j + i * 20] == 80 || maze[j + i * 20] == 160 || maze[j + i * 20] == 255)
				{
					maze_Dm[i + 1][j + 1] = 1;
				}
			}
		}
	}
	int down, right, top, left;
	int top_right, top_left, down_right, down_left;
	int current_i = 1;
	int current_j = 1;
	int no_answer = 0;
	int iter = 0;
	int end = (current_i == 20) && (current_j == 20);
	int sum = 0;
	while (!end)
	{
		iter++;
		top_left = maze_Dm[current_i - 1][current_j - 1];
		top = maze_Dm[current_i - 1][current_j];
		top_right = maze_Dm[current_i - 1][current_j + 1];
		left = maze_Dm[current_i][current_j - 1];
		right = maze_Dm[current_i][current_j + 1];
		down_left = maze_Dm[current_i + 1][current_j - 1];
		down = maze_Dm[current_i + 1][current_j];
		down_right = maze_Dm[current_i + 1][current_j + 1];

		if (right == 1&&GrayValue==3)
		{
			maze_Dm[current_i][current_j] = 2;
			current_j++;
			sum++;
		}
		else if (down == 1)
		{
			maze_Dm[current_i][current_j] = 2;
			current_i++;
			sum++;
		}
		else if (right == 1)
		{
			maze_Dm[current_i][current_j] = 2;
			current_j++;
			sum++;
		}
		else if (down_right == 1 && down == 0 && right == 0)
		{
			maze_Dm[current_i][current_j] = 2;
			current_i++;
			current_j++;
			sum++;
		}
		else if (down_left == 1 && down == 0 && left == 0)
		{
			maze_Dm[current_i][current_j] = 2;
			current_i++;
			current_j--;
			sum++;
		}
		else if (top_right == 1 && top == 0 && right == 0)
		{
			maze_Dm[current_i][current_j] = 2;
			current_i--;
			current_j++;
			sum++;
		}
		else if (top == 1)
		{
			maze_Dm[current_i][current_j] = 2;
			current_i--;
			sum++;
		}
		else if (left == 1)
		{
			maze_Dm[current_i][current_j] = 2;
			current_j--;
			sum++;
		}
		else if (top_left == 1 && top == 0 && left == 0)
		{
			maze_Dm[current_i][current_j] = 2;
			current_i--;
			current_j--;
			sum++;
		}
		else if (down_right == 2 && down == 0 && right == 0)
		{
			maze_Dm[current_i][current_j] = 3;
			current_j++;
			current_i++;
			sum--;
		}
		else if (right == 2)
		{
			maze_Dm[current_i][current_j] = 3;
			current_j++;
			sum--;
		}
		else if (down == 2)
		{
			maze_Dm[current_i][current_j] = 3;
			current_i--;
			sum--;
		}
		else if (down_left == 2 && down == 0 && left == 0)
		{
			maze_Dm[current_i][current_j] = 3;
			current_i++;
			current_j--;
			sum--;
		}
		else if (top_right == 2 && top == 0 && right == 0)
		{
			maze_Dm[current_i][current_j] = 3;
			current_i--;
			current_j++;
			sum--;
		}
		else if (top == 2)
		{
			maze_Dm[current_i][current_j] = 3;
			current_i--;
			sum--;
		}
		else if (left == 2)
		{
			maze_Dm[current_i][current_j] = 3;
			current_j--;
			sum--;
		}
		else if (top_left == 2 && top == 0 && left == 0)
		{
			maze_Dm[current_i][current_j] = 3;
			current_i--;
			current_j--;
			sum--;
		}
		else
		{
			no_answer = 1;
			cout << "There is no path to End";
			break;
		}
		end = (current_i == 20) && (current_j == 20);
	}

	if (!no_answer)
	{
		maze_Dm[20][20] = 2;
		for (int i = 0; i < 22; i++)
		{
			for (int j = 0; j < 22; j++)
			{
				if (maze_Dm[i][j] == 2) {
					path[(i - 1) * 20 + (j - 1)] = 255;
				}
				//cout << maze_Dm[i][j];
			}
			//cout << endl;
		}
	}
	if (GrayValue==1)
	{
		cout <<"Gray-value of Road{80} => m-D sum = " << sum << endl;
	}
	else if (GrayValue == 2)
	{
		cout << "Gray-value of Road{80, 160} => m-D sum = " << sum << endl;
	}
	else if (GrayValue == 3)
	{
		cout << "Gray-value of Road{80, 160, 255} => m-D sum = " << sum << endl;
	}
	cout << endl;
	

}
void EightD(unsigned char* maze, unsigned char* path, int GrayValue) {
	int maze_D8[22][22];
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			path[i * 20 + j] = 0;
		}
	}

	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 22; j++)
		{
			maze_D8[i][j] = 0;
		}
	}
	if (GrayValue == 1) {
		for (int i = 0; i < 21; i++)
		{
			for (int j = 0; j < 21; j++)
			{
				if (maze[j + i * 20] == 80)
				{
					maze_D8[i + 1][j + 1] = 1;
				}
			}
		}
	}
	else if (GrayValue == 2) {
		for (int i = 0; i < 21; i++)
		{
			for (int j = 0; j < 21; j++)
			{
				if (maze[j + i * 20] == 80 || maze[j + i * 20] == 160)
				{
					maze_D8[i + 1][j + 1] = 1;
				}
			}
		}
	}
	else if (GrayValue == 3) {
		for (int i = 0; i < 21; i++)
		{
			for (int j = 0; j < 21; j++)
			{
				if (maze[j + i * 20] == 80 || maze[j + i * 20] == 160 || maze[j + i * 20] == 255)
				{
					maze_D8[i + 1][j + 1] = 1;
				}
			}
		}
	}
	int down, right, top, left;
	int top_right, top_left, down_right, down_left;
	int current_i = 1;
	int current_j = 1;
	int no_answer = 0;
	int iter = 0;
	int end = (current_i == 20) && (current_j == 20);
	int sum = 0;
	while (!end)
	{
		iter++;
		top_left = maze_D8[current_i - 1][current_j - 1];
		top = maze_D8[current_i - 1][current_j];
		top_right = maze_D8[current_i - 1][current_j + 1];
		left = maze_D8[current_i][current_j - 1];
		right = maze_D8[current_i][current_j + 1];
		down_left = maze_D8[current_i + 1][current_j - 1];
		down = maze_D8[current_i + 1][current_j];
		down_right = maze_D8[current_i + 1][current_j + 1];

		if (down_right == 1)
		{
			maze_D8[current_i][current_j] = 2;
			current_i++;
			current_j++;
			sum++;
		}
		else if (right == 1)
		{
			maze_D8[current_i][current_j] = 2;
			current_j++;
			sum++;
		}
		else if (down == 1)
		{
			maze_D8[current_i][current_j] = 2;
			current_i++;
			sum++;
		}
		else if (down_left == 1)
		{
			maze_D8[current_i][current_j] = 2;
			current_i++;
			current_j--;
			sum++;
		}
		else if (top_right == 1)
		{
			maze_D8[current_i][current_j] = 2;
			current_i--;
			current_j++;
			sum++;
		}
		else if (top == 1)
		{
			maze_D8[current_i][current_j] = 2;
			current_i--;
			sum++;
		}
		else if (left == 1)
		{
			maze_D8[current_i][current_j] = 2;
			current_j--;
			sum++;
		}
		else if (top_left == 1)
		{
			maze_D8[current_i][current_j] = 2;
			current_i--;
			current_j--;
			sum++;
		}
		else if (down_right == 2)
		{
			maze_D8[current_i][current_j] = 3;
			current_j++;
			current_i++;
			sum--;
		}
		else if (right == 2)
		{
			maze_D8[current_i][current_j] = 3;
			current_j++;
			sum--;
		}
		else if (down == 2)
		{
			maze_D8[current_i][current_j] = 3;
			current_i--;
			sum--;
		}
		else if (down_left == 2)
		{
			maze_D8[current_i][current_j] = 3;
			current_i++;
			current_j--;
			sum--;
		}
		else if (top_right == 2)
		{
			maze_D8[current_i][current_j] = 3;
			current_i--;
			current_j++;
			sum--;
		}
		else if (top == 2)
		{
			maze_D8[current_i][current_j] = 3;
			current_i--;
			sum--;
		}
		else if (left == 2)
		{
			maze_D8[current_i][current_j] = 3;
			current_j--;
			sum--;
		}
		else if (top_left == 2)
		{
			maze_D8[current_i][current_j] = 3;
			current_i--;
			current_j--;
			sum--;
		}
		else
		{
			no_answer = 1;
			cout << "There is no path to End";
			break;
		}
		end = (current_i == 20) && (current_j == 20);
	}

	if (!no_answer)
	{
		maze_D8[20][20] = 2;
		for (int i = 0; i < 22; i++)
		{
			for (int j = 0; j < 22; j++)
			{
				if (maze_D8[i][j] == 2) {
					path[(i - 1) * 20 + (j - 1)] = 255;
				}
				//cout << maze_D8[i][j];
			}
			//cout << endl;
		}
	}
	if (GrayValue == 1)
	{
		cout << "Gray-value of Road{80} => 8-D sum = " << sum << endl;
	}
	else if (GrayValue == 2)
	{
		cout << "Gray-value of Road{80, 160} => 8-D sum = " << sum << endl;
	}
	else if (GrayValue == 3)
	{
		cout << "Gray-value of Road{80, 160, 255} => 8-D sum = " << sum << endl;
	}
	cout << endl;

}
void FourD(unsigned char* maze,unsigned char* path, int GrayValue) {
	int maze_D4[22][22];
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			path[i * 20 + j] = 0;
		}
	}
	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 22; j++)
		{
			maze_D4[i][j] = 0;
		}
	}

	if (GrayValue == 1) {
		for (int i = 0; i < 21; i++)
		{
			for (int j = 0; j < 21; j++)
			{
				if (maze[j + i * 20] == 80)
				{
					maze_D4[i + 1][j + 1] = 1;
				}
			}
		}

	}
	else if (GrayValue == 2) {
		for (int i = 0; i < 21; i++)
		{
			for (int j = 0; j < 21; j++)
			{
				if (maze[j + i * 20] == 80|| maze[j + i * 20] == 160)
				{
					maze_D4[i + 1][j + 1] = 1;
				}
			}
		}

	}
	else if (GrayValue == 3) {
		for (int i = 0; i < 21; i++)
		{
			for (int j = 0; j < 21; j++)
			{
				if (maze[j + i * 20] == 80|| maze[j + i * 20] == 160|| maze[j + i * 20] == 255)
				{
					maze_D4[i + 1][j + 1] = 1;
				}
			}
		}

	}


	int current_i = 1, current_j = 1, no_answer = 0;
	int top, down, left, right, iter = 0;
	int end = (current_i == 20) && (current_j == 20);
	int sum = 0;
	while (!end)
	{
		iter++;
		top = maze_D4[current_i - 1][current_j];
		down = maze_D4[current_i + 1][current_j];
		left = maze_D4[current_i][current_j - 1];
		right = maze_D4[current_i][current_j + 1];


		if (right == 1)
		{
			maze_D4[current_i][current_j] = 2;
			current_j++;
			sum++;
		}
		else if (down == 1)
		{
			maze_D4[current_i][current_j] = 2;
			current_i++;
			sum++;
		}
		else if (left == 1)
		{
			maze_D4[current_i][current_j] = 2;
			current_j--;
			sum++;
		}
		else if (top == 1)
		{
			maze_D4[current_i][current_j] = 2;
			current_i--;
			sum++;
		}
		else if (right == 2)
		{
			maze_D4[current_i][current_j] = 3;
			current_j++;
			sum--;
		}
		else if (down == 2)
		{
			maze_D4[current_i][current_j] = 3;
			current_i++;
			sum--;
		}
		else if (left == 2)
		{
			maze_D4[current_i][current_j] = 3;
			current_j--;
			sum--;
		}
		else if (top == 2)
		{
			maze_D4[current_i][current_j] = 3;
			current_i--;
			sum--;
		}
		else
		{
			no_answer = 1;
			cout << "There is no path to End";
			break;
		}
		end = (current_i == 20) && (current_j == 20);
	}

	if (!no_answer)
	{
		maze_D4[20][20] = 2;
		for (int i = 0; i < 22; i++)
		{
			for (int j = 0; j < 22; j++)
			{
				if(maze_D4[i][j]==2)
					path[(i - 1) * 20 + (j - 1)] = 255;
				//cout << maze_D4[i][j];
			}
			//cout << endl;
		}
	}

	if (GrayValue == 1)
	{
		cout << "Gray-value of Road{80} => 4-D sum = " << sum << endl;
	}
	else if (GrayValue == 2)
	{
		cout << "Gray-value of Road{80, 160} => 4-D sum = " << sum << endl;
	}
	else if (GrayValue == 3)
	{
		cout << "Gray-value of Road{80, 160, 255} => 4-D sum = " << sum << endl;
	}
	cout << endl;

}

void graylevel(unsigned char* ori, unsigned char* gray, int h, int w,int t) {
	int tmp = 0;
	double a = 0;
	a = 255 / (pow(2, 8 - t) - 1);
	//cout << a << endl;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			tmp = ori[i * w + j];
			tmp = tmp / pow(2, t);
			tmp = tmp * a;
			gray[i * w + j] = Edge(tmp,0,255);
		}
	}
}

int Edge(int x, int min, int max)
{
	if (x < min) { x = min; }
	else if (x > max) { x = max; }
	return x;
}
float CubicHermite(float A, float B, float C, float D, float t)
{
	float a = -A / 2.0f + (3.0f * B) / 2.0f - (3.0f * C) / 2.0f + D / 2.0f;
	float b = A - (5.0f * B) / 2.0f + 2.0f * C - D / 2.0f;
	float c = -A / 2.0f + C / 2.0f;
	float d = B;

	return a * t * t * t + b * t * t + c * t + d;
}
void zoom4(unsigned char* ori, unsigned char* zoom, int h, int w, double s) {
	double x, y;
	int weight =0;
	int intX, intY, z;
	for (int i = 0; i < h * s; i++) {
		for (int j = 0; j < w * s; j++) {
			x = j / s-0.5;
			y = i / s-0.5;
			intX = j / s;
			intY = i / s;
			//CLAMP(v, min, max)  else if (v > max) { v = max; }
			if (intX < 0) { intX = 0; }
			else if (intX > w - 1) { intX = w - 1; }
			if (intY < 0) { intY = 0; }
			else if (intY > h - 1) { intY = h - 1; }
			z = i * w * s + j;
			float col0 = CubicHermite(ori[Edge(intY - 1, 0, h - 1) * w + Edge(intX - 1, 0, w - 1)], ori[Edge(intY - 1, 0, h - 1) * w + Edge(intX + 0, 0, w - 1)], ori[Edge(intY - 1, 0, h - 1) * w + Edge(intX + 1, 0, w - 1)], ori[Edge(intY - 1, 0, h - 1) * w + Edge(intX + 2, 0, w - 1)], (x - intX));
			float col1 = CubicHermite(ori[Edge(intY + 0, 0, h - 1) * w + Edge(intX - 1, 0, w - 1)], ori[Edge(intY + 0, 0, h - 1) * w + Edge(intX + 0, 0, w - 1)], ori[Edge(intY + 0, 0, h - 1) * w + Edge(intX + 1, 0, w - 1)], ori[Edge(intY + 0, 0, h - 1) * w + Edge(intX + 2, 0, w - 1)], (x - intX));
			float col2 = CubicHermite(ori[Edge(intY + 1, 0, h - 1) * w + Edge(intX - 1, 0, w - 1)], ori[Edge(intY + 1, 0, h - 1) * w + Edge(intX + 0, 0, w - 1)], ori[Edge(intY + 1, 0, h - 1) * w + Edge(intX + 1, 0, w - 1)], ori[Edge(intY + 1, 0, h - 1) * w + Edge(intX + 2, 0, w - 1)], (x - intX));
			float col3 = CubicHermite(ori[Edge(intY + 2, 0, h - 1) * w + Edge(intX - 1, 0, w - 1)], ori[Edge(intY + 2, 0, h - 1) * w + Edge(intX + 0, 0, w - 1)], ori[Edge(intY + 2, 0, h - 1) * w + Edge(intX + 1, 0, w - 1)], ori[Edge(intY + 2, 0, h - 1) * w + Edge(intX + 2, 0, w - 1)], (x - intX));
			float value = CubicHermite(col0, col1, col2, col3, (y - intY));
			//weight = ;
			zoom[z] = value;
		}
	}

}

void zoom3(unsigned char* ori, unsigned char* zoom, int h, int w, double s) {
	double x, y;
	int weight = 0;
	int intX, intY,z;
	for (int i = 0; i < h*s; i++) {
		for (int j = 0; j < w*s; j++) {
			x = j / s;
			y = i / s;
			intX = j / s;
			intY = i / s;

			if (intX < 0) { intX = 0; }
			else if (intX >= w - 1) { intX = intX - 1; }
			if (intY < 0) { intY = 0; }
			else if (intY >= w - 1) { intY = intY - 1; }
			/*if (intX + 1 >= w) {
				intX = intX - 1;
			}
			if (intY + 1 >= w) {
				intY = intY - 1;
			}*/
			z = i* w* s + j;
			weight = (1 - x + intX) * (1 - y + intY) * ori[intY * w + intX] +    //(1 - x + intX) * (1 - y + intY)
				(x - intX) * (1 - y + intY) * ori[intY * w + (intX + 1)] +         //(x - intX) * (1 - y + intY)
				(1 - x + intX) * (y - intY) * ori[(intY + 1) * w + intX] +       //(1 - x + intX) * (y - intY)
				(x - intX) * (y - intY) * ori[(intY + 1) * w + intX + 1];        //(x - intX) * (y - intY)
			zoom[z] = weight;
		}
	}

}

void zoom2(unsigned char* ori, unsigned char* zoom, int h, int w,double s) {
	int x = 0, y = 0,z =0;
	for (int i = 0; i < h*s; i++) {
		for (int j = 0; j < w*s; j++) {
			x = i / s;
			y = j / s;
			z = i * w * s + j;
			//cout << x << "  " << y << endl;
			zoom[z] = ori[x * w + y];
		}
	}
}
void shrink1(unsigned char* ori, unsigned char* shr, int h, int w) {
	for (int i = 0; i < h/2; i++) {
		for (int j = 0; j < w/2; j++) {
			shr[i * w/2 + j] = ori[2 * (i * w + j)];
		}
	}
}

void zoom1(unsigned char* ori, unsigned char* zoom, int h, int w) {
	for (int i = 0; i < 10; i++) {     //height
		for (int j = 0; j < 48; j++) { //width
			for (int x = 0; x < 2; x++) {
				for (int y = 0; y < 2; y++) {
					zoom[(125 * w + 100) + 2 * (i * w + j) + x * w + y] = ori[(130 * w + 125) + (i * w + j)];
				}
			}
		}
	}
}

void openRaw(const char filename[], unsigned char* tmp, int h, int w)
{
	FILE* fp;
	errno_t err;
	int size = h * w;
	err = fopen_s(&fp, filename, "rb");
	if (err == 0)
	{
		fread_s(tmp, size, size, 1, fp);
		fclose(fp);
	}
	else
	{
		cout << "open " << filename << " error!" << endl;
	}
}

void storeRaw(const char filename[], unsigned char* tmp, int h, int w, int size) {
	FILE* resultFile;
	errno_t err;
	err = fopen_s(&resultFile, filename, "wb");

	if (err == 0)
	{
		fwrite(tmp, 1, size, resultFile);
		fclose(resultFile);
		cout << "save " << filename << " success~" << endl;
	}
	else
	{
		cout << "store " << filename << " error!" << endl;
	}
}
