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
    ipm_start = img_width * IPM_START_RATIO;
    ipm_end = img_width - ipm_start;
    img_src = img;
    cvtColor(img_src, img_gray, CV_RGB2GRAY);
    cvtColor(img_src, img_gray_copy, CV_RGB2GRAY);
    img_roi = img_gray(
            Rect(0, roi_start_y, img_width, roi_height));
    initIPM();
    plotlines.clear();
    plotlines_p1.clear();
    plotlines_p2.clear();
    //img_hough = Mat::zeros(img.rows, img.cols, img_gray.type());
}


void LaneDetection::detect(Mat &input)
{
    lines.clear();
    img_src = input;
    cvtColor(img_src, img_gray, CV_BGR2GRAY);
    cvtColor(img_src, img_gray_copy, CV_BGR2GRAY);
    blur(img_gray, img_gray, Size(3,3));
    //blur(img_gray_copy, img_gray_copy, Size(3,3));
    //Canny(img_canny, img_roi, CANNY_THRESHOLD_LOW, CANNY_THRESHOLD_HIGHT, 3);
    applyIPM(img_roi, img_roi);
    img_roi.copyTo(img_roi_copy);
    //Sobel(img_roi, img_roi, -1, 1, 0);
    Mat sobel1 = (Mat_<double>(3,3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
    Mat sobel2 = (Mat_<double>(3,3) << 1, 0, -1, 2, 0, -2, 1, 0, -1);
    Mat shift = (Mat_<double>(1,3) << 1, 1, 0);
    filter2D(img_roi, img_roi, -1, sobel1);
    threshold(img_roi, img_roi, 90, 100, THRESH_BINARY);
    filter2D(img_roi,img_roi, -1, shift);
    filter2D(img_roi_copy, img_roi_copy, -1, sobel2);
    threshold(img_roi_copy, img_roi_copy, 90, 100, THRESH_BINARY);
    bitwise_and(img_roi, img_roi_copy, img_roi);
    applyInverseIPM(img_roi, img_roi);
    //Scharr(img_gray, img_gray, -1, 0, 1);
    //HoughLines(img_roi, lines, 1, CV_PI/180, 40, 0, 0);
    //reArrange();
    HoughLinesP(img_roi, houghlines, 1, CV_PI/180, 30, 50, 200);
    //HoughLinesP(img_roi_copy, houghlines_copy, 1, CV_PI/180, 30, 3, 5);
    //cout<<houghlines.size()<<endl;
    filterHoughLines();
    mergeLines();
    plotLines(img_src);
    //renewHoughLinePoints();
    //plotHoughLines(img_src);
    //plotHoughLinesP(img_src);
    //plotHoughLinesPoints(img_src);
    //findLaneLines();
}

void LaneDetection::plotHoughLinesP(Mat &input)
{
    for( size_t i = 0; i < filteredHoughlines.size(); i++ )
    {
        //std::cout<<houghlines[i][0]<<" "<<houghlines[i][1]<<" "<<houghlines[i][2]<<" "<<houghlines[i][3]<<endl;
        line(input, Point(filteredHoughlines[i][0], filteredHoughlines[i][1] + roi_start_y),
            Point(filteredHoughlines[i][2], filteredHoughlines[i][3] + roi_start_y), Scalar(255, 0, 0), 1, 20);
    }
    /*
    for( size_t i = 0; i < filteredHoughlines.size(); i++ )
    {
        //std::cout<<houghlines[i][0]<<" "<<houghlines[i][1]<<" "<<houghlines[i][2]<<" "<<houghlines[i][3]<<endl;
        line(input, Point(filteredHoughlines_copy[i][0], filteredHoughlines_copy[i][1] + roi_start_y),
            Point(filteredHoughlines_copy[i][2], filteredHoughlines_copy[i][3] + roi_start_y), Scalar(0, 255, 0), 1, 20);
    }*/
}

void LaneDetection::plotHoughLines(Mat &input)
{
  //std::cout<<lines.size()<<endl;
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

void LaneDetection::plotLines(Mat &input)
{
  //std::cout<<lines.size()<<endl;
    for( size_t i = 0; i < plotlines.size(); i++ )  
    {  
        float m = plotlines[i][0], b = plotlines[i][1];  
        Point pt1, pt2;
        pt1.x = roi_start_y * m + b;
        pt1.y = roi_start_y;  
        pt2.x = (roi_start_y + roi_height) * m + b;
        pt2.y = roi_start_y + roi_height;  
        line(input, pt1, pt2, Scalar(0,0,255), 2, 20);
    }
    for( size_t i = 0; i < plotlines_p1.size(); i++ )  
    {  
        float m = plotlines_p1[i][0], b = plotlines_p1[i][1];  
        Point pt1, pt2;
        pt1.x = roi_start_y * m + b;
        pt1.y = roi_start_y;  
        pt2.x = (roi_start_y + roi_height) * m + b;
        pt2.y = roi_start_y + roi_height;  
        line(input, pt1, pt2, Scalar(0,0,255), 2, 20);
    }
    for( size_t i = 0; i < plotlines_p2.size(); i++ )  
    {  
        float m = plotlines_p2[i][0], b = plotlines_p2[i][1];  
        Point pt1, pt2;
        pt1.x = roi_start_y * m + b;
        pt1.y = roi_start_y;  
        pt2.x = (roi_start_y + roi_height) * m + b;
        pt2.y = roi_start_y + roi_height;  
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
    /*filteredHoughlines_copy.clear();
    for( size_t i = 0; i < houghlines_copy.size(); i++ )
    {
        if(houghlines_copy[i][0] == houghlines_copy[i][2])
        {
            filteredHoughlines_copy.push_back(houghlines_copy[i]);
        }
        k = (1.0 * houghlines_copy[i][3] - 1.0 * houghlines_copy[i][1]) / (1.0 * houghlines_copy[i][2] - 1.0 * houghlines_copy[i][0]);
        if(abs(k) > 0.1) {
            filteredHoughlines_copy.push_back(houghlines_copy[i]);
        }
    }*/
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

void LaneDetection::initIPM()
{
    Point2f src[4];
    Point2f dst[4];

    src[0].x = 0;
    src[0].y = 0;
    src[1].x = ipm_start;
    src[1].y = roi_height;
    src[2].x = ipm_end;
    src[2].y = roi_height;
    src[3].x = img_width;
    src[3].y = 0;

    dst[0].x = 0;
    dst[0].y = 0;
    dst[1].x = 0;
    dst[1].y = roi_height;
    dst[2].x = img_width;
    dst[2].y = roi_height;
    dst[3].x = img_width;
    dst[3].y = 0;

    tsf_ipm = getPerspectiveTransform(dst, src);
    tsf_ipm_inv = tsf_ipm.inv();
}
void LaneDetection::applyIPM(Mat &src, Mat &dst)
{
    warpPerspective(src, dst, tsf_ipm, src.size());
}

void LaneDetection::applyInverseIPM(Mat &src, Mat &dst)
{
    warpPerspective(src, dst, tsf_ipm_inv, src.size());
}

bool isEqual(const Vec4i& _l1, const Vec4i& _l2)
{
    Vec4i l1(_l1), l2(_l2);

    float length1 = sqrtf((l1[2] - l1[0])*(l1[2] - l1[0]) + (l1[3] - l1[1])*(l1[3] - l1[1]));
    float length2 = sqrtf((l2[2] - l2[0])*(l2[2] - l2[0]) + (l2[3] - l2[1])*(l2[3] - l2[1]));

    float product = (l1[2] - l1[0])*(l2[2] - l2[0]) + (l1[3] - l1[1])*(l2[3] - l2[1]);

    if (fabs(product / (length1 * length2)) < cos(CV_PI / 30))
        return false;

    float mx1 = (l1[0] + l1[2]) * 0.5f;
    float mx2 = (l2[0] + l2[2]) * 0.5f;

    float my1 = (l1[1] + l1[3]) * 0.5f;
    float my2 = (l2[1] + l2[3]) * 0.5f;
    float dist = sqrtf((mx1 - mx2)*(mx1 - mx2) + (my1 - my2)*(my1 - my2));

    if (dist > std::max(length1, length2) * 0.5f)
        return false;

    return true;
}

void LaneDetection::mergeLines()
{
    plotlines_p2 = plotlines_p1;
    plotlines_p1 = plotlines;
    plotlines.clear();
    //Vec2f nextLine;
    /*
    vector<vector<Vec2f> > groupMB;
    vector<vector<Vec4i> > groupLines;
    int m, b;
    for( size_t i = 0; i < filteredHoughlines.size(); i++ )
    {
        m = 1.0 * (filteredHoughlines[i][2] - filteredHoughlines[i][0])
         / (filteredHoughlines[i][3] - filteredHoughlines[i][1]);
        b = filteredHoughlines[i][0] - (filteredHoughlines[i][1] + roi_start_y) * nextLine[0];
        
    }
    */
    std::vector<int> labels;
    int numberOfLines = cv::partition(houghlines, labels, isEqual);
    cout<<labels.size()<<endl;
    //plotlines.push_back(nextLine);
    //cout<<plotlines.size()<<endl;
}
