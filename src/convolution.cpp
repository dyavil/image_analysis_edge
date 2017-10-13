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

// Applique la convolution a une image
void Convolution::apply(cv::Mat & img, bool normalize) {
    
    cv::Mat temp;
    int midW = width / 2;
    int midH = height / 2;
    int nbChannels = img.channels();
    
    switch(nbChannels) {
        case 1: {
            temp = cv::Mat(img.rows, img.cols, CV_32FC1);

            for(int y = 0; y < img.rows; ++y) {
                for(int x = 0; x < img.cols; ++x) {

                    float gray = 0;
                    
                    for(int i = -midW; i < midW + 1; ++i) {
                        for(int j = -midH; j < midH + 1; ++j) {
                            float c = conv[i + midW][j + midH];
                            float gPixel = 0;
                            if(0 <= x+i && x+i < img.cols && 0 <= y+j && y+j < img.rows) {
                                gPixel = img.at<float>(y+j, x+i);
                            }
                            gray += c * gPixel;                      
                        }
                    }
                    if(normalize) { gray /= coeff; }
                    temp.at<float>(y, x) = gray;  
                }
            }
            break;
        }
        case 2: {
            // Not implemented
            break;
        }
        default: {
            temp = cv::Mat(img.rows, img.cols, CV_32FC3);
        
            for(int y = 0; y < img.rows; ++y) {
                for(int x = 0; x < img.cols; ++x) {

                    float blue = 0;
                    float green = 0;
                    float red = 0; 

                    for(int i = -midW; i < midW + 1; ++i) {
                        for(int j = -midH; j < midH + 1; ++j) {
                            float c = conv[i + midW][j + midH];
                            cv::Vec3f rgbPixel = cv::Vec3f(0, 0, 0);
                            if(0 <= x+i && x+i < img.cols && 0 <= y+j && y+j < img.rows) {
                                rgbPixel = img.at<cv::Vec3f>(y+j, x+i);
                            }
                            blue += c * rgbPixel.val[0];
                            green += c * rgbPixel.val[1];
                            red += c * rgbPixel.val[2];
                        }
                    }
                    if(normalize) {
                        blue /= coeff;
                        green /= coeff;
                        red /= coeff;
                    }
                    
                    temp.at<cv::Vec3f>(y, x) =  cv::Vec3f(blue, green, red);
                }
            }
            break;
        }
    }
    
    img = temp; 
}
