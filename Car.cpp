//
// Created by vladimir on 13.04.19.
//

/*#define END 100
#define RIGHT 101
#define LEFT 102
#define GO 103
#define SPEED 100
#define CROSS_SPEED 50
#define C_ZONE 22
#define DLG_ZONE 40*/

#include "Car.h"
#include "Map.h"

#include <iostream>
#include <random>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <fstream>

extern std::mutex mute;

#define MSGSZ 128
using namespace std;

int func(msgbuf &buffer, int msgid);
int find(Cross_* from, Cross_* to);

void delegate(Cross_* from, Cross_* to)
{
    ++to->counter[find(from, to)].second;
    //cout << to->counter[find(from, to)].first << " " << to->counter[find(from, to)].second << endl;
}

void undelegate(Cross_* from, Cross_* to)
{
    --to->counter[find(from, to)].second;
    //cout << to->counter[find(from, to)].first << " " << to->counter[find(from, to)].second << endl;
}

int find(Cross_* from, Cross_* to) {
    for (int i = 0; i < (to->counter).size(); ++i) {
        if (to->counter[i].first == from) {
            return i;
        }
    }
    return 0;
}

int is_right(Car_* car, Cross_* goal)
{
    //cout << "Try to turn" << endl;
    //cout << car-> goals[0]->x << ";" << car->goals[0]->y << " to " << car->goals[1]->x << ";" << car->goals[1]->y << endl;
     if (car->goals[1]->y == car->goals[0]->y)
    {
        if ((car->goals[1]->y - goal->y)*(car->goals[1]->x - car->goals[0]->x) < 0) {
            return 1;
        }
        else
            return 0;
    }
    if (car->goals[1]->x == car->goals[0]->x)
    {
        if ((car->goals[1]->x - goal->x)*(car->goals[1]->y - car->goals[0]->y) > 0) {
            return 1;
        }
        else {
            return 0;
        }
    }
    return 3;
}

int is_right_turn(Car_* car, Cross_* goal)
{
    //cout << "Try to turn" << endl;
    //cout << car-> goals[0]->x << ";" << car->goals[0]->y << " to " << car->goals[1]->x << ";" << car->goals[1]->y << endl;
    if (car->goals[2]->y == car->goals[1]->y)
    {
        if ((car->goals[2]->y - goal->y)*(car->goals[2]->x - car->goals[1]->x) < 0)
            return 1;
        else
            return 0;
    }
    if (car->goals[2]->x == car->goals[1]->x)
    {
        if ((car->goals[2]->x - goal->x)*(car->goals[2]->y - car->goals[1]->y) > 0)
            return 1;
        else
            return 0;
    }
    return 3;
}

int is_left(Car_* car, Cross_* goal)
{
    //cout << "Try to turn" << endl;
    //cout << car-> goals[0]->x << ";" << car->goals[0]->y << " to " << car->goals[1]->x << ";" << car->goals[1]->y << endl;
    if (car->goals[1]->y == car->goals[0]->y)
    {
        if ((car->goals[1]->y - goal->y)*(car->goals[1]->x - car->goals[0]->x) > 0)
            return 1;
        else
            return 0;
    }
    if (car->goals[1]->x == car->goals[0]->x)
    {
        if ((car->goals[1]->x - goal->x)*(car->goals[1]->y - car->goals[0]->y) < 0)
            return 1;
        else
            return 0;
    }
    return 3;
}


int turn_right(Car_* cr)
{
    cout << cr->id << ": Let's turn_right" << endl;
    for (auto c: cr->goals[1]->rels)
    {
        if (is_right(cr, c)) {
            cr->goals[/*cr->goals.size() - 1*/2] = c;
            cout << "it, s OK" << endl;
        }
    }
    return RIGHT;
}
int turn_left(Car_* cr)
{
    cout << cr->id << ": Let's turn_left" << endl;
    for (auto c: cr->goals[1]->rels)
    {
        if (is_left(cr, c)) {
            cr->goals[/*cr->goals.size() - 1*/2] = c;
            cout << "it, s OK" << endl;
        }
    }
    return LEFT;
}
int go_forward(Car_* cr)
{
    /*if (dt != 0)
        tm = clock() - dt;
    dt = 0;*/
    //cout << "Go at: " << cr->x << " " << cr->y << endl;
    cr->speed = SPEED;
    return GO;
}
int stop(Car_* cr){
    //cout << cr->id << ": Let's stop" << endl;
    /*if (dt == 0)
        dt = clock() - time;*/
    //cout << "Stop at: " << cr->x << " " << cr->y << endl;
    //cr->pre_speed = cr->speed;
    cr->speed = 0;
    return 0;
}
int crash(Car_* car)
{
    cout << car->id << ": finish" << endl;
    if (car->id != 0)
        msgctl(car->id, IPC_RMID, NULL);
    car->x = 0;
    car->y = 0;
    return 1;
}

int func(msgbuf &buffer, Car_* cr, float &tm, float &dt)
{
    int x = 0;
    if (!strcmp(buffer.mtext, "turn_right"))
        x = turn_right(cr);
    if (!strcmp(buffer.mtext, "turn_left"))
        x = turn_left(cr);
    if (!strcmp(buffer.mtext, "go_forward"))
        x = go_forward(cr);
    if (!strcmp(buffer.mtext, "stop")) {
        x = stop(cr);
    }
    if (!strcmp(buffer.mtext, "end"))
        x = crash(cr);
    return x;
}

