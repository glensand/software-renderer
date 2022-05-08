#include "raster_renderer.h"
#include "tga_image.h"
#include "model.h"
#include "core/line.h"

raster_renderer::raster_renderer(tga_image& back_buffer)
    : m_back_buffer(back_buffer) {
    auto&& size = back_buffer.size();
    m_z_buffer.resize(size.x * size.y, std::numeric_limits<int>::max());
}

void raster_renderer::draw(const model& in_model) {
    auto&& verticies = in_model.get_vert();
    auto&& triangles = in_model.get_trian();
    const vector3f light_dir{ 0, 0, -1 };
    for(auto&& [p0, p1, p2] : triangles) {
        auto norm = cross(verticies[p2] - verticies[p0], verticies[p1] - verticies[p0]);
        norm.normalize();
        const auto intencity = dot(norm, light_dir);
        if(intencity > 0) {
            render_triangle(to_screen_space(verticies[p0]), to_screen_space(verticies[p1]),
                 to_screen_space(verticies[p2]), color(intencity));
        }
    }
}

tga_image::color raster_renderer::color(double intencity) {
    return { 
        (unsigned char)(255.0 * intencity), 
        (unsigned char)(255.0 * intencity), 
        (unsigned char)(255.0 * intencity), 
        0
    };
}

void raster_renderer::render_triangle(vector3i t0, vector3i t1, vector3i t2, tga_image::color color) {
    if (t0.y == t1.y && t0.y == t2.y) return;
    
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y > t2.y) std::swap(t1, t2);
    
    auto&& t0_t2_equation = line::equation3::build(t0, t2);
    auto&& t0_t1_equation = line::equation3::build(t0, t1);
    auto&& t1_t2_equation = line::equation3::build(t1, t2);

    for(auto i{ t0.y }; i <= t2.y; ++i) {
        auto&& x0 = t0_t2_equation.x(i);
        auto&& equation = i > t1.y ? t1_t2_equation : t0_t1_equation;
        auto&& x1 = equation.x(i);

        if(x0 > x1) std::swap(x0, x1);

        // for z buffer
        auto&& z0 = t0_t2_equation.z(i);
        auto&& z1 = equation.z(i);
        auto&& z_x = line::equation2::build({ z0, x0 }, { z1, x1 });

        for(auto j { x0 }; j <= x1; ++j ) {
            if(depth_test({j, i, z_x(j)}))
                m_back_buffer.set(j, i, color);
        }
    }
}

vector3i raster_renderer::to_screen_space(const vector3f& p){
    auto&& [width, height] = m_back_buffer.size();
    return { 
        int((p.x + 1.) * width / 2.), 
        int((p.y + 1.) * height / 2.), 
        int((p.z + 1.) * 1000000)
    };
}

bool raster_renderer::depth_test(vector3i&& point) {
    auto&& pixel = m_z_buffer[point.y * m_back_buffer.size().x + point.x];
    bool result = false;
    if(point.z <= pixel) {
        pixel = point.z;
        result = true;
    }
    else{
        int stub = 0;
    }
    return result;
}
