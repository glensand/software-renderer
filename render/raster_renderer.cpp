#include "raster_renderer.h"
#include "tga_image.h"
#include "model.h"
#include "core/line.h"

raster_renderer::raster_renderer(tga_image& back_buffer)
    : m_back_buffer(back_buffer) {
    auto&& size = back_buffer.size();
    m_z_buffer.resize(size.x * size.y, std::numeric_limits<double>::min());
}

void raster_renderer::draw(const model& in_model) {
    auto&& verticies = in_model.get_vert();
    auto&& triangles = in_model.get_trian();
    const vector3f light_dir{ 0, 0, -1 };
    for(auto&& trianle : triangles) {
        auto&& verts = trianle.indicies;
        auto norm = cross(verticies[verts[2].vert] - verticies[verts[0].vert], 
            verticies[verts[1].vert] - verticies[verts[0].vert]);
        norm.normalize();
        if(const auto intencity = dot(norm, light_dir); intencity > 0) {
            render_triangle(trianle, intencity, in_model);
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

void raster_renderer::render_triangle(model::triangle t, double intencity, const model& model) {
    if (is_collapsed(t, model))
        return;
    
    sort_verticies(t, model);
    
    auto&& verticies = model.get_vert();
    auto&& t0 = to_screen_space(verticies[t.indicies[0].vert]);
    auto&& t1 = to_screen_space(verticies[t.indicies[1].vert]);  
    auto&& t2 = to_screen_space(verticies[t.indicies[2].vert]);

    auto&& t0_t2 = t2 - t0;
    auto&& t0_t1 = t1 - t0;
    auto&& t1_t2 = t2 - t1;

    auto&& height = t2.y - t0.y;
    if(height == 0) height = 1;
    for(auto i{ t0.y }; i <= t2.y; ++i) {
        auto&& p0 = t0 + t0_t2 * (double(i - t0.y) / height);
        auto&& segment_height = i <= t1.y ? t1.y - t0.y : t2.y - t1.y;
        if(segment_height == 0) segment_height = 1;
        auto&& p1 = i <= t1.y ? t0 + t0_t1 * (double(i - t0.y) / double(segment_height)) :
            t1 + t1_t2 * (double(i - t1.y) / double(segment_height));

        if(p0.x > p1.x) std::swap(p0, p1);
        auto&& p0_p1 = p1 - p0;
        auto&& length = p1.x - p0.x;
        if(length == 0) length = 1;
        for(auto j{ p0.x }; j <= p1.x; ++j) {
            auto&& cur_p = p0 + p0_p1 * (double(j - p0.x) / (double)length);
            if(depth_test({j, i, cur_p.z})){
                m_back_buffer.set(j, i, color(intencity));
            }
        }
    }
}

vector3i raster_renderer::to_screen_space(const vector3f& p){
    auto&& [width, height] = m_back_buffer.size();
    return { 
        int((p.x + 1.) * width / 2.), 
        int((p.y + 1.) * height / 2.), 
        int((p.z + 1.) * width / 2.)
    };
}

bool raster_renderer::depth_test(const vector3i& point) {
    auto&& pixel = m_z_buffer[point.y * m_back_buffer.size().x + point.x];
    bool result = false;
    if(point.z > pixel) {
        pixel = point.z;
        result = true;
    }
    return result;
}

void raster_renderer::sort_verticies(model::triangle& t, const model& model) {
    auto&& verticies = model.get_vert();
    auto&& indicies = t.indicies;
    if (verticies[indicies[0].vert].y > verticies[indicies[1].vert].y) 
        std::swap(indicies[0], indicies[1]);

    if (verticies[indicies[0].vert].y > verticies[indicies[2].vert].y) 
        std::swap(indicies[0], indicies[2]);

    if (verticies[indicies[1].vert].y > verticies[indicies[2].vert].y) 
        std::swap(indicies[1], indicies[2]);
}

bool raster_renderer::is_collapsed(const model::triangle& t, const model& model) {
    auto&& verticies = model.get_vert();
    auto&& eps = std::numeric_limits<double>::epsilon();
    return std::abs(verticies[t.indicies[0].vert].y - verticies[t.indicies[1].vert].y) < eps 
        && std::abs(verticies[t.indicies[0].vert].y - verticies[t.indicies[2].vert].y) < eps;
}