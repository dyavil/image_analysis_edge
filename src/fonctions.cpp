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
    //std::cout << ", " << verticalPrewitt[0][1] << std::endl;
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
            float min = 500;

            float max1 = 0;
            float min1 = 500;
            int count1 = 0;
            for(int y = 0; y < img.rows; ++y) {
                for(int x = 0; x < img.cols; ++x) {
                    img.at<float>(y, x) = sqrt(imgV.at<float>(y, x)*imgV.at<float>(y, x) + imgH.at<float>(y, x)*imgH.at<float>(y, x));
                    int mtemp = 0;
                    if((abs(imgH.at<float>(y, x)) - imgV.at<float>(y, x)) > 0) mtemp = 1;
                    //calul pente
                    //if(imgV.at<float>(y, x) == 0 && imgH.at<float>(y, x) == 0) pente.at<float>(y, x) = ;
                    /*if(imgV.at<float>(y, x) == 0) {
                        pente.at<float>(y, x) = -PI/2;
                    }
                    else if(imgH.at<float>(y, x) == 0) pente.at<float>(y, x) = 0;
                    else {
                        pente.at<float>(y, x) = (atan2(imgH.at<float>(y, x), imgV.at<float>(y, x)));
                    }*/
                    //float p1 = (atan2(imgH.at<float>(y, x), imgV.at<float>(y, x)));
                    float p2 = (atan2(imgV.at<float>(y, x), imgH.at<float>(y, x)));
                    //p1 = p1 * (180/PI);
                    p2 = p2 * (180/PI);
                    //std::cout << p1 << ", " << p2 << std::endl;
                    pente.at<float>(y, x) = p2;
                    //else pente.at<float>(y, x) = p2;
                    //if(imgV.at<float>(y, x) == imgH.at<float>(y, x)) pente.at<float>(y, x) = 600;
                    //pente.at<float>(y, x) = pente.at<float>(y, x)* (180/PI);
                    if(pente.at<float>(y, x) > max) max = pente.at<float>(y, x);
                    if(pente.at<float>(y, x) < min) min = pente.at<float>(y, x);
                    if(p2 > max) max1 = p2;
                    if(p2 < min) min1 = p2;

                }
            }
            std::cout << "max " << max << ", min " << min << std::endl;
            std::cout << "max1 " << max1 << ", min1 " << min1 << std::endl;
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
            /*if(tmp >=0 && tmp < 45) pente.at<float>(i, j) = 0;
            else if(tmp >=45 && tmp < 90) pente.at<float>(i, j) = 30;
            else if(tmp >=90 && tmp < 135) pente.at<float>(i, j) = 60;
            else if(tmp >=135 && tmp <= 180) pente.at<float>(i, j) = 90;
            else if(tmp >=-45 && tmp < 0) pente.at<float>(i, j) = 120;
            else if(tmp >=-90 && tmp < -45) pente.at<float>(i, j) = 150;
            else if(tmp >=-135 && tmp < -90) pente.at<float>(i, j) = 180;
            else if(tmp >=-180 && tmp < -135) pente.at<float>(i, j) = 210;*/
            //moin efficace....va savoir pk
            if(tmp >=-22 && tmp < 23) pente.at<float>(i, j) = 10;
            else if(tmp >=23 && tmp < 67) pente.at<float>(i, j) = 30;
            else if(tmp >=67 && tmp < 112) pente.at<float>(i, j) = 60;
            else if(tmp >=112 && tmp < 158) pente.at<float>(i, j) = 90;
            else if(tmp >=158 || tmp < -158) pente.at<float>(i, j) = 120;
            else if(tmp >=-158 && tmp < -112) pente.at<float>(i, j) = 150;
            else if(tmp >=-112 && tmp < -67) pente.at<float>(i, j) = 180;
            else if(tmp >=-67 && tmp < -22) pente.at<float>(i, j) = 210;
            else std::cout << "zarb" << std::endl;;
        }
    }
}

void getRangeVal(float angle){

}



// Indique si un pixel d'une image binaire possède un voisin
bool hasNeighbor(const cv::Mat & img, float seuil, unsigned int x, unsigned int y) {
    for(int i = -1; i < 2; ++i){
        for(int j = -1; j < 2; ++j){
            if(0 <= x+i && x+i < img.rows && 0 <= y+j && y+j < img.cols) {
                if(!(i == 0 && j == 0)){
                    if(img.at<float>(x+i, y+j) > seuil){
                        return true;
                    }
                }
            }
        }        
    }

    return false;
} 


