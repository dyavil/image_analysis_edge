#ifndef FONCTIONS
#define FONCTIONS

#include "convolution.hpp"
#include "iostream"

enum Color { gray = 0, rgb };   // Image en niveau de gris ou couleurs

// Detecte les contours d'un image
cv::Mat detectContours(cv::Mat & img, Color col, bool thin=false);

// Indique si un pixel d'une image binaire possède un voisin
bool hasNeighbor(const cv::Mat & img, uchar seuil, unsigned int x, unsigned int y);

// Applique l'hystérésis sur une image en niveaux de gris
cv::Mat hysteresis (const cv::Mat & img, uchar seuilBas = 5, uchar seuilHaut = 10);


void ThinVertical(cv::Mat & pSrc, cv::Mat & pDst);

void ThinHorizontal(cv::Mat & pSrc, cv::Mat & pDst);

#endif // FONCTIONS
