#include "convolution.hpp"



// Constrcuteur

Convolution::Convolution(std::vector<std::vector<float>> filter) : conv(filter), coeff(0) {

    

    // Taille de la convolution

    height = filter.size();

    width = filter[0].size();

    

    // Vérification du filtre

    for(unsigned int i = 1; i < height; ++i) {

        if(filter[i].size() != width) {

            std::cout << "Taille de filtre incorrecte" << std::endl;

            std::exit(1);

        }

    }

    

    // Calcul du coefficient

    for(int y = 0; y < height; ++y) {

        for(int x = 0; x < width; ++x) {

            if(conv[y][x] > 0) { coeff += conv[y][x]; }

        }

    }

    

    if(coeff == 0) {coeff = 1;}

}



// Applique la convolution a une image

void Convolution::apply(cv::Mat & img, bool normalize) {

    

    cv::Mat temp;

    int midH = height / 2;

    int midW = width / 2;

    int nbChannels = img.channels();

    

    switch(nbChannels) {

        case 1: {

            temp = cv::Mat(img.rows, img.cols, CV_32FC1);



            for(int y = 0; y < img.rows; ++y) {

                for(int x = 0; x < img.cols; ++x) {



                    float gray = 0;

                    

                    for(int j = -midH; j < midH + 1; ++j) {

                        for(int i = -midW; i < midW + 1; ++i) {

                            float c = conv[j + midH][i + midW];

                            float gPixel = 0;

                            if(0 <= x+i && x+i < img.cols && 0 <= y+j && y+j < img.rows) {

                                gPixel = img.at<float>(y+j, x+i);

                            }

                            gray += c * gPixel;                      

                        }

                    }

                    if(normalize) { gray /= coeff; }

                    temp.at<float>(y, x) = gray;  

                }

            }

            break;

        }

        case 2: {

            // Not implemented

            break;

        }

        default: {

            temp = cv::Mat(img.rows, img.cols, CV_32FC3);

        

            for(int y = 0; y < img.rows; ++y) {

                for(int x = 0; x < img.cols; ++x) {



                    float blue = 0;

                    float green = 0;

                    float red = 0; 



                    for(int j = -midH; j < midH + 1; ++j) {

                        for(int i = -midW; i < midW + 1; ++i) {

                            float c = conv[j + midH][i + midW];

                            cv::Vec3f rgbPixel = cv::Vec3f(0, 0, 0);

                            if(0 <= x+i && x+i < img.cols && 0 <= y+j && y+j < img.rows) {

                                rgbPixel = img.at<cv::Vec3f>(y+j, x+i);

                            }

                            blue += c * rgbPixel.val[0];

                            green += c * rgbPixel.val[1];

                            red += c * rgbPixel.val[2];

                        }

                    }

                    if(normalize) {

                        blue /= coeff;

                        green /= coeff;

                        red /= coeff;

                    }

                    

                    temp.at<cv::Vec3f>(y, x) =  cv::Vec3f(blue, green, red);

                }

            }

            break;

        }

    }



    img = temp; 

}



// Applique la convolution a un seul pixel d'une image

void Convolution::applyToPixel(cv::Mat & img, unsigned int y, unsigned int x, bool normalize) {

    

    int midH = height / 2;

    int midW = width / 2;

    float gray = 0;

    

    for(int j = -midH; j < midH + 1; ++j) {

        for(int i = -midW; i < midW + 1; ++i) {

            float c = conv[j + midH][i + midW];

            float gPixel = 0;

            if(0 <= x+i && x+i < img.cols && 0 <= y+j && y+j < img.rows) {

                gPixel = img.at<float>(y+j, x+i);

            }

            gray += c * gPixel;                      

        }

    }

    

    if(normalize) { gray /= coeff; }

    

    img.at<float>(y, x) = gray;  

}