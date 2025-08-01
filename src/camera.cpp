
#include"../headers/camera.hpp"
#include<cmath>
using namespace std;
using namespace math_space;

camera::camera(double _focal, double _fov_horizontal, double _fov_vertical) : focal_length(_focal), fov_horizontal(_fov_horizontal), fov_vertical(_fov_vertical){
    this->lens_width = tan(this->fov_horizontal/2.0) * this->focal_length * 2.0;
    this->lens_height = tan(this->fov_vertical/2.0) * this->focal_length * 2.0;
    this->lens_ratio = this->lens_width / this->lens_height;
}

//x is the number of rows, y is the number of columns
vector<vec4> camera::generate_uv_directions(int x, int y){
    double delta_x = this->lens_width / double(y);
    double delta_y = this->lens_height / double(x);

    vec4 top_left = vec4({-tan(this->fov_horizontal/2.0) * this->focal_length, tan(this->fov_vertical/2.0) * this->focal_length, -this->focal_length, 0});
    vec4 edge_offset = vec4({delta_x/2.0, delta_y/2.0, 0, 0});
    vector<vec4> transformed_directions; transformed_directions.reserve(x*y);

    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            vec4 uv_coord = top_left + vec4({delta_x*j, -delta_y*i, 0, 0}) + edge_offset;
            vec4 dir = uv_coord.unit();
            vec4 dir_transformed = this->transform * dir;
            transformed_directions.push_back(dir_transformed);
        }
    }
    return transformed_directions;
}

matrix4 camera::get_transform() const{
    return this->transform;
}

void camera::set_transform(const matrix4& new_transform){
    this->transform = new_transform;
}
