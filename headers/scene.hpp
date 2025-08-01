
#pragma once
#include<vector>
#include"../headers/plane.hpp"
#include"./sphere.hpp"
#include"./camera.hpp"
#include"./surface.hpp"
#include"./point_light.hpp"
#include"./util.hpp"
#include"./math_lib.hpp" //Need to be included last!

struct light_influence{
    math_space::vec4 color;
    double brightness;
};

class scene{
    std::vector<plane> plane_list;
    std::vector<sphere> sphere_list;
    std::vector<point_light> point_light_list;
    constexpr static double EPSILON = 1e-5;

    camera cam;

    const math_space::matrix4 default_transform = math_space::matrix4({
        {1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1}
    });

    constexpr static double focal = 1.0;
    constexpr static double fov_horizontal = math_space::PI/2.0;
    constexpr static double fov_vertical = math_space::PI/2.0;
public:
    scene();
    math_space::intersection_result raycast(const math_space::ray& _ray);
    surface_space::surface render(int x, int y);
    
    math_space::vec4 calculate_light_influence_hit(math_space::vec4 hit_point, math_space::vec4 hit_normal, math_space::vec4 hit_color);
    math_space::vec4 calculate_light_influence_nohit(math_space::vec4 origin, math_space::vec4 normal);
    double light_cast(const math_space::vec4& origin, const math_space::vec4& normal, const point_light& source);
    
    
    math_space::vec4 blend_color_vec4(const math_space::vec4& a, const math_space::vec4& b, double t);
    inline COLOR_FORMAT_RGB_U8U8U8 vec4_to_U8_format(const math_space::vec4& a);

    math_space::vec4 calculate_color_from_ray(const math_space::intersection_result& r, const math_space::ray& _ray, int bounce, bool has_bounced = false);

    void add_sphere(double radius, const math_space::matrix4& transform, math_space::vec4 color);
    void add_sphere(double radius);
    void add_plane(const math_space::matrix4& _transform, math_space::vec4 _color, double _width, double _height);
    void add_point_light(double brightness, const math_space::matrix4& transform, double radius, math_space::vec4 color);

    void set_camera_transform(const math_space::matrix4& transform);
};

