//
// Created by vladimir on 09.03.19.
//

#ifndef CAR_PROJECT_CAR_H
#define CAR_PROJECT_CAR_H

#include "Cross.h"
#include <mutex>
#include "Building.h"

class Cross_;

class Car_{
public:
    int x;
    int y;
    int id;
    int speed;
    int pre_speed;
    std::mutex* mutx;
    std::vector <Cross_*> goals;
    Car_(int, int);
    void* run();
    void move(float &);
    Car_();
    void setid(int i);
};

#endif //CAR_PROJECT_CAR_H
