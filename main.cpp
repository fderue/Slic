#include <iostream>
#include <opencv2/opencv.hpp>
#include "Slic.h"

using namespace std;
using namespace cv;
int main() {

    Mat im = imread("/Users/derue/Pictures/test_pic/koala.jpg");

    int nspx = 500;
    float wc = 40;
    Slic seg;
    seg.initialize(im,nspx,wc);
    seg.generateSpx(im);
    seg.display_contours(im,Scalar(255,0,0));

    //seg.display_meanColor(im);
    imshow("seg",im);

    cout<<"end"<<endl;

    waitKey();

    return 0;
}