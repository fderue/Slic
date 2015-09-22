//
// Created by derue on 15-09-16.
//

#ifndef SLIC_SLIC_H
#define SLIC_SLIC_H

#define MAXIT 5


#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

typedef vector< vector<int> > vec2di;

struct center;

class Slic
{
private:
    int m_nSpx;
    int m_wc;
    int m_width;
    int m_height;
    int m_diamSpx;

    vec2di m_labels;
    vector<vector<float> > m_allDist;
    vector<center> m_allCenters;



public:

    Slic(){}
    Slic(int nspx,float wc):m_nSpx(nspx),m_wc(wc){}
    ~Slic(){}

    void initialize(Mat& frame,int nspx,float wc);
    void generateSpx(Mat& frame);
    void enforceConnectivity();
    void findCenters(Mat& frame);
    void updateCenters(Mat& frame);

    void display_contours(Mat& image,Scalar colour);
    void display_meanColor(Mat& frame);

};

struct center
{
    Point xy;
    float Lab[3];
    center():xy(Point(0,0)){
        Lab[0] = 0.f;
        Lab[1] = 0.f;
        Lab[1] = 0.f;
    }
};


#endif //SLIC_SLIC_H
