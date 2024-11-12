#pragma once
#include <stdexcept>
#include <iostream>

struct Color 
{
    float R;
    float G;
    float B;
    float A;
};


#define DIE_ON_NULL(VALUE, MSG) \
    if (VALUE == NULL) { \
        std::cout << MSG; \
        throw std::runtime_error(MSG); \
    }


#define DIE(MSG) \
        std::cout << MSG; \
        throw std::runtime_error(MSG); 