#include <opencv2/highgui/highgui.hpp>
#include "fonctions.hpp"

int main(int argc, char ** argv) {

    // Variables
    cv::Mat imgTest = cv::imread("./data/test.png", 1);
    cv::Mat lenaGray = cv::imread("./data/lenaGray.jpg", 0);
    cv::Mat lenaRGB = cv::imread("./data/lenaRGB.jpg", 1);
    cv::Mat imgDst1, imgDst2, imgDst3, imgDst4, imgDst5, imgDst6, imgDst7, imgDst8, imgDst9;
    
    // Traitements
    imgDst1 = detectContours(imgTest, rgb);
    imgDst2 = detectContours(lenaRGB, rgb);
    imgDst3 = convertToGrayScale(imgDst2);
    imgDst4 = hysteresis(imgDst3, 8, 12);
    imgDst5 = detectContours(lenaGray, gray, false);
    imgDst6 = hysteresis(imgDst5, 80, 100);
    imgDst7 = detectContours(lenaGray, gray, true);
    imgDst8 = hysteresis(imgDst7, 80, 100);
    imgDst9 = refineContours(imgDst6);
    
    // Affichage
    //cv::namedWindow("test", cv::WINDOW_AUTOSIZE);
    //cv::imshow("test", imgDst1);
    
    //cv::namedWindow("contoursRGB", cv::WINDOW_AUTOSIZE);
    //cv::imshow("contoursRGB", imgDst2);
    
    //cv::namedWindow("contoursRGBtoGray", cv::WINDOW_AUTOSIZE);
    //cv::imshow("contoursRGBtoGray", imgDst3);
    
    //cv::namedWindow("hysteresisRGB", cv::WINDOW_AUTOSIZE);
    //cv::imshow("hysteresisRGB", imgDst4);
    
    //cv::namedWindow("contoursGray", cv::WINDOW_AUTOSIZE);
    //cv::imshow("contoursGray", imgDst5);
    
    //cv::namedWindow("contoursGrayThin", cv::WINDOW_AUTOSIZE);
    //cv::imshow("contoursGrayThin", imgDst7);
    
    cv::namedWindow("hysteresisGray", cv::WINDOW_AUTOSIZE);
    cv::imshow("hysteresisGray", imgDst6);

    cv::namedWindow("hysteresisGrayThin", cv::WINDOW_AUTOSIZE);
    cv::imshow("hysteresisGrayThin", imgDst8);
    
    cv::namedWindow("hysteresisGrayRefined", cv::WINDOW_AUTOSIZE);
    cv::imshow("hysteresisGrayRefined", imgDst9);
    
    cv::waitKey();

    return 0;
    
}
