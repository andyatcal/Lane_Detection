#include "lanedetection.h"

LaneDetection::LaneDetection() 
{

}
void LaneDetection::init(Mat &img)
{
    img_width = img.cols;
    img_height = img.rows;
    roi_start_y = img_height * ROI_START_Y_RATIO;
    roi_height = img_height * ROI_HEIGHT_RATIO;

    img_src = img;
    cvtColor(img_src, img_gray, CV_BGR2GRAY);
    img_roi = img_gray(
            Rect(0, roi_start_y, img_width, roi_height));
}


void LaneDetection::detect(Mat &input)
{
    img_src = input;
    cvtColor(img_src, img_gray, CV_BGR2GRAY);
    blur(img_gray, img_gray, Size(3,3));
    Canny(img_roi, img_roi, CANNY_THRESHOLD_LOW, CANNY_THRESHOLD_HIGHT, 3);
    HoughLinesP(img_roi, houghlines, 1, CV_PI/180, 20, 5, 3);
    filterHoughLines();
    renewHoughLinePoints();
    //plotHoughLines(img_src);
    plotHoughLinesPoints(img_src);
    findLaneLines();
}

void LaneDetection::plotHoughLines(Mat &input)
{
    for( size_t i = 0; i < filteredHoughlines.size(); i++ )
    {
        //std::cout<<houghlines[i][0]<<" "<<houghlines[i][1]<<" "<<houghlines[i][2]<<" "<<houghlines[i][3]<<endl;
        line(input, Point(filteredHoughlines[i][0], filteredHoughlines[i][1] + roi_start_y),
            Point(filteredHoughlines[i][2], filteredHoughlines[i][3] + roi_start_y), Scalar(0,0,255), 2, 20);
    }
}

void LaneDetection::plotHoughLinesPoints(Mat &input)
{
    for( size_t i = 0; i < houghlinespoints.size(); i++ )
    {
        circle(input, houghlinespoints[i], 3 , Scalar(0, 0, 255), -1, 8 );
    }
}

void LaneDetection::filterHoughLines()
{
    float k;
    filteredHoughlines.clear();
    for( size_t i = 0; i < houghlines.size(); i++ )
    {
        if(houghlines[i][0] == houghlines[i][2])
        {
            filteredHoughlines.push_back(houghlines[i]);
        }
        k = (1.0 * houghlines[i][3] - 1.0 * houghlines[i][1]) / (1.0 * houghlines[i][2] - 1.0 * houghlines[i][0]);
        if(abs(k) > 0.2) {
            filteredHoughlines.push_back(houghlines[i]);
        }
    }
}

void LaneDetection::renewHoughLinePoints()
{
    houghlinespoints.clear();
    for( size_t i = 0; i < filteredHoughlines.size(); i++ )
    {
        houghlinespoints.push_back(Point(filteredHoughlines[i][0],filteredHoughlines[i][1] + roi_start_y));
        houghlinespoints.push_back(Point(filteredHoughlines[i][2],filteredHoughlines[i][3] + roi_start_y));
    }
}

void LaneDetection::findLaneLines()
{
    Vec4i newLine;
    //fitLine(houghlinespoints, newLine, CV_DIST_L2, 0, 0.01, 0.01);
}