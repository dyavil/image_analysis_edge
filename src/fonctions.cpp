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
void detectContours(cv::Mat & img, cv::Mat & pente, Filter method) {   
    int nbChannels = img.channels();
    cv::Mat imgH = img.clone();
    cv::Mat imgV = img.clone();
    pente = cv::Mat::ones(img.rows, img.cols, CV_32FC1);
    
    // TODO : Ré-ajouter les fontions thin de Livaï /!\
    
    // Filtres de Prewitt
    std::vector<std::vector<float>> horizontalPrewitt = { {1, 1, 1}, {0, 0, 0}, {-1, -1, -1} };
    std::vector<std::vector<float>> verticalPrewitt = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
    
    // Filtres de Sobel
    std::vector<std::vector<float>> horizontalSobel = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };
    std::vector<std::vector<float>> verticalSobel = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    
    // Convolutions
    Convolution convHorizontalPrewitt(horizontalPrewitt);
    Convolution convVerticalPrewitt(verticalPrewitt);
    Convolution convHorizontalSobel(horizontalSobel);
    Convolution convVerticalSobel(verticalSobel);
    
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
            float max = 0;
            float min = 0;
            for(int y = 0; y < img.rows; ++y) {
                for(int x = 0; x < img.cols; ++x) {
                    img.at<float>(y, x) = sqrt(imgV.at<float>(y, x)*imgV.at<float>(y, x) + imgH.at<float>(y, x)*imgH.at<float>(y, x));

                    //calul pente
                    if(imgV.at<float>(y, x) == 0) {
                        pente.at<float>(y, x) = -PI/2;
                    }
                    else if(imgH.at<float>(y, x) == 0) pente.at<float>(y, x) = 0;
                    else {
                        pente.at<float>(y, x) = (atan(imgH.at<float>(y, x)/imgV.at<float>(y, x)));
                    }
                    pente.at<float>(y, x) = pente.at<float>(y, x)* 180/PI;
                    if(pente.at<float>(y, x) > max) max = pente.at<float>(y, x);
                    if(pente.at<float>(y, x) < min) min = pente.at<float>(y, x);

                }
            }
            std::cout << "max " << max << ", min " << min << std::endl;
            computePenteRange(pente);
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
                
                    float blue = sqrt(pixV.val[0]*pixV.val[0] + pixH.val[0]*pixH.val[0]);
                    float green = sqrt(pixV.val[1]*pixV.val[1] + pixH.val[1]*pixH.val[1]);
                    float red = sqrt(pixV.val[2]*pixV.val[2] + pixH.val[2]*pixH.val[2]);
                    
                    img.at<cv::Vec3f>(y, x) = cv::Vec3f(blue, green, red);
                }
            }
            break;
        }
    }
}

// Calcule une orientation générale du gradian
void computePenteRange(cv::Mat & pente){
    for (int i = 0; i < pente.rows; ++i)
    {
        for (int j = 0; j < pente.cols; ++j)
        {
            float tmp = pente.at<float>(i, j);
            /*
            if(tmp >=0 && tmp < 45) pente.at<float>(i, j) = 0;
            else if(tmp >=45 && tmp < 90) pente.at<float>(i, j) = 30;
            else if(tmp >=90 && tmp < 135) pente.at<float>(i, j) = 60;
            else if(tmp >=135 && tmp <= 180) pente.at<float>(i, j) = 90;
            else if(tmp >=-45 && tmp < 0) pente.at<float>(i, j) = 120;
            else if(tmp >=-90 && tmp < -45) pente.at<float>(i, j) = 150;
            else if(tmp >=-135 && tmp < -90) pente.at<float>(i, j) = 180;
            else if(tmp >=-180 && tmp < -135) pente.at<float>(i, j) = 210;
            */
            //moin efficace....va savoir pk
            if(tmp >=-22 && tmp < 23) pente.at<float>(i, j) = 0;
            else if(tmp >=23 && tmp < 67) pente.at<float>(i, j) = 30;
            else if(tmp >=67 && tmp < 112) pente.at<float>(i, j) = 60;
            else if(tmp >=112 && tmp < 158) pente.at<float>(i, j) = 90;
            else if(tmp >=158 || tmp < -158) pente.at<float>(i, j) = 120;
            else if(tmp >=-158 && tmp < -112) pente.at<float>(i, j) = 150;
            else if(tmp >=-112 && tmp < -67) pente.at<float>(i, j) = 180;
            else if(tmp >=-67 && tmp < -22) pente.at<float>(i, j) = 210;
            else std::cout << "zarb" << std::endl;
        }
    }
}


