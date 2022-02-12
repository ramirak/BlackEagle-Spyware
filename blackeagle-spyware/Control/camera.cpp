#include "camera.h"
#include "iostream"

int camera()
{
    // open the first webcam plugged in the computer
    cv::VideoCapture camera(0);
    if (!camera.isOpened()) {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    // this will contain the image from the webcam
    cv::Mat frame;

    // capture the next frame from the webcam
    camera >> frame;
    imwrite("pic.jpg", frame);
    // display the frame until you press a key

    return 0;
}