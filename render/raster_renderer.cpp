#include "raster_renderer.h"
#include "tga_image.h"
#include "model.h"
#include "core/line.h"

void raster_renderer::draw(const model& in_model, tga_image& in_image) {
    auto&& verticies = in_model.get_vert();
    auto&& triangles = in_model.get_trian();
    const auto [width, height] = in_image.size();
    const vector3f light_dir{ 0, 0, -1 };
    for(auto&& [p0, p1, p2] : triangles) {
        auto norm = cross(verticies[p2] - verticies[p0], 
            verticies[p1] - verticies[p0]);
        norm.normalize();
        const auto intencity = dot(norm, light_dir);
        if(intencity > 0) {
            auto&& ss_p0 = point { int((verticies[p0].x + 1.) * width / 2.), int((verticies[p0].y + 1.) * height / 2.)};
            auto&& ss_p1 = point { int((verticies[p1].x + 1.) * width / 2.), int((verticies[p1].y + 1.) * height / 2.)};
            auto&& ss_p2 = point { int((verticies[p2].x + 1.) * width / 2.), int((verticies[p2].y + 1.) * height / 2.)};
            render_triangle(ss_p0, ss_p1, ss_p2, in_image, color(intencity));
        }
    }
}

tga_image::color raster_renderer::color(double intencity) {
    return { 
        (unsigned char)(255.0 * intencity), 
        (unsigned char)(255.0 * intencity), 
        (unsigned char)(255.0 * intencity), 255
    };
}

void raster_renderer::render_triangle(point t0, point t1, point t2, tga_image &image, tga_image::color color) {
    if (t0.y==t1.y && t0.y==t2.y) return;
    
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y > t2.y) std::swap(t1, t2);
    
    auto&& t0_t2_equation = line::equation::build(t0, t2);
    auto&& t0_t1_equation = line::equation::build(t0, t1);
    auto&& t1_t2_equation = line::equation::build(t1, t2);
    for(auto i{ t0.y }; i <= t2.y; ++i) {
        auto&& x0 = t0_t2_equation.compute(i);
        auto&& x1 = i > t1.y ? t1_t2_equation.compute(i) : t0_t1_equation.compute(i);
        if(x0 > x1) std::swap(x0, x1);
        for(auto j { x0 }; j <= x1; ++j ){
            image.set(j, i, color);
        }
    }
}
