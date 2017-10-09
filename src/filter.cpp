#include "filter.hpp"


Filter::Filter(cv::Mat init){
	init.copyTo(matrice);
}

cv::Mat Filter::passFilter(cv::Mat source){
	cv::Mat res;
	source.copyTo(res);
	for(int j = 0; j < source.rows; ++j){
        for(int k=0; k < source.cols; ++k){
            //std::cout << source.at<cv::Vec3b>(j, k) << std::endl;
            int tmp = 0;
            int tmpg = 0;
            int tmpb= 0;
            //res.row(j).col(k) = source.row(j-1).col(x-1) * matrice.row(0).col(0)
            int dl = matrice.rows/2;
            int dm = matrice.cols/2;
            
            for(int l = 0; l < matrice.rows; l++){
                for (int m = 0; m < matrice.cols; ++m)
                {
                    if(j-dl < 0 || k-dm < 0) tmp +=0;
                    else{
                        //cv::Vec3b vtmp = source.at<cv::Vec3b>(j-dl, k-dm);
                        //if(vtmp.val[0] != vtmp.val[1] != vtmp.val[2]) std::cout << "ftmp" << std::endl;
                        int vtmp = (int)source.at<uchar>(j-dl, k-dm);
                        tmp += vtmp * matrice.at<float>(l, m);
                       
                        //tmpg += vtmp.val[1] * matrice.at<float>(l, m);
                        //tmpb += vtmp.val[2] * matrice.at<float>(l, m);
                    }
                    dm--;   
                }
                dl--;
            }
            //res.at<uchar>(j, k) = (abs(tmp));
            if(abs(tmp) > 5) res.at<uchar>(j, k) = 255;
            else res.at<uchar>(j, k) = 0;


            if(abs(tmp) > 130) res.at<uchar>(j, k) = 255;
            else if(abs(tmp) > 100){
                for(int l = -1; l < 2; l++){
                    for(int m = -1; m < 2; m++){
                        if(j+l > 0 && m+l > 0){
                            if (res.at<uchar>(j+l, k+l) > 200){
                                res.at<uchar>(j, k) = 255;
                                break;
                            }
                        } 
                    }
                }
            }
            else res.at<uchar>(j, k) = 0;

            //if((tmp/(matrice.rows*matrice.cols)) < 255) std::cout << tmp/(matrice.rows*matrice.cols) <<std::endl;
            //res.at<cv::Vec3b>(j, k)[0] = tmp/(matrice.rows*matrice.cols);
            //res.at<cv::Vec3b>(j, k)[1] = tmpg/(matrice.rows*matrice.cols);
            //res.at<cv::Vec3b>(j, k)[2] = tmpb/(matrice.rows*matrice.cols);
        }
    }
    return res;
}

