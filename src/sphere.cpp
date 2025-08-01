
#include"../headers/sphere.hpp"
using namespace std;
using namespace math_space;

sphere::sphere(double _r, const matrix4& _transform, vec4 _color) : transform(_transform), color(_color), radius(_r) {};

intersection_result sphere::ray_intersect(const ray& _ray) const{
    vec4 p = _ray.get_point();
    vec4 v = _ray.get_dir();
    vec4 q = this->transform.offset();

    double p0 = p[0];
    double p1 = p[1];
    double p2 = p[2];
    
    double v0 = v[0];
    double v1 = v[1];
    double v2 = v[2];

    double q0 = q[0];
    double q1 = q[1];
    double q2 = q[2];
    double r = this->radius;

    double dir_magnitude_squared = v.dot(v);
    double a = (-2.0*p0*v0 + 2.0*q0*v0 - 2.0*p1*v1 + 2*q1*v1 - 2.0*p2*v2 + 2.0*q2*v2);

    double under_square_sign = (a*a - 
    4.0*(-r*r + p0*p0 + p1*p1 + p2*p2 - 2.0*p0*q0 + q0*q0 - 2.0*p1*q1 + q1*q1 - 2.0*p2*q2 + q2*q2)
    *dir_magnitude_squared);

    if(under_square_sign < 10e-5) return NO_INTERSECTION;

    double s = 1.0 / (2.0*dir_magnitude_squared);
    double t0 = s * (a + sqrt(under_square_sign));
    double t1 = s * (a - sqrt(under_square_sign));
    if(t0 < 10e-5 || t1 < 10e-5) return NO_INTERSECTION;

    if(t0 > t1) swap(t0, t1);
    return {
        true,
        t0,
        this->color,
        (v*t0 + p - q).unit(),
        OBJECT_TYPE::SPHERE
    };
}

const math_space::matrix4& sphere::get_transform() const{
    return this->transform;
}
