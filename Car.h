//
// Created by vladimir on 09.03.19.
//

#ifndef CAR_PROJECT_CAR_H
#define CAR_PROJECT_CAR_H

#define END 100
#define RIGHT 101
#define LEFT 102
#define GO 103
#define SPEED 70
#define CROSS_SPEED 50
#define C_ZONE 20
#define DLG_ZONE 60
#define DISTANCE 2

#include "Cross.h"
#include <mutex>
#include "Building.h"
#include <chrono>

class Cross_;

class Car_{
public:
    int x;
    int y;
    int delegate_flag;
    int id;
    int speed;
    int pre_speed;
    std::mutex* mutx;
    std::vector <Cross_*> goals;
    Car_(int, int);
    void* run();
    int color;
    std::chrono::milliseconds           ticker;
    void move(std::chrono::milliseconds &, Car_*);
    Car_();
    void setid(int i);
};

#endif //CAR_PROJECT_CAR_H
