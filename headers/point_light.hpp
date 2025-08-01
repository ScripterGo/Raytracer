#pragma once
#include"sphere.hpp"

class point_light : public sphere{
    double brightness;
public:
    point_light(double _brightness, const math_space::matrix4& _transform, double radius = 1.0, math_space::vec4 _color = {255,255,255});
    math_space::vec4 light_interpolation(math_space::vec4 _color, math_space::vec4 pos, math_space::vec4 normal);
    double calculate_brightness(math_space::vec4 pos, math_space::vec4 normal);
    
    
    math_space::intersection_result ray_intersect(const math_space::ray& _ray);

    /*
        color interpolation
        (255,255,255) * brigntess / d^2 
    */

   math_space::vec4 get_color();
   double get_brightness();
};

