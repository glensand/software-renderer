/* Copyright (C) 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/software-renderer
 */

#pragma once

#include "renderer.h"
#include "tga_image.h"
#include "core/rmath.h"
#include "model.h"

#include <vector>

class raster_renderer final : public renderer {
public:
    raster_renderer(tga_image& back_buffer);
    virtual ~raster_renderer() = default;

    virtual void draw(const model& in_model) override;
private:

    void render_triangle(model::triangle t, double intencity, const model& model);
    static void sort_verticies(model::triangle& t, const model& model);
    static bool is_collapsed(const model::triangle& t, const model& model); 
    tga_image::color color(double intencity);
    
    vector3i to_screen_space(const vector3f& p);
    bool depth_test(const vector3i& depth);

    tga_image& m_back_buffer;
    std::vector<int> m_z_buffer;
};
