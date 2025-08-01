
#pragma once
#include<vector>
#include"util.hpp"

namespace surface_space{
    static const COLOR_FORMAT_RGB_U8U8U8 PLACEHOLDER = {0,255,0};

    class surface{
        int width;
        int height;
        std::vector<std::vector<COLOR_FORMAT_RGB_U8U8U8>> color_grid;
    public:
        surface() = delete;
        surface(int _width, int _height);
        void render();
        void set(int row, int col, COLOR_FORMAT_RGB_U8U8U8 v);
        COLOR_FORMAT_RGB_U8U8U8 get(int row, int col);
    };
}

