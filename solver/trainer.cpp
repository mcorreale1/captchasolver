#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <string>
#include <limits>
#include <algorithm>

using namespace std;
using namespace cv;

int main(int argc, char **argv) {

}

void showImage(string name, Mat& img) {
    namedWindow(name);
    imshow(name, img);
}
