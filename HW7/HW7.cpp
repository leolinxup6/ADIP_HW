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
Mat process(unsigned char* src, float* filter, int h, int w);
Mat DFT(unsigned char* src, int h, int w);
int processing(Mat& complexI, float* filter, int h, int w);
Mat updateMag(Mat complex);
int IDFT(Mat complexI, Mat& dst, int h, int w);
int reject_gau(float* filter, float d0, float W, int h, int w);
int D_filter_uv(float* filter, int h, int w);
int D1_filter_uv(float* filter, float u0, float v0, int h, int w);
int D2_filter_uv(float* filter, float u0, float v0, int h, int w);
int notching_gau(float* filter, float shift_u, float shift_v, float d0, int h, int w);

Mat process_inv(unsigned char* src, float* filter1, float* filter2, int h, int w);
int invfilter(float* filter1, float* filter2, int h, int w);
int processing_inv(Mat& complexI, float* filter1, float* filter2, int h, int w);
int wiefilter(float* filter_re, float* filter_im, float K, int h, int w);
int main() {
	//LARGE_INTEGER m_nFreq;
	//LARGE_INTEGER m_nBeginTime;
	//LARGE_INTEGER nEndTime;
	//float time[15];
	//QueryPerformanceFrequency(&m_nFreq); // 獲取時鍾周期
	//QueryPerformanceCounter(&m_nBeginTime); // 計時開始
	//QueryPerformanceCounter(&nEndTime);// 計時截止
	//time[x] = (float)(nEndTime.QuadPart - m_nBeginTime.QuadPart) / m_nFreq.QuadPart;
	char kirby_filname[50] = "kirby512.raw";
	char flower_filname[50] = "motion_flower.raw";
	const int height = 512, width = 512, size = height * width;
	unsigned char* kirby = new unsigned char[size];
	unsigned char* flower = new unsigned char[size];
	openRaw(kirby_filname, kirby, height, width);
	openRaw(flower_filname, flower, height, width);



	Mat kirby_mat = Mat(height, width, CV_8UC1, kirby);
	imwrite("kirby.png", kirby_mat);

	Mat flower_mat = Mat(height, width, CV_8UC1, flower);
	imwrite("flower.png", flower_mat);



	int a = 0, b = 0;
	while (a == 0) {
		cout << "Enter the question number : " << endl << "1. Periodic Noise" << endl << "2. Debluring" << endl << "3. Exit" << endl;
		cin >> b;
		switch (b) {
		case 1: {
			cout << "Enter the question number : " << endl << "1. Notch filter" << endl << "2. Band-reject filter" << endl;
			cin >> b;

			switch (b) {
			case 1: {
				float shift_u, shift_v, d0;
				float* test = new float[size];
				shift_u = 81;
				shift_v = 16;
				d0 = 50;
				notching_gau(test, shift_u, shift_v, d0, height, width);
				Mat notch_mat = process(kirby, test, height, width);

				imwrite("notch_gau.png", notch_mat);
				imshow("notch_gau", notch_mat);
				waitKey();
				destroyWindow("notch_gau");
			}
				  break;
			case 2: {
				float W;
				float d0;
				d0 = 83;
				W = 20;
				cout << "d0 : ";
				cin >> d0;
				cout << "W : ";
				cin >> W;
				float* reject_filter = new float[size];
				reject_gau(reject_filter, d0, W, height, width);
				Mat reject_mat = process(kirby, reject_filter, height, width);

				imwrite("reject_gau.png", reject_mat);
				imshow("reject_gau", reject_mat);
				waitKey();
				destroyWindow("reject_gau");
			}
				  break;
			default:
				break;
			}

			
		}
			  break;
		case 2: {
			cout << "Enter the question number : " << endl << "1. Inverse filter" << endl << "2. Wiener filter" << endl;
			cin >> b;

			switch (b) {
			case 1: {
				float* inverse_re = new float[size];
				float* inverse_im = new float[size];
				invfilter(inverse_re, inverse_im, height, width);
				Mat inverse_mat = process_inv(flower, inverse_re, inverse_im, height, width);

				imwrite("inverse.png", inverse_mat);
				imshow("inverse", inverse_mat);
				waitKey();
				destroyWindow("inverse");
			}
				  break;
			case 2: {
				float* wiener_re = new float[size];
				float* wiener_im = new float[size];
				float K;
				K = 0.01;
				cout << "K = ";
				cin >> K;
				wiefilter(wiener_re, wiener_im, K, height, width);
				Mat wiener_mat = process_inv(flower, wiener_re, wiener_im, height, width);

				imwrite("wiener.png", wiener_mat);
				imshow("wiener", wiener_mat);
				waitKey();
				destroyWindow("wiener");
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
int wiefilter(float* filter_re, float* filter_im,float K, int h, int w) {
	int const size = h * w;
	double tmp, temp;
	double re, im;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (i == 0) {
				filter_re[i * w + j] = 1;
				filter_im[i * w + j] = 1;
			}
			else {
				tmp = M_PI * -0.01 * (i);
				re = (double)((1.0 / tmp) * sin(tmp) * cos(tmp));
				im = (double)((1.0 / tmp) * sin(tmp) * -sin(tmp));
				temp = re * re + im * im;
				filter_re[i * w + j] = (double)(re / (temp + K));
				filter_im[i * w + j] = (double)(-im / (temp + K));
			}
		}
	}
	for (int i = 0; i < h / 2; i++) {
		for (int j = 0; j < w / 2; j++) {
			tmp = filter_re[i * w + j];
			filter_re[i * w + j] = filter_re[(h * w / 2 + w / 2) + (i * w + j)];
			filter_re[(h * w / 2 + w / 2) + (i * w + j)] = tmp;
			tmp = filter_re[(w / 2) + (i * w + j)];
			filter_re[(w / 2) + i * w + j] = filter_re[(h * w / 2) + (i * w + j)];
			filter_re[(h * w / 2) + (i * w + j)] = tmp;
			tmp = filter_im[i * w + j];
			filter_im[i * w + j] = filter_im[(h * w / 2 + w / 2) + (i * w + j)];
			filter_im[(h * w / 2 + w / 2) + (i * w + j)] = tmp;
			tmp = filter_im[(w / 2) + (i * w + j)];
			filter_im[(w / 2) + i * w + j] = filter_im[(h * w / 2) + (i * w + j)];
			filter_im[(h * w / 2) + (i * w + j)] = tmp;
		}
	}
	Mat filtermat = Mat(h, w, CV_32FC1, filter_re);
	normalize(filtermat, filtermat, 0, 1, NORM_MINMAX);
	//filtermat.convertTo(filtermat, CV_8UC1, 255, 0);

	Mat filter2mat = Mat(h, w, CV_32FC1, filter_im);
	normalize(filter2mat, filter2mat, 0, 1, NORM_MINMAX);
	//filter2mat.convertTo(filter2mat, CV_8UC1, 255, 0);
	return 1;
}
int invfilter(float* filter1, float*filter2, int h, int w) {
	int const size = h * w;
	double tmp,temp;
	double re, im;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (i == 0) {
				filter1[i * w + j] = 1;
				filter2[i * w + j] = 1;
			}
			else {
				tmp = M_PI * -0.01 * (i);
				re = (double)((1.0 / tmp) * sin(tmp) * cos(tmp));
				im = (double)((1.0 / tmp) * sin(tmp) * -sin(tmp));
				temp = (double)(re * re + im * im);
				filter1[i * w + j] = (double)(re / temp);
				filter2[i * w + j] = (double)(-im / temp);
			}
		}
	}

	for (int i = 0; i < h / 2; i++) {
		for (int j = 0; j < w / 2; j++) {
			tmp = filter1[i * w + j];
			filter1[i * w + j] = filter1[(h * w / 2 + w / 2) + (i * w + j)];
			filter1[(h * w / 2 + w / 2) + (i * w + j)] = tmp;
			tmp = filter1[(w / 2) + (i * w + j)];
			filter1[(w / 2) + i * w + j] = filter1[(h * w / 2) + (i * w + j)];
			filter1[(h * w / 2) + (i * w + j)] = tmp;
			tmp = filter2[i * w + j];
			filter2[i * w + j] = filter2[(h * w / 2 + w / 2) + (i * w + j)];
			filter2[(h * w / 2 + w / 2) + (i * w + j)] = tmp;
			tmp = filter2[(w / 2) + (i * w + j)];
			filter2[(w / 2) + i * w + j] = filter2[(h * w / 2) + (i * w + j)];
			filter2[(h * w / 2) + (i * w + j)] = tmp;
		}
	}
	Mat filtermat = Mat(h, w, CV_32FC1, filter1);
	normalize(filtermat, filtermat, 0, 1, NORM_MINMAX);
	//filtermat.convertTo(filtermat, CV_8UC1, 255, 0);

	Mat filter2mat = Mat(h, w, CV_32FC1, filter2);
	normalize(filter2mat, filter2mat, 0, 1, NORM_MINMAX);
	//filter2mat.convertTo(filter2mat, CV_8UC1, 255, 0);
	return 1;
}
int processing_inv(Mat& complexI, float* filter1, float* filter2, int h, int w) {
	int const size = h * w;
	Mat planes[] = {
		Mat::zeros(complexI.size(),CV_32F),
		Mat::zeros(complexI.size(),CV_32F)
	};
	split(complexI, planes);
	float* re_freq = new float[size];
	float* im_freq = new float[size];
	re_freq = (float*)planes[0].data;
	im_freq = (float*)planes[1].data;
	float* re_filt = new float[size];
	float* im_filt = new float[size];
	for (int i = 0; i < size; i++) {
		re_filt[i] = (float)(1.0 * re_freq[i] * filter1[i] - 1.0 * im_freq[i] * filter2[i]);
		im_filt[i] = (float)(1.0 * re_freq[i] * filter2[i] + 1.0 * im_freq[i] * filter1[i]);
	}

	planes[0] = Mat(h, w, CV_32F, re_filt);
	planes[1] = Mat(h, w, CV_32F, im_filt);
	merge(planes, 2, complexI);
	return 1;
}
Mat process_inv(unsigned char* src, float* filter1,float* filter2, int h, int w) {
	Mat complexI = DFT(src, h, w);
	Mat MagI = updateMag(complexI);
	processing_inv(complexI, filter1, filter2, h, w);
	Mat MagO = updateMag(complexI);
	Mat out;
	IDFT(complexI, out, h, w);
	return out;
}
Mat process(unsigned char* src, float* filter, int h, int w) {
	int const size = h * w;
	//float* re_freq = new float[size];
	//float* im_freq = new float[size];
	Mat complexI = DFT(src, h, w);
	Mat MagI = updateMag(complexI);
	processing(complexI, filter, h, w);
	Mat MagO = updateMag(complexI);
	Mat out;
	IDFT(complexI, out, h, w);
	return out;
}
int reject_gau(float* filter, float d0, float W, int h, int w) {
	int const size = h * w;
	float* d = new float[size];
	D_filter_uv(d, h, w);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			float tmp;
			tmp = (pow(d[i * w + j], 2) - pow(d0, 2)) / (d[i * w + j] * W);
			filter[i * w + j] = (float)1 - exp(-0.5 * pow((tmp), 2));
		}
	}
	float tmp;
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
	//Mat filtermat = Mat(h, w, CV_32FC1, filter);
	//normalize(filtermat, filtermat, 0, 1, NORM_MINMAX);
	//filtermat.convertTo(filtermat, CV_8UC1, 255, 0);
	return 1;
}
int notching_gau(float* filter,float shift_u,float shift_v,float d0, int h, int w) {
	int const size = h * w;
	float* d1 = new float[size];
	float* d2 = new float[size];
	float u0 = (w / 2) + shift_u;
	float v0 = (h / 2) + shift_v;
	D1_filter_uv(d1, u0, v0, h, w);
	u0 = (w / 2) - shift_u;
	v0 = (h / 2) - shift_v;
	D2_filter_uv(d2, u0, v0, h, w);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			float tmp;
			tmp = (d1[i * w + j]*d2[i * w + j]/pow(d0,2));
			filter[i * w + j] = (float)1 - exp(-0.5 * (tmp));
		}
	}
	float tmp;
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
	return 1;
}
int D_filter_uv(float* filter, int h, int w) {
	float* temp = new float[h * w];
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			filter[i * w + j] = sqrt(pow((j - (w * 1.0) / 2), 2) + pow((i - (h * 1.0) / 2), 2));
		}
	}

	return 1;
}
int D1_filter_uv(float* filter,float u0,float v0, int h, int w) {
	float* temp = new float[h * w];
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			filter[i * w + j] = sqrt(pow((j - u0), 2) + pow((i - v0), 2));
		}
	}

	return 1;
}
int D2_filter_uv(float* filter, float u0, float v0, int h, int w) {
	float* temp = new float[h * w];
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			filter[i * w + j] = sqrt(pow((j -u0), 2) + pow((i - v0), 2));
		}
	}

	return 1;
}
Mat updateMag(Mat complex) {
	Mat magI;
	Mat planes[] = {
		Mat::zeros(complex.size(), CV_32F),
		Mat::zeros(complex.size(), CV_32F)
	};
	split(complex, planes);
	magnitude(planes[0], planes[1], magI);
	magI += Scalar::all(1);
	log(magI, magI);
	normalize(magI, magI, 1, 0, NORM_INF);
	int x = magI.cols / 2;
	int y = magI.rows / 2;
	Mat tmp;
	Mat q0(magI, Rect(0, 0, x, y));
	Mat q1(magI, Rect(x, 0, x, y));
	Mat q2(magI, Rect(0, y, x, y));
	Mat q3(magI, Rect(x, y, x, y));
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);
	normalize(magI, magI, 0, 1, NORM_MINMAX);
	magI.convertTo(magI, CV_8UC1, 255, 0);
	return magI;
}
int processing(Mat& complexI,float* filter,int h,int w) {
	int const size = h * w;
	Mat planes[] = {
		Mat::zeros(complexI.size(),CV_32F),
		Mat::zeros(complexI.size(),CV_32F)
	};
	split(complexI, planes);
	float* re_freq = new float[size];
	float* im_freq = new float[size];
	re_freq = (float*)planes[0].data;
	im_freq = (float*)planes[1].data;
	float* re_filt = new float[size];
	float* im_filt = new float[size];
	for (int i = 0; i < size; i++) {
		re_filt[i] = (float)1.0 * re_freq[i] * filter[i];
		im_filt[i] = (float)1.0 * im_freq[i] * filter[i];

	}
	planes[0] = Mat(h, w, CV_32F, re_filt);
	planes[1] = Mat(h, w, CV_32F, im_filt);
	merge(planes, 2, complexI);
	return 1;
}
Mat DFT(unsigned char* src, int h, int w) {
	int const size = h * w;
	Mat imgi = Mat(h, w, CV_8UC1, src);
	int R = getOptimalDFTSize(imgi.rows);
	int C = getOptimalDFTSize(imgi.cols);
	Mat pad;
	copyMakeBorder(imgi, pad, 0, R - imgi.rows, 0, C - imgi.cols, BORDER_CONSTANT, Scalar::all(0));
	Mat planes[] = { 
		Mat_<float>(pad),
		Mat::zeros(pad.size(),CV_32F) 
	};
	Mat complexI;
	merge(planes, 2, complexI);
	dft(complexI, complexI);
	split(complexI, planes);

	return complexI;
}
int IDFT(Mat complexI, Mat& dst, int h, int w) {
	Mat invDFT;
	idft(complexI, invDFT/*, DFT_SCALE | DFT_REAL_OUTPUT*/);
	Mat planes[] = {
	Mat::zeros(complexI.size(), CV_32F),
	Mat::zeros(complexI.size(), CV_32F)
	};
	split(invDFT, planes);
	magnitude(planes[0], planes[1], invDFT);
	normalize(invDFT, invDFT, 0, 1, NORM_MINMAX);
	invDFT.convertTo(invDFT, CV_8UC1, 255, 0);
	invDFT.copyTo(dst);
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