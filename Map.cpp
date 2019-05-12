//
// Created by vladimir and Egorka on 14.04.19.
//


#include "Map.h"

#include <SFML/Graphics.hpp>
#include <zconf.h>
#include <iostream>


void Map_::draw()
{
    window->clear(sf::Color::Green);
    for (auto c:bldngs)
    {
        sf::RectangleShape rectangle;
        rectangle.setSize(sf::Vector2f(c->length * MULTI, c->width * MULTI));
        rectangle.setPosition((c->x - c->length) * MULTI, (c->y - c->width) * MULTI);
        rectangle.setFillColor(sf::Color::Black);
        window->draw(rectangle);
    }
    for (auto c:cross)
    {
        if (c->x || c->y) {

            for (auto p:(c->rels))
            {
                if ((c->x <= p->x) && (c->y <= p->y)) {
                    sf::RectangleShape road;
                    road.setSize(sf::Vector2f((abs(c->x - p->x) + 30)*MULTI, (abs(c->y - p->y) + 30)*MULTI));
                    road.setPosition((c->x - 15)*MULTI, (c->y - 15)*MULTI);
                    road.setFillColor(sf::Color::Black);
                    window->draw(road);
                }
            }
        }
    }

    for (auto c:cross)
    {
        if (c->x || c->y) {

            for (auto p:(c->rels))
            {
                if ((c->x <= p->x) && (c->y <= p->y)) {
                    sf::RectangleShape line;
                    line.setSize(sf::Vector2f((abs(c->x - p->x) + 2)*MULTI, (abs(c->y - p->y) + 2)*MULTI));
                    line.setPosition((c->x - 1)*MULTI, (c->y - 1)*MULTI);
                    line.setFillColor(sf::Color::White);
                    window->draw(line);
                }
            }
        }
    }
    for (auto c:cross)
    {
        if (c->x || c->y)
        {
            sf::RectangleShape road;
            road.setSize(sf::Vector2f(30 * MULTI, 30 * MULTI));
            road.setPosition((c->x - 15) * MULTI, (c->y - 15) * MULTI);
            road.setFillColor(sf::Color::Black);
            window->draw(road);
        }
    }
    for (auto c:cars)
    {
        if (c->x || c->y)
        {
            sf::RectangleShape car;
            //c->mutx->lock();
            if (c->goals[1]->x - c->goals[0]->x > 0) {
                for (int i = c->x-CAR_LENGTH; i < c->x+CAR_LENGTH; ++i)
                    bitmap[c->y][i].first = 1;
                car.setSize(sf::Vector2f(CAR_LENGTH * MULTI*2, CAR_WIDTH * MULTI*2));
                car.setPosition((c->x - CAR_LENGTH) * MULTI, (c->y + 6) * MULTI);
                car.setFillColor(sf::Color::White);
            }
            if (c->goals[1]->x - c->goals[0]->x < 0)
            {
                for (int i = c->x-CAR_LENGTH; i < c->x+CAR_LENGTH; ++i)
                    bitmap[c->y][i].second = 1;
                car.setSize(sf::Vector2f(CAR_LENGTH * MULTI*2, CAR_WIDTH * MULTI*2));
                car.setPosition((c->x - CAR_LENGTH) * MULTI, (c->y - 2*CAR_WIDTH - 6) * MULTI);
                car.setFillColor(sf::Color::White);
            }
            if (c->goals[1]->y - c->goals[0]->y > 0)
            {
                for (int i = c->y-CAR_LENGTH; i < c->y+CAR_LENGTH; ++i)
                    bitmap[i][c->x].first = 1;
                car.setSize(sf::Vector2f(CAR_WIDTH * MULTI*2, CAR_LENGTH * MULTI*2));
                car.setPosition((c->x - 6 - CAR_WIDTH*2) * MULTI, (c->y - CAR_LENGTH) * MULTI);
                car.setFillColor(sf::Color::White);
            }
            if (c->goals[1]->y - c->goals[0]->y < 0)
            {
                for (int i = c->y-CAR_LENGTH; i < c->y+CAR_LENGTH; ++i)
                    bitmap[i][c->x].second = 1;
                car.setSize(sf::Vector2f(CAR_WIDTH * MULTI*2, CAR_LENGTH * MULTI*2));
                car.setPosition((c->x + 6) * MULTI, (c->y - CAR_LENGTH) * MULTI);
                car.setFillColor(sf::Color::White);
            }
            //c->mutx->unlock();
            window->draw(car);
        }
    }
    window->display();
}

Map_* Map_:: Map = nullptr;

Map_* Map_::get()
{
    if (Map != nullptr)
    {
        return Map;
    }
    Map = new Map_;
    return Map;
}

Map_::Map_()
{
    Map = this;
    std::pair<int, int> pr;
    pr.first = 0;
    pr.second = 0;
    std::vector<std::pair<int, int>> s(MAP_X, pr);
    for (int i = 0; i < MAP_Y; ++i)
    {
        bitmap.push_back(s);
        //std::cout << bitmap[i][0].first << bitmap[i][0].second << std::endl;
    }
}

void Map_::start()
{
    window = new sf::RenderWindow(sf::VideoMode(MAP_X*MULTI, MAP_Y*MULTI), "Car_project");
    event = new sf::Event;
}