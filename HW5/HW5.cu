#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <ctime>
#include <opencv2\opencv.hpp>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"


using namespace std;
using namespace cv;

void openRaw(const char filename[], unsigned char* tmp, int h, int w);
void storeRaw(const char filename[], unsigned char* tmp, int h, int w, int size);
double MSE(unsigned char* image1, unsigned char* image2, int h, int w);
double PSNR(unsigned char* image1, unsigned char* image2, int h, int w);
int DFT_phase(unsigned char* phase, double* re, double* im, int h, int w);
int DFT_2D(unsigned char* img, unsigned char* dst, unsigned char* phase, int h, int w);
__global__ void dft_2d_compute(unsigned char* src, double* out, double* re, double* im, int h, int w);
int fit_image(unsigned char* dst, double* out, int h, int w);
int opencv_dft(unsigned char* src, Mat& dst, int h, int w);
int opencv_phase(unsigned char* src, Mat& ph, int h, int w);
int IDFT(unsigned char* src, unsigned char* dst, int h, int w);
__global__ void idft_2d_compute(double* src, double* out, double* re, double* im, int h, int w);
int main()
{
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	LARGE_INTEGER nEndTime;
	double time[15];
	char rect_filename[50] = "rect256.raw";
	char circle_filename[50] = "circle256.raw";
	char square_filename[50] = "Square256.raw";
	char square_rota_filename[50] = "square256_rota.raw";
	const int height = 256, width = 256, size = 256 * 256;
	unsigned char* rect = new unsigned char[size]();
	unsigned char* circle = new unsigned char[size]();
	unsigned char* square = new unsigned char[size]();
	unsigned char* square_rota = new unsigned char[size]();
	unsigned char* rect_phase = new unsigned char[size]();
	unsigned char* circle_phase = new unsigned char[size]();
	unsigned char* square_phase = new unsigned char[size]();
	unsigned char* square_rota_phase = new unsigned char[size]();
	unsigned char* rect_out = new unsigned char[size]();
	unsigned char* circle_out = new unsigned char[size]();
	unsigned char* square_out = new unsigned char[size]();
	unsigned char* square_rota_out = new unsigned char[size]();
	unsigned char* rect_IDFT = new unsigned char[size]();
	unsigned char* circle_IDFT = new unsigned char[size]();
	unsigned char* square_IDFT = new unsigned char[size]();
	unsigned char* square_rota_IDFT = new unsigned char[size]();
	Mat rect_opencv_out, circle_opencv_out, square_opencv_out, square_rota_opencv_out;
	openRaw(rect_filename, rect, height, width);
	openRaw(circle_filename, circle, height, width);
	openRaw(square_filename, square, height, width);
	openRaw(square_rota_filename, square_rota, height, width);

	Mat rect_mat = Mat(height, width, CV_8UC1, rect);
	Mat circle_mat = Mat(height, width, CV_8UC1, circle);
	Mat square_mat = Mat(height, width, CV_8UC1, square);
	Mat square_rota_mat = Mat(height, width, CV_8UC1, square_rota);
	imwrite("rect.png", rect_mat);
	imwrite("circle.png", circle_mat);
	imwrite("square.png", square_mat);
	imwrite("square_rota.png", square_rota_mat);

	QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
	QueryPerformanceCounter(&m_nBeginTime); // 計時開始
	DFT_2D(rect, rect_out, rect_phase, height, width);
	QueryPerformanceCounter(&nEndTime);// 計時截止
	time[0] = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) / m_nFreq.QuadPart;
	Mat rect_DFT_mat = Mat(height, width, CV_8UC1, rect_out);
	Mat rect_phase_mat = Mat(height, width, CV_8UC1, rect_phase);
	imwrite("rect_DFT.png", rect_DFT_mat);
	imwrite("rect_phase.png", rect_phase_mat);
	//cout << time[0] << "  s" << endl;

	QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
	QueryPerformanceCounter(&m_nBeginTime); // 計時開始
	DFT_2D(circle, circle_out, circle_phase, height, width);
	QueryPerformanceCounter(&nEndTime);// 計時截止
	time[1] = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) / m_nFreq.QuadPart;
	Mat circle_DFT_mat = Mat(height, width, CV_8UC1, circle_out);
	Mat circle_phase_mat = Mat(height, width, CV_8UC1, circle_phase);
	imwrite("circle_DFT.png", circle_DFT_mat);
	imwrite("circle_phase.png", circle_phase_mat);

	QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
	QueryPerformanceCounter(&m_nBeginTime); // 計時開始
	DFT_2D(square, square_out, square_phase, height, width);
	QueryPerformanceCounter(&nEndTime);// 計時截止
	time[2] = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) / m_nFreq.QuadPart;
	Mat square_DFT_mat = Mat(height, width, CV_8UC1, square_out);
	Mat square_phase_mat = Mat(height, width, CV_8UC1, square_phase);
	imwrite("square_DFT.png", square_DFT_mat);
	imwrite("square_phase.png", square_phase_mat);

	QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
	QueryPerformanceCounter(&m_nBeginTime); // 計時開始
	DFT_2D(square_rota, square_rota_out, square_rota_phase, height, width);
	QueryPerformanceCounter(&nEndTime);// 計時截止
	time[3] = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) / m_nFreq.QuadPart;
	Mat square_rota_DFT_mat = Mat(height, width, CV_8UC1, square_rota_out);
	Mat square_rota_phase_mat = Mat(height, width, CV_8UC1, square_rota_phase);
	imwrite("square_rota_DFT.png", square_rota_DFT_mat);
	imwrite("square_rota_phase.png", square_rota_phase_mat);

	//opencv

	Mat rect_opencv_phase;
	Mat circle_opencv_phase;
	Mat square_opencv_phase;
	Mat square_rota_opencv_phase;
	QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
	QueryPerformanceCounter(&m_nBeginTime); // 計時開始
	opencv_dft(rect, rect_opencv_out, height, width);
	opencv_phase(rect, rect_opencv_phase, height, width);
	QueryPerformanceCounter(&nEndTime);// 計時截止
	time[4] = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) / m_nFreq.QuadPart;
	imwrite("rect_DFT_opencv.png", rect_opencv_out);
	imwrite("rect_phase_opencv.png", rect_opencv_phase);
	//cout << time[4] << "  s" << endl;

	QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
	QueryPerformanceCounter(&m_nBeginTime); // 計時開始
	opencv_dft(circle, circle_opencv_out, height, width);
	opencv_phase(circle, circle_opencv_phase, height, width);
	QueryPerformanceCounter(&nEndTime);// 計時截止
	time[5] = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) / m_nFreq.QuadPart;
	imwrite("circle_DFT_opencv.png", circle_opencv_out);
	imwrite("circle_phase_opencv.png", circle_opencv_phase);

	QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
	QueryPerformanceCounter(&m_nBeginTime); // 計時開始
	opencv_dft(square, square_opencv_out, height, width);
	opencv_phase(square, square_opencv_phase, height, width);
	QueryPerformanceCounter(&nEndTime);// 計時截止
	time[6] = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) / m_nFreq.QuadPart;
	imwrite("square_DFT_opencv.png", square_opencv_out);
	imwrite("square_phase_opencv.png", square_opencv_phase);

	QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
	QueryPerformanceCounter(&m_nBeginTime); // 計時開始
	opencv_dft(square_rota, square_rota_opencv_out, height, width);
	opencv_phase(square_rota, square_rota_opencv_phase, height, width);
	QueryPerformanceCounter(&nEndTime);// 計時截止
	time[7] = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) / m_nFreq.QuadPart;
	imwrite("square_rota_DFT_opencv.png", square_rota_opencv_out);
	imwrite("square_rota_phase_opencv.png", square_rota_opencv_phase);

	// IDFT

	QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
	QueryPerformanceCounter(&m_nBeginTime); // 計時開始
	IDFT(rect, rect_IDFT, height, width);
	QueryPerformanceCounter(&nEndTime);// 計時截止
	time[8] = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) / m_nFreq.QuadPart;
	Mat rect_IDFT_mat = Mat(height, width, CV_8UC1, rect_IDFT);
	imwrite("rect_IDFT.png", rect_IDFT_mat);
	//cout << time[8] << "  s" << endl;

	QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
	QueryPerformanceCounter(&m_nBeginTime); // 計時開始
	IDFT(circle, circle_IDFT, height, width);
	QueryPerformanceCounter(&nEndTime);// 計時截止
	time[9] = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) / m_nFreq.QuadPart;
	Mat circle_IDFT_mat = Mat(height, width, CV_8UC1, circle_IDFT);
	imwrite("circle_IDFT.png", circle_IDFT_mat);

	QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
	QueryPerformanceCounter(&m_nBeginTime); // 計時開始
	IDFT(square, square_IDFT, height, width);
	QueryPerformanceCounter(&nEndTime);// 計時截止
	time[10] = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) / m_nFreq.QuadPart;
	Mat square_IDFT_mat = Mat(height, width, CV_8UC1, square_IDFT);
	imwrite("square_IDFT.png", square_IDFT_mat);

	QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
	QueryPerformanceCounter(&m_nBeginTime); // 計時開始
	IDFT(square_rota, square_rota_IDFT, height, width);
	QueryPerformanceCounter(&nEndTime);// 計時截止
	time[11] = (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) / m_nFreq.QuadPart;
	Mat square_rota_IDFT_mat = Mat(height, width, CV_8UC1, square_rota_IDFT);
	imwrite("square_rota_IDFT.png", square_rota_IDFT_mat);

