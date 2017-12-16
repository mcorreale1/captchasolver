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

Mat applyOperator(Mat &img, vector<vector<int> > &kernel, int mode);
void generateData(string in, string out);
vector<ImageData> generateTraining(char* dataIn, char* out);
Mat generateImage(string path);
float sigmoid(float z);
void showImage(string name, Mat& img);
void applyTraining(vector<ImageData> &data);
int main(int argc, char **argv) {
    //generateData(argv[1], argv[2]);
    generateTraining(argv[1], argv[2]);
}

void applyTraining(vector<ImageData> &data) {
    std::vector<std::vector<int> > theta(10, std::vector<int>(1, VECTOR_SIZE));

}

vector<ImageData> generateTraining(char* dataIn, char* out) {
    ifstream input(dataIn);
    ifstream output(out);
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
        Mat img(cv::Size(IMAGE_WIDTH,IMAGE_HEIGHT), 0);
        for(int x = 0; x < img.rows; ++x) {
            for(int y = 0; y < img.cols; ++y) {
                img.at<uchar>(x,y) = (uchar)id.pixels[x*(IMAGE_WIDTH) + y];
            }
        }
        sizeIndex++;
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
            /*
            std::cout << "HERE:  " <<  img.type() << std::endl;
        Mat img2(cv::Size(IMAGE_WIDTH,IMAGE_HEIGHT), 0);
        for(int x = 0; x < img2.rows; ++x) {
            for(int y = 0; y < img2.cols; ++y) {
                img2.at<uchar>(x,y) = pixels[x*100 + y];
            }
        }
        showImage("TEST", img2);
        waitKey();
        return;
        */
            values.pop_back();
            //return;
            output << values << endl;
            //data.push_back(ImageData{(Classes)imgClass, pixels});
        }
    }

}

float sigmoid(float z) {
    return (1.0 / (1.0 + exp(-z)));
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