// Indique si un pixel d'une image binaire possède un voisin
bool hasNeighbor(const cv::Mat & img, float seuil, unsigned int x, unsigned int y) {
    for(int j = -1; j < 2; ++j) {
        for(int i = -1; i < 2; ++i) {
            if(!(i == 0 && j == 0)) {
                if(0 <= x+i && x+i < img.cols && 0 <= y+j && y+j < img.rows) {
                    if(img.at<float>(y+j, x+i) > seuil){
                        return true;
                    }
                }
            }
        }        
    }

    return false;
} 


// Applique l'hystérésis sur une image en niveaux de gris
cv::Mat hysteresis(const cv::Mat & img, float seuilBas, float seuilHaut) {
    if(seuilBas > seuilHaut) {
        std::cout << "Filtre bas inférieur au filtre haut" << std::endl;
        std::exit(1);
    }

    cv::Mat ret = img.clone();
    
    for(int x = 0; x < img.rows; x++) {
        for(int y = 0; y < img.cols; y++) {
            if(img.at<float>(x, y) > seuilHaut) { 
                ret.at<float>(x, y) = 255; 
            } else if(img.at<float>(x, y) < seuilBas) {
                ret.at<float>(x, y) = 0;
            } else {
                if(hasNeighbor(ret, seuilHaut, x, y)) {
                    ret.at<float>(x, y) = 255;
                } else {
                    ret.at<float>(x, y) = 0;
                }
            }
        }
    }
    
    return ret;
}


cv::Mat autoHysteresis(const cv::Mat & img){    
    cv::Mat mean, deviation;
    cv::Mat ret = img.clone();
    meanStdDev(img, mean, deviation);
    ret = hysteresis(img, deviation.at<double>(0, 0), deviation.at<double>(0, 0)+30);
    return ret;
}


cv::Mat seuilGlob(const cv::Mat & img, int type){
    cv::Mat mean, deviation;
    meanStdDev(img, mean, deviation);
    cv::Mat ret = img.clone();
    double val;
    if(type == 0) val = deviation.at<double>(0, 0);
    else val = mean.at<double>(0, 0);
    for(int x = 0; x < img.rows; x++) {
        for(int y = 0; y < img.cols; y++) {
            if(img.at<float>(x, y) >= val) { 
                ret.at<float>(x, y) = 255; 
            }else {
                ret.at<float>(x, y) = 0; 
            }
        }
    }
    
    return ret;
}


cv::Mat seuilLoc(const cv::Mat & img, int n){
    cv::Mat ret = img.clone();
    for(int x = n/2; x < img.rows-n/2; x++) {
        for(int y = n/2; y < img.cols-n/2; y++) {
            float tot = 0;
            for (int i = -n/2; i <= n/2; ++i)
            {
                for (int j = -n/2; j <= n/2; ++j)
                {
                    tot += img.at<float>(x+i, y+j);
                }
            }
            //std::cout << x << " " << y << std::endl;
            float val = tot/(n*n);
            if(img.at<float>(x, y) >= val) { 
                ret.at<float>(x, y) = 255; 
            }else {
                ret.at<float>(x, y) = 0; 
            }
        }
    }
    return ret;
}


