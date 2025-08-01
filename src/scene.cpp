
#include"../headers/scene.hpp"

using namespace std;
using namespace surface_space;
using namespace math_space;

scene::scene() : cam(focal, fov_horizontal, fov_vertical) {}

//x is the number of rows, y is the number of columns
surface scene::render(int x, int y){
    vector<vec4> uv_directions = this->cam.generate_uv_directions(x,y);
    surface res_surface(y,x);
    vec4 P = this->cam.get_transform().offset();

    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            ray new_ray = ray(P, uv_directions[i*y + j]);
            intersection_result res = this->raycast(new_ray);
            vec4 res_color = this->calculate_color_from_ray(res, new_ray, 8);

            COLOR_FORMAT_RGB_U8U8U8 reformatted_color = this->vec4_to_U8_format(res_color);
            res_surface.set(i,j, reformatted_color);
        } 
    }
    return res_surface;
}

intersection_result scene::raycast(const ray& _ray){
    intersection_result curr_best = NO_INTERSECTION;

    for(int i = 0; i < (int) this->sphere_list.size(); i++){
        intersection_result res = this->sphere_list[i].ray_intersect(_ray);
        if(res.did_intersect == false) continue;
        if(curr_best.did_intersect == false || res.t < curr_best.t){
            curr_best = res;
        };
    }
    for(int i = 0; i < (int) this->plane_list.size(); i++){
        intersection_result res = this->plane_list[i].ray_intersect(_ray);
        if(res.did_intersect == false) continue;
        if(curr_best.did_intersect == false || res.t < curr_best.t){
            curr_best = res;
        };
    }
    for(int i = 0; i < (int) this->point_light_list.size(); i++){
        intersection_result res = this->point_light_list[i].ray_intersect(_ray);
        if(res.did_intersect == false) continue;
        if(curr_best.did_intersect == false || res.t < curr_best.t){
            curr_best = res;
        };
    }

    return curr_best;
}

//t denotes how much of b should be included
vec4 scene::blend_color_vec4(const vec4& a, const vec4& b, double t){
    double buff[3];
    const double* ap = a._data().data();
    const double* bp = b._data().data();
    color_blending(ap,bp,t,buff);
    
    return {buff[0], buff[1], buff[2], 0};
}

double scene::light_cast(const vec4& origin, const vec4& normal, const point_light& source){
    vec4 dir = (source.get_transform().offset() - origin).unit();
    double d = (source.get_transform().offset() - origin).magnitude();
    ray r(origin + normal * scene::EPSILON, dir);
    intersection_result res = this->raycast(r);

    if(res.did_intersect == true && res.typ == POINT_LIGHT){
        return 1;
    }else{
        return 0.1;
    }
}

vec4 scene::calculate_light_influence_hit(vec4 hit_point, vec4 hit_normal, vec4 hit_color){
    double r = 0;
    vec4 color = hit_color;

    for(int i = 0; i < this->point_light_list.size(); i++){
        vec4 light_color = this->point_light_list[i].get_color();
        vec4 light_origin = this->point_light_list[i].get_transform().offset();
        vec4 light_dir = (light_origin - hit_point).unit();
        
        double light_brightness = this->point_light_list[i].get_brightness();
        double d = (light_origin - hit_point).magnitude();
        double scaled_brightness = min(light_brightness / (d*d), 1.0);

        double max_scaling_factor = min(1.0 / hit_color[0], min(1.0/hit_color[1], 1.0/hit_color[2]));
        double normal_scalar = max(light_dir.dot(hit_normal), 0.0); //Assuming hit_normal is normalized
        double color_similarity = min(light_color.dot(color) / (light_color.magnitude() * color.magnitude()), max_scaling_factor);
        double shadow_multiplier = this->light_cast(hit_point, hit_normal, this->point_light_list[i]);

        double blend_factor = min(max((color_similarity - 0.7)/d * shadow_multiplier, 0.0), max_scaling_factor);

        r +=  normal_scalar * scaled_brightness * color_similarity * shadow_multiplier;
        color = blend_color_vec4(color, light_color, blend_factor);
    }

    return color * min(r, 1.0);
}

vec4 scene::calculate_light_influence_nohit(vec4 origin, vec4 normal){
    vec4 color = {0,0,0,0};
    double r = 0;

    for(int i = 0; i < this->point_light_list.size(); i++){
        vec4 light_color = this->point_light_list[i].get_color();
        vec4 light_origin = this->point_light_list[i].get_transform().offset();

        double light_brightness = this->point_light_list[i].get_brightness();
        double d = (light_origin - origin).magnitude();
        double scaled_brightness = min(light_brightness / (d*d), 1.0);
        double shadow_multiplier = this->light_cast(origin, normal, this->point_light_list[i]);

        vec4 light_dir = (light_origin - origin).unit();
        double normal_scalar = max(light_dir.dot(normal), 0.0); //Assuming hit_normal is normalized
        double blend_factor = min(normal_scalar * scaled_brightness * shadow_multiplier/d, 1.0);
        
        r += normal_scalar * scaled_brightness * shadow_multiplier;
        color = blend_color_vec4(color, light_color, blend_factor);
    }

    return color * min(r, 1.0);
}

vec4 scene::calculate_color_from_ray(const intersection_result& ray_res, const ray& _ray, int bounce, bool has_bounced){
    if(ray_res.did_intersect == true){
        if(ray_res.typ == OBJECT_TYPE::POINT_LIGHT) return ray_res.color;
        vec4 hit_normal = ray_res.normal;
        vec4 intersection_point = _ray.get_dir()*ray_res.t  + _ray.get_point();
        vec4 color = this->calculate_light_influence_hit(intersection_point, hit_normal, ray_res.color);

        if(bounce > 0){
            double r = 0.95;

            matrix4 refl_matrix = matrix4::from_line_reflection(hit_normal);
            vec4 new_dir = refl_matrix * (-_ray.get_dir());
            ray new_ray(intersection_point + new_dir*EPSILON, new_dir);
            intersection_result ray_res = this->raycast(new_ray);
            vec4 other_color = this->calculate_color_from_ray(ray_res, new_ray, bounce-1, true);

            return this->blend_color_vec4(color, other_color, r);
        }

        return color;
    }else{
        vec4 point = _ray.get_point();
        vec4 point_normal = _ray.get_dir();
        return {0,0,0,0};
        /*
        if(has_bounced == false) return {0,0,0,0};
        return this->calculate_light_influence_nohit(point, point_normal);
        */
    }
}

void scene::add_sphere(double radius, const matrix4& transform, vec4 color){
    this->sphere_list.push_back(sphere(radius, transform, color));
}
void scene::add_sphere(double radius){
    this->sphere_list.push_back(sphere(radius, this->default_transform, {255,0,0}));
}
void scene::add_plane(const matrix4& transform, vec4 color, double width, double height){
    this->plane_list.push_back(plane(transform, color, width, height));
}
void scene::add_point_light(double brightness, const matrix4& transform, double radius, vec4 color){
    this->point_light_list.push_back(point_light(brightness, transform, radius, color));
}

void scene::set_camera_transform(const matrix4& transform){
    this->cam.set_transform(transform);
};

COLOR_FORMAT_RGB_U8U8U8 scene::vec4_to_U8_format(const vec4& a){
    return {
        static_cast<uint16_t>(a[0] * 255.0),
        static_cast<uint16_t>(a[1] * 255.0),
        static_cast<uint16_t>(a[2] * 255.0)
    };
}
