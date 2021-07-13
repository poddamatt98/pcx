#include "pcx.h"
#include <iostream>


int main() {
    mat<uint8_t> img(0, 0);

    load_pcx("bunny.pcx", img);
    return 0;
}
