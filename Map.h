//
// Created by vladimir on 13.04.19.
//

#ifndef CAR_PROJECT_MAP_H
#define CAR_PROJECT_MAP_H

#include "Building.h"
#include "Cross.h"
#include "Car.h"

#include <vector>
#include <SFML/Graphics.hpp>

class Map_{
public:
    int pid;
    static Map_* get();
    static Map_* Map;
    sf::RenderWindow* window;
    sf::Event* event;
    std::vector <Building_*> bldngs;
    std::vector <Car_*> cars;
    std::vector <Cross_*> cross;
    void start();
    void draw();
    Map_();
};

#endif //CAR_PROJECT_MAP_H
