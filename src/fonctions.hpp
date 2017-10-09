#ifndef FONCTIONS
#define FONCTIONS

#include <opencv2/opencv.hpp>
#include <cmath>

enum Color { gray = 0, rgb };   // Image en niveau de gris ou couleurs


// Detecte les contours d'un image
cv::Mat detectContours(cv::Mat & img, Color col) {
    
    cv::Mat imgV, imgH;
    cv::Mat ret = img.clone();
    
    std::vector<std::vector<float>> verticalFilter = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
    std::vector<std::vector<float>> horizontalFilter = { {1, 1, 1}, {0, 0, 0}, {-1, -1, -1} };
        
    Convolution convV(verticalFilter, 3, 3);
    Convolution convH(horizontalFilter, 3, 3);
    
    switch(col) {
    
        case gray:
            imgV = convV.applyToGray(img);
            imgH = convH.applyToGray(img);
            
           for(int x = 1; x < img.rows - 1; x++) {
                for(int y = 1; y < img.cols - 1; y++) {
                    ret.at<uchar>(x, y) = uchar(std::max(imgV.at<uchar>(x, y), imgH.at<uchar>(x, y)));
                }
            } 
            
            break;
            
        case rgb:   
            imgV = convV.applyToRGB(img);
            imgH = convH.applyToRGB(img);
    
            for(int x = 1; x < img.rows - 1; x++) {
                for(int y = 1; y < img.cols - 1; y++) {
                    cv::Vec3b pixV = imgV.at<cv::Vec3b>(x, y);
                    cv::Vec3b pixH = imgH.at<cv::Vec3b>(x, y);
                
                    int red = std::max(pixV.val[0], pixH.val[0]);
                    int green = std::max(pixV.val[1], pixH.val[1]);
                    int blue = std::max(pixV.val[2], pixH.val[2]);
                    
                    ret.at<cv::Vec3b>(x, y) = cv::Vec3b(red, green, blue);
                }
            }     
        
            break;
    }
    
    return ret;
}

#endif // FONCTIONS
