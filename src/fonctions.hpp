#ifndef FONCTIONS
#define FONCTIONS

#include <opencv2/highgui/highgui.hpp>
#include "convolution.hpp"
#include "iostream"

#define PI  3.1416

enum Filter { Prewitt, Sobel };

// Charge une image dans une matrice de floats
cv::Mat loadImg(std::string path);

// Affiche la matrice dans une fenêtre
void showImg(const cv::Mat & img, std::string name, bool convert = true);

// Convertit une image en niveau de gris
void convertToGrayScale(cv::Mat & img);

// Detecte les contours d'un image
void detectContours(cv::Mat & img, cv::Mat & pente, Filter method = Prewitt);

// Indique si un pixel d'une image binaire possède un voisin
bool hasNeighbor(const cv::Mat & img, float seuil, unsigned int x, unsigned int y);

// Applique l'hystérésis sur une image en niveaux de gris
cv::Mat hysteresis(const cv::Mat & img, float seuilBas = 5, float seuilHaut = 10);

// Réduit les contours
void refineContours(cv::Mat & img, bool diagonales = false, cv::Mat pente = cv::Mat());

// Supprime les pixels solitaires
void suppressAlonePixels(cv::Mat img);

//seuillage auto d'apres l'ecart type
cv::Mat autoHysteresis(const cv::Mat & img);

//seuil fixe, global (moyenne ou ecart-type (par defaut))
cv::Mat seuilGlob(const cv::Mat & img, int type=0);

//seuil local, moyenne de matrice n*n autour du point
cv::Mat seuilLoc(const cv::Mat & img,int n=3);

// Calcule une orientation générale du gradian
void computePenteRange(cv::Mat & rawPente);

cv::Mat makeChain(const cv::Mat & img, const cv::Mat & pente);

cv::Mat newThin(const cv::Mat & img, const cv::Mat & pente, cv::Mat & hyst);

cv::Mat fillContours(const cv::Mat & img, const cv::Mat & pente, const cv::Mat & base);


#endif // FONCTIONS
