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
    cvtColor(img_src, img_gray, CV_RGB2GRAY);
    img_roi = img_gray(
            Rect(0, roi_start_y, img_width, roi_height));
    //img_hough = Mat::zeros(img.rows, img.cols, img_gray.type());
}


void LaneDetection::detect(Mat &input)
{
    lines.clear();
    img_src = input;
    cvtColor(img_src, img_gray, CV_BGR2GRAY);
    blur(img_gray, img_gray, Size(3,3));
    //Canny(img_canny, img_roi, CANNY_THRESHOLD_LOW, CANNY_THRESHOLD_HIGHT, 3);
    Sobel(img_roi, img_roi, -1, 1, 0);
    sobel1 = Mat
    filter2D(img_roi, img_roi, -1, sobel1)
    //threshold(img_roi, img_roi, 90, 100, THRESH_BINARY);
    //Scharr(img_gray, img_gray, -1, 0, 1);
    //HoughLines(img_roi, lines, 1, CV_PI/180, 40, 0, 0);
    //reArrange();
    HoughLinesP(img_roi, houghlines, 1, CV_PI/180, 30, 3, 5);
    //cout<<houghlines.size()<<endl;
    filterHoughLines();
    //renewHoughLinePoints();
    //plotHoughLines(img_src);
    plotHoughLinesP(img_src);
    //plotHoughLinesPoints(img_src);
    //findLaneLines();
}

void LaneDetection::plotHoughLinesP(Mat &input)
{
    for( size_t i = 0; i < filteredHoughlines.size(); i++ )
    {
        //std::cout<<houghlines[i][0]<<" "<<houghlines[i][1]<<" "<<houghlines[i][2]<<" "<<houghlines[i][3]<<endl;
        line(input, Point(filteredHoughlines[i][0], filteredHoughlines[i][1] + roi_start_y),
            Point(filteredHoughlines[i][2], filteredHoughlines[i][3] + roi_start_y), Scalar(255, 0, 0), 2, 20);
    }
}

void LaneDetection::plotHoughLines(Mat &input)
{
  std::cout<<lines.size()<<endl;
  for( size_t i = 0; i < lines.size(); i++ )  
  {  
     float rho = lines[i][0], theta = lines[i][1];  
     Point pt1, pt2;  
     double a = cos(theta), b = sin(theta);  
     double x0 = a*rho, y0 = b*rho;  
     pt1.x = cvRound(x0 + 2000*(-b));  
     pt1.y = cvRound(y0 + 2000*(a)) + roi_start_y;  
     pt2.x = cvRound(x0 - 2000*(-b));  
     pt2.y = cvRound(y0 - 2000*(a)) + roi_start_y;  
     line(input, pt1, pt2, Scalar(0,0,255), 2, 20);
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
        if(abs(k) > 0.1) {
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
    
}

void LaneDetection::reArrange()
{
    for( size_t i = 0; i < houghlines.size(); i++ )
    {
        Vec4i line = houghlines[i];
        if(line[1] > line[3]) 
        {
            int temp = line[3];
            line[3] = line[1];
            line[1] = temp;
        } else if (line[1] == line[3] && line[0] > line[2])
        {
            int temp = line[0];
            line[0] = line[2];
            line[2] = temp;
        }
    }
}