// Réduit les contours
void refineContours(cv::Mat & img, bool diagonales, cv::Mat pente) {
    std::vector<std::vector<float>> verticalRight = { {-1, 1, 0} };
    std::vector<std::vector<float>> horizontalUp = { {0}, {1}, {-1} };
    std::vector<std::vector<float>> verticalLeft = { {0, 1, -1} };
    std::vector<std::vector<float>> horizontalDown = { {-1}, {1}, {0} };
    std::vector<std::vector<float>> diag1 = { {-1, 0, 0}, {0, 1, 0}, {0, 0, 0} };
    std::vector<std::vector<float>> diag2 = { {0, 0, -1}, {0, 1, 0}, {0, 0, 0} };
    
    Convolution convVR(verticalRight);
    Convolution convHU(horizontalUp);
    Convolution convVL(verticalLeft);
    Convolution convHD(horizontalDown);
    Convolution convDiag1(diag1);
    Convolution convDiag2(diag2);
    
    int largeurMax = 6;     // Largeur max des contours
    
    // On itère selon la largeur max des contours
    for(int i = 0; i < largeurMax/2; ++i) {

        // 1ère passe : supprime les pixels a droite/dessus
        img = autoHysteresis(img);  
        
        cv::Mat imgVR = img.clone();
        cv::Mat imgHU = img.clone();
        
        convVR.apply(imgVR);
        convHU.apply(imgHU);
        
        for(int y = 0; y < img.rows; ++y) {
            for(int x = 0; x < img.cols; ++x) {
                img.at<float>(y, x) = std::max(imgVR.at<float>(y, x), imgHU.at<float>(y, x));
            }
        }
        
        // 2ème passe : supprime les pixels a gauche/dessous
        img = autoHysteresis(img);
        
        cv::Mat imgVL = img.clone();
        cv::Mat imgHD = img.clone();
        
        convVL.apply(imgVL);
        convHD.apply(imgHD);
        
        for(int y = 0; y < img.rows; ++y) {
            for(int x = 0; x < img.cols; ++x) {
                img.at<float>(y, x) = std::max(imgVL.at<float>(y, x), imgHD.at<float>(y, x));
            }
        }
        
        // 3ème passe : supprime les pixels sur les diagonales
        if(diagonales) {
            img = autoHysteresis(img);
            for(int y = 0; y < img.rows; ++y) {
                for(int x = 0; x < img.cols; ++x) {
                    if(pente.at<float>(y, x) == 210) {  // <-- TODO: Changer la valeur
                        convDiag1.applyToPixel(img, y, x);
                    }
                }
            }
            
            img = autoHysteresis(img);
            for(int y = 0; y < img.rows; ++y) {
                for(int x = 0; x < img.cols; ++x) {
                    if(pente.at<float>(y, x) == 30) {   // <-- TODO: Changer la valeur
                        convDiag2.applyToPixel(img, y, x);
                    }
                }
            }
        }
    }
}


// Supprime les pixels solitaires
void suppressAlonePixels(cv::Mat img) {
    cv::Mat temp = img.clone();

    for(int y = 0; y < img.rows; ++y) {
        for(int x = 0; x < img.cols; ++x) {
        
            if(!hasNeighbor(img, 254, x, y)) {
                
                img.at<float>(y, x) = 0;            
            }
            
        
        }
    }
    
    img = temp;
}


// Réduit les contours en fonction de leur pente
cv::Mat ThinAll(const cv::Mat & img, const cv::Mat & pente){
    cv::Mat ret = img.clone();
        int count = 0;
        int count2 = 0;
        int count3 = 0;
        for(int i = 1; i < img.rows-1; ++i) {
            for(int j = 1; j < img.cols-1; ++j) {
                if(img.at<float>(i, j) < 0) std::cout << "below" << std::endl;
                float n1 = pente.at<float>(i-1, j-1);
                float n2 = pente.at<float>(i-1, j);
                float n3 = pente.at<float>(i-1, j+1);
                float n4 = pente.at<float>(i, j-1);
                float n5 = pente.at<float>(i, j+1);
                float n6 = pente.at<float>(i+1, j-1);
                float n7 = pente.at<float>(i+1, j);
                float n8 = pente.at<float>(i+1, j+1);
                //test pente puis module
                count2++;
                if(pente.at<float>(i, j) == n1) {
                    if(img.at<float>(i-1, j-1) >= img.at<float>(i, j)) {
                        ret.at<float>(i, j)=0;
                        count3++;
                    }
                } 
                else if(pente.at<float>(i, j) == n2){
                    if(img.at<float>(i-1, j) >= img.at<float>(i, j)){
                        ret.at<float>(i, j)=0;
                        count3++;
                    } 
                } 
                else if(pente.at<float>(i, j) == n3){
                    if(img.at<float>(i-1, j+1) >= img.at<float>(i, j)) ret.at<float>(i, j)=0;
                } 
                else if(pente.at<float>(i, j) == n4){
                    if(img.at<float>(i, j-1) >= img.at<float>(i, j)) ret.at<float>(i, j)=0;
                } 
                else if(pente.at<float>(i, j) == n5){
                    if(img.at<float>(i, j+1) >= img.at<float>(i, j)) ret.at<float>(i, j)=0;
                } 
                else if(pente.at<float>(i, j) == n6){
                    if(img.at<float>(i+1, j-1) >= img.at<float>(i, j)) ret.at<float>(i, j)=0;
                }
                else if(pente.at<float>(i, j) == n7){
                    if(img.at<float>(i+1, j) >= img.at<float>(i, j)) ret.at<float>(i, j)=0;
                } 
                else if(pente.at<float>(i, j) == n8) {
                    if(img.at<float>(i+1, j+1) >= img.at<float>(i, j)) ret.at<float>(i, j)=0;
                }
                else count++;
            }
        }
         //std::cout << count2 << " count " << count << std::endl;
        return ret;
}

