#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <signal.h>
#include <poll.h>
#include <thread>
#include <functional>
#include "Car.h"
#include "Map.h"

#define MSGSZ 128
using namespace std;

mutex mute;
int delay;

int send_command(int id, char* com)
{
    int             msgqid = msgget(id, IPC_CREAT | 0666);
    struct msgbuf   msg;
    if (id == 0) {
        if (!strcmp(com, "end"))
            return 1;
        else
            return 0;
    }
    msg.mtype =     1;
    strcpy(msg.mtext, com);
    msgsnd(msgqid, &msg, strlen(msg.mtext)+1, IPC_NOWAIT);
    return 0;
}

void cars_create(int numb, vector<Car_*> cars)
{
    for (int i = 0; i < numb; ++i)
    {
        cars[i]->setid(i);// =   i+1;
    }
}

void msg_init(int* id, vector<Car_*> cars, int n)
{
    errno =         0;
    for (int i = 1; i <= n; ++i) {
        id[i] = msgget(cars[i-1]->id, IPC_CREAT | 0666);
        msgctl(id[i], IPC_RMID, NULL);
        id[i] = msgget(cars[i-1]->id, IPC_CREAT | 0666);
    }
    int crashes =   0;
    for (int i = 1; i <= n; ++i)
    {
        if (id[i] == 0) {
            crashes++;
            cout << "Car " << i << " init crashed" << endl;
        }
    }
    if (crashes != 0)
        cout << crashes << " Car init has been crashed" << endl;
}

void* run()
{
    return NULL;
}

void cars_init(int num, vector<Car_*> cars, thread** pthreads)
{
    for (int i = 0; i < num; ++i) {
        pthreads[i] = new thread([cars, i]{cars[i]->run();});
    }
}

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
    cout << "Hello!" << endl;
    Map_* m =           Map_::get();
    ifstream            finp("/home/vladimir/CarProject/data", ios_base::in);
    vector <Cross_*>    cross;
    Cross_              crs[100];
    Cross_*             ex;
    Cross_*             elem;
    int                 i = 0;
    char                code = 0;
    int                 x, y;
    int                 x_el = 0, y_el = 0;
    vector<Car_*>        cars;
    int num = 0;
    char c;

    while(code != 'e' && !finp.eof()) {
        finp >> x >> c >> y >> code;
        ex = search(&(cross), x, y);
        if (ex == nullptr) {
            crs[i].x = x;
            crs[i].y = y;
            cross.push_back(crs + i);
            ++i;
        }
    }
    finp.close();
    finp.open("/home/vladimir/CarProject/data", ios_base::in);
    i = 0;
    code = 0;
    while(code != 'e' && !finp.eof())
    {
        finp >> x_el >> c >> y_el >> code;
        elem = search(&(cross), x_el, y_el);
        if (elem == nullptr)
            return 0;
        while (code != 'e' && code != 'n')
        {
            finp >> x >> c >> y >> code;
            ex = search(&(cross), x, y);
            if (ex == nullptr)
                return 0;
            elem->rels.push_back(ex);
            elem->counter.push_back({ex, 0});
        }
        ++i;
    }
    int n = 0;
    code = 0;
    while(code != 'e' && !finp.eof())
    {
        i = 0;
        finp >> x >> c >> y >> code;
        cars.push_back(new Car_(x, y));
        ex = search(&(cross), x, y);
        if (ex == nullptr)
            return 0;
        cars[n]->goals.push_back(ex);
        while (code != 'e' && code != 'n')
        {
            finp >> x >> c >> y >> code;
            ++i;
            cout<< x << " " << c << " " << y << " " << code << endl;
            ex = search(&(cross), x, y);
            if (ex == nullptr)
                return 0;
            cars[n]->goals.push_back(ex);
        }
        ++n;
    }
    finp.close();
    for (int t = 0; t<n; ++t)
    {
        if (cars[t]->goals.size() == 1) {
            num = rand() % (cars[t]->goals[cars[t]->goals.size() - 1]->rels.size()) + 0;
            cars[t]->goals.push_back(cars[t]->goals[0]->rels[num]);
            num = rand() % (cars[t]->goals[cars[t]->goals.size() - 1]->rels.size()) + 0;
            cars[t]->goals.push_back(cars[t]->goals[1]->rels[num]);
        }
        if (cars[t]->goals.size() == 2) {
            num = rand() % (cars[t]->goals[cars[t]->goals.size() - 1]->rels.size()) + 0;
            cars[t]->goals.push_back(cars[t]->goals[1]->rels[num]);
        }
    }

    thread*      thr[n];
    int         msid[n+1];
    char        command[100] = {};
    int         id = 0;
    int         ser_end = 0;

    cars_create(n, cars);
    msg_init(msid, cars, n);
    cars_init(n, cars, thr);

    m->start();
    m->draw();
    int flag = 0;
    struct pollfd arr[1];
    arr[0].fd = 0;
    arr[0].events = POLLIN;
    cout << int(sf::Color::White.r) << endl << int(sf::Color::White.g) << endl << int(sf::Color::White.b) << endl;

    while((m->window)->isOpen())
    {
        while(m->window->pollEvent(*(m->event))) {
            if ((m->event)->type == sf::Event::Closed) {
                m->window->close();
            }
        }
        ser_end = 0;
        flag = poll(arr, 1, 2);
        if (flag == 1)
        {
            cin >> id >> command;

            if (cin.fail())
            {
                cout << "Specify the identifier please" << endl;
                cin.clear();
                cin >> command;
                id = 0;
            }

            if (id > n || id < 0)
                cout << "Invalid identifier of car" << endl;
            else
                ser_end = send_command(id, command);
        }
        m->draw();
        if (ser_end == 1)
        {
            m->window->close();
            break;
        }
    }


    struct msgbuf   buf;
    buf.mtype =     1;
    char null[100] = {};

    for (i = 1; i < n+1; ++i)
    {
        strcpy(buf.mtext, null);
        msgrcv(msid[i], &buf, MSGSZ, 1, IPC_NOWAIT);
    }

    strcpy(command, "end");
    for (i = 1; i <= n; ++i) {
        for (int k = 0; k < MAP_X; ++k)
            for (int l = 0; l < MAP_Y; ++l) {
                Map_::get()->bitmap[l][k].first = 0;
                Map_::get()->bitmap[l][k].second = 0;
            }
        send_command(i, command);
    }
    for (i = 0; i < n; ++i)
        thr[i]->join();

    cout << "Server crashed" << endl;
    return 0;
}