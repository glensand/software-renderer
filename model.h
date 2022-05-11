/* Copyright (C) 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/software-renderer
 */

#pragma once

#include <vector>
#include <tuple>
#include <string_view>
#include "tga_image.h"
#include "core/rmath.h"

class model final {
public:

    struct triangle final {
        struct indicies_t final {
            unsigned vert{ 0 }, uv{ 0 }, normal { 0 };
        } indicies[3];

        triangle(indicies_t* in_indicies) {
            for(auto i{ 0 }; i < 3; ++i)
                indicies[i] = in_indicies[i];
        }

        triangle(){}
    };

    using verticies_t = std::vector<vector3f>;

    // vector3i: vert/uv/normal
    using triangles_t = std::vector<triangle>;

    model(const char* model_file, const char* diffuse_file);
    
    const verticies_t& get_vert() const { return m_verticies; }
    const triangles_t& get_trian() const { return m_triangles; }
    const std::vector<vector2f>& get_uvs() const { return m_uvs; }
    
    tga_image::color diffuse(const point& uv) const;
    void load_texture(std::string_view texture_name);

    vector2f uv(unsigned triangle, unsigned vert);
private:
    verticies_t m_verticies;
    verticies_t m_normals;
    triangles_t m_triangles;
    
    std::vector<vector2f> m_uvs;
    tga_image m_diffuse_texture;
};