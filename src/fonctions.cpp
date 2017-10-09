#include "fonctions.hpp"


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


/*
if(abs(tmp) > 5) res.at<uchar>(j, k) = 255;
else res.at<uchar>(j, k) = 0;

if(abs(tmp) > 130) res.at<uchar>(j, k) = 255;
else if(abs(tmp) > 100){
    for(int l = -1; l < 2; l++){
        for(int m = -1; m < 2; m++){
            if(j+l > 0 && m+l > 0){
                if (res.at<uchar>(j+l, k+l) > 200){
                    res.at<uchar>(j, k) = 255;
                    break;
                }
            } 
        }
    }
}
else res.at<uchar>(j, k) = 0;
*/
