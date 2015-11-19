//
// Created by derue on 15-09-16.
//


#include "Slic.h"


void Slic::initialize(Mat& frame,int nspx,float wc)
{
    m_nSpx = nspx;
    m_wc = wc;
    m_height = frame.rows;
    m_width = frame.cols;
	m_diamSpx = (int)sqrt(m_width*m_height / (float)m_nSpx);

    //initialize labels
    m_labels.resize(m_height);
    m_allDist.resize(m_height);
    for(int j=0; j<m_height; j++){
        m_labels[j] = vector<int>(m_width,-1);
        m_allDist[j] = vector<float>(m_width,FLT_MAX);
    }

}


void Slic::generateSpx(Mat & frame)
{
	m_allCenters.clear();
	//cout << "supposed number of spx " << m_nSpx << endl;
    Mat frameLab;
	cvtColor(frame,frameLab,CV_BGR2Lab);
    frameLab.convertTo(frameLab,CV_32FC3);
    //initializa clusters
    int diamSpx_d2 = m_diamSpx/2;
    for(int y = diamSpx_d2-1; y<m_height; y+=m_diamSpx)
    {
        Vec3f* frameLab_r = frameLab.ptr<Vec3f>(y);
        for(int x=diamSpx_d2-1; x<m_width; x+=m_diamSpx)
        {
            center c;
            c.xy = Point(x,y);

            c.Lab[0] = frameLab_r[x][0];
            c.Lab[1] = frameLab_r[x][1];
            c.Lab[2] = frameLab_r[x][2];
            m_allCenters.push_back(c);
        }
    }
    // move to lowest gradient ....
    m_nSpx = m_allCenters.size(); //real number of spx

    //initialize labels and dist

    for(int i=0; i<m_height; i++)
        for(int j=0; j<m_width; j++)
        {
        m_labels[i][j] = -1;
        m_allDist[i][j] = FLT_MAX;
        }


    // iterate
    for(int it=0; it<MAXIT; it++)
    {
        findCenters(frameLab);
        updateCenters(frameLab);

    }
	//for (int i = 0; i < 10; i++)cout <<"Lab "<< m_allCenters[i].Lab[0] << endl;

    //enforceConnectivity();

   // cout<<"real number of spx "<<m_nSpx<<endl;
}

void Slic::display_meanColor(Mat& frame)
{
    Mat frameLab = Mat(frame.size(),CV_32FC3);
    for(int i=0; i<frame.rows;i++)
    {
        for(int j=0; j<frame.cols; j++)
        {
            int idxC = m_labels[i][j];
            if(idxC!=-1)
                frameLab.at<Vec3f>(i,j) = Vec3f(m_allCenters[idxC].Lab[0],m_allCenters[idxC].Lab[1],m_allCenters[idxC].Lab[2]);
        }
    }
    frameLab.convertTo(frameLab,CV_8UC3);
    cvtColor(frameLab,frame,CV_Lab2BGR);
}

inline float slicDistance(center& c, float x, float y, float L, float a, float b,float S,float m)
{
    float dc2 = pow(c.Lab[0]-L,2)+pow(c.Lab[1]-a,2)+pow(c.Lab[2]-b,2);
    float ds2 = pow(c.xy.x-x,2)+pow(c.xy.y-y,2);


    return sqrt(dc2+ds2/(S*S)*m*m);


}
void Slic::findCenters(Mat& frame)
{
    int diamSpx3d2 = m_diamSpx;
    for(int c=0; c<m_allCenters.size(); c++)
    {
        Point xy_c = m_allCenters[c].xy;
        if(xy_c.x!=-1) {
            for (int i = xy_c.y - diamSpx3d2; i < xy_c.y + diamSpx3d2; i++) {
                for (int j = xy_c.x - diamSpx3d2; j < xy_c.x + diamSpx3d2; j++) {
                    if (i >= 0 && i < m_height && j >= 0 && j < m_width) {
                        Vec3f lab = frame.at<Vec3f>(i, j);
                        float d = slicDistance(m_allCenters[c], j, i, lab.val[0], lab.val[1], lab.val[2], m_diamSpx, m_wc);

                        if (d < m_allDist[i][j]) {
							m_allDist[i][j] = d;
                            m_labels[i][j] = c;
                        }
                    }
                }
            }
        }
    }

}
void Slic::updateCenters(Mat& frame)
{
    //clear center value
    vector<int> counter(m_allCenters.size(),0);
    for(int i=0; i<m_allCenters.size(); i++)
    {
        m_allCenters[i].xy.x = m_allCenters[i].xy.y = m_allCenters[i].Lab[0] = m_allCenters[i].Lab[1] = m_allCenters[i].Lab[1] = 0;
    }
    for(int i=0; i<m_height; i++)
    {
        for(int j=0; j<m_width; j++)
        {

            int idxC = m_labels[i][j];


                Vec3f lab = frame.at<Vec3f>(i, j);
                m_allCenters[idxC].xy += Point(j, i);

                m_allCenters[idxC].Lab[0] += lab.val[0];
                m_allCenters[idxC].Lab[1] += lab.val[1];
                m_allCenters[idxC].Lab[2] += lab.val[2];

                counter[idxC]++;
        }
    }
    for(int i=0; i<m_allCenters.size(); i++)
    {
        if(counter[i]!=0)
        {
            m_allCenters[i].xy/=counter[i];

            m_allCenters[i].Lab[0]/=counter[i];
            m_allCenters[i].Lab[1]/=counter[i];
            m_allCenters[i].Lab[2]/=counter[i];
        }
        else
        {
            m_allCenters[i].xy.x = -1; // reject a center which accept no pixel
            m_nSpx--;
        }
    }
}


