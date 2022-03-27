#include "camera.h"

int camera(const char* filename)
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

	if (!frame.data)
		imwrite(filename, frame);

	return 0;
}