#include <iostream>

#include "parseInfo.h"

Crossings::Crossings():
    num_(0)
{
    try
    {
        lct_ = new Location[num_];
    }
    catch(const std::bad_alloc & e)
    {
        std::cout << "\nOps, smth wrong with new memory for Location\n" << std::endl;
    }
}

Crossings::~Crossings()
{
}

bool Crossings::openDocument()
{
    std::fsteam fs;

    char path[PATH_SIZE] = {0};
    std::cout << "Please, enter the name of the file to open:" << std::endl;
    std::cin  >> path >> std::endl;

    fs.open(path, std::fsteam::in);
    Cross = getCrossings(fs);
    fs.close();

    return true;
}

bool Crossings::getCrossings(std::fsteam & fs)
{

}

bool Crossings::checkCrossing(Location *location)
{

}