int a = 0, b = 0;
	while (a == 0) {
		cout << "Enter the question number : " << endl << "1. my own DFT" << endl << "2. openCV DFT" << endl << "3. IDFT" << endl << "4. Exit" << endl;
		cin >> b;
		switch (b) {
		case 1: {
			imshow("rect_DFT", rect_DFT_mat);
			waitKey();
			destroyWindow("rect_DFT");
			imshow("rect_phase", rect_phase_mat);
			waitKey();
			destroyWindow("rect_phase");

			imshow("circle_DFT", circle_DFT_mat);
			waitKey();
			destroyWindow("circle_DFT");
			imshow("circle_phase", circle_phase_mat);
			waitKey();
			destroyWindow("circle_phase");

			imshow("square_DFT", square_DFT_mat);
			waitKey();
			destroyWindow("square_DFT");
			imshow("square_phase", square_phase_mat);
			waitKey();
			destroyWindow("square_phase");

			imshow("square_rota_DFT", square_rota_DFT_mat);
			waitKey();
			destroyWindow("square_rota_DFT");
			imshow("square_rota_phase", square_rota_phase_mat);
			waitKey();
			destroyWindow("square_rota_phase");

			cout << "rectangle My DFT times : " << time[0] << "  s" << endl;
			cout << "circle My DFT times : " << time[1] << "  s" << endl;
			cout << "square My DFT times : " << time[2] << "  s" << endl;
			cout << "square rotation My DFT times : " << time[3] << "  s" << endl;
			double times = time[0] + time[1] + time[2] + time[3];
			cout << "total My DFT times : " << times << "  s" << endl << endl;
		}
			  break;
		case 2: {

			imshow("rect_opencv_out", rect_opencv_out);
			waitKey();
			destroyWindow("rect_opencv_out");

			imshow("circle_opencv_out", circle_opencv_out);
			waitKey();
			destroyWindow("circle_opencv_out");

			imshow("square_opencv_out", square_opencv_out);
			waitKey();
			destroyWindow("square_opencv_out");

			imshow("square_rota_opencv_out", square_rota_opencv_out);
			waitKey();
			destroyWindow("square_rota_opencv_out");


			imshow("rect_opencv_phase", rect_opencv_phase);
			waitKey();
			destroyWindow("rect_opencv_phase");

			imshow("circle_opencv_phase", circle_opencv_phase);
			waitKey();
			destroyWindow("circle_opencv_phase");

			imshow("square_opencv_phase", square_opencv_phase);
			waitKey();
			destroyWindow("square_opencv_phase");

			imshow("square_rota_opencv_phase", square_rota_opencv_phase);
			waitKey();
			destroyWindow("square_rota_opencv_phase");

			cout << "My retangle DFT with openCV PSNR : " << PSNR(rect_out, rect_opencv_out.data, height, width) << "  db  MSE : " << MSE(rect_out, rect_opencv_out.data, height, width) << endl;
			cout << "My circle DFT with openCV PSNR : " << PSNR(circle_out, circle_opencv_out.data, height, width) << "  db  MSE : " << MSE(circle_out, circle_opencv_out.data, height, width) << endl;
			cout << "My square DFT with openCV PSNR : " << PSNR(square_out, square_opencv_out.data, height, width) << "  db  MSE : " << MSE(square_out, square_opencv_out.data, height, width) << endl;
			cout << "My square rotation DFT with openCV PSNR : " << PSNR(square_rota_out, square_rota_opencv_out.data, height, width) << "  db  MSE : " << MSE(square_rota_out, square_rota_opencv_out.data, height, width) << endl;

			cout << "My retangle DFT phase with openCV PSNR : " << PSNR(rect_phase, rect_opencv_phase.data, height, width) << "  db  MSE : " << MSE(rect_phase, rect_opencv_phase.data, height, width) << endl;
			cout << "My circle DFT phase with openCV PSNR : " << PSNR(circle_phase, circle_opencv_phase.data, height, width) << "  db  MSE : " << MSE(circle_phase, circle_opencv_phase.data, height, width) << endl;
			cout << "My square DFT phase with openCV PSNR : " << PSNR(square_phase, square_opencv_phase.data, height, width) << "  db  MSE : " << MSE(square_phase, square_opencv_phase.data, height, width) << endl;
			cout << "My square rotation DFT phase with openCV PSNR : " << PSNR(square_rota_phase, square_rota_opencv_phase.data, height, width) << "  db  MSE : " << MSE(square_rota_phase, square_rota_opencv_phase.data, height, width) << endl;

			cout << "rectangle opencv DFT times : " << time[4] << "  s" << endl;
			cout << "circle opencv DFT times : " << time[5] << "  s" << endl;
			cout << "square opencv DFT times : " << time[6] << "  s" << endl;
			cout << "square rotation opencv DFT times : " << time[7] << "  s" << endl;
			double times = time[4] + time[5] + time[6] + time[7];
			cout << "total opencv DFT times : " << times << "  s" << endl << endl;
			//Mat rect_opencv_phase;
			//Mat circle_opencv_phase;
			//Mat square_opencv_phase;
			//Mat square_rota_opencv_phase;
		}
			  break;
		case 3: {

			imshow("rect_IDFT", rect_IDFT_mat);
			waitKey();
			destroyWindow("rect_IDFT");

			imshow("circle_IDFT", circle_IDFT_mat);
			waitKey();
			destroyWindow("circle_IDFT");

			imshow("square_IDFT", square_IDFT_mat);
			waitKey();
			destroyWindow("square_IDFT");

			imshow("square_rota_IDFT", square_rota_IDFT_mat);
			waitKey();
			destroyWindow("square_rota_IDFT");

			cout << "rectangle IDFT with retangle PSNR : " << PSNR(rect, rect_IDFT, height, width) << "  db  MSE : " << MSE(rect, rect_IDFT, height, width) << endl;
			cout << "circle IDFT with circle PSNR : " << PSNR(rect, rect_IDFT, height, width) << "  db  MSE : " << MSE(rect, rect_IDFT, height, width) << endl;
			cout << "square IDFT with square PSNR : " << PSNR(rect, rect_IDFT, height, width) << "  db  MSE : " << MSE(rect, rect_IDFT, height, width) << endl;
			cout << "square rotation IDFT with square rotation PSNR : " << PSNR(rect, rect_IDFT, height, width) << "  db  MSE : " << MSE(rect, rect_IDFT, height, width) << endl;

			cout << "rectangle IDFT times : " << time[8] << "  s" << endl;
			cout << "circle IDFT times : " << time[9] << "  s" << endl;
			cout << "square IDFT times : " << time[10] << "  s" << endl;
			cout << "square rotation IDFT times : " << time[11] << "  s" << endl;
			double times = time[8] + time[9] + time[10] + time[11];
			cout << "total IDFT times : " << times << "  s" << endl << endl;
		}
			  break;
		case 4: {
			a = 1;
		}
			  break;
		default:
			break;
		}
	}
	return 0;
	
}
int DFT_phase(unsigned char* phase, double* re, double* im, int h, int w) {
	const int size = h * w;
	double* temp = new double[size];
	double a = 0;
	for (int i = 0; i < size; i++) {
		
		a = atan2(im[i], re[i]);
		temp[i] = a;
	}


	fit_image(phase, temp, h, w);
	return 1;
}
int opencv_phase(unsigned char* src, Mat& ph, int h, int w) {
	//test phase
	Mat img, pad12, complexI12;
	img = Mat(h, w, CV_8UC1, src);
	int Rr = getOptimalDFTSize(img.rows);
	int Cc = getOptimalDFTSize(img.cols);
	copyMakeBorder(img, pad12, 0, Rr - img.rows, 0, Cc - img.cols, BORDER_CONSTANT, Scalar::all(0));
	Mat planesph[] = { Mat_<float>(pad12), Mat::zeros(pad12.size(), CV_32F) };
	merge(planesph, 2, complexI12);
	dft(complexI12, complexI12);
	split(complexI12, planesph);
	phase(planesph[0], planesph[1], ph, false);
	int a = ph.cols / 2;
	int b = ph.rows / 2;
	Mat temp;
	Mat c0(ph, Rect(0, 0, a, b));
	Mat c1(ph, Rect(a, 0, a, b));
	Mat c2(ph, Rect(0, b, a, b));
	Mat c3(ph, Rect(a, b, a, b));
	c0.copyTo(temp);
	c3.copyTo(c0);
	temp.copyTo(c3);
	c1.copyTo(temp);
	c2.copyTo(c1);
	temp.copyTo(c2);
	normalize(ph, ph, 0, 1, NORM_MINMAX);
	ph.convertTo(ph, CV_8UC1, 255, 0);
	//end test
	
	return 1;
}

