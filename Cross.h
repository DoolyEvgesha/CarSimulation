//
// Created by vladimir on 15.04.19.
//

#ifndef SFMLAPP_CROSS_H
#define SFMLAPP_CROSS_H

#include "Car.h"
#include "Building.h"
#include <mutex>
#include <list>

class Car_;

#include <vector>

class Cross_ : public Coordinates_ {
public:
    Cross_();
    Cross_(int, int);
    std::vector <Cross_*> rels;
    std::list <Car_*> delegators;
    std::vector <std::pair<Cross_*, int>> counter;
    std::mutex* mtx_reserv;
    std::mutex* mtx;
};


#endif //SFMLAPP_CROSS_H
