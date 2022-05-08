#include "wireframe_renderer.h"
#include "tga_image.h"
#include "model.h"

#include "core/line.h"

namespace {

    constexpr tga_image::color White{ 255, 255, 255, 0 };

    struct line_renderer final {
        line_renderer(tga_image& in_image, const tga_image::color& in_color) 
            : image(in_image)
            , color(in_color){}
        
        using compute_callback_t = std::function<void(int, int)>;

        line_renderer& draw(const vector3f& p1, const vector3f& p2) {
            const auto [width, height] = image.size();
            line::rasterize({ (int)width, (int)height }, p1, p2, 
                [this](int x, int y) { image.set(x, y, color); });
            return *this;
        }
        tga_image& image;
        const tga_image::color& color;
    };

}

wireframe_renderer::wireframe_renderer(tga_image& back_buffer)
    : m_back_buffer(back_buffer) {

}

void wireframe_renderer::draw(const model& in_model) {
    auto&& verticies = in_model.get_vert();
    auto&& triangles = in_model.get_trian();
    for(auto&& [p0, p1, p2] : triangles) {
        line_renderer(m_back_buffer, White)
            .draw(verticies[p0], verticies[p1])
            .draw(verticies[p1], verticies[p2])
            .draw(verticies[p2], verticies[p0]);
    }
}
