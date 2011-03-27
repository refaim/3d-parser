#include <iostream>
#include <cstdlib>

#include "aiVector3D.h"

int main()
{
    aiVector3D x;
    x.Set(1.0f, 2.0f, 3.0f);
    std::cout << x.x << ' ' << x.y << ' ' << x.z << std::endl;
    return EXIT_SUCCESS;
}
