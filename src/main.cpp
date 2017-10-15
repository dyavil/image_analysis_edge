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
    cv::Mat imgDst12;
    cv::Mat imgDst13;
    cv::Mat imgDst14;
   
    // Traitements
    convertToGrayScale(imgLena1);
    detectContours(imgLena1, pente);
    hyst = hysteresis(imgLena1, 80, 110);
    
    convertToGrayScale(imgLena2);
    detectContours(imgLena2, pente1, Sobel);
    //std::cout << imgLena2 << std::endl;

    hyst2 = hysteresis(imgDst8, 80, 100);
    

    imgDst12 = seuilGlob(imgLena1);
    imgDst13 = seuilLoc(imgLena1, 9);
    imgDst14 = autoHysteresis(imgLena1);
    
    imgDst8 = ThinAll(imgDst14, pente);
    
    imgDst9 = makeChain(imgDst8, pente);
    imgDst11 = imgDst9;
    imgDst10 = fillContours(imgDst9, pente);


    
    // Affichage
    showImg(imgLena1, "contoursPrewitt");
    showImg(imgLena2, "contoursSobel");
    showImg(imgDst8, "affinement multi");
    showImg(imgDst12, "seuil global - ecart type");
    showImg(imgDst13, "seuil local");
    showImg(hyst, "hysteresis avec non auto");
    showImg(imgDst14, "hysteresis auto");
    
    cv::waitKey();

    return 0;
    
}
