#include "CImg.h"
#include "canny_class.h"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cimg_library;

float pi = 3.14159265358979323846;

CImg<float> hough(CImg<float> img)
{
	const int angle_num = 180;
	float ang = pi / 180;
	float SIN[angle_num], COS[angle_num];
	int width, height;
	width = img._width; height = img._height;

	for (int i = 0, float t = 0; i < angle_num; i++, t += ang)
	{
		SIN[i] = sin(t);
		COS[i] = cos(t);
	}

	const int n = round(sqrt(pow(width, 2) + pow(height, 2)));
	int A[n];

	cimg_forXY(img, x, y)
	{
		if (img(x, y) != 0)
		{
			for (int i = 0; i < angle_num; i++)
			{

			}
		}
	}
}

int main()
{
	string filename[] = { "dataset (1).bmp", "dataset (2).bmp", "dataset (3).bmp", "dataset (4).bmp", "dataset (5).bmp", "dataset (6).bmp" };
	for (int ni = 0; ni < 6; ni++)
	{
		canny_img img("Dataset\\" + filename[ni], 6.0f, 3.5f);
		CImg<float> c_img=img.CannyDiscrete();

	}
}