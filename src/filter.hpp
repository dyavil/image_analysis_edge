#ifndef FILTER_H
#define FILTER_H

#include "opencv2/core/core.hpp"
#include <iostream>


class Filter{
public:
	Filter(cv::Mat init);
	~Filter(){}

	cv::Mat passFilter(cv::Mat source);


	cv::Mat matrice;

};


#endif