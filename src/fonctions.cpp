#include "fonctions.hpp"

// Convertit une image en niveau de gris
cv::Mat convertToGrayScale(cv::Mat & img) {
    cv::Mat ret(img.rows, img.cols, CV_8UC1);

    for(int y = 0; y < img.rows; ++y) {
        for(int x = 0; x < img.cols; ++x) {
            cv::Vec3b pixel = img.at<cv::Vec3b>(y, x);
            uchar blue = pixel.val[0];
            uchar green = pixel.val[1];
            uchar red = pixel.val[2];
            ret.at<uchar>(y, x) = uchar(0.07*blue + 0.72*green + 0.21*red);
        }
    }

    return ret;
}

// Detecte les contours d'un image
cv::Mat detectContours(cv::Mat & img, Color col, bool thin) {   
    cv::Mat imgV, imgH;
    cv::Mat ret = img.clone();
    
    // Filtre de Prewitt
    std::vector<std::vector<float>> verticalFilter = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
    std::vector<std::vector<float>> horizontalFilter = { {1, 1, 1}, {0, 0, 0}, {-1, -1, -1} };
        
    Convolution convV(verticalFilter, 3, 3);
    Convolution convH(horizontalFilter, 3, 3);
    
    switch(col) {
    
        case gray:
            imgV = convV.applyToGray(img);
            imgH = convH.applyToGray(img);
            
            if (thin) {
                imgV = ThinVertical(imgV);
                imgH = ThinHorizontal(imgH);
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

// Réduit les contours verticaux
cv::Mat ThinVertical(const cv::Mat & img) {
    cv::Mat ret = img.clone();
    
    for(int i = 0; i < img.rows; ++i) {
        for(int j = 0; j < img.cols; ++j) {
            if(img.at<uchar>(i, j) != 0) {
                uchar downNeighbor = img.at<uchar>(i, j-1);;
                uchar upNeighbor = img.at<uchar>(i, j+1);;
                int p1 = img.at<uchar>(i, j) - downNeighbor;
                int p2 = img.at<uchar>(i, j) - upNeighbor;
                if(p1 < 0 || p2 < 0)  {
                    ret.at<uchar>(i, j) = 0;
                }
            }
        }
    }
    
    return ret;
} 

// Réduit les contours horizontaux
cv::Mat ThinHorizontal(const cv::Mat & img) {
        cv::Mat ret = img.clone();
        
        for(int i = 0; i < img.rows; ++i) {
            for(int j = 0; j < img.cols; ++j) {
                if(img.at<uchar>(i, j) != 0) {
                    uchar leftNeighbor = img.at<uchar>(i-1, j);
                    uchar rightNeighbor = img.at<uchar>(i+1, j);;
                    int p1 = img.at<uchar>(i, j) - leftNeighbor;
                    int p2 = img.at<uchar>(i, j) - rightNeighbor;
                    if(p1 < 0 || p2 < 0)  {
                        ret.at<uchar>(i, j) = 0;
                    }
                }
            }
        }
        
        return ret;
} 
