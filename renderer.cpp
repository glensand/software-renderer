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

        void render_triangle(const vector3& p0_in, const vector3& p1_in, 
            const vector3& p2_in, tga_image& in_image, const tga_image::color& color) {

            if(std::abs(p0_in.y - p1_in.y) < std::numeric_limits<double>::epsilon()
                || std::abs(p0_in.y - p2_in.y) < std::numeric_limits<double>::epsilon())
                return;
            
            const auto [width, height] = in_image.size();

            std::vector<vector3> tr{ p0_in, p1_in, p2_in };
            std::sort(begin(tr), end(tr), [](const vector3& p0, const vector3& p1){
                return p0.y < p1.y;
            });

            std::vector<point> p0_p2, p0_p1_p2;

            line::rasterize({ (int)width, (int)height }, tr[0], tr[2], 
                    [&](int x, int y) { p0_p2.emplace_back(point{ x, y }); });

            line::rasterize({ (int)width, (int)height }, tr[0], tr[1], 
                    [&](int x, int y) { p0_p1_p2.emplace_back(point{ x, y }); });

            line::rasterize({ (int)width, (int)height }, tr[1], tr[2], 
                    [&](int x, int y) { p0_p1_p2.emplace_back(point{ x, y }); });

            auto&& small = p0_p2;
            auto&& large = p0_p1_p2;
            if(small.size() > large.size())
                std::swap(small, large);

            for(auto&& large_p : large){
                for(auto&& small_p : small){
                    line::brezenhaim(large_p, small_p, 
                        [&](int x, int y) { in_image.set(x, y, color); });
                }
            }
        }

        void render_triangle2(point t0, point t1, point t2, 
            tga_image &image, tga_image::color color) {
            if (t0.y==t1.y && t0.y==t2.y) return; // i dont care about degenerate triangles
            if (t0.y>t1.y) std::swap(t0, t1);
            if (t0.y>t2.y) std::swap(t0, t2);
            if (t1.y>t2.y) std::swap(t1, t2);
            int total_height = t2.y-t0.y;
            for (int i=0; i<total_height; i++) {
                bool second_half = i>t1.y-t0.y || t1.y==t0.y;
                int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
                float alpha = (float)i/total_height;
                float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here
                auto A =               t0 + (t2-t0)*alpha;
                auto B = second_half ? t1 + (t2-t1)*beta : t0 + (t1-t0)*beta;
                if (A.x>B.x) std::swap(A, B);
                for (int j=A.x; j<=B.x; j++) {
                    image.set(j, t0.y+i, color); // attention, due to int casts t0.y+i != A.y
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

        const vector3 light_dir{ 0, 0, -1 };

        for(auto&& [p0, p1, p2] : triangles) {
            auto norm = cross(verticies[p2] - verticies[p0], 
                verticies[p1] - verticies[p0]);

            norm.normalize();

            const auto intencity = norm * light_dir;

            if(intencity > 0) {
                detail::render_triangle(verticies[p0], verticies[p1], verticies[p2], in_image, 
                    detail::by_intencity(intencity));
            }
        }
    }

    void rasterize2(const model& in_model, tga_image& in_image) {
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

                detail::render_triangle2(ss_p0, ss_p1, ss_p2, in_image, 
                    detail::by_intencity(intencity));
            }
        }
    }
}