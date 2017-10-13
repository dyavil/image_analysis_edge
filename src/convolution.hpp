#ifndef CONVOLUTION
#define CONVOLUTION

#include <opencv2/imgproc/imgproc.hpp>
#include "math.h"

class Convolution {
    
public:
    // Constrcuteur
    Convolution(std::vector<std::vector<float>> _conv, int _width, int _height);

    // Applique la convolution a une image
    void apply(cv::Mat & img, bool normalize = false);

    
private:
    // Attributs
    std::vector< std::vector<float> > conv;
    unsigned int width, height;
    float coeff;
    
};

#endif // CONVOLUTION
