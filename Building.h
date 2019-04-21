//
// Created by vladimir on 13.04.19.
//

#ifndef CAR_PROJECT_BUILDING_H
#define CAR_PROJECT_BUILDING_H

#include <SFML/Graphics.hpp>


class Coordinates_{
public:
    int x;
    int y;
    Coordinates_();
    Coordinates_ & operator=(Coordinates_ const &cr);
};

class Building_ : public Coordinates_{
public:
    int width;
    int length;
    Building_();
    Building_(int, int, int, int);
    Building_(int, int);
};

#endif //CAR_PROJECT_BUILDING_H
