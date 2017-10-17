#include "fonctions.hpp"

int main(int argc, char ** argv) {

    // Images
    cv::Mat imgTest = loadImg("./data/test.png");
    cv::Mat imgLena1 = loadImg("./data/cany.png");
    cv::Mat imgLena2 = loadImg("./data/lena.jpg");
    cv::Mat pente;
    cv::Mat pente2;
    cv::Mat hyst;
    cv::Mat hyst2;
    cv::Mat hyst3;
    cv::Mat pente1;
    cv::Mat imgDst8;
    cv::Mat imgDst9;
    cv::Mat imgDst10;
    cv::Mat imgDst11;
    cv::Mat imgDst12;
    cv::Mat imgDst13;
    cv::Mat imgDst14;
    cv::Mat imgDst15;
   
    // Traitements
    convertToGrayScale(imgLena1);

    convertToGrayScale(imgTest);

    imgDst10 = imgTest.clone();
    detectContours(imgLena1, pente);
    hyst = autoHysteresis(imgLena1);
    cv::Mat temppente = pente.clone();
    imgDst8 = newThin(imgLena1, pente, hyst);

    detectContours(imgTest, pente2);
    hyst3 = hysteresis(imgTest, 1 ,3);
    imgDst15 = newThin(imgTest, pente2, hyst3);

    imgDst9 = makeChain(imgDst8, pente);
    /*

    hyst = hysteresis(imgLena1, 80, 110);
    
    convertToGrayScale(imgLena2);
    detectContours(imgLena2, pente1, Sobel);
    //std::cout << imgLena2 << std::endl;

    hyst2 = hysteresis(imgDst8, 80, 100);
    

    imgDst12 = seuilGlob(imgLena1);
    imgDst13 = seuilLoc(imgLena1, 21);
    imgDst14 = autoHysteresis(imgLena1);
    
    

    imgDst14 = autoHysteresis(imgDst8);
    
    imgDst9 = makeChain(imgDst8, pente);
    imgDst11 = imgDst9;*/
    imgDst10 = fillContours(imgDst9, pente, imgLena1);


    
    // Affichage
    showImg(hyst3, "contoursPrewitt");
    showImg(imgLena1, "contoursSobel");
    showImg(imgDst10, "affinement multi");
    showImg(imgDst8, "completion contour");
    //showImg(imgDst12, "seuil global - ecart type");
    //showImg(imgDst13, "seuil local");
    //showImg(hyst, "hysteresis avec non auto");
    //showImg(imgDst14, "hysteresis auto");
    
    cv::waitKey();

    return 0;
    
}
