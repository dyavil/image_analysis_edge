#include <opencv2/highgui/highgui.hpp>
#include "fonctions.hpp"

int main(int argc, char ** argv){

    // Variables
    cv::Mat imgTest = cv::imread("./data/test.png", 1);
    cv::Mat lenaGray = cv::imread("./data/lenaGray.jpg", 0);
    cv::Mat lenaRGB = cv::imread("./data/lenaRGB.jpg", 1);
    cv::Mat imgDst1;
    cv::Mat imgDst2;
    cv::Mat imgDst3;
    cv::Mat imgDst4;
    
    // Traitements
    imgDst1 = detectContours(imgTest, rgb);
    imgDst2 = detectContours(lenaRGB, rgb);
    imgDst3 = detectContours(lenaGray, gray);
    imgDst4 = hysteresis(imgDst3, 80, 100);
    
    // Affichage
    cv::namedWindow("test", cv::WINDOW_AUTOSIZE);
    cv::imshow("test", imgDst1);
    
    cv::namedWindow("contoursRGB", cv::WINDOW_AUTOSIZE);
    cv::imshow("contoursRGB", imgDst2);
    
    cv::namedWindow("contoursGray", cv::WINDOW_AUTOSIZE);
    cv::imshow("contoursGray", imgDst3);
    
    cv::namedWindow("hysteresisGray", cv::WINDOW_AUTOSIZE);
    cv::imshow("hysteresisGray", imgDst4);
    
    cv::waitKey();

    return 0;
}