void Slic::enforceConnectivity()
{
    int label = 0, adjlabel = 0;
    int lims = (m_width * m_height) / (m_nSpx);
    lims= lims>>2;


    const int dx4[4] = { -1, 0, 1, 0 };
    const int dy4[4] = { 0, -1, 0, 1 };

    vector<vector<int> >newLabels;
    for (int i = 0; i < m_height; i++)
    {
        vector<int> nv(m_width, -1);
        newLabels.push_back(nv);
    }

    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            if (newLabels[i][j] == -1)
            {
                vector<Point> elements;
                elements.push_back(Point(j, i));
                for (int k = 0; k < 4; k++)
                {
                    int x = elements[0].x + dx4[k], y = elements[0].y + dy4[k];
                    if (x >= 0 && x < m_width && y >= 0 && y < m_height)
                    {
                        if (newLabels[y][x] >= 0)
                        {
                            adjlabel = newLabels[y][x];
                        }
                    }
                }
                int count = 1;
                for (int c = 0; c < count; c++)
                {
                    for (int k = 0; k < 4; k++)
                    {
                        int x = elements[c].x + dx4[k], y = elements[c].y + dy4[k];
                        if (x >= 0 && x < m_width && y >= 0 && y < m_height)
                        {
                            if (newLabels[y][x] == -1 && m_labels[i][j]==m_labels[y][x])
                            {
                                elements.push_back(Point(x, y));
                                newLabels[y][x] = label;//m_labels[i][j];
                                count += 1;
                            }
                        }
                    }
                }
                if (count <= lims) {
                    for (int c = 0; c < count; c++) {
                        newLabels[elements[c].y][elements[c].x] = adjlabel;
                    }
                    label-=1;
                }
                label+=1;
            }
        }
    }
    m_nSpx = label;
    for(int i=0; i<newLabels.size(); i++)
        for(int j=0; j<newLabels[i].size(); j++)
            m_labels[i][j]=newLabels[i][j];

}


void Slic::display_contours(Mat& image,Scalar colour) {
    const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
    const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};

    /* Initialize the contour vector and the matrix detailing whether a pixel
     * is already taken to be a contour. */
    vector<Point> contours;
    vector<vector<bool> > istaken;
    for (int i = 0; i < image.rows; i++) {
        vector<bool> nb;
        for (int j = 0; j < image.cols; j++) {
            nb.push_back(false);
        }
        istaken.push_back(nb);
    }

    /* Go through all the pixels. */
    for(int i=0; i<image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {

            int nr_p = 0;

            /* Compare the pixel to its 8 neighbours. */
            for (int k = 0; k < 8; k++) {
                int x = j + dx8[k], y = i + dy8[k];

                if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                    if (istaken[y][x] == false && m_labels[i][j] != m_labels[y][x]) {
                        nr_p += 1;
                    }
                }
            }
            /* Add the pixel to the contour list if desired. */
            if (nr_p >= 2) {
                contours.push_back(Point(j,i));
                istaken[i][j] = true;
            }

        }
    }

    /* Draw the contour pixels. */
    for (int i = 0; i < (int)contours.size(); i++) {
        image.at<Vec3b>(contours[i].y,contours[i].x) = Vec3b(colour[0],colour[1],colour[2]);
    }

}