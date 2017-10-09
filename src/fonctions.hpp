#ifndef FONCTIONS
#define FONCTIONS

#include <opencv2/opencv.hpp>
#include <cmath>
#include "convolution.hpp"

enum Color { gray = 0, rgb };   // Image en niveau de gris ou couleurs

// Detecte les contours d'un image
cv::Mat detectContours(cv::Mat & img, Color col);

#endif // FONCTIONS
