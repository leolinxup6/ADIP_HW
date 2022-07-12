#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

void openRaw(const char filename[], unsigned char* tmp, int h, int w);
void storeRaw(const char filename[], unsigned char* tmp, int h, int w, int size);
int main() {
	int const width = 256;
	int const height = 256;
	int x = 0,y = 0;
	int size = width * height;
	unsigned char ans[width*height];
	FILE* fp;
	
	unsigned char* lena = new unsigned char[size];
	openRaw("lena_256.raw", lena, height, width);
	//1.2 b (1)
	cout << "1.2 b (1)" << endl;
	//cout << "0th row & 0th column pixel : " << (int)lena[0] << endl;
	cout << "123rd row & 234th column pixel : " << (int)lena[234 + 123 * 256] << endl;
	//1.2 b (2)
	cout << "1.2 b (2)" << endl;
	cout << "5487th pixel : " << (int)lena[5486] << endl;
	x = 5486 / height;
	y = 5486 % width;
	cout << "5487th pixel : coordinate " << x << " row & " << y << " column pixel " << endl;
	//1.2 c
  	Mat lenamat = Mat(height, width, CV_8UC1, lena);
  	imshow("1.2 c", lenamat);
  	waitKey();
  	destroyWindow("1.2 c");
	imwrite("1_2_c.png", lenamat);
	//1.2 d
	unsigned char rotate[width*height];
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) 
			rotate[j * width + i] = lena[i * width + j];
	Mat rotatemat = Mat(height, width, CV_8UC1, rotate);
	imshow("1.2 d", rotatemat);
	waitKey();
	destroyWindow("1.2 d");
	//imwrite("1_2_d.png", rotatemat);
	storeRaw("1_2_d.raw", rotate, height, width, size);

	//for (int i = 0; i < width; i++)
	//	for (int j = 0; j < height; j++)
	//		ans[i * width + j] = rotate[i][j];
	//1.2 e
	unsigned char spilt[width*height];
	int block1[4] = { 0, 1, 2, 3 };
	int block2[4] = { 0, 1, 2, 3 };
	srand(time(NULL));
	int transfer, a, b;
	for (int i = 0; i < 5; i++)
	{
		a = rand() % 4;
		b = rand() % 4;
		transfer = block1[a];
		block1[a] = block1[b];
		block1[b] = transfer;
	}
	for (int i = 0; i < 5; i++)
	{
		a = rand() % 4;
		b = rand() % 4;
		transfer = block2[a];
		block2[a] = block2[b];
		block2[b] = transfer;
	}
//	for (transfer = 0; transfer < 4; transfer++)
//		cout << block[transfer] << endl;
	for (int a = 0; a < 4; a++)
		for (int b = 0; b < 4; b++)
			for (int i = 0; i < width / 4; i++)
				for (int j = 0; j < height / 4; j++)
					spilt[(i + block1[a] * width / 4)*width+j + block2[b] * height / 4] = lena[height * (i + a * width / 4) + b * height / 4 + j];

