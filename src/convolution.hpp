#ifndef CONVOLUTION
#define CONVOLUTION

#include <opencv2/opencv.hpp>
#include <cmath>

class Convolution {
    
public:
    // Constrcuteur
    Convolution(std::vector<std::vector<float>> _conv, int _width, int _height) : conv(_conv), width(_width), height(_height), coeff(0) {
        for(int x = 0; x < width; ++x) {
            for(int y = 0; y < height; ++y) {
                coeff += std::abs(conv[x][y]);
            }
        }
        if(coeff == 0) {coeff = 1;}
    }


    // Applique la convolution a une image grise
    cv::Mat applyToGray(const cv::Mat & source) {
	    
        cv::Mat res;
	    source.copyTo(res);
     
	    for(int j = 0; j < source.rows; ++j){
            for(int k=0; k < source.cols; ++k){
                
                float gray = 0;
                int dl = width / 2;
                int dm = height / 2;
                
            for(int l = 0; l < height; l++) {
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
            }
        }
        
        return res;
    }


    // Applique la convolution a une image couleur
    cv::Mat applyToRGB(const cv::Mat & img) {
        
        cv::Mat ret = img.clone();
        int midW = width / 2;
        int midH = height / 2;
        
        for(int x = midW; x < img.rows - midW; ++x) {
            for(int y = midH; y < img.cols - midH; ++y) {
                
                float red = 0;
                float green = 0;
                float blue = 0;
                
                for(int i = -midW; i < midW + 1; ++i) {
                    for(int j = -midH; j < midH + 1; ++j) {
                                
                        cv::Vec3b pixel = img.at<cv::Vec3b>(x + i, y + j);
                        float c = conv[i + midW][j + midH];
                        
                        red += pixel.val[0] * c;
                        green += pixel.val[1] * c;
                        blue += pixel.val[2] * c;                       
                    }
                }
                
                if(red < 0) { red = 0; }
                if(green < 0) { green = 0; }
                if(blue < 0) { blue = 0; }
                
                red /= coeff;
                green /= coeff;
                blue /= coeff;
                  
                ret.at<cv::Vec3b>(x, y) =  cv::Vec3b(red, green, blue);          
            }
        }
        
        return ret;       
    }

    
private:
    // Attributs
    std::vector< std::vector<float> > conv;
    int width, height;
    float coeff;
    
};

#endif // CONVOLUTION