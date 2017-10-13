#include "fonctions.hpp"

// Charge une image dans une matrice de floats
cv::Mat loadImg(std::string path) {
    cv::Mat ret = cv::imread(path, 1);
    ret.convertTo(ret, CV_32FC3);
    return ret;
}

// Affiche la matrice dans une fenêtre
void showImg(const cv::Mat & img, std::string name, bool convert) {
    cv::Mat temp = img.clone();
    if(convert) { img.convertTo(temp, CV_8UC1); }
    cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
    cv::imshow(name, temp);
}

// Convertit une image en niveau de gris
void convertToGrayScale(cv::Mat & img) {
    cv::Mat temp(img.rows, img.cols, CV_32FC1);
    
    for(int y = 0; y < img.rows; ++y) {
        for(int x = 0; x < img.cols; ++x) {
            cv::Vec3f pixel = img.at<cv::Vec3f>(y, x);
            float blue = pixel.val[0];
            float green = pixel.val[1];
            float red = pixel.val[2];
            temp.at<float>(y, x) = uchar(0.07*blue + 0.72*green + 0.21*red);
        }
    }
    
    img = temp;
}

// Detecte les contours d'un image
void detectContours(cv::Mat & img, Filter method) {   
    int nbChannels = img.channels();
    cv::Mat imgH = img.clone();
    cv::Mat imgV = img.clone();
    
    // TODO : Ré-ajouter les fontions thin de Livaï /!\
    
    // TODO : Changer la methode de fusion ? abs > max ?
    
    // Filtres de Prewitt
    std::vector<std::vector<float>> horizontalPrewitt = { {1, 1, 1}, {0, 0, 0}, {-1, -1, -1} };
    std::vector<std::vector<float>> verticalPrewitt = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
    
    // Filtres de Sobel
    std::vector<std::vector<float>> horizontalSobel = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };
    std::vector<std::vector<float>> verticalSobel = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    
    // Convolutions 
    Convolution convHorizontalPrewitt(horizontalPrewitt, 3, 3);
    Convolution convVerticalPrewitt(verticalPrewitt, 3, 3);
    Convolution convHorizontalSobel(horizontalSobel, 3, 3);
    Convolution convVerticalSobel(verticalSobel, 3, 3);
    
    // Application des filtres horizontaux & verticaux
    switch(method) {
        case Sobel:
            convHorizontalSobel.apply(imgH);
            convVerticalSobel.apply(imgV);
            break;
            
        default:
            convHorizontalPrewitt.apply(imgH);
            convVerticalPrewitt.apply(imgV);
            break;
    }
    
    // Fusion des 2 images obtenues
    switch(nbChannels) {
        case 1: {
            for(int y = 0; y < img.rows; ++y) {
                for(int x = 0; x < img.cols; ++x) {
                    img.at<float>(y, x) = std::max(imgV.at<float>(y, x), imgH.at<float>(y, x));
                }
            }
            break;
        }
        case 2: {
            // Not implemented
            break;
        }
        default: {
        
            for(int y = 0; y < img.rows; ++y) {
                for(int x = 0; x < img.cols; ++x) {
                    cv::Vec3f pixV = imgV.at<cv::Vec3f>(y, x);
                    cv::Vec3f pixH = imgH.at<cv::Vec3f>(y, x);
                
                    float blue = std::max(pixV.val[2], pixH.val[2]);
                    float green = std::max(pixV.val[1], pixH.val[1]);
                    float red = std::max(pixV.val[0], pixH.val[0]);
                    
                    img.at<cv::Vec3f>(y, x) = cv::Vec3f(blue, green, red);
                }
            }
            break;
        }
    }
}

/*
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

// Réduit les contours
cv::Mat refineContours(const cv::Mat & img, int largeur) {
    cv::Mat imgV, imgH;
    cv::Mat ret = img.clone();
    
    std::vector<std::vector<float>> verticalRight = { {-1, 1, 0} };
    std::vector<std::vector<float>> horizontalUp = { {0}, {1}, {-1} };
    std::vector<std::vector<float>> verticalLeft = { {0, 1, -1} };
    std::vector<std::vector<float>> horizontalDown = { {-1}, {1}, {0} };
    
    Convolution convVR(verticalRight, 1, 3);
    Convolution convHU(horizontalUp, 3, 1);
    Convolution convVL(verticalLeft, 1, 3);
    Convolution convHD(horizontalDown, 3, 1);
    
    // On itère selon la largeur max des contours
    for(int i = 0; i < (largeur-1)/2; ++i) {
        // 1ère passe : supprime les pixels a droite/dessus
        imgV = convVR.applyToGray(ret, false);
        imgH = convHU.applyToGray(ret, false);
        
        for(int y = 0; y < ret.rows; ++y) {
            for(int x = 0; x < ret.cols; ++x) {
                ret.at<uchar>(y, x) = std::max(imgV.at<uchar>(y, x), imgH.at<uchar>(y, x));
            }
        }
        
        // 2ème passe : supprime les pixels a gauche/dessous
        imgV = convVL.applyToGray(ret, false);
        imgH = convHD.applyToGray(ret, false);
        
        for(int y = 0; y < ret.rows; ++y) {
            for(int x = 0; x < ret.cols; ++x) {
                ret.at<uchar>(y, x) = std::max(imgV.at<uchar>(y, x), imgH.at<uchar>(y, x));
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
*/
