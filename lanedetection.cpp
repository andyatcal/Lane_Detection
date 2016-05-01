#include "lanedetection.h"

LaneDetection::LaneDetection() 
{

}
void LaneDetection::init(Mat &img)
{
    img_width = img.cols;
    img_height = img.rows;
    //std::cout<<"Initialize detector: "<<img_width<<" "<<img_height<<endl;
}


void LaneDetection::detect(Mat &input)
{

}