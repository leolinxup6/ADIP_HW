#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

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
int average10(unsigned char* aver10);
int average200(unsigned char* aver200);
int averageALL( unsigned char* averAll);
int high_boost_4(unsigned char* walkbridge, unsigned char* out, int h, int w, int a, bool zero);
int high_boost_8(unsigned char* walkbridge, unsigned char* out, int h, int w, int a, bool zero);
int guassian(unsigned char* src, unsigned char* out, int h, int w, double sigma);
int DoG(unsigned char* src, unsigned char* out, int h, int w, double sigma1, double sigma2);
void Histogram(unsigned char* image, int* hist, int h, int w);
void histDisplay(unsigned char* src, int h, int w, const char* filename);
int hist_matching(unsigned char* src1, unsigned char* src2, unsigned char* dst, int h, int w);
int local_hist_EQ(unsigned char* src, unsigned char* dst, int h, int w, int local);
void hist_EQ(unsigned char* image, unsigned char* eq, int h, int w);
int main() {
	int height = 512, width = 512;
	unsigned char* aver10 = new unsigned char[960 * 540];
	unsigned char* aver200 = new unsigned char[960 * 540];
	unsigned char* averAll = new unsigned char[960 * 540];
	unsigned char* house = new unsigned char[height * width];
	unsigned char* lena = new unsigned char[height * width];
	unsigned char* walkbridge = new unsigned char[height * width];
	unsigned char* walkbridgeout4 = new unsigned char[height * width];
	unsigned char* walkbridgeout8 = new unsigned char[height * width];
	unsigned char* turtle = new unsigned char[height * width];
	unsigned char* turtleout1 = new unsigned char[height * width];
	unsigned char* turtleout2 = new unsigned char[height * width];
	unsigned char* turtleout3 = new unsigned char[height * width];
	openRaw("house512.raw", house, height, width);
	openRaw("lena512.raw", lena, height, width);
	openRaw("turtle512.raw", turtle, height, width);
	openRaw("walkbridge.raw", walkbridge, height, width);
	int end = 0, a = 0,b = 0 ;
	while (end == 0) {
		cout << "Enter the question number : " << endl << "1. Histogram processing" << endl << "2. Image averaging" << endl << "3. Edge detection" << endl << "4. Guassian filter" << endl << "5. Exit" << endl;
		cin >> a;
		switch (a)
		{
		case 1: {
			//1.

			cout << "Enter the question number : " << endl << "1. Histogram matching" << endl << "2. Local histogram equalization" << endl;
			cin >> b;
			switch (b)
			{
			case 1: {
				unsigned char* out1 = new unsigned char[height * width];
				histDisplay(lena, height, width, "lena_hist.png");
				histDisplay(house, height, width, "house_hist.png");
				hist_matching(house, lena, out1, height, width);
				histDisplay(out1, height, width,"hist_match_hist.png");
				Mat housemat = Mat(height, width, CV_8UC1, house);
				imshow("house_original", housemat);
				waitKey();
				destroyWindow("house_original");
				imwrite("house_original.png", housemat);
				Mat out1mat = Mat(height, width, CV_8UC1, out1);
				imshow("house_histogram_matching", out1mat);
				waitKey();
				destroyWindow("house_histogram_matching");
				imwrite("house_histogram_matching.png", out1mat);
			}
					break;
			case 2: {
				unsigned char* out2 = new unsigned char[height * width];
				memset(out2, 255, sizeof(uchar) * height * width);
				local_hist_EQ(house, out2, height, width, 2);
				histDisplay(out2, height, width, "out2_hist.png");
				Mat out2mat = Mat(height, width, CV_8UC1, out2);
				imshow("house_local_histogram_EQ_2", out2mat);
				waitKey();
				destroyWindow("house_local_histogram_EQ_2");
				imwrite("house_local_histogram_EQ_2.png", out2mat);

				local_hist_EQ(house, out2, height, width, 8);
				histDisplay(out2, height, width, "out8_hist.png");
				Mat out8mat = Mat(height, width, CV_8UC1, out2);
				imshow("house_local_histogram_EQ_8", out8mat);
				waitKey();
				destroyWindow("house_local_histogram_EQ_8");
				imwrite("house_local_histogram_EQ_8.png", out8mat);

				local_hist_EQ(house, out2, height, width, 256);
				histDisplay(out2, height, width, "out256_hist.png");
				Mat out256mat = Mat(height, width, CV_8UC1, out2);
				imshow("house_local_histogram_EQ_256", out256mat);
				waitKey();
				destroyWindow("house_local_histogram_EQ_256");
				imwrite("house_local_histogram_EQ_256.png", out256mat);
			}
					break;
			default:
				cout << "Not this question !!" << endl;
				break;
			}
		}
			  break;
		case 2: {
			//2.
			average10(aver10);
			average200(aver200);
			averageALL(averAll);
			Mat aver10mat = Mat(540, 960, CV_8UC1, aver10);
			Mat aver200mat = Mat(540, 960, CV_8UC1, aver200);
			Mat averALLmat = Mat(540, 960, CV_8UC1, averAll);
			imshow("average10", aver10mat);
			waitKey();
			imshow("average100", aver200mat);
			waitKey();
			imshow("averageAll", averALLmat);
			waitKey();
			destroyAllWindows();
			imwrite("aver10.png", aver10mat);
			imwrite("aver200.png", aver200mat);
			imwrite("averALL.png", averALLmat);
		}
			  break;
		case 3: {
			//3.

			cout << "Enter the question number : " << endl << "1. Laplacian filter" << endl << "2. High-boost filter" << endl;
			cin >> b;
			switch (b)
			{
			case 1: {
				int a;
				bool zero = true;
				a = 0;//edge detect
				high_boost_4(walkbridge, walkbridgeout4, height, width, a, zero);
				high_boost_8(walkbridge, walkbridgeout8, height, width, a, zero);
				Mat out4_zeromat = Mat(height, width, CV_8UC1, walkbridgeout4);
				imshow("laplacian4_zero", out4_zeromat);
				waitKey();
				destroyWindow("laplacian4_zero");
				imwrite("laplacian4_zero.png", out4_zeromat);
				Mat out8_zeromat = Mat(height, width, CV_8UC1, walkbridgeout8);
				imshow("laplacian8_zero", out8_zeromat);
				waitKey();
				destroyWindow("laplacian8_zero");
				imwrite("laplacian8_zero.png", out8_zeromat);

				zero = false;
				high_boost_4(walkbridge, walkbridgeout4, height, width, a, zero);
				high_boost_8(walkbridge, walkbridgeout8, height, width, a, zero);
				Mat out4_replimat = Mat(height, width, CV_8UC1, walkbridgeout4);
				imshow("laplacian4_repli", out4_replimat);
				waitKey();
				destroyWindow("laplacian4_repli");
				imwrite("laplacian4_repli.png", out4_replimat);
				Mat out8_replimat = Mat(height, width, CV_8UC1, walkbridgeout8);
				imshow("laplacian8_repli", out8_replimat);
				waitKey();
				destroyWindow("laplacian8_repli");
				imwrite("laplacian8_repli.png", out8_replimat);
			}
					break;
			case 2: {
				int a;
				bool zero = true;
				a = 1;//high boost
				high_boost_4(walkbridge, walkbridgeout4, height, width, a, zero);
				Mat high_boost4_zero_A1mat = Mat(height, width, CV_8UC1, walkbridgeout4);
				imshow("high_boost4_zero_A1", high_boost4_zero_A1mat);
				waitKey();
				destroyWindow("high_boost4_zero_A1");
				imwrite("high_boost4_zero_A1.png", high_boost4_zero_A1mat);
				high_boost_8(walkbridge, walkbridgeout8, height, width, a, zero);
				Mat high_boost8_zero_A1mat = Mat(height, width, CV_8UC1, walkbridgeout4);
				imshow("high_boost8_zero_A1", high_boost8_zero_A1mat);
				waitKey();
				destroyWindow("high_boost8_zero_A1");
				imwrite("high_boost8_zero_A1.png", high_boost8_zero_A1mat);
				a = 2;//high boost
				high_boost_4(walkbridge, walkbridgeout4, height, width, a, zero);
				Mat high_boost4_zero_A2mat = Mat(height, width, CV_8UC1, walkbridgeout4);
				imshow("high_boost4_zero_A2", high_boost4_zero_A2mat);
				waitKey();
				destroyWindow("high_boost4_zero_A2");
				imwrite("high_boost4_zero_A2.png", high_boost4_zero_A2mat);
				high_boost_8(walkbridge, walkbridgeout8, height, width, a, zero);
				Mat high_boost8_zero_A2mat = Mat(height, width, CV_8UC1, walkbridgeout4);
				imshow("high_boost8_zero_A2", high_boost8_zero_A2mat);
				waitKey();
				destroyWindow("high_boost8_zero_A2");
				imwrite("high_boost8_zero_A2.png", high_boost8_zero_A2mat);
				a = 4;//high boost
				high_boost_4(walkbridge, walkbridgeout4, height, width, a, zero);
				Mat high_boost4_zero_A4mat = Mat(height, width, CV_8UC1, walkbridgeout4);
				imshow("high_boost4_zero_A4", high_boost4_zero_A4mat);
				waitKey();
				destroyWindow("high_boost4_zero_A4");
				imwrite("high_boost4_zero_A4.png", high_boost4_zero_A4mat);
				high_boost_8(walkbridge, walkbridgeout8, height, width, a, zero);
				Mat high_boost8_zero_A4mat = Mat(height, width, CV_8UC1, walkbridgeout4);
				imshow("high_boost8_zero_A4", high_boost8_zero_A4mat);
				waitKey();
				destroyWindow("high_boost8_zero_A4");
				imwrite("high_boost8_zero_A4.png", high_boost8_zero_A4mat);

				zero = false;//repli
				a = 1;
				high_boost_4(walkbridge, walkbridgeout4, height, width, a, zero);
				Mat high_boost4_repli_A1mat = Mat(height, width, CV_8UC1, walkbridgeout4);
				imshow("high_boost4_repli_A1", high_boost4_repli_A1mat);
				waitKey();
				destroyWindow("high_boost4_repli_A1");
				imwrite("high_boost4_repli_A1.png", high_boost4_repli_A1mat);
				high_boost_8(walkbridge, walkbridgeout8, height, width, a, zero);
				Mat high_boost8_repli_A1mat = Mat(height, width, CV_8UC1, walkbridgeout4);
				imshow("high_boost8_repli_A1", high_boost8_repli_A1mat);
				waitKey();
				destroyWindow("high_boost8_repli_A1");
				imwrite("high_boost8_repli_A1.png", high_boost8_repli_A1mat);
				a = 2;//high boost
				high_boost_4(walkbridge, walkbridgeout4, height, width, a, zero);
				Mat high_boost4_repli_A2mat = Mat(height, width, CV_8UC1, walkbridgeout4);
				imshow("high_boost4_repli_A2", high_boost4_repli_A2mat);
				waitKey();
				destroyWindow("high_boost4_repli_A2");
				imwrite("high_boost4_repli_A2.png", high_boost4_repli_A2mat);
				high_boost_8(walkbridge, walkbridgeout8, height, width, a, zero);
				Mat high_boost8_repli_A2mat = Mat(height, width, CV_8UC1, walkbridgeout4);
				imshow("high_boost8_repli_A2", high_boost8_repli_A2mat);
				waitKey();
				destroyWindow("high_boost8_repli_A2");
				imwrite("high_boost8_repli_A2.png", high_boost8_repli_A2mat);
				a = 4;//high boost
				high_boost_4(walkbridge, walkbridgeout4, height, width, a, zero);
				Mat high_boost4_repli_A4mat = Mat(height, width, CV_8UC1, walkbridgeout4);
				imshow("high_boost4_repli_A4", high_boost4_repli_A4mat);
				waitKey();
				destroyWindow("high_boost4_repli_A4");
				imwrite("high_boost4_repli_A4.png", high_boost4_repli_A4mat);
				high_boost_8(walkbridge, walkbridgeout8, height, width, a, zero);
				Mat high_boost8_repli_A4mat = Mat(height, width, CV_8UC1, walkbridgeout4);
				imshow("high_boost8_repli_A4", high_boost8_repli_A4mat);
				waitKey();
				destroyWindow("high_boost8_repli_A4");
				imwrite("high_boost8_repli_A4.png", high_boost8_repli_A4mat);

			}
					break;
			default:
				cout << "Not this question !!" << endl;
				break;
			}

		}
			  break;
		case 4: {	
			//4.
			cout << "Enter the question number : " << endl << "1. Guassian filter" << endl << "2. DoG" << endl;
			cin >> b;
			switch (b)
			{
			case 1: {

				double sigma = 0.8;
				guassian(turtle, turtleout1, height, width, sigma);
				sigma = 1.3;
				guassian(turtle, turtleout2, height, width, sigma);
				sigma = 2.0;
				guassian(turtle, turtleout3, height, width, sigma);
				Mat turmat = Mat(height, width, CV_8UC1, turtle);
				imshow("turtle_original", turmat);
				waitKey();
				destroyWindow("turtle_original");
				imwrite("turtle_original.png", turmat);
				Mat out1mat = Mat(height, width, CV_8UC1, turtleout1);
				imshow("turtle_8", out1mat);
				waitKey();
				destroyWindow("turtle_8");
				imwrite("turtle_8.png", out1mat);
				Mat out2mat = Mat(height, width, CV_8UC1, turtleout2);
				imshow("turtle_13", out2mat);
				waitKey();
				destroyWindow("turtle_13");
				imwrite("turtle_13.png", out2mat);
				Mat out3mat = Mat(height, width, CV_8UC1, turtleout3);
				imshow("turtle_20", out3mat);
				waitKey();
				destroyWindow("turtle_20");
				imwrite("turtle_20.png", out3mat);
			}
					break;
			case 2: {
				unsigned char* out = new unsigned char[height * width];
				DoG(turtle, out, height, width, 0.5, 1.5);
				Mat outputmat = Mat(height, width, CV_8UC1, out);
				imshow("turtle_DoG", outputmat);
				waitKey();
				destroyWindow("turtle_DoG");
				imwrite("turtle_DoG.png", outputmat);
			}
					break;
			default:
				cout << "Not this question !!" << endl;
				break;
			}

		}
			  break;
		case 5: {
			end = 1;
		}
			  break;
		default:
			cout << "Not this question !!" << endl;
			break;
		}
	}
	return 0;
}
int local_hist_EQ(unsigned char* src,unsigned char* dst,int h ,int w,int local) {
	Mat dstmat = Mat(h, w, CV_8UC1, dst);
	unsigned char** src_slice = new unsigned char* [local * local];
	for (int i = 0; i < local * local; i++) {
		src_slice[i] = new unsigned char[h * w / pow(local, 2)];
	}
	unsigned char** dst_slice = new unsigned char* [local * local];
	for (int i = 0; i < local * local; i++) {
		dst_slice[i] = new unsigned char[h * w / pow(local, 2)];
	}
	for (int x = 0; x < local; x++) {
		for (int y = 0; y < local; y++) {
			for (int i = 0; i < w / local; i++) {
				for (int j = 0; j < h / local; j++) {
					src_slice[x + y * local][i + j * (w / local)] = src[x * w / local + y * (w * h / local) + i + j * w];
				}
			}
		}
	}
	for (int i = 0; i < pow(local, 2); i++) {
		hist_EQ(src_slice[i], dst_slice[i], h / local, w / local);
	}

	for (int x = 0; x < local; x++) {
		for (int y = 0; y < local; y++) {
			for (int i = 0; i < w / local; i++) {
				for (int j = 0; j < h / local; j++) {
					dst[x * w / local + y * (w * h / local) + i + j * w] = dst_slice[x + y * local][i + j * (w / local)];
				}
			}
		}
	}
	return 1;
}
void hist_EQ(unsigned char* image, unsigned char* eq, int h, int w) {
	int* hist = new int[256];
	int* temp = new int[256];
	Histogram(image, hist, h, w);
	double* cdf = new double[256];
	for (int i = 0; i < 256; i++) {
		cdf[i] = (double)hist[i] / (h * w);
		if (i != 0)
			cdf[i] = cdf[i - 1] + cdf[i];
		temp[i] = cdf[i] * 255;
	}
	for (int i = 0; i < h * w; i++) {
		int a = 0;
		a = image[i];
		eq[i] = temp[a];
	}
}
void cdfDisplay(int histogram[], const char* name)
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
	//destroyWindow(name);
	//imwrite(name, histImage);
}
void histDisplay(unsigned char* src, int h,int w, const char* filename)
{
	int hist[256];
	Histogram(src, hist, h, w);

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
	imshow(filename, histImage);
	waitKey();
	destroyWindow(filename);
	imwrite(filename, histImage);
}
int hist_matching(unsigned char* src1,unsigned char* src2,unsigned char* dst,int h,int w) {
	int* hist_src1 = new int[256];
	int* hist_src2 = new int[256];
	double* cdf1 = new double[256];
	double* cdf2 = new double[256];
	int* temp1 = new int[256];
	int* temp2 = new int[256];
	Histogram(src1, hist_src1, h, w);
	Histogram(src2, hist_src2, h, w);
	for (int i = 0; i < 256; i++) {
		cdf1[i] = (double)hist_src1[i] / (h * w);
		if (i != 0)
			cdf1[i] = cdf1[i - 1] + cdf1[i];
		temp1[i] = cdf1[i] * 255;
	}
	for (int i = 0; i < 256; i++) {
		cdf2[i] = (double)hist_src2[i] / (h * w);
		if (i != 0)
			cdf2[i] = cdf2[i - 1] + cdf2[i];
		temp2[i] = cdf2[i] * 255;
	}
	//cdfDisplay(temp1, "temp1_cdf");
	//cdfDisplay(temp2, "temp2_cdf");
	for (int i = 0; i < h * w; i++) {
		int a = 0;
		a = src1[i];
		for (int j = 0; j < 255; j++) {
			if (temp2[j] > a) {
				dst[i] = temp1[j];
				break;
			}
		}
	}
	return 0;
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
int DoG(unsigned char* src, unsigned char* out, int h, int w, double sigma1, double sigma2) {
	unsigned char* temp1 = new unsigned char[h * w];
	unsigned char* temp2 = new unsigned char[h * w];
	guassian(src, temp1, h, w, sigma1);
	guassian(src, temp2, h, w, sigma2);
	for (int i = 0; i < h * w; i++) {
		out[i] = temp1[i] - temp2[i];
	}
	return 0;
}
int guassian(unsigned char* src, unsigned char* out, int h, int w, double sigma) {
	int* temp = new int[(h + 4) * (w + 4)];
	unsigned char* test = new unsigned char[(h + 4) * (w + 4)];
	memset(out, 0, sizeof(uchar) * h * w);
	memset(temp, 0, sizeof(int) * (h + 4) * (w + 4));
	temp[0] = src[2 * w + 2];
	temp[1] = src[2 * w + 1];
	temp[w + 4] = src[w + 2];
	temp[w + 5] = src[w + 1];
	temp[w + 2] = src[3 * w - 2];
	temp[w + 3] = src[3 * w - 3];
	temp[2 * w + 6] = src[2 * w - 2];
	temp[2 * w + 7] = src[2 * w - 3];
	temp[(h + 2) * (w + 4)] = src[w * (h - 2) + 2];
	temp[(h + 2) * (w + 4) + 1] = src[w * (h - 2) + 1];
	temp[(h + 3) * (w + 4)] = src[w * (h - 3) + 2];
	temp[(h + 3) * (w + 4) + 1] = src[w * (h - 3) + 1];
	temp[(h + 2) * (w + 4) + w + 2] = src[w * (h - 2) + w - 2];
	temp[(h + 2) * (w + 4) + 1 + w + 2] = src[w * (h - 2) + w - 3];
	temp[(h + 3) * (w + 4) + w + 2] = src[w * (h - 3) + w - 2];
	temp[(h + 3) * (w + 4) + 1 + w + 2] = src[w * (h - 3) + w - 3];
	for (int i = 2; i < w + 2; i++) {//x-edge padding
		temp[i + (w + 4)] = src[w + i - 2];
		temp[i] = src[2 * w + (i - 2)];
		temp[i + (w + 4) * (h + 2)] = src[i - 2 + w * (h - 2)];
		temp[i + (w + 4) * (h + 3)] = src[i - 2 + w * (h - 3)];
	}
	for (int i = 2; i < h + 2; i++) {//y-edge padding
		temp[(w + 4) * i + 1] = src[w * (i - 2) + 1];
		temp[(w + 4) * i] = src[w * (i - 2) + 2];
		temp[(w + 4) * i + w + 3] = src[w * (i - 2) + w - 3];
		temp[(w + 4) * i + w + 2] = src[w * (i - 2) + w - 2];
	}
	for (int i = 2; i < h + 2; i++) {
		for (int j = 2; j < w + 2; j++) {
			temp[i * (w + 4) + j] = src[(i - 2) * w + j - 2];
		}
	}
	//test
	for (int i = 0; i < h + 4; i++) {
		for (int j = 0; j < w + 4; j++) {
			test[i * (w + 4) + j] = temp[i * (w + 4) + j];
		}
	}
	Mat inmat = Mat(h, w, CV_8UC1, src);
	Mat testmat = Mat(h + 4, w + 4, CV_8UC1, test);
	//end test
	int step[25][2] = {
		{-2,-2},{-1,-2},{0,-2},{1,-2},{2,-2},
		{-2,-1},{-1,-1},{0,-1},{1,-1},{2,-1},
		{-2, 0},{-1, 0},{0, 0},{1, 0},{2, 0},
		{-2, 1},{-1, 1},{0, 1},{1, 1},{2, 1},
		{-2, 2},{-1, 2},{0, 2},{1, 2},{2, 2}
	};
	double constant[25];
	double sum = 0;
	for (int i = 0; i < 25; i++) {
		constant[i] = (1 / (2 * M_PI * pow(sigma, 2))) * exp(-(pow(step[i][0], 2) + pow(step[i][1], 2)) / (2 * pow(sigma, 2)));
		sum = sum + constant[i];
	}
	for (int i = 0; i < 25; i++) {
		constant[i] = constant[i] / sum;
	}
	//process
	double a = 0;
	for (int i = 2; i < w + 2; i++) {
		for (int j = 2; j < h + 2; j++) {
			a = 0;
			for (int z = 0; z < 25; z++) {
				int x = i + step[z][0];
				int y = j + step[z][1];
				a = a + (temp[x + y * (w + 4)] * constant[z]);
			}
			if (a < 0)
				out[i - 2 + (j - 2) * w] = 0;
			else if (a > 255)
				out[i - 2 + (j - 2) * w] = 255;
			else
				out[i - 2 + (j - 2) * w] = a;
		}
	}
	////test
	//int t = 1;
	//for (int i = 0; i < 25; i++) {
	//	cout << constant[i] << "   ";
	//	if (t == 5) {
	//		cout << endl;
	//		t = 0;
	//	}
	//	t++;
	//}
	//cout<<endl;
	return 0;
}
int high_boost_8(unsigned char* walkbridge, unsigned char* out, int h, int w, int a, bool zero) {
	int* temp = new int[(h + 2) * (w + 2)];
	int* temp2 = new int[(h + 2) * (w + 2)];
	memset(out, 0, sizeof(uchar) * h * w);
	memset(temp, 0, sizeof(int) * (h + 2) * (w + 2));
	if (!zero) {
		temp[0] = walkbridge[0];
		temp[w + 1] = walkbridge[w - 1];
		temp[(h + 1) * (w + 2)] = walkbridge[(h - 1) * w];
		temp[(h + 1) * (w + 2) + w + 1] = walkbridge[(h - 1) * w + w - 1];
		for (int i = 1; i < w + 1; i++) {
			temp[i] = walkbridge[i - 1];
			temp[i + (w + 2) * (h + 1)] = walkbridge[i - 1 + w * (h - 1)];
		}
		for (int i = 1; i < h + 1; i++) {
			temp[(w + 2) * i] = walkbridge[w * (i - 1)];
			temp[(w + 2) * i + w + 1] = walkbridge[w * (i - 1) + w - 1];
		}

	}
	for (int i = 1; i < h + 1; i++) {
		for (int j = 1; j < w + 1; j++) {
			temp[i * (w + 2) + j] = walkbridge[(i - 1) * w + j - 1];
		}
	}
	int sum = 0, d = 0;
	int step[8][2] = { {-1,-1},{0,-1},{1,-1},{-1,0},{1,0},{-1,1},{0,1},{1,1} };
	for (int i = 1; i < w + 1; i++) {
		for (int j = 1; j < h + 1; j++) {
			sum = 0;
			for (int z = 0; z < 8; z++) {
				int x = i + step[z][0];
				int y = j + step[z][1];
				sum = sum + temp[x + y * (w + 2)];
			}
			//因為相減可能小於0，因此加入abs使其特徵凸顯
			//d = abs(8 * walkbridge[(i - 1) + (j - 1) * w] - sum);
			d = 8 * walkbridge[(i - 1) + (j - 1) * w] - sum;
			if (d < 0)
				d = 0;
			else if (d > 255)
				d = 255;
			d = a * walkbridge[(i - 1) + (j - 1) * w] + d;
			if (a != 0) {
				d = d / a;
			}
			if (d < 0)
				out[(i - 1) + (j - 1) * w] = 0;
			else if (d > 255)
				out[(i - 1) + (j - 1) * w] = 255;
			else
				out[(i - 1) + (j - 1) * w] = d;
		}
	}
	return 0;
}
int high_boost_4(unsigned char* walkbridge, unsigned char* out, int h, int w, int a, bool zero) {
	int* temp = new int[(h + 2) * (w + 2)];
	int* temp2 = new int[(h + 2) * (w + 2)];
	memset(out, 0, sizeof(uchar) * h * w);
	memset(temp, 0, sizeof(int) * (h + 2) * (w + 2));
	if (!zero) {
		temp[0] = walkbridge[0];
		temp[w + 1] = walkbridge[w - 1];
		temp[(h + 1) * (w + 2)] = walkbridge[(h - 1) * w];
		temp[(h + 1) * (w + 2) + w + 1] = walkbridge[(h - 1) * w + w - 1];
		for (int i = 1; i < w + 1; i++) {
			temp[i] = walkbridge[i - 1];
			temp[i + (w + 2) * (h + 1)] = walkbridge[i - 1 + w * (h - 1)];
		}
		for (int i = 1; i < h + 1; i++) {
			temp[(w + 2) * i] = walkbridge[w * (i - 1)];
			temp[(w + 2) * i + w + 1] = walkbridge[w * (i - 1) + w - 1];
		}
	}
	for (int i = 1; i < h + 1; i++) {
		for (int j = 1; j < w + 1; j++) {
			temp[i * (w + 2) + j] = walkbridge[(i - 1) * w + j - 1];
		}
	}
	int d = 0;
	int sum = 0;
	int step[4][2] = { {0,-1},{-1,0},{1,0},{0,1} };
	for (int i = 1; i < w + 1; i++) {
		for (int j = 1; j < h + 1; j++) {
			sum = 0;
			for (int z = 0; z < 4; z++) {
				int x = i + step[z][0];
				int y = j + step[z][1];
				sum = sum + temp[x + y * (w + 2)];
			}
			d = 4 * walkbridge[(i - 1) + (j - 1) * w] - sum;
			if (d < 0)
				d = 0;
			else if (d > 255)
				d = 255;
			//d = abs(a * walkbridge[(i - 1) + (j - 1) * w] + d);
			d = (a * walkbridge[(i - 1) + (j - 1) * w] + d);
			if (a != 0) {
				d = d / a;
			}
			if (d < 0)
				out[(i - 1) + (j - 1) * w] = 0;
			else if (d > 255)
				out[(i - 1) + (j - 1) * w] = 255;
			else
				out[(i - 1) + (j - 1) * w] = d;
		}
	}
	return 0;
}
int average10(unsigned char* aver10) {
	int* temp = new int[960 * 540];
	memset(temp, 0, sizeof(int) * 960 * 540);
	VideoCapture cap("street.avi");
	if (!cap.isOpened()) {
		cout << "ERROR opening video!!" << endl;
		return -1;
	}
	//frame 477 960*540
	int count = 0;
	while (1) {
		Mat frame;
		cap >> frame;
		if (frame.empty())
			break;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		if (count < 45)
			count++;
		else {
			count = 0;
			unsigned char* t = frame.data;
			for (int i = 0; i < 960; i++) {
				for (int j = 0; j < 540; j++) {
					temp[i + j * 960] = temp[i + j * 960] + (int)t[i + j * 960];
				}
			}

		}
	}

	for (int i = 0; i < 960; i++) {
		for (int j = 0; j < 540; j++) {
			temp[i + j * 960] = temp[i + j * 960] / 10;
			aver10[i + j * 960] = temp[i + j * 960];
		}
	}
	cap.release();
	return 0;
}
int average200(unsigned char* aver200) {
	int* temp = new int[960 * 540];
	memset(temp, 0, sizeof(int) * 960 * 540);
	VideoCapture cap("street.avi");
	if (!cap.isOpened()) {
		cout << "ERROR opening video!!" << endl;
		return -1;
	}
	//frame 477 960*540
	int a = 0;
	int count = 0;
	while (1) {
		Mat frame;
		cap >> frame;
		if (frame.empty())
			break;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		if (count < 1)
			count++;
		else {
			count = 0;
			unsigned char* t = frame.data;
			if (a < 200) {
				for (int i = 0; i < 960; i++) {
					for (int j = 0; j < 540; j++) {
						temp[i + j * 960] = temp[i + j * 960] + (int)t[i + j * 960];
					}
				}
			}
			a++;
		}
	}
	for (int i = 0; i < 960; i++) {
		for (int j = 0; j < 540; j++) {
			temp[i + j * 960] = temp[i + j * 960] / 200;
			aver200[i + j * 960] = temp[i + j * 960];
		}
	}
	cap.release();
	return 0;

}
int averageALL(unsigned char* averAll) {
	int* temp = new int[960 * 540];
	memset(temp, 0, sizeof(int) * 960 * 540);
	VideoCapture cap("street.avi");
	if (!cap.isOpened()) {
		cout << "ERROR opening video!!" << endl;
		return -1;
	}
	//frame 477 960*540
	while (1) {
		Mat frame;
		cap >> frame;
		if (frame.empty())
			break;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		unsigned char* t = frame.data;
		for (int i = 0; i < 960; i++) {
			for (int j = 0; j < 540; j++) {
				temp[i + j * 960] = temp[i + j * 960] + (int)t[i + j * 960];
			}
		}
	}

	for (int i = 0; i < 960; i++) {
		for (int j = 0; j < 540; j++) {
			temp[i + j * 960] = temp[i + j * 960] / 477;
			averAll[i + j * 960] = temp[i + j * 960];
		}
	}
	cap.release();
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