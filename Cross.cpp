//
// Created by vladimir on 15.04.19.
//

#include "Cross.h"
#include "Map.h"

Cross_::Cross_()
{
    mtx = new std::mutex;
    mtx_reserv = new std::mutex;
    Map_::get()->cross.push_back(this);
}

Cross_::Cross_(int d_x, int d_y){
    mtx = new std::mutex;
    mtx_reserv = new std::mutex;
    x = d_x;
    y = d_y;
    Map_::get()->cross.push_back(this);
}