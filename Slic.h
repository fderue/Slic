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
    float m_wc;
    int m_width;
    int m_height;
    int m_diamSpx;

    Mat m_labels;
    vector<vector<float> > m_allDist;
    vector<center> m_allCenters;

	void enforceConnectivity();
	void findCenters(Mat& frame);
	void updateCenters(Mat& frame);

public:

    Slic(){}
    //Slic(int nspx,float wc):m_nSpx(nspx),m_wc(wc){}
    ~Slic(){}

	static enum InitType{
		SLIC_SIZE,
		SLIC_NSPX
	};

	/*SLIC_SIZE -> initialize by specifying the spx size
	SLIC_NSPX -> initialize by specifying the number of spx*/
    void initialize(Mat& frame,int nspx,float wc,Slic::InitType type);
	void resetVariables();
    void generateSpx(Mat& frame);

	Mat getLabels();
	int getNspx();
	int getSspx();


    void display_contours(Mat& image,Scalar colour=Scalar(255,0,0));
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
