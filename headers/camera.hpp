
#pragma once
#include<vector>
#include"math_lib.hpp"

class camera{
    math_space::matrix4 transform;
    const double focal_length;
    const double fov_horizontal;
    const double fov_vertical;
    
    double lens_width;
    double lens_height;
    double lens_ratio;

public:
    camera(double _focal, double _fov_x, double _fov_y);
    std::vector<math_space::vec4> generate_uv_directions(int x, int y);

    math_space::matrix4 get_transform() const;
    void set_transform(const math_space::matrix4& new_transform);
};