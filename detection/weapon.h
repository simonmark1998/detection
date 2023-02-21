#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Weapon
{
    public:
        double threshold;
        string name;
        Mat templateImage;

        Weapon(string name, Mat templateImage);

        Mat getTemplateImage();
        string getName();
        double getThreshold();
        void setThreshold(double threshold);
};

