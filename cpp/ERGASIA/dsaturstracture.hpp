#include <iostream>

struct a_data
{
    int vertex;
    int color;
    bool operator<(const a_data &a)const
    {
    return this->color<a.color;
    }
    void print()
    {
        std::cout<<"\tVertex:"<<this->vertex<<"-->"<<this->color<<std::endl;
    }
};