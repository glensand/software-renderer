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
        255
    };
}

void raster_renderer::render_triangle(vector3i t0, vector3i t1, vector3i t2, tga_image::color color) {
    if (t0.y == t1.y && t0.y == t2.y) return;
    
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y > t2.y) std::swap(t1, t2);
    
    auto&& t0_t2 = t2 - t0;
    auto&& t0_t1 = t1 - t0;
    auto&& t1_t2 = t2 - t1;

    auto&& height = t2.y - t0.y;
    for(auto i{ t0.y }; i <= t2.y; ++i) {
        auto&& p0 = t0 + t0_t2 * (double(i - t0.y) / height);
        auto&& p1 = i <= t1.y ? t0 + t0_t1 * (double(i - t0.y) / (t1.y - t0.y)) :
            t1 + t1_t2 * (double(i - t1.y) / (t2.y - t1.y));
        if(p0.x > p1.x) std::swap(p0, p1);
        auto&& p0_p1 = p1 - p0;
        auto&& length = p1.x - p0.x;
        for(auto j{ p0.x }; j <= p1.x; ++j) {
            auto&& cur_p = p0 + p0_p1 * (double(j - p0.x) / length);
            if(depth_test({j, i, cur_p.z})){
                m_back_buffer.set(j, i, color);
            }
        }
    }
}

vector3i raster_renderer::to_screen_space(const vector3f& p){
    auto&& [width, height] = m_back_buffer.size();
    return { 
        int((p.x + 1.) * width / 2.), 
        int((p.y + 1.) * height / 2.), 
        int((p.z + 1.) * 255. / 2.)
    };
}

bool raster_renderer::depth_test(const vector3i& point) {
    auto&& pixel = m_z_buffer[point.y * m_back_buffer.size().x + point.x];
    bool result = false;
    if(point.z <= pixel) {
        pixel = point.z;
        result = true;
    }
    return result;
}
