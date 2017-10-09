#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "filter.hpp"
#include <iostream>
#include <string>



using namespace cv;

void kirsh(const Mat & in, Mat & out){

}


int main(int argc, char ** argv){

    Mat image;


    float data[3][3] = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
    float data1[3][3] = {{1, 0, -1}, {1, 0, -1}, {1, 0, -1}};
    //float data[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
    Mat filter=Mat(3, 3, CV_32F, data);
    Mat filter1=Mat(3, 3, CV_32F, data1);
    Filter fil = Filter(filter);
    Filter fil1 = Filter(filter1);

    float data2[3][3] = {{-1, -1, -1}, {0, 0, 0}, {1, 1, 1}};
    Mat filter2=Mat(3, 3, CV_32F, data2);
    Filter fil2 = Filter(filter2);

    image = imread("./data/lena.jpg", 0);
    

    Point anchor = Point( -1, -1 );
    double delta = 0;
    int ddepth = -1;
    Mat dst, dst2, dstp;
    dstp = image;

    if(image.empty()) {
        std::cout << "fichier non trouvé" << std::endl;
        return -1;
    }
    Mat res,res1, res2;
    //image.copyTo(res);
    res = fil.passFilter(image);
    res1 = fil1.passFilter(image);
    res2 = fil2.passFilter(image);


    filter2D(image, dst, ddepth , filter, anchor, delta, BORDER_DEFAULT );
    filter2D(image, dst2, ddepth , filter2, anchor, delta, BORDER_DEFAULT );
    namedWindow("Affichage", WINDOW_AUTOSIZE);
    namedWindow("Affichage2", WINDOW_AUTOSIZE);
    /*Mat gray_image, gray_image2;
    cvtColor(res, gray_image, CV_BGR2GRAY );
    cvtColor(res2, gray_image2, CV_BGR2GRAY );*/
    imshow("Affichage", res+res2);
    imshow("Affichage2", image);

    waitKey(0);

    return 0;
}
