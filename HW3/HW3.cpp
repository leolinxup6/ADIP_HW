#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <ctime>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;

void openRaw(const char filename[], unsigned char* tmp, int h, int w);
void storeRaw(const char filename[], unsigned char* tmp, int h, int w, int size);
double MSE(unsigned char* image1, unsigned char* image2, int h, int w);
double PSNR(unsigned char* image1, unsigned char* image2, int h, int w);
void DecToBi(unsigned char* image,unsigned char** Bi,int h,int w);
void hidden(unsigned char** image1, unsigned char** image2, unsigned char* hide,int h,int w);
void separation(unsigned char* hide,unsigned char* sepa, int h, int w);
void BiToDec(unsigned char** Bi, unsigned char* image, int h, int w);
void negative(unsigned char* merge_bi,int h,int w);
void LogTrans(unsigned char* process, unsigned char* image, int c, int h, int w);
void PowerLaw(unsigned char* process, unsigned char* image, double c, double r, int h, int w);
void Histogram(unsigned char* image, int* hist, int h, int w);
void histDisplay(int histogram[], const char* name);
void hist_EQ(unsigned char* image, int* hist, int* hist_eq, unsigned char* eq, int h, int w);
int main() {
	const int height = 512, width = 512;

	char lenafilename[50]="lena_256.raw";
	char baboonfilename[50] = "baboon_256.raw";
	unsigned char* test = new unsigned char[256 * 256];
	unsigned char* lena = new unsigned char[256 * 256];
	unsigned char* baboon = new unsigned char [256 * 256];
	unsigned char* hide = new unsigned char[256 * 256];
	unsigned char* sepa = new unsigned char[256 * 256];
	unsigned char** lena_bi = new unsigned char*[8];
	unsigned char** baboon_bi = new unsigned char*[8];
	for (int i = 0; i < 8; i++) {
		lena_bi[i] = new unsigned char[256 * 256];
		baboon_bi[i] = new unsigned char[256 * 256];
	}
	openRaw(lenafilename, lena, 256, 256);
	openRaw(baboonfilename, baboon, 256, 256);

	char filename[8][50] = { "a512x512.raw","b512x512.raw","c512x512.raw","d512x512.raw","e512x512.raw","f512x512.raw","g512x512.raw","h512x512.raw" };
	unsigned char* merge = new unsigned char[height * width];
	unsigned char** merge_bi = new unsigned char* [8];
	for (int i = 0; i < 8; i++) {
		merge_bi[i] = new unsigned char[height * width];
	}
	openRaw(filename[5], merge_bi[0], height, width);
	openRaw(filename[3], merge_bi[1], height, width);
	openRaw(filename[4], merge_bi[2], height, width);
	openRaw(filename[0], merge_bi[3], height, width);
	openRaw(filename[2], merge_bi[4], height, width);
	openRaw(filename[7], merge_bi[5], height, width);
	openRaw(filename[6], merge_bi[6], height, width);
	openRaw(filename[1], merge_bi[7], height, width);

	char manfilename[50] = "cameraman_b512.raw";
	char roomfilename[50] = "livingroom_d512.raw";
	unsigned char* camera_b = new unsigned char[height * width];
	unsigned char* room_d = new unsigned char[height * width];
	unsigned char* process = new unsigned char[height * width];
	openRaw(manfilename, camera_b, height, width);
	openRaw(roomfilename, room_d, height, width);

	Mat cameramat = Mat(height, width, CV_8UC1, camera_b);
	imwrite("man.png", cameramat);
	Mat roommat = Mat(height, width, CV_8UC1, room_d);
	imwrite("room.png", roommat);
	
	int* hist = new int[256];
	int* hist_eq = new int[256];
	unsigned char* eq = new unsigned char[height*width];

	int end = 0, a = 0, b = 0,c=0;
	while (end == 0) {
		cout << "Enter the question number : " << endl << "1. Bit plane" << endl << "2. Bit-plane and negative" << endl << "3. Gray level transformation" << endl << "4.Histogram equalization" << endl << "5. End" << endl << "==> ";
		cin >> a;
		if (a == 1 || a == 3 || a == 4) {
			cout << "Enter the question number : " << endl << "(1)" << endl << "(2)" << endl << "==> ";
			cin >> b;
		}
		switch (a)
		{
		case 1:
			switch (b) {
			case 1: {
				DecToBi(lena, lena_bi, 256, 256);
				DecToBi(baboon, baboon_bi, 256, 256);
				hidden(lena_bi, baboon_bi, hide, 256, 256);
				cout << "MSE of Lina with baboon with respect to Lena : " << MSE(lena, hide, 256, 256) << endl << "PSNR of Lina with baboon with respect to Lena : " << PSNR(lena, hide, 256, 256) << endl;
				//Mat lenamat = Mat(256, 256, CV_8UC1, lena);
				Mat hidemat = Mat(256, 256, CV_8UC1, hide);
				imshow("Lina_with_baboon", hidemat);
				waitKey();
				destroyWindow("Lina_with_baboon");
				imwrite("lina_with_baboon.png", hidemat);
			}
				break;
			case 2: {
				separation(hide, sepa, 256, 256);
				cout << "MSE of extracted baboon with respect to baboon : " << MSE(baboon, sepa, 256, 256) << endl << "PSNR of extracted baboon with respwct to baboon : " << PSNR(baboon, sepa, 256, 256) << endl;
				Mat sepamat = Mat(256, 256, CV_8UC1, sepa);
				imshow("extracted_baboon", sepamat);
				waitKey();
				destroyWindow("extracted_baboon");
				imwrite("extracted_baboon.png", sepamat);
			}
				break;
			default :
				break;
			}
			break;
		case 2:{
			//23467 negative
			//015 positive
			if (c == 0) {
				//negative(merge_bi[0], height, width);
				//negative(merge_bi[1], height, width);	
				//negative(merge_bi[2], height, width);
				negative(merge_bi[3], height, width);
				negative(merge_bi[4], height, width);
				//negative(merge_bi[5], height, width);
				negative(merge_bi[6], height, width);
				negative(merge_bi[7], height, width);
				c = 1;
			}
			BiToDec(merge_bi, merge, height, width);
			Mat mergemat = Mat(height, width, CV_8UC1, merge); 
			imshow("merge", mergemat);
			waitKey();
			destroyWindow("merge");
			imwrite("merge.png", mergemat);
		}
			break;
		case 3:
			switch (b) {
			case 1: {
				LogTrans(process, camera_b, 100, height, width);
				Mat promat = Mat(height, width, CV_8UC1, process);
				imshow("log_man_c100", promat);
				waitKey();
				destroyWindow("log_man_c100");
				imwrite("log_man_c100.png", promat);
				LogTrans(process, camera_b, 20, height, width);
				promat = Mat(height, width, CV_8UC1, process);
				imshow("log_man_c20", promat);
				waitKey();
				destroyWindow("log_man_c20");
				imwrite("log_man_c20.png", promat);
				LogTrans(process, room_d, 100, height, width);
				promat = Mat(height, width, CV_8UC1, process);
				imshow("log_room_c100", promat);
				waitKey();
				destroyWindow("log_room_c100");
				imwrite("log_room_c100.png", promat);
				LogTrans(process, room_d, 20, height, width);
				promat = Mat(height, width, CV_8UC1, process);
				imshow("log_room_c20", promat);
				waitKey();
				destroyWindow("log_room_c20");
				imwrite("log_room_c20.png", promat);
			}
				break;
			case 2: {
				//c1 500 衣服 1000 背景 c4 救不回來
				double c1 = 1000, c2 = 0.0000000000000000000002, c3 = 800, c4 = 0.0000025;
				PowerLaw(process, camera_b, c1, 0.2, height, width);
				Mat promat = Mat(height, width, CV_8UC1, process);
				imshow("powerlaw_camera_r2", promat);
				waitKey();
				destroyWindow("powerlaw_camera_r2");
				imwrite("powerlaw_camera_r2.png", promat);
				PowerLaw(process, camera_b, c2, 10, height, width);
				promat = Mat(height, width, CV_8UC1, process);
				imshow("powerlaw_camera_r10", promat);
				waitKey();
				destroyWindow("powerlaw_camera_r10");
				imwrite("powerlaw_camera_r10.png", promat);
				PowerLaw(process, room_d, c3, 0.2, height, width);
				promat = Mat(height, width, CV_8UC1, process);
				imshow("powerlaw_room_r2", promat);
				waitKey();
				destroyWindow("powerlaw_room_r2");
				imwrite("powerlaw_room_r2.png", promat);
				PowerLaw(process, room_d, c4, 10, height, width);
				promat = Mat(height, width, CV_8UC1, process);
				imshow("powerlaw_room_r10", promat);
				waitKey();
				destroyWindow("powerlaw_room_r10");
				imwrite("powerlaw_room_r10.png", promat);
			}
				break;
			default:
				break;
			}
			break;
		case 4:
			switch (b) {
			case 1: {
				Histogram(camera_b, hist, height, width);
				histDisplay(hist, "hist_ori_camera.png");
				Mat cameramat = Mat(height, width, CV_8UC1, camera_b);
				imshow("camera_b", cameramat);
				waitKey();
				destroyWindow("camera_b");
				imwrite("camera_b.png", cameramat);
				Histogram(room_d, hist, height, width);
				histDisplay(hist, "hist_ori_room.png");
				Mat roommat = Mat(height, width, CV_8UC1, room_d);
				imshow("room_d", roommat);
				waitKey();
				destroyWindow("room_d");
				imwrite("room_d.png", roommat);
			}
				break;
			case 2: {
				hist_EQ(camera_b, hist, hist_eq, eq, height, width);
				histDisplay(hist_eq, "hist_eq_camera.png");
				Mat camera_eqmat = Mat(height, width, CV_8UC1, eq);
				imshow("camera_b", camera_eqmat);
				waitKey();
				destroyWindow("camera_b");
				imwrite("eq_camera_b.png", camera_eqmat);
				hist_EQ(room_d, hist, hist_eq, eq, height, width);
				histDisplay(hist_eq, "hist_eq_room.png");
				Mat room_eqmat = Mat(height, width, CV_8UC1, eq);
				imshow("room_d", room_eqmat);
				waitKey();
				destroyWindow("room_d");
				imwrite("eq_room_d.png", room_eqmat);
			}
				break;
			default:
				break;
			}
			break;
		case 5:
			end = 1;
			break;
		default:
			break;
		}
	}
	return 0;
}
void histDisplay(int histogram[], const char* name)
{
	int hist[256];
	for (int i = 0; i < 256; i++)
	{
		hist[i] = histogram[i];
	}
	// draw the histograms
	int hist_w = 512, hist_h = 400;
	int bin_w = cvRound((double)hist_w / 256);
	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));

	// find the maximum intensity element from histogram
	int max = hist[0];
	for (int i = 1; i < 256; i++) {
		if (max < hist[i]) {
			max = hist[i];
		}
	}

	// normalize the histogram between 0 and histImage.rows
	for (int i = 0; i < 256; i++)
	{
		hist[i] = ((double)hist[i] / max) * histImage.rows;
	}


	// draw the intensity line for histogram
	for (int i = 0; i < 256; i++)
	{
		line(histImage, Point(bin_w * (i), hist_h), Point(bin_w * (i), hist_h - hist[i]), Scalar(0, 0, 0), 1, 8, 0);
	}

	// display histogram
	imshow(name, histImage);
	waitKey();
	destroyWindow(name);
	imwrite(name, histImage);
}

