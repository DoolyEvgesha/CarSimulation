//
// Created by vladimir on 09.03.19.
//

#ifndef CAR_PROJECT_CAR_H
#define CAR_PROJECT_CAR_H

#define END 100
#define RIGHT 101
#define LEFT 102
#define GO 103
#define SPEED 100
#define CROSS_SPEED 50
#define C_ZONE 20
#define DLG_ZONE 40

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
    void move(std::chrono::milliseconds &);
    Car_();
    void setid(int i);
};

#endif //CAR_PROJECT_CAR_H
