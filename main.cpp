/* Copyright (C) 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/software-renderer
 */

#include "tga_image.h"
#include "model.h"
#include "render/raster_renderer.h"

constexpr static unsigned Width { 720 };
constexpr static unsigned Height { 720 };
constexpr tga_image::color White{ 255, 255, 255, 0 };

int main(){
    tga_image img(Width, Height);
    model in_model("head.obj", "diffuse.tga");
    auto* renderer = new raster_renderer(img);
    renderer->draw(in_model);

    img.flip_x();
    img.store("out.tga");
    return 0;
}