#include <iostream>
#include <opencv2/opencv.hpp>
#include "Slic.h"


using namespace std;
using namespace cv;
int main() {

	Mat im = imread("C:/Recherche_FX/video/TB_CVPR2013/tiger1/img/0001.jpg"); //your image path
	CV_Assert(im.data!=nullptr);

	//param
	int diamSpx = 16;
	float wc = 35;
	int nIteration = 5;

	//segmentation
    Slic seg;
	seg.initialize(im, diamSpx, wc,nIteration, Slic::SLIC_SIZE);
	auto start = getTickCount();
	seg.generateSpx(im);
	auto end = getTickCount();
	cout << "Runtime segmentation cpu = " << (end - start) / getTickFrequency() << endl;
    seg.display_contours(im,Scalar(0,0,255));
    imshow("seg", im);
    waitKey();

    return 0;
}