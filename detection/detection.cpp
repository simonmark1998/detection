#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include "json/writer.h"
#include "weapon.h"
#include <fstream>
#include <string>
#include <algorithm>

using namespace cv;
using namespace std;
namespace fs = std::filesystem;

vector<Weapon> readWeapons();
void setThresholds(vector<Weapon> &weapons);

int main(int argc, char** argv)
{
    string path = "./input_files";
    vector<Weapon> weapons = readWeapons();
    setThresholds(weapons);


    for (const auto& image : fs::directory_iterator(path))
    {
        string img_name = image.path().filename().string();
        Mat img = imread(image.path().string());

        //lecs�kkentj�k a k�p m�ret�t, mert a kill feed minden esetben a jobb fels� sarokban van �gy csak azt kell vizsg�lni
        int width = img.cols;
        int height = img.rows;
        Rect roi(3 * width / 4, 0, width / 4, height / 4);

        Mat img_rgb = img(roi);

        Mat img_gray;

        cvtColor(img_rgb, img_gray, COLOR_BGR2BGRA);

        Json::Value AllEvents(Json::arrayValue);

        Mat resultImage = img_rgb;

        Json::Value event;

        for (int i = 0; i < weapons.size(); i++)
        {

            Mat templateImage = weapons.at(i).getTemplateImage();
            int w = templateImage.cols;
            int h = templateImage.rows;

            //Az �sszehasonl�t�st mintafelismer�ssel v�gezz�k
            matchTemplate(img_gray, templateImage, resultImage, TM_CCOEFF_NORMED);

            Mat locations;
            findNonZero(resultImage >= weapons.at(i).getThreshold(), locations);

            // t�rolja az el�z� tal�lat koordin�t�it
            Point prev_pt(-1, -1);

            for (int j = 0; j < locations.total(); j++)
            {
                Point pt(locations.at<Point>(j).x, locations.at<Point>(j).y);

                // ellen�rzi, hogy az �j tal�lat t�vols�ga mekkora az el�z� tal�latt�l
                if (prev_pt.x < 0 || norm(pt - prev_pt) > 10)
                {
                    rectangle(img_rgb, pt, Point(pt.x + w, pt.y + h), Scalar(0, 0, 255), 2);

                    event["id_" + img_name.substr(0,img_name.find("."))].append(weapons.at(i).getName());

                    // friss�ti az el�z� tal�lat koordin�t�it
                    prev_pt = pt;
                }
            }

        }

        //Ha van tal�lat, ki�ratjuk
        if (event)
        {
            AllEvents.append(event);
            Json::StyledWriter styledWriter;
            cout << styledWriter.write(AllEvents);
        }
    }


    waitKey();
    return 0;
}

//Fegyverek beolvas�sa
vector<Weapon> readWeapons()
{
    vector<Weapon> weapons;
    string path = "./templates";
    for (const auto& entry : fs::directory_iterator(path))
    {
        string name = entry.path().filename().string();
        Weapon* tmp_weapon = new Weapon(name.substr(0, name.find('.')), imread(entry.path().string(), IMREAD_UNCHANGED));

        cvtColor(tmp_weapon->templateImage, tmp_weapon->templateImage, COLOR_BGR2BGRA);

        weapons.push_back(*tmp_weapon);
    }

    return weapons;
}

//A fegyverek mintak�p�hez be�ll�tunk egy thresholdot. Mivel a k�pek min�s�ge v�ltoz� lehet, el�fordulhat, hogy m�s a t�r�shat�r
void setThresholds(vector<Weapon> &weapons)
{

    vector<string> ths;

    fstream newfile;
    newfile.open("thresholds.txt", ios::in);

    if (newfile.is_open()) { 
        string tp;
        while (getline(newfile, tp)) { 
            ths.push_back(tp);
        }
        newfile.close(); 
    }

    
    for (int i = 0; i < ths.size(); i++)
    {
        string fullstring = ths.at(i);
        string name = fullstring.substr(0, fullstring.find('-'));
        double tmp = stod(fullstring.substr(fullstring.find('-') + 1));
        
        for (int j = 0; j < weapons.size(); j++)
        {
            if (weapons.at(j).getName() == name)
            {
                weapons.at(j).setThreshold(tmp);
            }
        }

    }
}