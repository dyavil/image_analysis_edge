#include "contourhandler.h"


void contourHandler::loadImage(std::string fn){
    base = cv::imread(fn.c_str(), 1);
    base.convertTo(base, CV_32FC3);
    newfile = true;

    contoursdone = false;
    hystedone = false;
    thindone = false;
}


cv::Mat contourHandler::renderBase(){
    if(newfile){
        cv::Mat res = base.clone();
        base.convertTo(res, CV_8UC1);

        return res;
    }
    return base;
}


cv::Mat contourHandler::calculContours(){
    if(newfile){
        contours = base.clone();
        contoursdone = true;
        convertToGrayScale(contours);
        detectContours(contours, pente, fil);
        cv::Mat res = contours.clone();
        contours.convertTo(res, CV_8UC1);
        return res;
    }
    return base;
}


cv::Mat contourHandler::calculContoursMulti(){
    if(newfile){
        contours = base.clone();
        contoursdone = true;
        convertToGrayScale(contours);
        detectContoursMulti(contours, pente);
        cv::Mat res = contours.clone();
        contours.convertTo(res, CV_8UC1);
        return res;
    }
    return base;
}


cv::Mat contourHandler::autoHyste(){
    if(newfile){
        if(!contoursdone){
            contours = base.clone();
            contoursdone = true;
            convertToGrayScale(contours);
            detectContours(contours, pente, fil);
        }
        hysteresis = autoHysteresis(contours);
        cv::Mat res = hysteresis.clone();
        hystedone = true;
        hysteresis.convertTo(res, CV_8UC1);
        return res;
    }
    return base;
}

cv::Mat contourHandler::affine(){
    if(newfile){
        if(!hystedone){
            if(!contoursdone){
                contours = base.clone();
                contoursdone = true;
                convertToGrayScale(contours);
                detectContours(contours, pente, fil);
            }
            hysteresis = autoHysteresis(contours);
            hystedone = true;
        }
        thin = newThin(contours, pente, hysteresis);
        thindone = true;
        cv::Mat res = thin.clone();
        thin.convertTo(res, CV_8UC1);
        return res;
    }
    return base;
}


cv::Mat contourHandler::completeContours(){
    if(newfile){
        if(!thindone){
            if(!hystedone){
                if(!contoursdone){
                    contours = base.clone();
                    contoursdone = true;
                    convertToGrayScale(contours);
                    detectContours(contours, pente, fil);
                }
                hysteresis = autoHysteresis(contours);
                hystedone = true;
            }
            thin = newThin(contours, pente, hysteresis);
            thindone = true;
        }

        chaines = makeChain(thin, pente);
        resultat = fillContours(chaines, pente, contours);
        cv::Mat res = resultat.clone();
        resultat.convertTo(res, CV_8UC1);
        return res;
    }
    return base;
}

cv::Mat contourHandler::seuilGlobal(int type){
    if(newfile){
        if(!contoursdone){
            contours = base.clone();
            contoursdone = true;
            convertToGrayScale(contours);
            detectContours(contours, pente, fil);
        }
        resultat = seuilGlob(contours, type);
        cv::Mat res = resultat.clone();
        resultat.convertTo(res, CV_8UC1);
        return res;
    }
    return base;
}

cv::Mat contourHandler::seuilLocal(int n){
    if(newfile){
        if(!contoursdone){
            contours = base.clone();
            contoursdone = true;
            convertToGrayScale(contours);
            detectContours(contours, pente, fil);
        }
        resultat = seuilLoc(contours, n);
        cv::Mat res = resultat.clone();
        resultat.convertTo(res, CV_8UC1);
        return res;
    }
    return base;
}


cv::Mat contourHandler::chainageContour(){
    if(newfile){
        if(!thindone){
            if(!hystedone){
                if(!contoursdone){
                    contours = base.clone();
                    contoursdone = true;
                    convertToGrayScale(contours);
                    detectContours(contours, pente, fil);
                }
                hysteresis = autoHysteresis(contours);
                hystedone = true;
            }
            thin = newThin(contours, pente, hysteresis);
            thindone = true;
        }

        chaines = makeChain(thin, pente);
        cv::Mat res = chaines.clone();
        res.convertTo(res, CV_8UC1);
        return res;
    }
    return base;
}

cv::Mat contourHandler::showPenteColor(){
    if(newfile){
        if(!contoursdone){
            contours = base.clone();
            contoursdone = true;
            convertToGrayScale(contours);
            detectContours(contours, pente, fil);
        }
        cv::Mat res = showPente(contours, pente);
        res.convertTo(res, CV_8UC1);
        return res;
    }
    return base;
}

cv::Mat contourHandler::colorGradient(){
    if(newfile){
        if(!contoursdone){
            contours = base.clone();
            contoursdone = true;
            convertToGrayScale(contours);
            detectContours(contours, pente, fil);
        }
        cv::Mat res = showGradient(contours);
        res.convertTo(res, CV_8UC1);
        return res;
    }
    return base;
}



cv::Mat contourHandler::customHysteresis(){
    if(newfile){
        if(!contoursdone){
            contours = base.clone();
            contoursdone = true;
            convertToGrayScale(contours);
            detectContours(contours, pente, fil);
        }
        hysteresis = hysteresisS(contours, seuilBas, seuilHaut);
        cv::Mat res = hysteresis.clone();
        hystedone = true;
        hysteresis.convertTo(res, CV_8UC1);
        return res;
    }
    return base;
}


