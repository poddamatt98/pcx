//
// Created by podda on 05/07/2021.
//

#ifndef PCX_PCX_H
#define PCX_PCX_H
#include <iostream>
#include "mat.h"

bool load_pcx(const std::string& filename, mat<uint8_t>& img);

#endif //PCX_PCX_H
