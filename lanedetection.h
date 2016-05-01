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
     * @brief: Detect lanes for every frame from the camera.
     * @param input: is the input frame from camera.
     */
    void detect(Mat &input);

    /* The source image for every frame of camera. */
    Mat img_src;

    /* The gray scaled image for every frame. */
    Mat img_gray;

    /* The region of interests in the gray scale image. */
    Mat img_roi;

    /**
     * Plot the hough lines.
     * Paint the lines on the input.
     */
    void plotHoughLines(Mat &input);
private:
    /* Store the width and height of the image. */
    int img_width, img_height;
    /* Def of the region of interest. */
    int roi_start_y, roi_height;
    /* Store the hough lines of the image. */
    vector<Vec4i> houghlines;
};

#endif //LANEDETECTION_H_