#include <opencv2/opencv.hpp>
#include <raspicam_cv.h>
#include <iostream>


using namespace std;
using namespace cv;
using namespace raspicam;

Mat frame;

void Setup(int argc, char **argv, RaspiCam_Cv &Camera)
{

  // setting up camera parameters
  Camera.set(CAP_PROP_FRAME_WIDTH, ("-w", argc, argv, 400));
  Camera.set(CAP_PROP_FRAME_HEIGHT, ("-h", argc, argv, 240));
  Camera.set(CAP_PROP_BRIGHTNESS, ("-br", argc, argv, 50));
  Camera.set(CAP_PROP_CONTRAST, ("-co", argc, argv, 50));
  Camera.set(CAP_PROP_SATURATION, ("-sa", argc, argv, 50));
  Camera.set(CAP_PROP_GAIN, ("-g", argc, argv, 50));
  Camera.set(CAP_PROP_FPS, ("-fps", argc, argv, 100));
}

int main(int argc, char **argv)
{

  RaspiCam_Cv Camera;

  // argc and argv are command line arguments
  Setup(argc, argv, Camera);
  cout << "Connecting to camera" << endl;
  if (!Camera.open())
  {

    cout << "Failed to Connect" << endl;
  }

  cout << "Camera Id = " << Camera.getId() << endl;

  Camera.grab();
  Camera.retrieve(frame);
  imshow("camera frame", frame);
  waitKey(1);

  return 0;
}
