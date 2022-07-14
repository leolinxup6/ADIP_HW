#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <ctime>
#include <stack>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

void openRaw(const char filename[], unsigned char* tmp, int h, int w);
void block(unsigned char* input, unsigned char* src, int h, int w, int label);
int detect(unsigned char* src, unsigned char* dst, int h, int w);
void morph(unsigned char* src, unsigned char* dst, int h, int w);
void gray_erode(unsigned char* src, unsigned char* dst, int h, int w);
void gray_dilate(unsigned char* src, unsigned char* dst, int h, int w);
int main() {
	char filename1[] = "adip.raw";//512*512
	char filename2[] = "adip_noise.raw";//512*512
	char filename3[] = "gra_493x277.raw";//493*277
	char filename4[] = "walkbridge.raw";//512*512
	const int height = 512, width = 512, size = height * width;
	const int h = 277, w = 493, s = h * w;
	unsigned char* adip = new unsigned char[size];
	unsigned char* adip_noise = new unsigned char[size];
	unsigned char* gra = new unsigned char[s];
	unsigned char* walkbridge = new unsigned char[size];
	openRaw(filename1, adip, height, width);
	openRaw(filename2, adip_noise, height, width);
	openRaw(filename3, gra, h, w);
	openRaw(filename4, walkbridge, height, width);
	Mat adip_mat = Mat(height, width, CV_8UC1, adip);
	Mat adip_noise_mat = Mat(height, width, CV_8UC1, adip_noise);
	Mat gra_mat = Mat(h, w, CV_8UC1, gra);
	Mat walkbridge_mat = Mat(height, width, CV_8UC1, walkbridge);

    imwrite("adip.png", adip_mat);
    imwrite("adip_noise.png", adip_noise_mat);
    imwrite("gra.png", gra_mat);
    imwrite("walkbridge.png", walkbridge_mat);

    unsigned char* adip_bi = new unsigned char[size];
    for (int i = 0; i < size; i++) {
        if (adip[i] > 128)
            adip_bi[i] = 255;
        else
            adip_bi[i] = 0;
    }
    unsigned char* adip_label = new unsigned char[size];
    Mat label_mat = Mat(height, width, CV_8UC1, adip_label);
    int labelvalue = detect(adip_bi, adip_label, height, width);

    unsigned char* adip_out1 = new unsigned char[size];
    Mat out1_mat = Mat(height, width, CV_8UC1, adip_out1);
    memcpy(adip_out1, adip, sizeof(uchar) * height * width );
    block(adip_out1, adip_label, height, width, labelvalue);

    unsigned char* adip_out2 = new unsigned char[size]{};
    
    Mat out2_mat = Mat(height, width, CV_8UC1, adip_out2);
    for (int i = 0; i < size; i++) {
        if (adip_label[i] != 0 && adip_label[i] != 5 && adip_label[i] != 6 && adip_label[i] != 7)
            adip_out2[i] = 255;
    }

    unsigned char* adip_out4 = new unsigned char[size] {};
    Mat out4_mat = Mat(height, width, CV_8UC1, adip_out4);
    morph(adip_noise, adip_out4, height, width);

    unsigned char* out_erode = new unsigned char[size];
    Mat out_erode_mat = Mat(height, width, CV_8UC1, out_erode);
    gray_erode(walkbridge, out_erode, height, width);

    unsigned char* out_dilate = new unsigned char[size];
    Mat out_dilate_mat = Mat(height, width, CV_8UC1, out_dilate);
    gray_dilate(walkbridge, out_dilate, height, width);

    imwrite("adip_out1.png", out1_mat);
    imwrite("adip_out2.png", out2_mat);
    imwrite("adip_out4.png", out4_mat);
    imwrite("out_erode.png", out_erode_mat);
    imwrite("out_dilate.png", out_dilate_mat);
	return 1;
}
void gray_erode(unsigned char* src, unsigned char* dst, int h, int w) {
    unsigned char* padding = new unsigned char[(h + 2) * (w + 2)];
    unsigned char* pad_out = new unsigned char[(h + 2) * (w + 2)];
    //Mat out = Mat(h, w, CV_8UC1, dst);
    //Mat padding_mat = Mat(h + 2, w + 2, CV_8UC1, padding);
    //Mat pad_out_mat = Mat(h + 2, w + 2, CV_8UC1, pad_out);
    memset(padding, 0, sizeof(uchar) * (h + 2) * (w + 2));
    for (int i = 1; i < w + 1; i++) {
        for (int j = 1; j < h + 1; j++) {
            padding[i + j * (w + 2)] = src[(i - 1) + (j - 1) * w];
        }
    }
    //memcpy(pad_out, padding, sizeof(uchar) * (h + 2) * (w + 2));
    int k, l;
    int a = 0;
    int step[9][2] = { {-1,-1},{ 0,-1},{ 1,-1},
                       {-1, 0},{ 0, 0},{ 1, 0},
                       {-1, 1},{ 0, 1},{ 1, 1} };
    for (int i = 1; i < w + 1; i++) {
        for (int j = 1; j < h + 1; j++) {
            a = 0;
            int max = padding[i + j * (w + 2)];
            for (int z = 0; z < 9; z++) {
                k = i + step[z][0];
                l = j + step[z][1];
                if (padding[k + l * (w + 2)] > max)
                    max = padding[k + l * (w + 2)];
            }
            pad_out[i + j * (w + 2)] = max;
        }
    }
    for (int i = 1; i < w + 1; i++) {
        for (int j = 1; j < h + 1; j++) {
            dst[i - 1 + (j - 1) * (w)] = pad_out[i + j * (w + 2)];
        }
    }
    return;
}
void gray_dilate(unsigned char* src, unsigned char* dst, int h, int w) {

    unsigned char* padding = new unsigned char[(h + 2) * (w + 2)];
    unsigned char* pad_out = new unsigned char[(h + 2) * (w + 2)];
    Mat out = Mat(h, w, CV_8UC1, dst);
    Mat padding_mat = Mat(h + 2, w + 2, CV_8UC1, padding);
    Mat pad_out_mat = Mat(h + 2, w + 2, CV_8UC1, pad_out);
    memset(padding, 255, sizeof(uchar) * (h + 2) * (w + 2));
    for (int i = 1; i < w + 1; i++) {
        for (int j = 1; j < h + 1; j++) {
            padding[i + j * (w + 2)] = src[(i - 1) + (j - 1) * w];
        }
    }
    int k, l;
    int a = 0;
    int step[9][2] = { {-1,-1},{ 0,-1},{ 1,-1},
                       {-1, 0},{ 0, 0},{ 1, 0},
                       {-1, 1},{ 0, 1},{ 1, 1} };
    for (int i = 1; i < w + 1; i++) {
        for (int j = 1; j < h + 1; j++) {
            a = 0;
            int min = padding[i + j * (w + 2)];
            for (int z = 0; z < 9; z++) {
                k = i + step[z][0];
                l = j + step[z][1];
                if (padding[k + l * (w + 2)] < min)
                    min = padding[k + l * (w + 2)];
            }
            pad_out[i + j * (w + 2)] = min;
        }
    }
    for (int i = 1; i < w + 1; i++) {
        for (int j = 1; j < h + 1; j++) {
            dst[i - 1 + (j - 1) * (w)] = pad_out[i + j * (w + 2)];
        }
    }
    return;
}
void morph(unsigned char* src, unsigned char* dst, int h, int w) {
    unsigned char* padding = new unsigned char[(w) * (h)]{};
    unsigned char* pad1 = new unsigned char[(h + 2) * (w + 2)]{};
    unsigned char* pad2 = new unsigned char[(h + 2) * (w + 2)]{};
    memset(padding, 255, sizeof(uchar)*(w + 2) * (h + 2));
    //Mat paddingmat = Mat(h + 2, w + 2, CV_8UC1, padding);
    //Mat pad1mat = Mat(h + 2, w + 2, CV_8UC1, pad1);
    //Mat pad2mat = Mat(h + 2, w + 2, CV_8UC1, pad2);
    //Mat dst_mat = Mat(h, w, CV_8UC1, dst);
    for (int i = 1; i < w + 1; i++) {
        for (int j = 1; j < h + 1; j++) {
            padding[i + j * (w + 2)] = src[(i - 1) + (j - 1) * w];
        }
    }
    memcpy(pad1, padding, sizeof(uchar) * (h + 1) * (w + 2));
    memcpy(pad2, padding, sizeof(uchar) * (h + 1) * (w + 2));
    int k, l;
    int a = 0;
    int step[9][2] = { {-1,-1},{0,-1},{1,-1},{-1,0},{0,0} , {1, 0}, { -1,1 }, { 0,1 }, { 1,1 } };


    for (int x = 0; x < 1; x++) {
        for (int i = 1; i < w + 1; i++) {
            for (int j = 1; j < h + 1; j++) {
                a = 0;
                for (int z = 0; z < 9; z++) {
                    k = i + step[z][0];
                    l = j + step[z][1];
                    if (pad1[k + l * (w + 2)] > 0)
                        a = 1;
                }
                if (a == 1)
                    pad2[i + j * (w + 2)] = 255;
                else
                    pad2[i + j * (w + 2)] = 0;
            }
        }
        memcpy(pad1, pad2, sizeof(uchar) * (h + 2) * (w + 2));
    }
    for (int x = 0; x < 1; x++) {
        for (int i = 1; i < w + 1; i++) {
            for (int j = 1; j < h + 1; j++) {
                a = 0;
                for (int z = 0; z < 9; z++) {
                    k = i + step[z][0];
                    l = j + step[z][1];
                    if (pad1[k + l * (w + 2)] == 0)
                        a = 1;
                }
                if (a == 1)
                    pad2[i + j * (w + 2)] = 0;
                else
                    pad2[i + j * (w + 2)] = 255;
            }
        }
        memcpy(pad1, pad2, sizeof(uchar) * (h + 2) * (w + 2));
    }
    for (int i = 1; i < w + 1; i++) {
        for (int j = 1; j < h + 1; j++) {
            dst[i - 1 + (j - 1) * (w)] = pad1[i + j * (w + 2)];
        }
    }
    for (int i = 0; i < w; i++) {
        dst[i + w * (h - 1)] = 255;
    }
    return;
}
void block(unsigned char* dst, unsigned char* src, int h, int w, int label) {
    int maxX = 0, maxY = 0, minX = 512, minY = 512;
    for (int a = 1; a < label + 1; a++) {
        maxX = 0;
        maxY = 0;
        minX = 512;
        minY = 512;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if (src[i * w + j] == a) {
                    if (j > maxX)
                        maxX = j;
                    else if (j < minX)
                        minX = j;
                    if (i > maxY)
                        maxY = i;
                    else if (i < minY)
                        minY = i;
                }
            }
        }
        for (int i = minX; i < maxX; i++) {
            dst[i + minY * w] = 255;
        }
        for (int i = minX; i < maxX + 1; i++) {
            dst[i + maxY * w] = 255;
        }
        for (int i = minY; i < maxY; i++) {
            dst[minX + i * w] = 255;
        }
        for (int i = minY; i < maxY; i++) {
            dst[maxX + i * w] = 255;
        }
    }
}
int detect(unsigned char* src, unsigned char* dst, int h, int w) {
    int labelValue = 0;
    int seed, neighbor;
    stack<int> intStack;    
    int area = 0;
    if (!intStack.empty()) intStack.pop(); 
    memcpy(dst, src, sizeof(uchar) * h * w);
    Mat blurshrinklenaMat = Mat(w, h, CV_8UC1, dst);
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (dst[i * w + j] == 255)
            {
                area = 0;
                labelValue++;
                int k = i;
                int l = j;
                seed = i * w + j;
                dst[i * w + j] = labelValue;
                intStack.push(i);
                intStack.push(j);
                while (!intStack.empty())
                {
                    neighbor = (i + 1) * w + j;
                    if ((((i + 1) < h) && j < w) && (dst[neighbor] == 255))
                    {
                        dst[neighbor] = labelValue;
                        intStack.push(i + 1);
                        intStack.push(j);
                        area++;
                    }
                    neighbor = (i - 1) * w + j;
                    if (((i != 0) && j < w) && (dst[neighbor] == 255))
                    {
                        dst[neighbor] = labelValue;
                        intStack.push(i - 1);
                        intStack.push(j);
                        area++;
                    }
                    neighbor = i * w + j + 1;
                    if (((i < h) && (j + 1 < w)) && (dst[neighbor] == 255))
                    {
                        dst[neighbor] = labelValue;
                        intStack.push(i);
                        intStack.push(j + 1);
                        area++;
                    }
                    neighbor = i * w + j - 1;
                    if ((((i < h) && (j != 0)) && dst[neighbor] == 255))
                    {
                        dst[neighbor] = labelValue;
                        intStack.push(i);
                        intStack.push(j - 1);
                        area++;
                    }
                    j = intStack.top();
                    intStack.pop();
                    i = intStack.top();
                    intStack.pop();
                }

            }

        }
    }
    return labelValue;
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