
#pragma once
#include<stdint.h>
#include<cmath>

struct COLOR_FORMAT_RGB_U8U8U8{
    uint16_t r;
    uint16_t g;
    uint16_t b;
};

enum OBJECT_TYPE{
    UNKNOWN = 0,
    SPHERE = 1,
    PLANE = 2,
    POINT_LIGHT = 3
};

inline void color_blending(const double a[3], const double b[3], const double& t, double r[3]){
    for(int i = 0; i < 3; i++){
        r[i] = sqrt((1.0 - t) * a[i]*a[i] + t * b[i]*b[i]); 
    }
}

