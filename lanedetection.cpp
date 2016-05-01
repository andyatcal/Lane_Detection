#include "lanedetection.h"

LaneDetection::LaneDetection() 
{

}
void LaneDetection::init(Mat &img)
{
    img_width = img.cols;
    img_height = img.rows;
    int roi_start_y = img_height * ROI_START_Y_RATIO;
    int roi_height = img_height * ROI_HEIGHT_RATIO;

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
}