int opencv_dft(unsigned char* src, Mat& dst, int h, int w) {
	Mat imgi, imgo, pad, complexI;
	imgi = Mat(h, w, CV_8UC1, src);
	int R = getOptimalDFTSize(imgi.rows);
	int C = getOptimalDFTSize(imgi.cols);
	copyMakeBorder(imgi, pad, 0, R - imgi.rows, 0, C - imgi.cols, BORDER_CONSTANT, Scalar::all(0));
	Mat planes[] = { Mat_<float>(pad), Mat::zeros(pad.size(), CV_32F) };
	merge(planes, 2, complexI);
	dft(complexI, complexI);
	split(complexI, planes);
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
	imgo.copyTo(dst);

	return 1;
}
int IDFT(unsigned char* src, unsigned char* dst, int h, int w) {
	const int size = h * w;
	double* re = new double[size];
	double* im = new double[size];
	double* out = new double[size];
	double* idft = new double[size];

	dim3 grid(8, 8);
	dim3 thread(32, 32);
	unsigned char* d_src;
	double* d_re;
	double* d_im;
	double* d_out;
	double* d_idft;
	size_t size1 = size * sizeof(uchar);
	size_t size2 = size * sizeof(double);
	cudaMalloc((void**)&d_src, size1);
	cudaMemcpy(d_src, src, size1, cudaMemcpyHostToDevice);
	cudaMalloc((void**)&d_re, size2);
	cudaMalloc((void**)&d_im, size2);
	cudaMalloc((void**)&d_out, size2);
	cudaMalloc((void**)&d_idft, size2);
	dft_2d_compute << <grid, thread >> > (d_src, d_out, d_re, d_im, h, w);
	idft_2d_compute << <grid, thread >> > (d_out, d_idft, d_re, d_im, h, w);
	cudaMemcpy(idft, d_idft, size2, cudaMemcpyDeviceToHost);
	fit_image(dst, idft, h, w);
	cudaFree(d_src);
	cudaFree(d_re);
	cudaFree(d_im);
	cudaFree(d_out);
	cudaFree(d_idft);
	return 1;

}
int DFT_2D(unsigned char* src, unsigned char* dst, unsigned char* phase, int h, int w) {
	const int size = h * w;
	double* re = new double[size];
	double* im = new double[size];
	double* out = new double[size];

	dim3 grid(8, 8);
	dim3 thread(32, 32);
	unsigned char* d_src;
	double* d_re;
	double* d_im;
	double* d_out;
	size_t size1 = size * sizeof(uchar);
	size_t size2 = size * sizeof(double);
	cudaMalloc((void**)&d_src, size1);
	cudaMemcpy(d_src, src, size1, cudaMemcpyHostToDevice);
	cudaMalloc((void**)&d_re, size2);
	cudaMalloc((void**)&d_im, size2);
	cudaMalloc((void**)&d_out, size2);
	dft_2d_compute << <grid, thread >> > (d_src, d_out, d_re, d_im, h, w);
	cudaMemcpy(re, d_re, size2, cudaMemcpyDeviceToHost);
	cudaMemcpy(im, d_im, size2, cudaMemcpyDeviceToHost);
	cudaMemcpy(out, d_out, size2, cudaMemcpyDeviceToHost);
	fit_image(dst, out, h, w);
	DFT_phase(phase,re,im,h,w);
	cudaFree(d_src);
	cudaFree(d_re);
	cudaFree(d_im);
	cudaFree(d_out);
	return 1;
}

