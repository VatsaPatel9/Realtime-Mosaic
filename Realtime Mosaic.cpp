#include <iostream>
#include<stdio.h>
#include <sstream>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>


using namespace std;
using namespace cv;
Mat frame; //frame image matrix 
int blur_degree, blue, green ,red, ct = 0, i , j;
Point start; // Rectangle start point
Point endPoint; // Rectangle end point 
int video[100][4]; // 100 frames 

void blurFrame(int blur_degree, int x, int y) {
	for (int u = 0; u < blur_degree; u++)
	{
		for (int v = 0; v < blur_degree; v++)
		{
			frame.ptr<uchar>(y + v)[(x + u) * 3 + 0] = blue / (blur_degree * blur_degree);
			frame.ptr<uchar>(y + v)[(x + u) * 3 + 1] = green / (blur_degree * blur_degree);
			frame.ptr<uchar>(y + v)[(x + u) * 3 + 2] = red / (blur_degree * blur_degree);

		}
	}

}

void getRGBFrame(int x, int y) {
	for (i = 0; i < blur_degree; i++)
	{
		for (j = 0; j < blur_degree; j++)
		{
			blue += frame.ptr<uchar>(y + j)[(x + i) * 3 + 0];
			green += frame.ptr<uchar>(y + j)[(x + i) * 3 + 1];
			red += frame.ptr<uchar>(y + j)[(x + i) * 3 + 2];

		}
	}
}
void rOI()
{
	for (int row = 0; row < 100; row++)
	{
		//Blur each frame one by one using Rectangle coorodaitantes 
		if (video[row][0] != 0 && video[row][1] != 0 && video[row][2] != 0 && video[row][3] != 0)
		{
			for (int x = video[row][0] + 2; x < video[row][2] - blur_degree; x += blur_degree)
			{
				for (int y = video[row][1] + 2; y < video[row][3] - blur_degree; y += blur_degree)
				{
					blue = green = red = 0;
					getRGBFrame(x, y);
					blurFrame(blur_degree, x, y);
				}
			}
		}
	}

}

void onMouse(int event, int x, int y, int flags, void* userdata)
{
	//on mouse click event for creating Rectangle 
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		start = Point(x, y);
		break;

	case EVENT_LBUTTONUP:
		endPoint = Point(x, y);
		ct = ct + 1;
		for (int i = ct; i < ct + 1; i++)
		{
			video[i][0] = start.x;
			video[i][1] = start.y;
			video[i][2] = endPoint.x;
			video[i][3] = endPoint.y;
		}
		break;
	default:
		break;

	}
}

int main(int argc, char** argv)
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			video[i][j] = 0;
		}
	}
	VideoCapture cam(0);
	namedWindow("Camera");
	setMouseCallback("Camera", onMouse);

	blur_degree = 5;

	while (true)
	{
		cam >> frame;

		for (int i = 0; i < 100; i++)
		{
			if (video[i][0] != 0 && video[i][1] != 0 && video[i][2] != 0 && video[i][3] != 0)
			{
				rOI();
				rectangle(frame, Point(video[i][0], video[i][1]), Point(video[i][2], video[i][3]), Scalar(0, 0, 255), 2);
			}
		}

		uchar c = waitKey(1);
		if (c == 82 || c == 114) // r or R
		{
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					video[i][j] = 0;
				}
			}

		}
		if (c == 68 || c == 100) // d or D
		{
			blur_degree -= 1;
		}
		if (c == 73 || c == 105) // i or I
		{
			blur_degree += 1;
		}
		imshow("Camera", frame);
		if (c == 83 || c == 115) // s or S 
		{
			imwrite("Frame.jpg", frame);
		}
		if (c == 27) //Esc break the frames 
		{
			break;
		}
	}
	return 1;
}