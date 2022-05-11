/* Copyright (C) 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/software-renderer
 */

#pragma once

#include <vector>
#include "core/rmath.h"

class tga_image final {
public:

    struct color final {
	    uint8_t bgra[4];

        color(uint8_t* in_bgra) {
            for(auto i{ 0 }; i < 4; ++i)
                bgra[i] = in_bgra[i];
        }

        constexpr color() {
            bgra[0] = bgra[1] = bgra[2] = 0;
            bgra[3] = 255;
        }

        constexpr color(uint8_t b, uint8_t g, uint8_t r, uint8_t a){
            bgra[0] = b;
            bgra[1] = g;
            bgra[2] = r;
            bgra[3] = a;
        }

        template<typename TMult>
        friend color operator*(const color& c, TMult m) {
            auto res = c;
            for(auto i{ 0 }; i < 3; ++i)
                res.bgra[i] *= m;
            return res;
        }
    };

    tga_image(unsigned width = 0, unsigned height = 0, unsigned bpp = 4);

    void flip_x();
    
    color& get(unsigned x, unsigned y) const { return *(color*)(m_data.data() + (y * m_width + x) * m_bpp) ; }
    void set(unsigned x, unsigned y, const color& c) { get(x, y) = c; }
    point size() const { return { (int)m_width, (int)m_height }; }

    bool load(const char* file);
    void store(const char* file) const;
private:

    bool load_rle(std::istream& in);

    unsigned m_width;
    unsigned m_height;
    unsigned m_bpp;

    std::vector<uint8_t> m_data;
};