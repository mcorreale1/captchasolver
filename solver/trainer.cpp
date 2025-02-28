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
#include <fstream>
#include <sstream>
#include <armadillo>

using namespace std;
using namespace cv;

#define IMAGE_WIDTH  100
#define IMAGE_HEIGHT  100
#define VECTOR_SIZE  10000

enum Classes {ZERO = 0, ONE = 1, TWO = 2, THREE = 3, FOUR = 4, 
                FIVE = 5, SIX = 6, SEVEN = 7, EIGHT = 8, NINE = 9,
                A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z};
struct ImageData {
    Classes c;
    std::vector<uchar> pixels;
};

#include "minimize.hpp"

Mat applyOperator(Mat &img, vector<vector<int> > &kernel, int mode);
void generateData(string in, string out);
vector<ImageData> generateTraining(char* dataIn);
Mat generateImage(string path);
void showImage(string name, Mat& img);
void applyTraining(vector<ImageData> &data, vector<uchar> &img);
vector<uchar> flattenImage(char* file);

int main(int argc, char **argv) {
    //generateData(argv[1], argv[2]);
    //generateTraining(argv[1], argv[2]);
    
    cout << "gathering training data" << endl;
    auto data = generateTraining(argv[1]);
    cout << "applying training" << endl;
    vector<uchar> image = flattenImage(argv[2]);
    applyTraining(data, image);
}

vector<uchar> flattenImage(char* file) {
    vector<uchar> pixels(VECTOR_SIZE, 0);
    Mat img = generateImage(file);
    for(int x = 0; x < img.rows; ++x) {
        for(int y = 0; y < img.cols; ++y) {
            pixels[x * img.rows + y] = img.at<uchar>(x,y);
        }
    }
    return pixels;
}


void applyTraining(vector<ImageData> &data, vector<uchar> &image) {
    std::vector<std::vector<float> > theta(10, std::vector<float>(VECTOR_SIZE, 0));

    Minimize mini(data, theta);
    int iter = 5;
    double alpha = 0.01;
    mini.runOneVsAll(alpha, iter, image);
    //gradientDescent(data, alpha, iter, theta);

    //cout << theta[0].size() << endl;
}

float multiply(vector<uchar> &v1, vector<float> &v2) {
    float value = v2[0];
    for(int i = 0; i < VECTOR_SIZE; ++i) {
        value += v1[i] * v2[i + 1];
    }
    return value;
}
vector<ImageData> generateTraining(char* dataIn) {
    ifstream input(dataIn);
    vector<ImageData> data;
    string line;
    int sizeIndex = 0;
    while(getline(input, line)) {
        ImageData id;
        int count = 0;
        char cl = (char)line[0];
        Classes dataClass = (Classes)(((int)cl) - 48);
        id.c = dataClass;
        stringstream ss(line.substr(2));

        char delim;
        int v;
        ss >> v;
        id.pixels.push_back((uchar) v);
        while(ss >> delim >> v && delim == ',') {
            id.pixels.push_back((uchar) v);
        }
        data.push_back(id);
    }   
    return data; 
}

Mat generateImage(string path) {
    Mat img_raw = imread(path); 
    Mat img;
    cvtColor(img_raw, img, CV_BGR2GRAY);
    for(int x = 0; x < img.rows; ++x) {
        for(int y = 0; y < img.cols; ++y) {
            if(img.at<uchar>(x,y) <= 175) {
                img.at<uchar>(x,y) = 0;
            } else {
                img.at<uchar>(x,y) = 255;
            }
        }
    }
    vector<vector<int> > kernel = {
        {1,1,1},
        {1,1,1},
        {1,1,1}};
    vector<vector<int> > kernel2 = {
        {0,1,1,1,0},
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1},
        {0,1,1,1,0}};

    img = applyOperator(img, kernel2, 0);
    img = applyOperator(img, kernel2, 1);
    return img;
}


void generateData(string in, string out) {
    ofstream output(out);
    std::vector<ImageData> data;
    for(int c = 0; c < 10; ++c) {
        for(int e = 0; e < 100; ++e) {
            int imgClass = c;
            string imgNumber;
            std::cout << std::to_string(e) << std::endl;
            if(e < 10) {
                imgNumber = "00" + std::to_string(e);
            } else {
                imgNumber = "0" + std::to_string(e);
            }
            string file = in + std::to_string(imgClass) + "_" + imgNumber + ".bmp";
            std::cout << file << std::endl;
            Mat img = generateImage(file);
            vector<uchar> pixels;
            output << imgClass << ":";            
            string values = "";
            for(int x = 0; x < img.cols; ++x) {
                for(int y = 0; y < img.rows; ++y) {
                    //pixels.push_back(img.at<uchar>(x,y));
                    values = values + std::to_string((int)img.at<uchar>(x,y)) + ",";
                }
            }
            values.pop_back();
            output << values << endl;
        }
    }

}


//Kernal is an n by n sized vector of 1s or 0s
//Mode is 0 for dialation, 1 for erosion
Mat applyOperator(Mat &img, vector<vector<int> > &kernel, int mode) {
    int shift = (kernel.size() - 1) / 2;
    cv::Mat output(img.size(), img.type());
    for(int x = 0; x < img.rows; ++x) {
        for(int y = 0; y < img.cols; ++y) {
            vector<uchar> pixels;
            for(int i = 0; i < kernel.size(); ++i) {
                for(int j = 0; j < kernel.size(); ++j) {
                    if(x + i - shift >= img.rows || y + j - shift >= img.cols ||
                       x + i - shift < 0 || y + j - shift < 0) {
                        continue;
                    }
                    if(kernel[i][j] == 1) {
                        pixels.push_back(img.at<uchar>(x + i - shift, y + j - shift));
                    }
                }
            }
            if(mode == 0) {
                auto it = std::max_element(pixels.begin(), pixels.end());
                output.at<uchar>(x, y) = (*it);
            } else {
                auto it = std::min_element(pixels.begin(), pixels.end());
                output.at<uchar>(x, y) = (*it);
            }
        }
    }
    return output; 
}


void showImage(string name, Mat& img) {
    namedWindow(name);
    imshow(name, img);
}
