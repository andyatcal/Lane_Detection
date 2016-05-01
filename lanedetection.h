#ifndef LANEDETECTION_H_
#define LANEDETECTION_H_

using namespace cv;
using namespace std;

class LaneDetection {
public:
    /**
     * Constructor
     */
    LaneDetection();
    /**
     * Initialize the general dimensions
     */
    void init(Mat &img);
    /**
     * Detect lanes for every frame from the camera.
     */
    void detect(Mat &input);

    /* The source image for every frame of camera. */
    Mat img_src;

    /* The gray scaled image for every frame. Used as the input for Canny Edge Detector */
    Mat img_gray;
    
private:
    int img_width, img_height;
    vector<Vec4i> houghlines;
};

#endif //LANEDETECTION_H_