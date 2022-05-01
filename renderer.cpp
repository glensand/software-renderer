#include "renderer.h"
#include "model.h"
#include "line.h"

namespace renderer {

    namespace detail {

        struct line_renderer final {
            line_renderer(tga_image& in_image, const tga_image::color& in_color) 
                : image(in_image)
                , color(in_color){}

            line_renderer& draw(const vector3& p1, const vector3& p2) {
                const auto [width, height] = image.size();

                int x0 = (p1.x + 1.) * width / 2.;
                int y0 = (p1.y + 1.) * height / 2.;
                int x1 = (p2.x + 1.) * width / 2.;
                int y1 = (p2.y + 1.) * height / 2.;

                line::brezenhaim({ x0, y0 }, { x1, y1 }, 
                    [this](int x, int y) { image.set(x, y, color); });

                return *this;
            }

            tga_image& image;
            const tga_image::color& color;
        };
    
    }

    void wireframe(const model& in_model, tga_image& in_image, const tga_image::color& in_color) {
        auto&& verticies = in_model.get_vert();
        auto&& triangles = in_model.get_trian();

        for(auto&& [p0, p1, p2] : triangles) {
            detail::line_renderer(in_image, in_color)
            .draw(verticies[p0], verticies[p1])
            .draw(verticies[p1], verticies[p2])
            .draw(verticies[p2], verticies[p0]);
        }
    }
}