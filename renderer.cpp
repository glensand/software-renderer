#include "renderer.h"
#include "model.h"
#include "line.h"

#include <random>

namespace renderer {

    namespace detail {

        struct line_renderer final {
            line_renderer(tga_image& in_image, const tga_image::color& in_color) 
                : image(in_image)
                , color(in_color){}

            using compute_callback_t = std::function<void(int, int)>;

            line_renderer& draw(const vector3& p1, const vector3& p2) {
                const auto [width, height] = image.size();

                line::rasterize({ (int)width, (int)height }, p1, p2, 
                    [this](int x, int y) { image.set(x, y, color); });

                return *this;
            }

            tga_image& image;
            const tga_image::color& color;
        };

        tga_image::color random_color() {
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<unsigned char> uni(0, 255);

            return tga_image::color{ uni(rng), uni(rng), uni(rng), 0 };
        }

        tga_image::color by_intencity(double intencity){
            return { (unsigned char)(255.0 * intencity), (unsigned char)(255.0 * intencity), (unsigned char)(255.0 * intencity), 255};
        }

        void render_triangle(point t0, point t1, point t2, 
            tga_image &image, tga_image::color color) {
            if (t0.y==t1.y && t0.y==t2.y) return; // i dont care about degenerate triangles
            
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

    void rasterize(const model& in_model, tga_image& in_image) {
        auto&& verticies = in_model.get_vert();
        auto&& triangles = in_model.get_trian();

        const auto [width, height] = in_image.size();

        const vector3 light_dir{ 0, 0, -1 };

        for(auto&& [p0, p1, p2] : triangles) {

            auto norm = cross(verticies[p2] - verticies[p0], 
                verticies[p1] - verticies[p0]);

            norm.normalize();

            const auto intencity = norm * light_dir;

            if(intencity > 0){
                auto&& ss_p0 = point { int((verticies[p0].x + 1.) * width / 2.), int((verticies[p0].y + 1.) * height / 2.)};
                auto&& ss_p1 = point { int((verticies[p1].x + 1.) * width / 2.), int((verticies[p1].y + 1.) * height / 2.)};
                auto&& ss_p2 = point { int((verticies[p2].x + 1.) * width / 2.), int((verticies[p2].y + 1.) * height / 2.)};

                detail::render_triangle(ss_p0, ss_p1, ss_p2, in_image, 
                    detail::by_intencity(intencity));
            }
        }
    }
}