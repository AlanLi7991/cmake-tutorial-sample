#include <iostream>
#include "hello.h"
#include "world.h"

int main(int argc, char **argv)
{
    std::string message = hello() + "," + world();
    std::cout << message << std::endl;
    return 0;
}