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

#include <vector>

struct point;

class raster_renderer final : public renderer {
public:
    raster_renderer(tga_image& back_buffer);
    virtual ~raster_renderer() = default;

    virtual void draw(const model& in_model) override;
private:
    tga_image::color color(double intencity);
    void render_triangle(vector3i t0, vector3i t1, vector3i t2, tga_image::color color);
    vector3i to_screen_space(const vector3f& p);
    bool depth_test(vector3i&& depth);

    tga_image& m_back_buffer;
    std::vector<int> m_z_buffer;
};
