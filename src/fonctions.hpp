#ifndef FONCTIONS
#define FONCTIONS

#include <opencv2/opencv.hpp>
#include <cmath>
#include "convolution.hpp"

enum Color { gray = 0, rgb };   // Image en niveau de gris ou couleurs

// Detecte les contours d'un image
cv::Mat detectContours(cv::Mat & img, Color col);

// Indique si un pixel d'une image binaire possède un voisin
bool hasNeighbor(const cv::Mat & img, int x, int y);

// Applique l'hystérésis sur une image en niveaux de gris
cv::Mat hysteresis (const cv::Mat & img, uchar seuilBas = 5, uchar seuilHaut = 10);

#endif // FONCTIONS
