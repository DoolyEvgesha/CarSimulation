#include <iostream>

#include "car.h"

Car::Car():
    id_     (0),
    length_ (0),
    width_  (0),
    lct_    (0)
{
    lct_.x_ = 0;
    lct_.y_ = 0;
}
