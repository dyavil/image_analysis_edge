#include <opencv2/opencv.hpp>
#include "convolution.hpp"
#include "fonctions.hpp"
#include <iostream>
#include <string>

using namespace cv;


int main(int argc, char ** argv){

    // Variables
    cv::Mat imgTest = cv::imread("./data/test.png", 1);
    cv::Mat lenaGray = cv::imread("./data/lenaGray.jpg", 0);
    cv::Mat lenaRGB = cv::imread("./data/lenaRGB.jpg", 1);
    cv::Mat imgDst1;
    cv::Mat imgDst2;
    cv::Mat imgDst3;
    cv::Mat imgDst4;
    
    // Convolution
    std::vector<std::vector<float>> filter = { {1, 1, 1}, {0, 0, 0}, {-1, -1, -1} };
    Convolution conv(filter, 3, 3);
    
    // Traitements
    imgDst1 = conv.applyToRGB(lenaRGB);
    imgDst2 = detectContours(lenaRGB, rgb);
    imgDst3 = detectContours(lenaGray, gray);
    imgDst4 = hysteresis(imgDst3, 80, 100);
    
    // Affichage
    //cv::namedWindow("filtre_vertical", cv::WINDOW_AUTOSIZE);
    //cv::imshow("filtre_vertical", imgDst1);
    
    cv::namedWindow("contoursRGB", cv::WINDOW_AUTOSIZE);
    cv::imshow("contoursRGB", imgDst2);
    
    cv::namedWindow("contoursGray", cv::WINDOW_AUTOSIZE);
    cv::imshow("contoursGray", imgDst3);
    
    cv::namedWindow("hysteresisGray", cv::WINDOW_AUTOSIZE);
    cv::imshow("hysteresisGray", imgDst4);
    
    cv::waitKey();

    return 0;
}
