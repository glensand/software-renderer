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
    for(auto&& triangle : triangles) {
        auto&& indeces = triangle.indicies;
        line_renderer(m_back_buffer, White)
            .draw(verticies[indeces[0].vert], verticies[indeces[1].vert])
            .draw(verticies[indeces[1].vert], verticies[indeces[2].vert])
            .draw(verticies[indeces[2].vert], verticies[indeces[0].vert]);
    }
}