//()
void hist_EQ(unsigned char* image, int* hist, int* hist_eq, unsigned char* eq, int h, int w) {

	Histogram(image, hist, h, w);
	double* cdf = new double[256];
	for (int i = 0; i < 256; i++) {
		cdf[i] = (double)hist[i] / (h * w);
		if (i != 0)
			cdf[i] = cdf[i - 1] + cdf[i];
		hist_eq[i] = cdf[i] * 255;
	}
	for(int i = 0; i < h* w; i++) {
		int a = 0;
		a = image[i];
		eq[i] = hist_eq[a];
	}
	for (int i = 0; i < 256; i++) {
		hist_eq[i] = 0;
	}
	for (int i = 0; i < h * w; i++) {
		for (int k = 0; k < 256; k++) {
			if (eq[i] == k)
				hist_eq[k]++;
		}
	}
	/*
	for (int i = 255; i > 0; i--) {
		hist_eq[i] = hist_eq[i] - hist_eq[i - 1];
	}*/
}
void Histogram(unsigned char* image, int* hist, int h, int w) {
	for (int i = 0; i < 256; i++) {
		hist[i] = 0;
	}
	for (int i = 0; i < h * w; i++) {
		for (int k = 0; k < 256; k++) {
			if (image[i] == k)
				hist[k]++;
		}
	}
}

