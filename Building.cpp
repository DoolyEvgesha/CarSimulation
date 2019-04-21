//
// Created by vladimir on 13.04.19.
//

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "Building.h"
#include "Map.h"

Building_::Building_()
{
    width = 0;
    length = 0;
    Map_::get()->bldngs.push_back(this);
}

Coordinates_ &Coordinates_::operator=(Coordinates_ const &cr)
{
    x = cr.x;
    y = cr.y;
}

Building_::Building_(int d_w, int d_l){
    width = d_w;
    length = d_l;
    Map_::get()->bldngs.push_back(this);
}

Building_::Building_(int d_x, int d_y, int d_w, int d_l){
    x = d_x;
    y = d_y;
    width = d_w;
    length = d_l;
    Map_::get()->bldngs.push_back(this);
}

Coordinates_::Coordinates_()
{
    x = 0;
    y = 0;
}