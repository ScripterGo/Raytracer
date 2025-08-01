
#pragma once
#include"./surface.hpp"
#include"./math_lib.hpp"

class sphere{
protected:    
    math_space::matrix4 transform;
    math_space::vec4 color;
    double radius;

public:
    sphere(double r, const math_space::matrix4& _transform, math_space::vec4 _color);

    math_space::intersection_result ray_intersect(const math_space::ray& r) const;

    const math_space::matrix4& get_transform() const;
};