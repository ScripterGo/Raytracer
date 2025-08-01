#pragma once
#include"../headers/math_lib.hpp"

class plane{
    math_space::matrix4 transform;
    math_space::vec4 color;

    double width;
    double height;
    math_space::vec4 normal;

public:
    plane(math_space::matrix4 _transform, math_space::vec4 _color, double _width, double _height);
    math_space::intersection_result ray_intersect(const math_space::ray& _ray);
};
