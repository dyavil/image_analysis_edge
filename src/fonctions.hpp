#ifndef FONCTIONS
#define FONCTIONS

#include "convolution.hpp"
#include "iostream"

enum Color { gray = 0, rgb };   // Image en niveau de gris ou couleurs

// Convertit une image en niveau de gris
cv::Mat convertToGrayScale(cv::Mat & img);

// Detecte les contours d'un image
cv::Mat detectContours(cv::Mat & img, Color col, bool thin=false);

// Indique si un pixel d'une image binaire possède un voisin
bool hasNeighbor(const cv::Mat & img, uchar seuil, unsigned int x, unsigned int y);

// Applique l'hystérésis sur une image en niveaux de gris
cv::Mat hysteresis (const cv::Mat & img, uchar seuilBas = 5, uchar seuilHaut = 10);

// Réduit les contours verticaux
cv::Mat ThinVertical(const cv::Mat & img);

// Réduit les contours horizontaux
cv::Mat ThinHorizontal(const cv::Mat & img);

#endif // FONCTIONS
