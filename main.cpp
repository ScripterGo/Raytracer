
#include <cassert>
#include<iostream>
#include"ray_tracing_config.h"
#include"headers/camera.hpp"
#include"headers/surface.hpp"
#include"headers/math_lib.hpp"
#include"headers/scene.hpp"

using namespace std;
using namespace surface_space;
using namespace math_space;

int main(){
    /*
    surface_space::surface S(256,256);
    S.set(0,0, {255, 0, 0});
    S.render();

    math_space::vec4 t = {1,2,3,4};
    t.print_it();
    math_space::matrix4 m = {
        {1,2,3,4},
        {3,4,5,6},
        {5,6,7,8},
        {8,9,10,11}
    };
    */

    scene A;

    math_space::matrix4 sphere_transform{
        {1, 0, 0, 0.4},
        {0, 1, 0, 1.0},
        {0, 0, 1, -4.5},
        {0, 0, 0, 1}
    };

    //math_space::matrix4 plane_transform = matrix4::from_x_rotation(-M_PI/2.0);
    math_space::matrix4 plane_transform = (matrix4::from_translation({0,0,-4,0}) * matrix4::from_x_rotation(M_PI/2.0));
    
    A.add_sphere(1.0, sphere_transform, {1.0,0,0,0});
    A.add_sphere(1.0, math_space::matrix4::from_translation({-3,0,0,0}) * sphere_transform, {0.6,0.0,0.4,0});
    A.add_sphere(0.5, math_space::matrix4::from_translation({-1.5,-0.5,1.7,0}) * sphere_transform, {0.8,0.8,0.8,0});
    A.add_plane(plane_transform, {0,0,0.5,0}, 10.0, 6.0);
    A.add_point_light(5, matrix4::from_translation({0.5, 2, -2, 0}), 0.2, {1.0,0.0,0.75,0});
    A.add_point_light(10, matrix4::from_translation({-0.5, 3.5, -2, 0}), 0.2, {1.0,1.0,1.0,0});
    A.add_point_light(5, matrix4::from_translation({1.5, 2, -3, 0}), 0.2, {1.0, 0.0, 0.0, 0.0});

    math_space::matrix4 cam_transform{
        {1, 0, 0, 0},
        {0, 1, 0, 3},
        {0, 0, 1, -0.8},
        {0, 0, 0, 1}
    };
    cam_transform = cam_transform * matrix4::from_x_rotation(math_space::PI / 6.0);

    //Translation has to be done first
    //cam_transform = cam_transform * matrix4::from_x_rotation(M_PI/8);

    A.set_camera_transform(cam_transform);
    surface S = A.render(1200,1200);
    
    S.render();
    /*
    math_space::matrix4 proj_matrix = math_space::matrix4::from_projection({1,1,1,0});
    (proj_matrix * vec4{1,1,0,0}).print_it();
    math_space::matrix4 refl_matrix = math_space::matrix4::from_line_reflection({1,0,0,0});
    (refl_matrix * vec4{2,0,-1,0}).print_it();
    */
    return 0;
}

