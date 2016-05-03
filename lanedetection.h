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
     */
    void plotHoughLines(Mat &input);

    /**
     * Plot the hough lines, probablistics.
     */
    void plotHoughLinesP(Mat &input);

    /**
     * Plot the points on the hough lines.
     */
    void plotHoughLinesPoints(Mat &input);

    /**
     * Filter out some HoughLines.
     */
    void filterHoughLines();

    /**
     * Debug function to get the average of all x coordinates. 
     */
    void avgOfX();

    /**
     * Put all points from the Filtered Hough Lines into a vector. 
     */
    void renewHoughLinePoints();

    /**
     * Find the lane seperate lines.
     */
    void findLaneLines();

    /**
     * Rearrange the founded houghlines so that the y coordinate of the first point 
     * is always smaller than the second point. When y coordinates are the same, the
     * x coordinate of the first point is smaller than the second point.
     */
    void reArrange();
private:
    /* Store the width and height of the image. */
    int img_width, img_height;
    /* Def of the region of interest. */
    int roi_start_y, roi_height;
    /* Store the hough transform P of the image. */
    vector<Vec4i> houghlines;
    /* Store the result of hough transform. */
    vector<Vec2f> lines;
    /* Filtered hough lines. */
    vector<Vec4i> filteredHoughlines;
    /* Store the points of houghlines here. */
    vector<Point> houghlinespoints;
    /* Store the lane lines.*/
    vector<Vec4i> lanelines;

};

#endif //LANEDETECTION_H_