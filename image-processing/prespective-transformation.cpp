#include <opencv2/opencv.hpp>
#include <raspicam_cv.h>
#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;
using namespace cv;
using namespace raspicam;

Mat frame, Matrix, framePers, frameGray, frameThresh, frameEdge, frameFinal, frameFinalDuplicate;
Mat ROILane;
int LeftLanePos, RightLanePos, frameCenter, laneCenter, Result;

RaspiCam_Cv Camera;

stringstream ss;

vector<int> histrogramLane;

// data type of points in px values



Point2f Source[] = {Point2f(50, 135), Point2f(253, 135), Point2f(10, 185), Point2f(291, 185)};
Point2f Destination[] = {Point2f(75,0), Point2f(230, 0), Point2f(75,240), Point2f(230, 240)};

void Setup(int argc, char **argv, RaspiCam_Cv &Camera)
{
    Camera.set(CAP_PROP_FRAME_WIDTH, ("-w", argc, argv,300));
    Camera.set(CAP_PROP_FRAME_HEIGHT, ("-h", argc, argv, 200));
    Camera.set(CAP_PROP_BRIGHTNESS, ("-br", argc, argv, 50));
    Camera.set(CAP_PROP_CONTRAST, ("-co", argc, argv, 50));
    Camera.set(CAP_PROP_SATURATION, ("-sa", argc, argv, 50));
    Camera.set(CAP_PROP_GAIN, ("-g", argc, argv, 50));
    Camera.set(CAP_PROP_FPS, ("-fps", argc, argv, 0));
}

void Capture()
{
    Camera.grab();
    Camera.retrieve(frame);
    // from BGR color space  to RGB color space
    cvtColor(frame, frame, COLOR_BGR2RGB);
}

//
void Perspective()
{
    // joining four points
    // 2px width
    line(frame, Source[0], Source[1], Scalar(0, 0, 255), 2);
    line(frame, Source[1], Source[3], Scalar(0, 0, 255), 2);
    line(frame, Source[3], Source[2], Scalar(0, 0, 255), 2);
    line(frame, Source[2], Source[0], Scalar(0, 0, 255), 2);

    line(frame, Destination[0], Destination[1], Scalar(0, 255, 0), 2);
    line(frame, Destination[1], Destination[3], Scalar(0, 255, 0), 2);
    line(frame, Destination[3], Destination[2], Scalar(0, 255, 0), 2);
    line(frame, Destination[2], Destination[0], Scalar(0, 255, 0), 2);

    Matrix = getPerspectiveTransform(Source, Destination);
    warpPerspective(frame, framePers, Matrix, Size(400, 240));
}

int main(int argc, char **argv)
{

    Setup(argc, argv, Camera);
    cout << "Connecting to camera" << endl;
    if (!Camera.open())
    {

        cout << "Failed to Connect" << endl;
    }

    cout << "Camera Id = " << Camera.getId() << endl;

    while (1)
    {
        auto start = std::chrono::system_clock::now();

        Capture();
        Perspective();

        namedWindow("orignal", WINDOW_KEEPRATIO);
        moveWindow("orignal", 0, 100);
        resizeWindow("orignal", 640, 480);
        imshow("orignal", frame);

        namedWindow("Perspective", WINDOW_KEEPRATIO);
        moveWindow("Perspective", 640, 100);
        resizeWindow("Perspective", 640, 480);
        imshow("Perspective", framePers);

        waitKey(1);
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;

        float t = elapsed_seconds.count();
        int FPS = 1 / t;
        cout << "FPS = " << FPS << endl;
    }

    return 0;
}
