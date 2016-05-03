#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "config.h"
#include "lanedetection.h"
#include "lanedetection.cpp"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    //Import a video.
    VideoCapture *capture;
    LaneDetection *detector = new LaneDetection;
    if (argc > 1) //From a file
    {
        capture = new VideoCapture(argv[1]);
        if (!capture->isOpened()) {
            printf("Can't open your file");
            exit(0);
        }
    } 
    else // From camera
    {
        capture = new VideoCapture(0); // From the camera
    }

    cvNamedWindow("videoInput", CV_WINDOW_AUTOSIZE); 
    cvMoveWindow("videoInput", 30, 30);

    //cvNamedWindow("grayOutput", CV_WINDOW_AUTOSIZE); 
    //cvMoveWindow("grayOutput", 330, 100);

    cvNamedWindow("videoOutput", CV_WINDOW_AUTOSIZE); 
    cvMoveWindow("videoOutput", 330, 100);

    Mat img;

    *capture >> img;
    detector -> init(img);
    while(true) {
        *capture >> img;
        detector -> detect(img);
        imshow("videoInput", detector -> img_src);
        //imshow("grayOutput", detector -> img_gray);
        imshow("videoOutput", detector -> img_roi);
        waitKey(0);
    }

    return 0;
}