// Applique l'hystérésis sur une image en niveaux de gris
cv::Mat hysteresisS(const cv::Mat & img, float seuilBas, float seuilHaut) {
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
    ret = hysteresisS(img, deviation.at<double>(0, 0), deviation.at<double>(0, 0)+30);
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

cv::Mat newThin(const cv::Mat & img, const cv::Mat & pente, cv::Mat & hyst){
    cv::Mat ret = hyst.clone();
    for(int i = 1; i < img.rows-1; ++i) {
        for(int j = 1; j < img.cols-1; ++j) {
            if(img.at<float>(i, j) > 0){

                //valeur voisins
                float n1 = img.at<float>(i-1, j-1);
                float n2 = img.at<float>(i-1, j);
                float n3 = img.at<float>(i-1, j+1);
                float n4 = img.at<float>(i, j-1);
                float n5 = img.at<float>(i, j+1);
                float n6 = img.at<float>(i+1, j-1);
                float n7 = img.at<float>(i+1, j);
                float n8 = img.at<float>(i+1, j+1);

                //valeur locales
                float plocal = pente.at<float>(i, j);
                float local = img.at<float>(i, j);

                if(plocal == 10  || plocal == 120){
                    if(n2 >local) ret.at<float>(i, j) = 0;
                
                    if(n7 > local) ret.at<float>(i, j) = 0;
                    
                }else if(plocal == 30 || plocal == 150){
                    if(n3 > local) ret.at<float>(i, j) = 0;
                
                    if(n6 > local) ret.at<float>(i, j) = 0;
                }else if(plocal == 60 || plocal == 180){
                    if(n5 > local) ret.at<float>(i, j) = 0;
                    if(n4 > local) ret.at<float>(i, j) = 0;
                }
                else if(plocal == 90 || plocal == 210){
                    if(n1 > local) ret.at<float>(i, j) = 0;
                    if(n8 > local) ret.at<float>(i, j) = 0;
                }
            }
        }
    }
    return ret;
}


cv::Mat makeChain(const cv::Mat & img, const cv::Mat & pente){
    cv::Mat ret = img.clone();
    for(int i = 1; i < img.rows-1; ++i) {
        for(int j = 1; j < img.cols-1; ++j) {
            if(img.at<float>(i, j) > 0){
                float n1 = img.at<float>(i-1, j-1);
                float n2 = img.at<float>(i-1, j);
                float n3 = img.at<float>(i-1, j+1);
                float n4 = img.at<float>(i, j-1);
                float n5 = img.at<float>(i, j+1);
                float n6 = img.at<float>(i+1, j-1);
                float n7 = img.at<float>(i+1, j);
                float n8 = img.at<float>(i+1, j+1);

                //valeur locales
                float plocal = pente.at<float>(i, j);
                float local = img.at<float>(i, j);

                if((n1+n2+n3+n4+n5+n6+n7+n8) > 255) {
                    if(((n1+n2 )-(n3+n4+n5+n6+n7+n8)) > 255) ret.at<float>(i, j) = 255;
                    else if(((n3+n2 )-(n1+n4+n5+n6+n7+n8)) > 255) ret.at<float>(i, j) = 255;
                    else if(((n7+n6 )-(n1+n4+n5+n2+n3+n8)) > 255) ret.at<float>(i, j) = 255;
                    else if(((n7+n8 )-(n1+n4+n5+n6+n2+n3)) > 255) ret.at<float>(i, j) = 255;
                    else if(((n1+n4 )-(n2+n3+n5+n6+n7+n8)) > 255) ret.at<float>(i, j) = 255;
                    else if(((n3+n5 )-(n1+n2+n4+n6+n7+n8)) > 255) ret.at<float>(i, j) = 255;
                    else if(((n4+n6 )-(n1+n2+n3+n5+n7+n8)) > 255) ret.at<float>(i, j) = 255;
                    else if(((n5+n8 )-(n1+n2+n3+n4+n6+n7)) > 255) ret.at<float>(i, j) = 255;
                    else ret.at<float>(i, j) = 90;
                }
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


bool recurNext(const cv::Mat & seuil, const cv::Mat & img, cv::Mat & ret, int i, int j, int dir, int & profondeur){
    if(!(j+1 < ret.cols && i+1 < ret.rows)) return false;
    float n1 = img.at<float>(i-1, j-1);
    float n2 = img.at<float>(i-1, j);
    float n3 = img.at<float>(i-1, j+1);
    float n4 = img.at<float>(i, j-1);
    float n5 = img.at<float>(i, j+1);
    float n6 = img.at<float>(i+1, j-1);
    float n7 = img.at<float>(i+1, j);
    float n8 = img.at<float>(i+1, j+1);
    int iinext;
    int ijnext;
    float next = 0;
    if(dir == 60){
        //check n1, n2, n3
        next = n1;
        iinext = i-1;
        ijnext = j-1;
        if(n2 > next) {
            next = n2;                
            iinext = i-1;
            ijnext = j;
        }
        if(n3 > next){
            next = n3;            
            iinext = i-1;
            ijnext = j+1;

        } 

    }else if(dir == 90){
        //check n2, n3, n5
        next = n2;
        iinext = i-1;
        ijnext = j;
        if(n3 > next) {
            next = n3;                
            iinext = i-1;
            ijnext = j+1;
        }
        if(n5 > next){
            next = n5;            
            iinext = i;
            ijnext = j+1;

        } 
    }else if(dir == 120){
        //check n3 n5 n8
        next = n3;
        iinext = i-1;
        ijnext = j+1;
        if(n5 > next) {
            next = n5;                
            iinext = i;
            ijnext = j+1;
        }
        if(n8 > next){
            next = n8;            
            iinext = i+1;
            ijnext = j+1;

        } 
    }
    else if(dir == 150){
        //check n5 n8 n7
        next = n5;
        iinext = i;
        ijnext = j+1;
        if(n8 > next) {
            next = n8;                
            iinext = i+1;
            ijnext = j+1;
        }
        if(n7 > next){
            next = n7;            
            iinext = i+1;
            ijnext = j;

        } 
    }else if(dir == 180){
        //check n6, n7, n8
        next = n6;
        iinext = i+1;
        ijnext = j-1;
        if(n7 > next) {
            next = n7;                
            iinext = i+1;
            ijnext = j;
        }
        if(n8 > next){
            next = n8;            
            iinext = i+1;
            ijnext = j+1;

        } 
    }else if(dir == 210){
        //check n4 n6 n7
        next = n4;
        iinext = i;
        ijnext = j-1;
        if(n6 > next) {
            next = n6;                
            iinext = i+1;
            ijnext = j-1;
        }
        if(n7 > next){
            next = n7;            
            iinext = i+1;
            ijnext = j;

        } 
    }else if(dir == 10){
        //check n1 n4 n6
        next = n1;
        iinext = i-1;
        ijnext = j-1;
        if(n4 > next) {
            next = n4;                
            iinext = i;
            ijnext = j-1;
        }
        if(n6 > next){
            next = n6;            
            iinext = i+1;
            ijnext = j-1;

        } 
    }else if(dir == 30){
        //check n4 n1 n2
        next = n4;
        iinext = i;
        ijnext = j-1;
        if(n1 > next) {
            next = n1;                
            iinext = i-1;
            ijnext = j-1;
        }
        if(n2 > next){
            next = n2;            
            iinext = i-1;
            ijnext = j;

        } 
    }

    if(!(ijnext+1 < ret.cols && iinext+1 < ret.rows && ijnext > 0 && iinext > 0) || next < 15) return false;
    float s1 = seuil.at<float>(iinext-1, ijnext-1);
    float s2 = seuil.at<float>(iinext-1, ijnext);
    float s3 = seuil.at<float>(iinext-1, ijnext+1);
    float s4 = seuil.at<float>(iinext, ijnext-1);
    float s5 = seuil.at<float>(iinext, ijnext+1);
    float s6 = seuil.at<float>(iinext+1, ijnext-1);
    float s7 = seuil.at<float>(iinext+1, ijnext);
    float s8 = seuil.at<float>(iinext+1, ijnext+1);
    if(profondeur > 0 && (s1+s2+s3+s4+s5+s6+s7+s8) > 0) return true;
    else {
        profondeur++;
        
    }
    if(recurNext(seuil,img, ret, iinext, ijnext, dir, profondeur)) {
        ret.at<float>(iinext, ijnext) = 255;
        return true;
    }

    return false;
}


cv::Mat fillContours(const cv::Mat & img, const cv::Mat & pente, const cv::Mat & base){
    cv::Mat ret = img.clone();
    for(int i = 1; i < img.rows-1; ++i) {
        for(int j = 1; j < img.cols-1; ++j) {
            if(img.at<float>(i, j)  == 255){
                //valeur voisins

                //valeur locales
                float plocal = pente.at<float>(i, j);
                float local = img.at<float>(i, j);


                int p = 0;
                recurNext(img, base, ret, i, j, plocal, p);

            }
        }
    }
    for(int i = 1; i < ret.rows-1; ++i) {
        for(int j = 1; j < ret.cols-1; ++j){
            if(ret.at<float>(i, j) > 0) ret.at<float>(i, j) = 255;
        }
    }

    return ret;
}


// Supprime les pixels solitaires
void suppressAlonePixels(cv::Mat & img) {
    cv::Mat temp = img.clone();
    int count = 0;
    for(int y = 0; y < img.rows; ++y) {
        for(int x = 0; x < img.cols; ++x) {
            if(temp.at<float>(y, x) != 0){

                if(!hasNeighbor(img, 200, y, x)) {
                    
                    temp.at<float>(y, x) = 0; 
                    count++;        
                }
            }
       
        }
    }
    std::cout << count << std::endl;
    img = temp.clone();
}


cv::Mat showGradient(const cv::Mat & img){
    cv::Mat ret = cv::Mat::ones(img.rows, img.cols, CV_32FC3);

    cv::Mat mean, deviation;
    meanStdDev(img, mean, deviation);
    double val = deviation.at<double>(0, 0);
    for(int y = 0; y < img.rows; ++y) {
        for(int x = 0; x < img.cols; ++x) {
            float b, g, r;
            float plocal = img.at<float>(y, x);
            if(plocal > 0 && plocal < 32){
                g = 0.0;
                r = 180;
                b =0.0;
            }else if(plocal >= 32 && plocal < 64){
                g = 80.0;
                r = 80;
                b =0.0;
            }else if(plocal >= 64 && plocal < 92){
                g = 180.0;
                r = 0;
                b =0.0;
            }else if(plocal >= 92 && plocal < 121){
                g = 80.0;
                r = 0;
                b = 80.0;
            }else if(plocal >= 121 && plocal < 152){
                g = 0.0;
                r = 0;
                b = 180.0;
            }else if(plocal >= 152 && plocal < 184){
                g = 80.0;
                r = 80;
                b = 80.0;
            }else if(plocal >= 184 && plocal < 216){
                g = 10.0;
                r = 100;
                b =10.0;
            }else if(plocal >= 216){
                g = 100.0;
                r = 10;
                b =100.0;
            }
            if(img.at<float>(y, x) > val) ret.at<cv::Vec3f>(y, x) = cv::Vec3f(b, g, r);

        }
    }

    return ret;
}


cv::Mat showPente(const cv::Mat & img, const cv::Mat & pente){
    cv::Mat ret = cv::Mat::ones(img.rows, img.cols, CV_32FC3);
    cv::Mat mean, deviation;
    meanStdDev(img, mean, deviation);
    double val = deviation.at<double>(0, 0);
    for(int y = 0; y < img.rows; ++y) {
        for(int x = 0; x < img.cols; ++x) {
            float b, g, r;
            float plocal = pente.at<float>(y, x);
            if(plocal == 10){
                r = 0.0;
                g = 180;
                b =0.0;
            }else if(plocal == 30){
                r = 80.0;
                g = 80;
                b =0.0;
            }else if(plocal == 60){
                r = 180.0;
                g = 0;
                b =0.0;
            }else if(plocal == 90){
                r = 80.0;
                g = 0;
                b = 80.0;
            }else if(plocal == 120){
                r = 0.0;
                g = 0;
                b = 180.0;
            }else if(plocal == 150){
                r = 80.0;
                g = 80;
                b = 80.0;
            }else if(plocal == 180){
                r = 10.0;
                g = 100;
                b =10.0;
            }else if(plocal == 210){
                r = 100.0;
                g = 10;
                b =100.0;
            }
            if(img.at<float>(y, x) > val) ret.at<cv::Vec3f>(y, x) = cv::Vec3f(b, g, r);

        }
    }
    return ret;
}

////////////////////////////////////////////////////

void detectContoursMulti(cv::Mat & img, cv::Mat & pente, Filter method) {
    int nbChannels = img.channels();
    cv::Mat imgH = img.clone();
    cv::Mat imgV = img.clone();
    cv::Mat imgHI = img.clone();
    cv::Mat imgVI = img.clone();
    cv::Mat imgUL = img.clone();
    cv::Mat imgUR = img.clone();
    cv::Mat imgDR = img.clone();
    cv::Mat imgDL = img.clone();
    pente = cv::Mat::ones(img.rows, img.cols, CV_32FC1);

    // TODO : Ré-ajouter les fontions thin de Livaï /!\

    // Filtres de Prewitt
    std::vector<std::vector<float>> horizontalPrewitt = { {1, 1, 1}, {0, 0, 0}, {-1, -1, -1} };
    std::vector<std::vector<float>> horizontalPrewittInv = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };
    std::vector<std::vector<float>> diagUpLeftPrewitt = { {1, 1, 0}, {1, 0, -1}, {0, -1, -1} };
    std::vector<std::vector<float>> diagUpRightPrewitt = { {0, 1, 1}, {-1, 0, 1}, {-1, -1, 0} };
    std::vector<std::vector<float>> diagDownRightPrewitt = { {-1, -1, 0}, {-1, 0, 1}, {0, 1, 1} };
    std::vector<std::vector<float>> diagDownLeftPrewitt = { {0, -1, -1}, {1, 0, -1}, {1, 1, 0} };
    std::vector<std::vector<float>> verticalPrewitt = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
    std::vector<std::vector<float>> verticalPrewittInv = { {1, 0, -1}, {1, 0, -1}, {1, 0, -1} };


    // Convolutions
    Convolution convHorizontalPrewitt(horizontalPrewitt);
    Convolution convHorizontalPrewittInv(horizontalPrewittInv);
    Convolution convVerticalPrewitt(verticalPrewitt);
    Convolution convVerticalPrewittInv(verticalPrewittInv);
    Convolution convdiagUpLeftPrewitt(diagUpLeftPrewitt);
    Convolution convdiagUpRightPrewitt(diagUpRightPrewitt);
    Convolution convdiagDownRightPrewitt(diagDownRightPrewitt);
    Convolution convdiagDownLeftPrewitt(diagDownLeftPrewitt);


    convHorizontalPrewitt.apply(imgH);
    convVerticalPrewitt.apply(imgV);
    convHorizontalPrewittInv.apply(imgHI);
    convVerticalPrewittInv.apply(imgVI);
    convdiagUpLeftPrewitt.apply(imgUL);
    convdiagUpRightPrewitt.apply(imgUR);
    convdiagDownRightPrewitt.apply(imgDR);
    convdiagDownLeftPrewitt.apply(imgDL);


    // Fusion des 2 images obtenues
    switch(nbChannels) {
        case 1: {
            for(int y = 0; y < img.rows; ++y) {
                for(int x = 0; x < img.cols; ++x) {
                    float maxI = imgH.at<float>(y, x);
                    int idMax = 10;
                    if(imgV.at<float>(y, x) > maxI){
                        maxI = imgV.at<float>(y, x);
                        idMax = 60;
                    }
                    if(imgHI.at<float>(y, x) > maxI){
                        maxI = imgHI.at<float>(y, x);
                        idMax = 120;
                    }
                    if(imgVI.at<float>(y, x) > maxI){
                        maxI = imgVI.at<float>(y, x);
                        idMax = 180;
                    }
                    if(imgUL.at<float>(y, x) > maxI){
                        maxI = imgUL.at<float>(y, x);
                        idMax = 210;
                    }
                    if(imgUR.at<float>(y, x) > maxI){
                        maxI = imgUR.at<float>(y, x);
                        idMax = 30;
                    }if(imgDR.at<float>(y, x) > maxI){
                        maxI = imgDR.at<float>(y, x);
                        idMax = 90;
                    }
                    if(imgDL.at<float>(y, x) > maxI){
                        maxI = imgDL.at<float>(y, x);
                        idMax = 150;
                    }
                    img.at<float>(y, x) = maxI;
                    pente.at<float>(y, x) = idMax;

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
