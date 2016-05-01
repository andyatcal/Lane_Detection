#include "lanedetection.h"

LaneDetection::LaneDetection() 
{

}
void LaneDetection::init(Mat &img)
{
    img_width = img.cols;
    img_height = img.rows;
}


void LaneDetection::detect(Mat &input)
{
    img_src = input;
    cvtColor(img_src, img_gray, CV_BGR2GRAY);
}