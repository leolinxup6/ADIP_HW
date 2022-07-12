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

int fit_image(unsigned char* dst, float* in, int h, int w);
void openRaw(const char filename[], unsigned char* tmp, int h, int w);
void storeRaw(const char filename[], unsigned char* tmp, int h, int w, int size);
float MSE(unsigned char* image1, unsigned char* image2, int h, int w);
float PSNR(unsigned char* image1, unsigned char* image2, int h, int w);
int filter(int a, unsigned char* src, float* H_filter,Mat& spec, Mat& out, int h, int w);

int H_filter_uv(float* filter, float rH, float rL, float c, float D0, int h, int w);
int idealLP(float* filter, float D0, int h, int w);
int gaussianHP(float* filter, float D0, int h, int w);
int D_filter_uv(int a, float* filter, int h, int w);
int main()
{
	//QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
	//QueryPerformanceCounter(&m_nBeginTime); // 計時開始
	//QueryPerformanceCounter(&nEndTime);// 計時截止
	//time[x] = (float)(nEndTime.QuadPart - m_nBeginTime.QuadPart) / m_nFreq.QuadPart;
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	LARGE_INTEGER nEndTime;
	float time[15];
	char city_filename[50] = "aerialcity512.raw";
	char house_filename[50] = "house512.raw";
	const int height = 512, width = 512, size = 512 * 512;
	unsigned char* city = new unsigned char[size]();
	unsigned char* house = new unsigned char[size]();
	openRaw(city_filename, city, height, width);
	openRaw(house_filename, house, height, width);

	Mat city_mat = Mat(height, width, CV_8UC1, city);
	Mat house_mat = Mat(height, width, CV_8UC1, house);
	imwrite("city.png", city_mat);
	imwrite("house.png", house_mat);



	
	int a = 0, b = 0;
	while (a == 0) {
		cout << "Enter the question number : " << endl << "1. Homomorphic filter" << endl << "2. Filter in frequency domain" << endl << "3. Exit" << endl;
		cin >> b;
		switch (b) {
		case 1: {
			float* H_filter = new float[size];
			float rH, rL, c, D0;

			//課本範例參數
			rH = 0.25;
			rL = 2;
			c = 1;
			D0 = 80;
			cout << "rL : ";
			cin >> rL;
			cout << "rH : ";
			cin >> rH;
			cout << "c : ";
			cin >> c;
			cout << "D0 : ";
			cin >> D0;
			H_filter_uv(H_filter, rL, rH, c, D0, height, width);
			Mat homo_house,mag_homo_house;
			filter(0,house, H_filter, mag_homo_house, homo_house, height, width);
			imwrite("homo_house.png", homo_house);

			imshow("homo_house", homo_house);
			waitKey();
			destroyWindow("homo_house");
		}
			  break;
		case 2: {
			cout << "Enter the question number : " << endl << "1. ideal Lowpass filter" << endl << "2. Gaussian highpass filter"<< endl;
			cin >> b;

			switch (b) {
			case 1: {
				float D0;

				float* ILPF = new float[size];
				D0 = 5;
				idealLP(ILPF, D0, height, width);
				Mat city_ILPF_5,mag_ILPF_5;
				filter(1,city, ILPF, mag_ILPF_5, city_ILPF_5, height, width);
				imwrite("city_ILPF_5.png", city_ILPF_5);
				imwrite("mag_ILPF_5.png", mag_ILPF_5);
				cout << "city with city_ILPF_5 PSNR : " << PSNR(city, city_ILPF_5.data, height, width) << " db MSE : " << MSE(city, city_ILPF_5.data, height, width) << endl;

				D0 = 25;
				idealLP(ILPF, D0, height, width);
				Mat city_ILPF_25;
				Mat	mag_ILPF_25;
				filter(1,city, ILPF, mag_ILPF_25, city_ILPF_25, height, width);
				imwrite("city_ILPF_25.png", city_ILPF_25);
				imwrite("mag_ILPF_25.png", mag_ILPF_25);
				cout << "city with city_ILPF_25 PSNR : " << PSNR(city_ILPF_25.data, city, height, width) << " db MSE : " << MSE(city_ILPF_25.data, city, height, width) << endl;

				D0 = 125;
				idealLP(ILPF, D0, height, width);
				Mat city_ILPF_125, mag_ILPF_125;
				filter(1,city, ILPF, mag_ILPF_125, city_ILPF_125, height, width);
				imwrite("city_ILPF_125.png", city_ILPF_125);
				imwrite("mag_ILPF_125.png", mag_ILPF_125);
				cout << "city with city_ILPF_125 PSNR : " << PSNR(city, city_ILPF_125.data, height, width) << " db MSE : " << MSE(city, city_ILPF_125.data, height, width) << endl;

				imshow("city_ILPF_5", city_ILPF_5);
				waitKey();
				destroyWindow("city_ILPF_5");
				imshow("mag_ILPF_5", mag_ILPF_5);
				waitKey();
				destroyWindow("mag_ILPF_5");
				imshow("city_ILPF_25", city_ILPF_25);
				waitKey();
				destroyWindow("city_ILPF_25");
				imshow("specy_ILPF_25", mag_ILPF_25);
				waitKey();
				destroyWindow("specy_ILPF_25");
				imshow("city_ILPF_125", city_ILPF_125);
				waitKey();
				destroyWindow("city_ILPF_125");
				imshow("mag_ILPF_125", mag_ILPF_125);
				waitKey();
				destroyWindow("mag_ILPF_125");
			}
				  break;
			case 2: {
				float D0;

				float* GHPF = new float[size];
				D0 = 5;
				gaussianHP(GHPF, D0, height, width);
				Mat city_GHPF_5, mag_GHPF_5;
				filter(2,city, GHPF, mag_GHPF_5, city_GHPF_5, height, width);
				imwrite("city_GHPF_5.png", city_GHPF_5);
				imwrite("mag_GHPF_5.png", mag_GHPF_5);
				cout << "city with city_GHPF_5 PSNR : " << PSNR(city, city_GHPF_5.data, height, width) << " db MSE : " << MSE(city, city_GHPF_5.data, height, width) << endl;

				D0 = 25;
				gaussianHP(GHPF, D0, height, width);
				Mat city_GHPF_25, mag_GHPF_25;
				filter(2,city, GHPF, mag_GHPF_25, city_GHPF_25, height, width);
				imwrite("city_GHPF_25.png", city_GHPF_25);
				imwrite("mag_GHPF_25.png", mag_GHPF_25);
				cout << "city with city_GHPF_25 PSNR : " << PSNR(city, city_GHPF_25.data, height, width) << " db MSE : " << MSE(city, city_GHPF_25.data, height, width) << endl;

				D0 = 125;
				gaussianHP(GHPF, D0, height, width);
				Mat city_GHPF_125, mag_GHPF_125;
				filter(2,city, GHPF, mag_GHPF_125, city_GHPF_125, height, width);
				imwrite("city_GHPF_125.png", city_GHPF_125);
				imwrite("mag_GHPF_125.png", mag_GHPF_125);
				cout << "city with city_GHPF_125 PSNR : " << PSNR(city, city_GHPF_125.data, height, width) << " db MSE : " << MSE(city, city_GHPF_125.data, height, width) << endl;

				imshow("city_GHPF_5", city_GHPF_5);
				waitKey();
				destroyWindow("city_GHPF_5");
				imshow("mag_GHPF_5", mag_GHPF_5);
				waitKey();
				destroyWindow("mag_GHPF_5");
				imshow("city_GHPF_25", city_GHPF_25);
				waitKey();
				destroyWindow("city_GHPF_25");
				imshow("mag_GHPF_25", mag_GHPF_25);
				waitKey();
				destroyWindow("mag_GHPF_25");
				imshow("city_GHPF_125", city_GHPF_125);
				waitKey();
				destroyWindow("city_GHPF_125");
				imshow("mag_GHPF_125", mag_GHPF_125);
				waitKey();
				destroyWindow("mag_GHPF_125");
			}
				  break;
			default:
				break;
			}
		
		}
			  break;
		case 3: {
			a = 1;
		}
			  break;
		default:
			break;
		}
	}
	return 0;

}
int fit_image(unsigned char* dst, float* in, int h, int w) {
	int size = h * w;
	double max = -100000, min = 10000000;
	for (int i = 0; i < size; i++) {
		if (in[i] > max)
			max = in[i];
		else if (in[i] < min)
			min = in[i];
	}
	for (int i = 0; i < size; i++) {
		in[i] = (in[i] - min) * 255 / (max - min) + 0.5;
	}
	for (int i = 0; i < size; i++) {
		dst[i] = (int)in[i];
	}
	return 1;
}
int gaussianHP(float* filter, float D0, int h, int w) {
	int const size = h * w;
	D_filter_uv(1, filter, h, w); 
	for (int i = 0; i < size; i++) {
		filter[i] = (float)1.0 * (1 - exp(-pow(filter[i], 2) * 1.0 / pow(D0, 2)));
	}
	return 1;
}
int idealLP(float* filter, float D0, int h, int w) {
	int const size = h * w;
	D_filter_uv(1, filter, h, w);
	for (int i = 0; i < size; i++) {
		if (filter[i] > D0)
			filter[i] = 0;
		else
			filter[i] = 1;
	}
	return 1;
}
int H_filter_uv(float* filter, float rH, float rL, float c, float D0, int h, int w) {
	D_filter_uv(0, filter, h, w);
	for (int i = 0; i < h * w; i++) {
		filter[i] = (float)(rH - rL) * 1.0 * (1 - exp((-c) * pow(filter[i], 2) * 1.0 / pow(D0, 2))) + rL;
	}
	return 1;
}
int D_filter_uv(int a, float* filter, int h, int w) {
	float* temp = new float[h * w];
	float P = (2 * w - 1), Q = (2 * h - 1);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			filter[i * w + j] = sqrt(pow((j - (w * 1.0) / 2), 2) + pow((i - (h * 1.0) / 2), 2));
		}
	}
	float tmp;
	if (a == 1) {
		for (int i = 0; i < h / 2; i++) {
			for (int j = 0; j < w / 2; j++) {
				tmp = filter[i * w + j];
				filter[i * w + j] = filter[(h * w / 2 + w / 2) + (i * w + j)];
				filter[(h * w / 2 + w / 2) + (i * w + j)] = tmp;
				tmp = filter[(w / 2) + (i * w + j)];
				filter[(w / 2) + i * w + j] = filter[(h * w / 2) + (i * w + j)];
				filter[(h * w / 2) + (i * w + j)] = tmp;
			}
		}

	}
	
	return 1;
}
int filter(int a, unsigned char* src, float* H_filter,Mat& spec, Mat& out, int h, int w) {
	int const size = h * w;
	float* cc = new float[size];
	if (a == 0) {
		for (int i = 0; i < size; i++) {
			cc[i] = log((float)src[i] + 1);
		}
	}
	else {
		for (int i = 0; i < size; i++) {
			cc[i] = (float)src[i];
		}
	}
	Mat imgi, imgo, pad, complexI;
	imgi = Mat(h, w, CV_32FC1, cc);
	int R = getOptimalDFTSize(imgi.rows);
	int C = getOptimalDFTSize(imgi.cols);
	copyMakeBorder(imgi, pad, 0, R - imgi.rows, 0, C - imgi.cols, BORDER_CONSTANT, Scalar::all(0));
	Mat planes[] = { Mat_<float>(pad), Mat::zeros(pad.size(), CV_32F) };
	merge(planes, 2, complexI);
	dft(complexI, complexI);
	split(complexI, planes);

	float* dft_re = new float[size];
	float* dft_im = new float[size];
	dft_re = (float*)planes[0].data;
	dft_im = (float*)planes[1].data;

	float* homo_uv_re = new float[size];
	float* homo_uv_im = new float[size];
	for (int i = 0; i < size; i++) {
		homo_uv_re[i] = (float)1.0 * dft_re[i] * H_filter[i];
		homo_uv_im[i] = (float)1.0 * dft_im[i] * H_filter[i];

	}
	planes[0] = Mat(h, w, CV_32F, homo_uv_re);
	planes[1] = Mat(h, w, CV_32F, homo_uv_im);

	Mat complexI12;
	merge(planes, 2, complexI);

	if (a == 2||a == 1) {
		magnitude(planes[0], planes[1], planes[0]);
		imgo = planes[0];
		imgo += Scalar::all(1);
		log(imgo, imgo);
		imgo = imgo(Rect(0, 0, imgo.cols & -2, imgo.rows & -2));

		int x = imgo.cols / 2;
		int y = imgo.rows / 2;
		Mat tmp;
		Mat q0(imgo, Rect(0, 0, x, y));
		Mat q1(imgo, Rect(x, 0, x, y));
		Mat q2(imgo, Rect(0, y, x, y));
		Mat q3(imgo, Rect(x, y, x, y));
		q0.copyTo(tmp);
		q3.copyTo(q0);
		tmp.copyTo(q3);
		q1.copyTo(tmp);
		q2.copyTo(q1);
		tmp.copyTo(q2);
		normalize(imgo, imgo, 0, 1, NORM_MINMAX);
		imgo.convertTo(imgo, CV_8UC1, 255, 0);
		imgo.copyTo(spec);
	}
	Mat tmp;
	idft(complexI, tmp, DFT_SCALE | DFT_REAL_OUTPUT);

	Mat invDFT;
	float* temp = new float[size];
	temp = (float*)tmp.data;
	if (a == 0) {
		for (int i = 0; i < size; i++) {
			temp[i] = exp(temp[i] + 1);
		}
	}
	invDFT = Mat(h, w, CV_32F, temp);
	normalize(invDFT, invDFT, 0, 1, NORM_MINMAX);
	invDFT.convertTo(invDFT, CV_8UC1, 255, 0);
	invDFT.copyTo(out);
	return 1;
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
float MSE(unsigned char* image1, unsigned char* image2, int h, int w) {
	int sum_sq = 0;
	float mse;

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
	return mse = fabs(sum_sq / (h * w));
}
float PSNR(unsigned char* image1, unsigned char* image2, int h, int w) {
	float mse = 0;
	mse = MSE(image1, image2, h, w);
	return fabs(10 * log10((255 * 255) / mse));
}