#include "fonctions.hpp"

int main(int argc, char ** argv) {

    // Images
    cv::Mat imgTest = loadImg("./data/test.png");
    cv::Mat imgLena1 = loadImg("./data/lena.jpg");
    cv::Mat imgLena2 = loadImg("./data/lena.jpg");
    cv::Mat pente;
    cv::Mat pente1;
    cv::Mat imgDst8;
   
    // Traitements
    convertToGrayScale(imgLena1);
    detectContours(imgLena1, pente);
    //hysteresis(imgLena1, 80, 100);
    
    convertToGrayScale(imgLena2);
    detectContours(imgLena2, pente1, Sobel);
    //std::cout << imgLena2 << std::endl;
    imgDst8 = ThinAll(imgLena1, pente);
    hysteresis(imgDst8, 80, 100);
    //normalize(pente, pente, 255, 0, cv::NORM_MINMAX);

    
    // Affichage
    showImg(imgLena1, "contoursPrewitt");
    showImg(imgLena2, "contoursSobel");
    //showImg(imgDst8, "affinement multi");
    //showImg(pente, "pente prewit");
    
    cv::waitKey();

    return 0;
    
}