void Car_::move(float& time) {
    int t = (int)((((float)clock()-time) / CLOCKS_PER_SEC)*speed);
    if (speed == 0)
    {time = clock();}
    if (t < 1)
        return;
    if (goals[1]->x-goals[0]->x > 0)
    {
        x += t;
        time = clock();
        return;
    }
    if (goals[1]->x-goals[0]->x < 0)
    {
        x -= t;
        time = clock();
        return;
    }
    if (goals[1]->y-goals[0]->y >0)
    {
        y += t;
        time = clock();
        return;
    }
    if (goals[1]->y-goals[0]->y < 0)
    {
        y -= t;
        time = clock();
        return;
    }
}

Car_::Car_()
{
    id = 0;
    speed = SPEED;
    x = 0;
    y = 0;
    mutx = new mutex;
    Map_::get()->cars.push_back(this);
}

void* Car_::run()
{
    float           tm;
    float           dt;
    Cross_*         ex = nullptr;
    int             num;
    srand(time(0));
    char            null[MSGSZ] = {};
    int             msgid = msgget(id, IPC_CREAT | 0666);
    struct msgbuf   buf = {};
    int cross_flag = 0;
    int g;
    int delegate_flag = 0;
    int rast_end;
    int rast_beg;
    int mtx_flag = 0;

    buf.mtype = 1;
    strcpy(buf.mtext, null);
    tm = clock();

    while (errno == 0 || !strcmp(buf.mtext, null)) {


        errno = 0;
        msgrcv(msgid, &buf, MSGSZ, 1, IPC_NOWAIT);
        g = func(buf, this, tm, dt);

        if (g == 1)
            return NULL;

        if (buf.mtext[0] != 0)
            strcpy(buf.mtext, null);



        //rast_btw = (goals[1]->x-goals[0]->x)+(goals[1]->y-goals[0]->y);
        rast_end = (goals[1]->x - x) + (goals[1]->y - y);
        rast_beg = (goals[0]->x - x) + (goals[0]->y - y);

        if (speed != 0) {
            if (((abs(rast_beg) < C_ZONE) || abs(rast_end) < C_ZONE) && speed != 0 && cross_flag != 1) {
                //cout << "stop speed" << endl;
                speed = CROSS_SPEED;
                cross_flag = 1;
            } else {
                if (((abs(rast_beg) > C_ZONE) && abs(rast_end) > C_ZONE) && (cross_flag == 1) && speed != 0) {
                    speed = SPEED;
                    cross_flag = 0;
                }
            }
        }

        if (delegate_flag == 1) {
            if (abs(rast_end) <= C_ZONE) {
                if (goals[1]->delegators.front()->id != id) {
                    if (speed != 0) {
                        stop(this);
                    }
                } else {
                    if (mtx_flag == 0) {
                        goals[1]->mtx->lock();
                        mtx_flag = 1;
                        speed = CROSS_SPEED;
                    }
                }
            } else
            {
                speed = CROSS_SPEED;
            }
        }

        if (delegate_flag == 0) {
            if (abs(rast_end) < DLG_ZONE) {
                goals[1]->mtx_reserv->lock();
                goals[1]->delegators.push_back(this);
                delegate(goals[0], goals[1]);
                goals[1]->mtx_reserv->unlock();
                delegate_flag = 1;
            }
        }

        if (delegate_flag == 1) {
            if ((abs)(rast_beg) > C_ZONE && abs(rast_end) > DLG_ZONE) {
                goals[0]->mtx_reserv->lock();
                goals[0]->delegators.pop_front();
                goals[0]->mtx_reserv->unlock();
                //cout << "Otpiska" << rast_beg << endl;
                speed = SPEED;
                delegate_flag = 0;
            }
        }


        //mutx->lock();
        this->move(tm);
        //mutx->unlock();
        //usleep(1000);

        if (abs(x - goals[1]->x) == 0 && abs(y - goals[1]->y) == 0) {
            //cout << sqrt((goals[1]->x-goals[0]->x)*(goals[1]->x-goals[0]->x) + (goals[1]->y-goals[0]->y)*(goals[1]->y-goals[0]->y)) / ((float)(clock()-a)/CLOCKS_PER_SEC) << endl;
            //mute.lock();
            //mute.unlock();
            mutx->lock();
            x = goals[1]->x;
            y = goals[1]->y;
            mutx->unlock();

            if (!((goals[2]->x == goals[1]->x && goals[1]->x == goals[0]->x) ||
                  (goals[2]->y == goals[1]->y && goals[1]->y == goals[0]->y)) || (goals[2] == goals[0])) {
                //    mutx->unlock();
                usleep(150000);
                //    mutx->lock();
            }

            //goals[1]->counter[find(goals[0], goals[1])].second--;
            num = rand() % (goals[goals.size() - 1]->rels.size()) + 0;
            Cross_ *r = goals[0];
            ex = goals[goals.size() - 1]->rels[num];
            undelegate(goals[0], goals[1]);
            goals.erase(goals.begin());
            if (goals.size() == 2)
                goals.push_back(ex);
            goals[0]->mtx->unlock();
            mtx_flag = 0;

            if (!((goals[1]->x == goals[0]->x && goals[0]->x == r->x) ||
                  (goals[1]->y == goals[0]->y && goals[0]->y == r->y)) || (goals[1] == r)) {
                //    mutx->unlock();
                usleep(150000);
                //    mutx->lock();
            }
            tm = clock();
        }usleep(50);
    }
    cout << "Car crushed" << endl;
    exit(0);
}

Car_::Car_(int d_x, int d_y){
    id = 0;
    speed = SPEED;
    x = d_x;
    y = d_y;
    mutx = new mutex;
    Map_::get()->cars.push_back(this);
}


void Car_::setid(int i)
{
    id = i+1;
}