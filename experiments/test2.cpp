#include <iostream>

struct a {
    int x;
    int y;
};

void modify(struct a thing[3])
{
    thing[0].x = 5;
    thing[0].y = 10;
}

int main()
{
    struct a thing[3] = {};

    thing[0].x = 1;
    thing[0].y = 2;

    modify(thing);
    

    std::cout << thing[0].x << ' ' << thing[0].y << '\n';
}
