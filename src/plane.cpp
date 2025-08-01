
#include"../headers/plane.hpp"
using namespace math_space;

plane::plane(matrix4 _transform, vec4 _color, double _width, double _height) : transform(_transform), color(_color), width(_width), height(_height) {
    this->normal = transform.look_vector();
}

/*
ray: P_0 + vt

N dot (P_0 + vt) = 0



*/

intersection_result plane::ray_intersect(const ray& _ray){
    vec4 v = _ray.get_dir();
    vec4 p = _ray.get_point();
    vec4 origin = transform.offset();

    if(abs(normal.dot(v)) < 10e-6){
        //Meaning the ray *almost runs parallel to the plane. Which means we just ignore it.
        return NO_INTERSECTION;
    }

    //There is an intersection, but we must check for bounds
    double t = (-normal.dot(p) + normal.dot(origin)) / normal.dot(v);
    if(t < 0.0) return NO_INTERSECTION;
    
    vec4 intersection_point = p + v*t;

    //Change of basis, from standard basis to the basis defined by v0, v1 for the plane
    //This assumes v0 and v1 are perpendicular
    
    vec4 v0 = transform.right_vector();
    vec4 v1 = transform.up_vector();

    double v0_coord = v0.dot(intersection_point - origin) / (v0.magnitude());
    double v1_coord = v1.dot(intersection_point - origin) / (v1.magnitude());

    if(v0_coord < -width/2.0 || v1_coord < -height/2.0) return NO_INTERSECTION;
    if(v0_coord > width/2.0 || v1_coord > height/2.0) return NO_INTERSECTION;
    
    return {
        true,
        t,
        this->color,
        normal,
        OBJECT_TYPE::PLANE
    };
}