__global__ void idft_2d_compute(double* src, double* out, double* re, double* im, int h, int w) {

	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;

	double real = 0.0, image = 0.0;
	double constant;

	for (int v = 0; v < h; v++) {
		for (int u = 0; u < w; u++) {
			constant = -2 * M_PI * ((u * x * 1.0 / w) + (v * y * 1.0 / h));
			real = real + re[u + v * w] * cos(constant) + im[u + v * w] * sin(constant);
			image = image + im[u + v * w] * cos(constant) - re[u + v * w] * sin(constant);
		}
	}
	double temp = (sqrt(real * real + image * image)) / (sqrt(h * w * 1.0));
	//填入
	out[x + y * w] = (temp);
}
__global__ void dft_2d_compute(unsigned char* src, double* out, double* re, double* im, int h, int w) {
	int u = threadIdx.x + blockIdx.x * blockDim.x;
	int v = threadIdx.y + blockIdx.y * blockDim.y;
	double real = 0.0, image = 0.0;
	double constant;

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			constant = 2 * M_PI * ((u * x * 1.0 / w) + (v * y * 1.0 / h));
			real = real + powf(-1, x + y) * src[x + y * w] * cos(constant);
			image = image - powf(-1, x + y) * src[x + y * w] * sin(constant);
		}
	}
	//填入
	out[u + v * w] = log((sqrt(real * real + image * image)) + 1);
	re[u + v * w] = real;
	im[u + v * w] = image;
}
int fit_image(unsigned char* dst, double* out, int h, int w) {
	int size = h * w;
	double max = -100000, min = 10000000;
	for (int i = 0; i < size; i++) {
		if (out[i] > max)
			max = out[i];
		else if (out[i] < min)
			min = out[i];
	}
	for (int i = 0; i < size; i++) {
		out[i] = (out[i] - min) * 255 / (max - min) + 0.5;
	}
	for (int i = 0; i < size; i++) {
		dst[i] = (int)out[i];
	}
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