#include "image.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
const cv::Scalar SCALAR_BLUE = cv::Scalar(255.0, 0.0, 0.0);
const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 200.0, 0.0);
const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);

void imagePlay(std::string filename)
{

    Mat image;
    image = imread(filename, CV_LOAD_IMAGE_COLOR);   		// Read the file

    if(!image.data)                              			// Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return;
    }

	namedWindow("originale", WINDOW_AUTOSIZE);					// Create a window for display.
    imshow("originale", image);                  				// Show our image inside it.
	moveWindow("originale", 10, 50);							// move window


    waitKey(0);                                 			// Wait for a keystroke in the window
    return;

}