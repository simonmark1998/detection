#include <iostream>
#include <opencv2/opencv.hpp>

class Weapon
{
    public:
        double threshold;
        std::string name;
        cv::Mat templateImage;

        Weapon(std::string name, cv::Mat templateImage);

        cv::Mat getTemplateImage();
        std::string getName();
        double getThreshold();
        void setThreshold(double threshold);
};

