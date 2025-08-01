
#include"../headers/point_light.hpp"
using namespace std;
using namespace math_space;

point_light::point_light(
    double _brightness, const matrix4& _transform, double _radius, vec4 _color) 
    : sphere(_radius, _transform, _color), brightness(_brightness) {}

vec4 point_light::light_interpolation(vec4 _color, vec4 pos, vec4 normal){
    vec4 diff = this->transform.offset() - pos;
    double d = diff.magnitude();

    double angle_scalar = normal.dot(diff) / (normal.magnitude()*diff.magnitude());
    vec4 res = _color * angle_scalar * max(this->calculate_brightness(pos, normal), 1.0);
    
    return res;
}

double point_light::calculate_brightness(vec4 pos, vec4 normal){
    vec4 diff = this->transform.offset() - pos;
    double d = diff.magnitude();
    
    double angle_scalar = normal.dot(diff) / (normal.magnitude()*d);
    return max(angle_scalar, 0.0) * (this->brightness/powf64(d - this->radius, 2.0));
}

intersection_result point_light::ray_intersect(const ray& _ray){
    intersection_result res = sphere::ray_intersect(_ray);
    res.typ = POINT_LIGHT;
    return res;
}

vec4 point_light::get_color(){
    return this->color;
}

double point_light::get_brightness(){
    return this->brightness;
}
