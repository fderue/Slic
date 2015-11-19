#include <iostream>
#include <opencv2/opencv.hpp>
#include "Slic.h"

#define NSPX 1000
#define WC 35

using namespace std;
using namespace cv;
int main() {

    Mat im = imread("D:/Pictures/cat.jpg");
	//remap
	/*
	Mat dst, map_x, map_y;
	dst.create(im.size(), im.type());
	map_x.create(im.size(), CV_32FC1);
	map_y.create(im.size(), CV_32FC1);

	for (int j = 0; j < im.rows; j++)
	{
		for (int i = 0; i < im.cols; i++) {
			map_x.at<float>(j, i) = i;
			map_y.at<float>(j, i) = im.rows - j-1;
		}
	}
	remap(im, dst, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));

	imshow("remap", dst);
	dst = im.clone();
	*/

    /*Slic seg;
    seg.initialize(im,NSPX,WC);
	auto start = getTickCount();
    seg.generateSpx(im);
	auto end = getTickCount();
	cout << "runtime = " << (end - start) / getTickFrequency()<<endl;
    seg.display_contours(im,Scalar(255,0,0));

    imshow("seg", im);

	seg.generateSpx(im);
	seg.display_contours(im, Scalar(255, 0, 0));

    cout<<"end"<<endl;*/



	VideoCapture cap("D:/Videos/Tiger1/img/%04d.jpg");
	//Mat frame0 = imread("D:/Videos/bolt/img/0001.jpg");
	Mat frame;
	cap >> frame;
	Slic slic;

	
	namedWindow("outCPU");
	while (cap.read(frame))
	{
		slic.initialize(frame, NSPX, WC);
		slic.generateSpx(frame);
		slic.display_contours(frame, Scalar(255, 0, 0));
		imshow("out", frame);
		waitKey();
	}

	
    waitKey();

    return 0;
}