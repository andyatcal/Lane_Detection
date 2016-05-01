#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

//VisualSystem g_v;

int main(int argc, char **argv)
{
    //Import a video.
    VideoCapture *capture;

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

    cvNamedWindow("videoImage", CV_WINDOW_AUTOSIZE); 
    cvMoveWindow("videoImage", 30, 0);
    Mat img_src;
    while(1) {
        *capture >> img_src;
        imshow("videoImage", img_src);
        waitKey(1);
    }

    return 0;
}
