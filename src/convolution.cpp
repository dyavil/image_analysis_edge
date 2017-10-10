#include "convolution.hpp"

// Constrcuteur
Convolution::Convolution(std::vector<std::vector<float>> _conv, int _width, int _height) : conv(_conv), width(_width), height(_height), coeff(0) {
    
    // Check filtre impair
    assert(_width % 2 == 1);
    assert(_height % 2 == 1);
    
    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            coeff += std::abs(conv[x][y]);
        }
    }
    
    if(coeff == 0) {coeff = 1;}
}

// Applique la convolution a une image grise
cv::Mat Convolution::applyToGray(const cv::Mat & source) {
    
    cv::Mat res;
    source.copyTo(res);
 
    for(int j = 0; j < source.rows; ++j){
        for(int k = 0; k < source.cols; ++k){
            
            float gray = 0;
            int dl = width / 2;
            int dm = height / 2;
            
            for(int l = 0; l < height; ++l) {
                for (int m = 0; m < width; ++m) {
                    if(j-dl < 0 || k-dm < 0) {
                        gray +=0;
                    } else {
                        int pix = (int)source.at<uchar>(j-dl, k-dm);
                        gray += pix * conv[l][m];
                    }
                    dm--;   
                }
                dm = height / 2;
                dl--;
            }
            
            res.at<uchar>(j, k) = abs(gray);            
        }
    }
    
    return res;
}

// Applique la convolution a une image couleur
cv::Mat Convolution::applyToRGB(const cv::Mat & img) {
    
    cv::Mat ret = img.clone();
    int midW = width / 2;
    int midH = height / 2;
    
    for(int y = 0; y < img.rows; ++y) {
        for(int x = 0; x < img.cols; ++x) {
            float blue = 0;
            float green = 0;
            float red = 0;            
            
            for(int i = -midW; i < midW + 1; ++i) {
                for(int j = -midH; j < midH + 1; ++j) {
                    cv::Vec3b pixel = cv::Vec3b(0, 0, 0);
                    if(0 <= x+i && x+i < img.cols && 0 <= y+j && y+j < img.rows) {
                        pixel = img.at<cv::Vec3b>(y+j, x+i);
                    }
                    float c = conv[i + midW][j + midH];
                    
                    blue += pixel.val[0] * c;
                    green += pixel.val[1] * c;
                    red += pixel.val[2] * c;
                }
            }
            
            if(blue < 0) { blue = 0; }
            if(green < 0) { green = 0; }
            if(red < 0) { red = 0; }
            
            blue /= coeff;
            green /= coeff;
            red /= coeff;
              
            ret.at<cv::Vec3b>(y, x) =  cv::Vec3b(blue, green, red);    
        }
    }
    
    return ret; 
}
