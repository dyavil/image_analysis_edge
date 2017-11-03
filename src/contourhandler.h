#ifndef CONTOURHANDLER_H
#define CONTOURHANDLER_H


#include "fonctions.hpp"
//classe liant l'interface graphique aux fonctionnalité développées
class contourHandler{
public:

    contourHandler():filename(""), newfile(false), contoursdone(false), hystedone(false), thindone(false), fil(Prewitt){}
    ~contourHandler(){}

    void loadImage(std::string fn);

    cv::Mat calculContours();

    cv::Mat calculContoursMulti();

    cv::Mat renderBase();

    cv::Mat autoHyste();

    cv::Mat affine();

    cv::Mat completeContours();

    cv::Mat seuilGlobal(int type=0);

    cv::Mat seuilLocal(int n=3);

    cv::Mat chainageContour();

    cv::Mat showPenteColor();

    cv::Mat colorGradient();

    cv::Mat customHysteresis();

    bool newfile;
    bool contoursdone;
    bool hystedone;
    bool thindone;

    float seuilBas;
    float seuilHaut;

    Filter fil;
    std::string filename;
    cv::Mat base;
    cv::Mat contours;
    cv::Mat pente;
    cv::Mat hysteresis;
    cv::Mat chaines;
    cv::Mat thin;
    cv::Mat resultat;

};



#endif // CONTOURHANDLER_H