void LogTrans(unsigned char* process, unsigned char* image, int c, int h, int w) {
	for (int i = 0; i < h * w; i++) {
		process[i] = c * log10(image[i] + 1);
	}
}
void PowerLaw(unsigned char* process, unsigned char* image, double c, double r, int h, int w) {
	int a = 0;
	for (int i = 0; i < h * w; i++) {
		a = image[i];
		process[i] = c * pow(a,r);
		if (process[i] > 255)
			process[i] = 255;
		else if (a < 0)
			process[i] = 0;
	}
}

void negative(unsigned char* merge,int h,int w) {
	unsigned char* positive = new unsigned char[h * w];
	for (int i = 0; i < h * w; i++) {
		merge[i] = 255 - merge[i];
	}
}
void separation(unsigned char* hide, unsigned char* sepa, int h, int w) {
	int bi = 0;
	for (int i = 0; i < h * w; i++) {
		sepa[i] = 0;
		bi = 0;
		for (int k = 0; k < 4; k++) {
			bi = hide[i] / pow(2, k);
			if (bi % 2 == 1) {
				sepa[i] = sepa[i] + pow(2, k + 4);
			}
		}
	}

}
//lena baboon
void hidden(unsigned char** image1, unsigned char** image2, unsigned char* hide, int h, int w) {
	int a = 0;
	for (int i = 0; i < h * w; i++) {
		a = 0;
		for (int j = 4; j < 8; j++) {
			a = a + image1[j][i] * pow(2, j);
		}
		for (int k = 0; k < 4; k++) {
			a = a + image2[k + 4][i] * pow(2, k);
		}
		hide[i] = a;
	}


}
void BiToDec(unsigned char** Bi, unsigned char* image, int h, int w){
	for (int i = 0; i < h * w; i++) {
		image[i] = 0;
		for (int k = 0; k < 8; k++) {
			image[i] = image[i] + (Bi[k][i] * pow(2, k));
		}
	}
}
void DecToBi(unsigned char* image, unsigned char** Bi, int h, int w) {
	int bi = 0;
	for (int k = 0; k < 8; k++) {
		for (int i = 0; i < h * w; i++) {
			bi = image[i] / pow(2, k);
			Bi[k][i] = 0;
			if (bi % 2 == 1) {
				Bi[k][i] = 1;
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

double MSE(unsigned char* image1, unsigned char* image2, int h, int w) {
	int sum_sq = 0;
	double mse;

	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			int p1 = image1[i * w + j];
			int p2 = image2[i * w + j];
			int err = p2 - p1;
			sum_sq += (err * err);
		}
	}
	return mse = (double)sum_sq / (h * w);
}
double PSNR(unsigned char* image1, unsigned char* image2, int h, int w) {
	double mse = 0;
	mse = MSE(image1, image2, h, w);
	return (10 * log10((255 * 255) / mse));
}