cv::Mat makeChain(const cv::Mat & img, const cv::Mat & pente){
    cv::Mat ret = img.clone();
    for(int i = 1; i < img.rows-1; ++i) {
        for(int j = 1; j < img.cols-1; ++j) {
            if(img.at<float>(i, j) != 0) {
                float topRightNeighbor = ret.at<float>(i-1, j+1);
                float middleRightNeighbor = ret.at<float>(i, j+1);
                float bottomRightNeighbor = ret.at<float>(i+1, j+1);
                float bottomMiddleNeighbor = ret.at<float>(i+1, j);
                float bottomLeftNeighbor = ret.at<float>(i+1, j-1);
                float middleLeftNeighbor = ret.at<float>(i, j-1);
                float topLeftNeighbor = ret.at<float>(i-1, j-1);
                float topMiddleNeighbor = ret.at<float>(i-1, j);
                if(pente.at<float>(i, j) == 0 || pente.at<float>(i, j) == 120){
                    if((middleLeftNeighbor >= 90 || topLeftNeighbor >= 90 || bottomLeftNeighbor >= 90) 
                        && (middleRightNeighbor >= 90 || topRightNeighbor >= 90 || bottomRightNeighbor >= 90)) ret.at<float>(i, j) = 90;
                }
                else if(pente.at<float>(i, j) == 30 || pente.at<float>(i, j) == 150){                  
                    if((topMiddleNeighbor >= 90 || topLeftNeighbor >= 90 || topRightNeighbor >= 90) 
                        && (bottomLeftNeighbor >= 90 || bottomMiddleNeighbor >= 90 || bottomRightNeighbor >= 90)) ret.at<float>(i, j) = 90;
                }

            }
        }
    }
    cv::Mat tret = ret.clone();
    for(int i = 1; i < ret.rows-1; ++i) {
        for(int j = 1; j < ret.cols-1; ++j) {
            float n1 = ret.at<float>(i-1, j+1);
            float n2 = ret.at<float>(i, j+1);
            float n3 = ret.at<float>(i+1, j+1);
            float n4 = ret.at<float>(i+1, j);
            float n5 = ret.at<float>(i+1, j-1);
            float n6 = ret.at<float>(i, j-1);
            float n7 = ret.at<float>(i-1, j-1);
            float n8 = ret.at<float>(i-1, j);
            if((n1+n2+n3+n4+n5+n6+n7+n8) < 1) tret.at<float>(i, j) = 0;
        }
    }

    return tret;
}