//	for (int i = 0; i < 4; i++)
//		for (int j = 0; j < size / 4; j++)
//			//spilt[block[i] * width / 4 + j / height][(j + block[i] * height / 4) % height] = lena[i * size / 4 + j];
//			rotate[block[i] * width / 4 + j / height][j % height] = lena[i * size / 4 + j];
//	for (int i = 0; i < 4; i++)
//		for (int j = 0; j < size / 4; j++)
//			//spilt[block[i] * width / 4 + j / height][(j + block[i] * height / 4) % height] = lena[i * size / 4 + j];
//			spilt[j % width][block[i] * height / 4 + j / height] = rotate[j % width][i * height / 4 + j / height];
//			//spilt[block[i] * width / 4 + j / height][j % height] = rotate[j % height][i * width / 4 + j % (width / 4)];


	Mat blockmat = Mat(height, width, CV_8UC1, spilt);
	imshow("1.2 e", blockmat);
	waitKey();
	destroyWindow("1.2 e");
	//imwrite("1_2_e.png", blockmat);
	storeRaw("1_2_e.raw", spilt, height, width, size);
	//1.2 f
	int p = 256;
	int const bound = 512;
	int const bigsize = bound * bound;
	unsigned char moon[bigsize];
	/*
	fopen_s(&fp, "lena_512.raw", "rb");
	unsigned char* lena_512 = new unsigned char[bigsize];
	fread(lena_512, sizeof(unsigned char), bigsize, fp);
	*/
	fopen_s(&fp, "p01.raw", "rb");
	unsigned char* moon1 = new unsigned char[size];
	fread(moon1, sizeof(unsigned char), size, fp);
	fopen_s(&fp, "p02.raw", "rb");
	unsigned char* moon2 = new unsigned char[size];
	fread(moon2, sizeof(unsigned char), size, fp);
	fopen_s(&fp, "p03.raw", "rb");
	unsigned char* moon3 = new unsigned char[size];
	fread(moon3, sizeof(unsigned char), size, fp);
	fopen_s(&fp, "p04.raw", "rb");
	unsigned char* moon4 = new unsigned char[size];
	fread(moon4, sizeof(unsigned char), size, fp);


	for (int i = 0; i < p; i++)
		for (int j = 0; j < p; j++)
			moon[(i + p) * bound + j + p] = moon1[i * p + j];
	for (int i = 0; i < p; i++)
		for (int j = 0; j < p; j++)
			moon[(i + p) * bound + j] = moon2[i * p + j];
	for (int i = 0; i < p; i++)
		for (int j = 0; j < p; j++)
			moon[i * bound + j] = moon3[i * p + j];
	for (int i = 0; i < p; i++)
		for (int j = 0; j < p; j++)
			moon[i * bound + j + p] = moon4[i * p + j];

	Mat moonmat = Mat(512, 512, CV_8UC1, moon);
	imshow("moon", moonmat);
	waitKey();
	destroyWindow("moon");
	//imwrite("moon.png", moonmat);
	storeRaw("moon.raw", moon, bound, bound, bigsize);

	//1.3 a
	unsigned char bright[width*height];
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (lena[i * width + j] + 50 > 255) {
				bright[i * width + j] = 255;
			}
			else {
				bright[i * width + j] = lena[i * width + j];
			}
		}
	}

	Mat brightmat = Mat(height, width, CV_8UC1, bright);
	imshow("1.3 a", brightmat);
	waitKey();
	destroyWindow("1.3 a");
	//imwrite("1_3_a.png", brightmat);
	storeRaw("1_3_a.raw", bright, height, width, size);
	//1.3 b
	int ran = 0;
	srand(time(NULL));
	unsigned char ranbri[width*height];
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			ran = rand() % 101 - 50;
			/*if (ran < -50 || ran>50) {
				cout << ran << endl;
			}*/
			if (lena[i * width + j] + ran > 255) {
				ranbri[i * width + j] = 255;
			}
			else if (lena[i * width + j] + ran < 0) {
				ranbri[i * width + j] = 0;
			}
			else {
				ranbri[i * width + j] = lena[i * width + j];
			}
		}
	}

	Mat ranbrimat = Mat(height, width, CV_8UC1, ranbri);
	imshow("1.3 b", ranbrimat);
	waitKey();
	destroyWindow("1.3 b");
	//imwrite("1_3_b.png", ranbrimat);
	storeRaw("1_3_b.raw", ranbri, height, width, size);
	//2
	unsigned char* name = new unsigned char[bigsize];
	openRaw("haha.raw", name, 512, 512);
	//fopen_s(&fp, "haha.raw", "rb");
	//unsigned char* name = new unsigned char[bigsize];
	//fread(name, sizeof(unsigned char), bigsize, fp);
	Mat hahamat = Mat(512, 512, CV_8UC1, name);
	//ellipse(hahamat, Point(250,512), Size(200, 140), 0, 180, 360, Scalar(0, 0, 0), 2);
	putText(hahamat, string("109318144"), Point(50, 50), FONT_HERSHEY_SCRIPT_COMPLEX, 2, Scalar(0, 0, 0), 1, LINE_8, false);
	putText(hahamat, string("HW finish~"), Point(30, 480), FONT_HERSHEY_SCRIPT_COMPLEX, 3, Scalar(0, 0, 0), 1, LINE_8, false);

	imshow("2", hahamat);
	waitKey();
	destroyWindow("2");
	imwrite("2.png", hahamat);

	//close file
	fclose(fp);
	system("PAUSE");
	return 0;
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
		cout << "open " << filename <<" error!" << endl;
	}
}

void storeRaw(const char filename[], unsigned char* tmp, int h, int w,int size) {
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
