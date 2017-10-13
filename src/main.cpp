#include "fonctions.hpp"

int main(int argc, char ** argv) {

    // Images
    cv::Mat imgTest = loadImg("./data/test.png");
    cv::Mat imgLena1 = loadImg("./data/lena.jpg");
    cv::Mat imgLena2 = loadImg("./data/lena.jpg");
   
    // Traitements
    convertToGrayScale(imgLena1);
    detectContours(imgLena1);
    
    convertToGrayScale(imgLena2);
    detectContours(imgLena2, Sobel);
    
    // Affichage
    showImg(imgLena1, "contoursPrewitt");
    showImg(imgLena2, "contoursSobel");
    
    cv::waitKey();

    return 0;
    
}