cv::Mat fillContours(const cv::Mat & img, const cv::Mat & pente){
    cv::Mat ret = img.clone();
    for(int i = 1; i < img.rows-1; ++i) {
            for(int j = 0; j < img.cols-1; ++j) {
                if(img.at<float>(i, j) != 0) {
                    float topRightNeighbor = img.at<float>(i-1, j+1);
                    float middleRightNeighbor = img.at<float>(i, j+1);
                    float bottomRightNeighbor = img.at<float>(i+1, j+1);
                    float bottomLeftNeighbor = img.at<float>(i+1, j-1);
                    float bottomMiddleNeighbor = img.at<float>(i+1, j);

                    if(j+2 < img.cols && i-2 > 0 && i+2 < img.rows && j-2 > 0){
                        if (((pente.at<float>(i, j) >= 30 && pente.at<float>(i, j) <= 60) || (pente.at<float>(i, j) >= 150 && pente.at<float>(i, j) <= 180))){
                            if(topRightNeighbor < img.at<float>(i, j)){
                                float nextTopRightNeighbor = img.at<float>(i-2, j+2);
                                float nextMiddleRightNeighbor = img.at<float>(i-1, j+2);
                                float nextBottomRightNeighbor = img.at<float>(i, j+2);
                                if(nextTopRightNeighbor > topRightNeighbor || nextBottomRightNeighbor > topRightNeighbor ||nextMiddleRightNeighbor > topRightNeighbor)
                                ret.at<float>(i-1, j+1) = img.at<float>(i, j);
                            }
                            if(middleRightNeighbor < img.at<float>(i, j)){
                                float nextTopRightNeighbor = img.at<float>(i-1, j+2);
                                float nextMiddleRightNeighbor = img.at<float>(i, j+2);
                                float nextBottomRightNeighbor = img.at<float>(i+1, j+2);
                                if(nextTopRightNeighbor > middleRightNeighbor || nextBottomRightNeighbor > middleRightNeighbor ||nextMiddleRightNeighbor > middleRightNeighbor)
                                ret.at<float>(i, j+1) = img.at<float>(i, j);
                            }
                            if(bottomRightNeighbor < img.at<float>(i, j)){
                                float nextTopRightNeighbor = img.at<float>(i, j+2);
                                float nextMiddleRightNeighbor = img.at<float>(i+1, j+2);
                                float nextBottomRightNeighbor = img.at<float>(i+2, j+2);
                                if(nextTopRightNeighbor > bottomRightNeighbor || nextBottomRightNeighbor > bottomRightNeighbor ||nextMiddleRightNeighbor > bottomRightNeighbor)
                                ret.at<float>(i+1, j+1) = img.at<float>(i, j);
                            }
                        }else{
                            if(bottomLeftNeighbor < img.at<float>(i, j)){
                                float nextBottomLeftNeighbor = img.at<float>(i+2, j-2);
                                float nextBottomMiddleNeighbor = img.at<float>(i+2, j-1);
                                float nextBottomRightNeighbor = img.at<float>(i+2, j);
                                if(nextBottomLeftNeighbor > bottomLeftNeighbor || nextBottomMiddleNeighbor > bottomLeftNeighbor || nextBottomLeftNeighbor > bottomLeftNeighbor)
                                ret.at<float>(i+1, j-1) = img.at<float>(i, j);
                            }
                            if(bottomMiddleNeighbor < img.at<float>(i, j)){
                                float nextBottomLeftNeighbor = img.at<float>(i+2, j-1);
                                float nextBottomMiddleNeighbor = img.at<float>(i+2, j);
                                float nextBottomRightNeighbor = img.at<float>(i+2, j+1);
                                if(nextBottomLeftNeighbor > bottomMiddleNeighbor || nextBottomMiddleNeighbor > bottomMiddleNeighbor || nextBottomLeftNeighbor > bottomMiddleNeighbor)
                                ret.at<float>(i+1, j) = img.at<float>(i, j);
                            }
                            if(bottomRightNeighbor < img.at<float>(i, j)){
                                float nextBottomLeftNeighbor = img.at<float>(i+2, j);
                                float nextBottomMiddleNeighbor = img.at<float>(i+2, j+1);
                                float nextBottomRightNeighbor = img.at<float>(i+2, j+2);
                                if(nextBottomLeftNeighbor > bottomRightNeighbor || nextBottomMiddleNeighbor > bottomRightNeighbor || nextBottomLeftNeighbor > bottomRightNeighbor)
                                ret.at<float>(i+1, j+1) = img.at<float>(i, j);
                            }
                        }
                        
                        
                    }
                }
            }
        }
        for(int i = 1; i < ret.rows-1; ++i) {
            for(int j = 1; j < ret.cols-1; ++j) {
                if(ret.at<float>(i, j) > 80) ret.at<float>(i, j) = 255;
            }
        }

        return ret;
}
