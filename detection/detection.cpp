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

        //lecsökkentjük a kép méretét, mert a kill feed minden esetben a jobb felsõ sarokban van így csak azt kell vizsgálni
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

            //Az összehasonlítást mintafelismeréssel végezzük
            matchTemplate(img_gray, templateImage, resultImage, TM_CCOEFF_NORMED);

            Mat locations;
            findNonZero(resultImage >= weapons.at(i).getThreshold(), locations);

            // tárolja az elõzõ találat koordinátáit
            Point prev_pt(-1, -1);

            for (int j = 0; j < locations.total(); j++)
            {
                Point pt(locations.at<Point>(j).x, locations.at<Point>(j).y);

                // ellenõrzi, hogy az új találat távolsága mekkora az elõzõ találattól
                if (prev_pt.x < 0 || norm(pt - prev_pt) > 10)
                {
                    rectangle(img_rgb, pt, Point(pt.x + w, pt.y + h), Scalar(0, 0, 255), 2);

                    event["id_" + img_name.substr(0,img_name.find("."))].append(weapons.at(i).getName());

                    // frissíti az elõzõ találat koordinátáit
                    prev_pt = pt;
                }
            }

        }

        //Ha van találat, kiíratjuk
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

//Fegyverek beolvasása
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

//A fegyverek mintaképéhez beállítunk egy thresholdot. Mivel a képek minõsége változó lehet, elõfordulhat, hogy más a tûréshatár
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