#include "fonctions.hpp"

int main(int argc, char ** argv) {

    // Images
    cv::Mat imgTest = loadImg("./data/test.png");
    cv::Mat imgLena1 = loadImg("./data/lena.jpg");
    cv::Mat imgLena2 = loadImg("./data/lena.jpg");
    cv::Mat pente;
    cv::Mat hyst;
    cv::Mat hyst2;
    cv::Mat pente1;
    cv::Mat imgDst8;
    cv::Mat imgDst9;
    cv::Mat imgDst10;
    cv::Mat imgDst11;
   
    // Traitements
    convertToGrayScale(imgLena1);
    detectContours(imgLena1, pente);
    hyst = hysteresis(imgLena1, 80, 110);
    
    convertToGrayScale(imgLena2);
    detectContours(imgLena2, pente1, Sobel);
    //std::cout << imgLena2 << std::endl;
    imgDst8 = ThinAll(imgLena1, pente);
    hyst2 = hysteresis(imgDst8, 80, 100);
    imgDst8 = ThinAll(hyst, pente);
    
    imgDst9 = makeChain(imgDst8, pente);
    imgDst11 = imgDst9;
    imgDst10 = fillContours(imgDst9, pente);

    for (int i = 0; i < 0; ++i)
    {
        imgDst8 = ThinAll(imgDst10, pente);
        imgDst9 = makeChain(imgDst8, pente);
        imgDst10 = fillContours(imgDst9, pente);
    }
    
    //normalize(pente, pente, 255, 0, cv::NORM_MINMAX);

    
    // Affichage
    showImg(imgLena1, "contoursPrewitt");
    showImg(imgLena2, "contoursSobel");
    showImg(imgDst11, "affinement multi");
    showImg(imgDst10, "pente prewit");
    
    cv::waitKey();

    return 0;
    
}
