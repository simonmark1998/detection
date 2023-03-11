#include "weapon.h"

using namespace std;
using namespace cv;

        Weapon::Weapon(string name, Mat templateImage)
        {
            this->name = name;
            this->templateImage = templateImage;
        }

        Mat Weapon::getTemplateImage()
        {
            return templateImage;
        }

        string Weapon::getName()
        {
            return name;
        }

        double Weapon::getThreshold()
        {
            return threshold;
        }

        void Weapon::setThreshold(double threshold)
        {
            this->threshold = threshold;
        }
