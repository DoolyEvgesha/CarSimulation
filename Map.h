//
// Created by vladimir on 13.04.19.
//

#ifndef CAR_PROJECT_MAP_H
#define CAR_PROJECT_MAP_H

#define MULTI 1.5
#define CAR_LENGTH 3
#define CAR_WIDTH 2
#define MAP_X 840
#define MAP_Y 470

#include "Building.h"
#include "Cross.h"
#include "Car.h"

#include <vector>
#include <SFML/Graphics.hpp>
#include <string>

class Map_{
public:
    int pid;
    static Map_* get();
    static Map_* Map;
    sf::RenderWindow* window;
    sf::Event* event;
    std::vector <Building_*> bldngs;
    std::vector <std::vector<std::pair<int, int>>> bitmap;
    std::vector <Car_*> cars;
    std::vector <Cross_*> cross;
    void start();
    void draw();
    Map_();
};

#endif //CAR_PROJECT_MAP_H
