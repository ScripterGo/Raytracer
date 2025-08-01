
#include"../headers/surface.hpp"
#include<iostream>

using namespace surface_space;

surface::surface(int _width, int _height) : width(_width), height(_height){
    this->color_grid.assign(_height, std::vector<COLOR_FORMAT_RGB_U8U8U8>(_width, PLACEHOLDER));
}

void surface::render(){
    std::cout << "P3\n" << this->width << ' ' << this->height << "\n255\n";

    for(int i = 0; i < this->height; i++){
        for(int j = 0; j < this->width; j++){
            std::cout << this->color_grid[i][j].r << ' ';
            std::cout << this->color_grid[i][j].g << ' ';
            std::cout << this->color_grid[i][j].b << '\n'; 
        }
    }
}

void surface::set(int row, int col, COLOR_FORMAT_RGB_U8U8U8 v){
    this->color_grid[row][col] = v;
}

COLOR_FORMAT_RGB_U8U8U8 surface::get(int row, int col){
    return this->color_grid[row][col];
}

