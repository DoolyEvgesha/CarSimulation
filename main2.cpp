//
// Created by vladimir on 14.04.19.
//

#include "Map.h"
#include "Cross.h"

#include <SFML/Graphics.hpp>
#include <zconf.h>
#include <fstream>
#include <iostream>
#include <algorithm>


using namespace std;

Cross_* search(vector <Cross_*>* vec, int x, int y)
{
    for (int i = 0; i < (vec->end() - vec->begin()); ++i)
    {
        if ((*vec)[i]->x == x && (*vec)[i]->y == y)
            return (*vec)[i];
    }
    return nullptr;
}

int main()
{
    char c, k;
    Map_* m =           Map_::get();
    m->start();
    ifstream            finp("/home/vladimir/data", ios_base::in);
    vector <Cross_*>    cross;
    Cross_              crs[100];
    Cross_*             ex;
    Cross_*             elem;
    int                 i = 0, j = 0;
    char                code = 0;
    int                 x, y;
    int                 x_el = 0, y_el = 0;

    while(code != 'e' && !finp.eof())
    {
        finp >> x >> c >> y >> code;
        ex = search(&(cross), x, y);
        if (ex == nullptr)
        {
            crs[i].x = x;
            crs[i].y = y;
            cross.push_back(crs + i);
            ++i;
        }
    }
    finp.close();
    finp.open("/home/vladimir/data", ios_base::in);
    i = 0;
    code = 0;
    while(code != 'e' && !finp.eof())
    {
        finp >> x_el >> c >> y_el >> code;
        elem = search(&(cross), x_el, y_el);
        while (code != 'e' && code != 'n')
        {
            finp >> x >> c >> y >> code;
            ex = search(&(cross), x, y);
            elem->rels.push_back(ex);
        }
        ++i;
    }
    finp.close();
    m->draw();
    while((m->window)->isOpen())
    {
        while(m->window->pollEvent(*(m->event))) {
            if ((m->event)->type == sf::Event::Closed) {
                m->window->close();
            }
        }
        m->draw();
        usleep(100000);
    }
}