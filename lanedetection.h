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

    /* The gray scaled image for every frame. */
    Mat img_gray_copy;

    /* The region of interests in the gray scale image. */
    Mat img_roi;

    /* The region of interests in the gray scale image. */
    Mat img_roi_copy;

    /**
     * Plot the hough lines.
     */
    void plotHoughLines(Mat &input);

    /**
     * Plot the hough lines, probablistics.
     */
    void plotHoughLinesP(Mat &input);

    /**
     * plot the merged lines.
     */
    void plotLines(Mat &input);

    /**
     * plot the candidate lines.
     */
    void plotCandidateLines(Mat &input);
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

    /**
     * Initialize IPM transformation.
     */
    void initIPM();
    /**
     * Apply IPM transformation. 
     */
    void applyIPM(Mat &src, Mat &dst);

    /**
     * Apply Inverse IPM transformation. 
     */
    void applyInverseIPM(Mat &src, Mat &dst);

    /**
     * merge the lines found by hough transform.
     */
    void mergeLines();

    /**
     * Filter the candidate lines with all kinds of predicate.
     */
    void filterCandiateLines();
private:
    /* Store the width and height of the image. */
    int img_width, img_height;
    /* The start and end of IPM. */
    int ipm_start, ipm_end;
    /* The matrix for IPM. */
    Mat tsf_ipm, tsf_ipm_inv;
    /* Def of the region of interest. */
    int roi_start_y, roi_height;
    /* Store the hough transform P of the image. */
    vector<Vec4i> houghlines;
    /* Store the hough transform P of the image. */
    vector<Vec4i> houghlines_copy;
    /* Store the result of hough transform. */
    vector<Vec2f> lines;
    /* Store the result of merged lines. */
    vector<Vec2f> plotlines;
    /* Store the result of merged lines. */
    vector<Vec3f> plotlines_p1;
    /* Store the result of merged lines. */
    //vector<Vec3f> plotlines_p2;
    /* Filtered hough lines. */
    vector<Vec4i> filteredHoughlines;
    /* Filtered hough lines. */
    vector<Vec4i> filteredHoughlines_copy;
    /* Store the points of houghlines here. */
    vector<Point> houghlinespoints;
    /* Store the lane lines.*/
    vector<Vec4f> candidatelines;
    /* Store the result of merged lines. */
    vector<Vec4f> previousBestLines;
    /* Used to compare lines from this frame to the previous frames.*/
    float top_diff, bottom_diff;

};

#endif //LANEDETECTION_H_