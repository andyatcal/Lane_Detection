#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "lanedetection.h"
#include "lanedetection.cpp"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    //Import a video.
    VideoCapture *capture;
    LaneDetection detector;
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

    cvNamedWindow("video", CV_WINDOW_AUTOSIZE); 
    cvMoveWindow("video", 30, 30);

    Mat img_src;

    *capture >> img_src;
    detector.init(img_src);
    while(1) {
        *capture >> img_src;
        detector.detect(img_src);
        imshow("video", img_src);
        waitKey(0);
    }

    return 0;
}
