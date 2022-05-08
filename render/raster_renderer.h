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

struct point;

class raster_renderer final : public renderer {
public:
    virtual ~raster_renderer() = default;

    virtual void draw(const model& in_model, tga_image& in_image) override;

private:
    tga_image::color color(double intencity);
    void render_triangle(point t0, point t1, point t2, 
            tga_image &image, tga_image::color color);
};
