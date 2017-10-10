#ifndef CONVOLUTION
#define CONVOLUTION

#include <opencv2/imgproc/imgproc.hpp>
#include "math.h"

class Convolution {
    
public:
    // Constrcuteur
    Convolution(std::vector<std::vector<float>> _conv, int _width, int _height);

    // Applique la convolution a une image grise
    cv::Mat applyToGray(const cv::Mat & source);

    // Applique la convolution a une image couleur
    cv::Mat applyToRGB(const cv::Mat & img);

    
private:
    // Attributs
    std::vector< std::vector<float> > conv;
    int width, height;
    float coeff;
    
};

#endif // CONVOLUTION
