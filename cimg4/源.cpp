#include "CImg.h"
#include "canny_class.h"
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;
using namespace cimg_library;

double pi = 3.14159265358979323846;

CImg<int> hough(CImg<float> img, CImg<float> src, double val)
{
	//������ֵ����
	const int angle_num = 180;
	int range_thre = 5;
	double fun_thre = 0.8;
	int point_thre = 3;
	double ang = pi / angle_num;
	int width = img._width, height = img._height;
	int X = width / 2, Y = height / 2;

	const int d = int(round(sqrt(pow(width, 2) + pow(height, 2))));

	//��ʼ��hough�ռ�洢����
	vector<vector<int>> s;
	s.resize(angle_num);
	for (int i = 0; i < angle_num; i++)
	{
		s[i].resize(2 * d);
	}

	//����hough�ռ����
	int max = 0;
	cimg_forXY(img, x, y)
	{
		if (img(x, y) != 0)
		{
			for (int i = 0; i < angle_num; i++)
			{
				int p = int(round((x - X)*cos(i*ang) + (y - Y)*sin(i*ang)));
				p += d;
				if (p < 0 || (p >= 2 * d))
					continue;
				else
				{
					s[i][p]++;
					if (s[i][p] > max)
						max = s[i][p];
				}
			}
		}
	}

	//Ѱ��hough�ռ����ߵĽ��㣬���佻�����������ֵ������һ�������ڵ����ֵ�������ܵ���դ����Ӱ��������Ƶ�ֵ
	vector<pair<int, int>> lines;
	int thre = int(max*val);
	for (int i = 0; i < angle_num; i++)
	{
		for (int j = 0; j < 2 * d; j++)
		{
			bool flag = 1;
			if (s[i][j] > thre)
			{
				for (int k = 0; k < int(lines.size()); k++)
				{
					if ((abs(lines[k].first - i) < range_thre || abs((angle_num - lines[k].first) + i) < range_thre) && abs(lines[k].second - j) < range_thre)
					{
						if (s[i][j] > s[lines[k].first][lines[k].second])
						{
							lines[k].first = i;
							lines[k].second = j;
						}
						flag = 0;
					}
				}
				if (flag)
				{
					lines.push_back(make_pair(i, j));
				}
			}
		}
	}

	//�ɽ���õ�ԭ�ռ�����Է���
	vector<vector<double>> fun;
	cout << "lines : " << endl;
	for (int i = 0; i < int(lines.size()); i++)
	{
		int row = lines[i].first;
		int col = lines[i].second;
		double dy = sin(row*ang), dx = cos(row*ang);
		cout << dx << "x + " << dy << "y = " << col - d + Y*dy + X*dx << endl;
		vector<double> t;
		t.push_back(dx);
		t.push_back(dy);
		t.push_back(col - d + Y*dy + X*dx);
		fun.push_back(t);
	}

	//���㽻�㣬����һ����������׼ȷ�������ظ�
	CImg<int> dst(src);
	unsigned char red[] = { 255,0,0 };
	unsigned char green[] = { 0,255,0 };
	vector<vector<double>> ver;
	cimg_forXY(dst, x, y)
	{
		int sum = 0;
		double error = 0;
		for (int i = 0; i<int(fun.size()); i++)
		{
			double re = x*fun[i][0] + y*fun[i][1];
			if (re<fun[i][2] + fun_thre&&re>fun[i][2] - fun_thre)
			{
				sum++;
				error += abs(fun[i][2] - re);
				dst(x, y, 0) = 0; dst(x, y, 1) = 255; dst(x, y, 0) = 0;
			}
		}
		if (sum >= 2)
		{		
			bool flag = 1;
			for (int i = 0; i<int(ver.size()); i++)
			{
				if (abs(x - ver[i][0]) < point_thre&&abs(y - ver[i][1]) < point_thre)
				{
					if (error < ver[i][2])
					{
						ver[i][0] = x;
						ver[i][1] = y;
						ver[i][2] = error;
					}
					flag = 0;
				}
			}
			if (flag)
			{
				vector<double> t;
				t.push_back(x);
				t.push_back(y);
				t.push_back(error);
				ver.push_back(t);
			}
		}
	}

	//����
	cout << "vertex : " << endl;
	for (int i = 0; i<int(ver.size()); i++)
	{
		cout << "( " << int(ver[i][0]) << " , " << int(ver[i][1]) << " )" << endl;
		dst.draw_circle(int(ver[i][0]), int(ver[i][1]), 6, red, 1);
	}
	cout << endl;
	return dst;
}


int main()
{
	//���ò���
	string filename[] = { "dataset (1).bmp", "dataset (2).bmp", "dataset (3).bmp", "dataset (4).bmp", "dataset (5).bmp", "dataset (6).bmp" };
	double thre_val[] = { 0.5,0.6,0.6,0.5,0.5,0.5 };
	float sigma[] = { 6.0f, 6.0f, 9.0f, 6.0f, 6.0f, 6.0f };
	float threshold[] = { 3.5f, 3.5f, 5.0f, 3.5f, 3.5f, 3.5f };
	
	//�Ƚ���canny�����ٽ���hough�任��������result�ļ�����
	for (int ni = 0; ni < 6; ni++)
	{
		cout << endl << filename[ni] << " : " << endl;
		canny_img img("Dataset\\" + filename[ni], sigma[ni], threshold[ni]);
		CImg<float> c_img=img.CannyDiscrete();
		CImg<float> src(("Dataset\\" + filename[ni]).c_str());
		hough(c_img, src, thre_val[ni]).save(("result\\" + filename[ni]).c_str());
	}
}