#include "tools.h"

unsigned char Tools::from_terrain_index_to_terrain_type(unsigned int i)
{
    if (i < 2) return 'w';
    else return 'l';
}

std::pair<unsigned int, unsigned int> Tools::from_tile_index_to_axis(unsigned int index)
{
    unsigned int y = index / COL;
    return std::pair<unsigned int, unsigned int>(index - y * COL, y);
}

unsigned int Tools::from_tile_axis_to_index(std::pair<unsigned int, unsigned int>& axis)
{
    return axis.first + axis.second * COL;
}

Tools::UniqueIDGen::UniqueIDGen()
{
    max = 0;
}

Tools::UniqueIDGen::~UniqueIDGen()
{
}

unsigned int Tools::UniqueIDGen::getID()
{
    unsigned int n;
    if (!reuse.empty()) {
        n = reuse.at(0);
        reuse.erase(reuse.begin());
    }
    else {
        n = max++;
    }
    return n;
}

void Tools::UniqueIDGen::recycle_uid(unsigned int n)
{
    reuse.push_back(n);
}
