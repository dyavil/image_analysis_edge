#ifndef FONCTIONS
#define FONCTIONS

#include <opencv2/highgui/highgui.hpp>
#include "convolution.hpp"
#include "iostream"

enum Filter { Prewitt, Sobel };

// Charge une image dans une matrice de floats
cv::Mat loadImg(std::string path);

// Affiche la matrice dans une fenêtre
void showImg(const cv::Mat & img, std::string name, bool convert = true);

// Convertit une image en niveau de gris
void convertToGrayScale(cv::Mat & img);

// Detecte les contours d'un image
void detectContours(cv::Mat & img, Filter method = Prewitt);

/*
// Indique si un pixel d'une image binaire possède un voisin
bool hasNeighbor(const cv::Mat & img, uchar seuil, unsigned int x, unsigned int y);

// Applique l'hystérésis sur une image en niveaux de gris
cv::Mat hysteresis (const cv::Mat & img, uchar seuilBas = 5, uchar seuilHaut = 10);

// Réduit les contours
cv::Mat refineContours(const cv::Mat & img, int largeur = 3);

// Réduit les contours verticaux
cv::Mat ThinVertical(const cv::Mat & img);

// Réduit les contours horizontaux
cv::Mat ThinHorizontal(const cv::Mat & img);
*/

#endif // FONCTIONS
