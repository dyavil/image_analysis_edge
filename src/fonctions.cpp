#include "fonctions.hpp"

// Detecte les contours d'un image
cv::Mat detectContours(cv::Mat & img, Color col, bool thin) {
    
    cv::Mat imgV, imgH, imgVt, imgHt;
    cv::Mat ret = img.clone();
    
    // Filtre de Prewitt
    std::vector<std::vector<float>> verticalFilter = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
    std::vector<std::vector<float>> horizontalFilter = { {1, 1, 1}, {0, 0, 0}, {-1, -1, -1} };
        
    Convolution convV(verticalFilter, 3, 3);
    Convolution convH(horizontalFilter, 3, 3);
    
    switch(col) {
    
        case gray:
            if (thin)
            {
                imgVt = convV.applyToGray(img);
                ThinVertical(imgVt, imgV);
                imgHt = convH.applyToGray(img);
                ThinHorizontal(imgHt, imgH);
            }
            else {
                imgV = convV.applyToGray(img);
                imgH = convH.applyToGray(img);
            }
            
            for(int y = 0; y < img.rows; ++y) {
                for(int x = 0; x < img.cols; ++x) {
                    ret.at<uchar>(y, x) = uchar(std::max(imgV.at<uchar>(y, x), imgH.at<uchar>(y, x)));
                }
            } 
            
            break;
            
        case rgb:   
            imgV = convV.applyToRGB(img);
            imgH = convH.applyToRGB(img);
    
            for(int y = 0; y < img.rows; ++y) {
                for(int x = 0; x < img.cols; ++x) {
                    cv::Vec3b pixV = imgV.at<cv::Vec3b>(y, x);
                    cv::Vec3b pixH = imgH.at<cv::Vec3b>(y, x);
                
                    int blue = std::max(pixV.val[2], pixH.val[2]);
                    int green = std::max(pixV.val[1], pixH.val[1]);
                    int red = std::max(pixV.val[0], pixH.val[0]);
                    
                    ret.at<cv::Vec3b>(y, x) = cv::Vec3b(blue, green, red);
                }
            }     
        
            break;
    }
    
    return ret;
}

// Indique si un pixel d'une image binaire possède un voisin
bool hasNeighbor(const cv::Mat & img, uchar seuil, unsigned int x, unsigned int y) {
    for(int i = -1; i < 2; ++i){
        for(int j = -1; j < 2; ++j){
            if(0 <= x+i && x+i < img.cols && 0 <= y+j && y+j < img.rows) {
                if(img.at<uchar>(x+i, y+j) > seuil){
                    return true;
                }
            }
        }        
    }

    return false;
} 

// Applique l'hystérésis sur une image en niveaux de gris
cv::Mat hysteresis (const cv::Mat & img, uchar seuilBas, uchar seuilHaut) {

    assert(seuilBas <= seuilHaut);

    cv::Mat ret = img.clone();
    
    for(int x = 0; x < img.rows; x++) {
        for(int y = 0; y < img.cols; y++) {
            if(img.at<uchar>(x, y) > seuilHaut) { 
                ret.at<uchar>(x, y) = 255; 
            } else if(img.at<uchar>(x, y) < seuilBas) {
                ret.at<uchar>(x, y) = 0;
            } else {
                if(hasNeighbor(ret, seuilHaut, x, y)) {
                    ret.at<uchar>(x, y) = 255;
                } else {
                    ret.at<uchar>(x, y) = 0;
                }
            }
        }
    }
    
    return ret;
}


void ThinVertical(cv::Mat & pSrc, cv::Mat & pDst) {
        int rows = pSrc.rows;
        int cols = pSrc.cols;
        pDst = pSrc.clone();
        for(int i = 0; i < rows; i++) {
                for(int j = 0; j < cols; j++) {
                        if(pSrc.at<uchar>(i, j) != 0) {
                                /// 8 voisins
                                
                                int neighbor0 = (int) pSrc.at<uchar>(i-1, j-1);
                                int neighbor1 = (int) pSrc.at<uchar>(i-1, j);
                                int neighbor2 = (int) pSrc.at<uchar>(i-1, j+1);
                                int neighbor3 = (int) pSrc.at<uchar>(i, j+1);;
                                int neighbor4 = (int) pSrc.at<uchar>(i+1, j+1);;
                                int neighbor5 = (int) pSrc.at<uchar>(i+1, j);;
                                int neighbor6 = (int) pSrc.at<uchar>(i+1, j-1);;
                                int neighbor7 = (int) pSrc.at<uchar>(i, j-1);;
                                int c1 = pSrc.at<uchar>(i, j) - neighbor7;
                                int c2 = pSrc.at<uchar>(i, j) - neighbor3;
                                if(c1 < 0 || c2 < 0)  {
                                    pDst.at<uchar>(i, j) = 0;
                                    //std::cout << "test" << std::endl;
                                }
                        }
                }
        }
} 

void ThinHorizontal(cv::Mat & pSrc, cv::Mat & pDst) {
        int rows = pSrc.rows;
        int cols = pSrc.cols;
        pDst = pSrc.clone();
        for(int i = 0; i < rows; i++) {
                for(int j = 0; j < cols; j++) {
                        if(pSrc.at<uchar>(i, j) != 0) {
                                /// 8 voisins
                                
                                int neighbor0 = (int) pSrc.at<uchar>(i-1, j-1);
                                int neighbor1 = (int) pSrc.at<uchar>(i-1, j);
                                int neighbor2 = (int) pSrc.at<uchar>(i-1, j+1);
                                int neighbor3 = (int) pSrc.at<uchar>(i, j+1);;
                                int neighbor4 = (int) pSrc.at<uchar>(i+1, j+1);;
                                int neighbor5 = (int) pSrc.at<uchar>(i+1, j);;
                                int neighbor6 = (int) pSrc.at<uchar>(i+1, j-1);;
                                int neighbor7 = (int) pSrc.at<uchar>(i, j-1);;
                                int c1 = pSrc.at<uchar>(i, j) - neighbor1;
                                int c2 = pSrc.at<uchar>(i, j) - neighbor5;
                                if(c1 < 0 || c2 < 0)  {
                                    pDst.at<uchar>(i, j) = 0;
                                    //std::cout << "test" << std::endl;
                                }
                        }
                }
        }
} 