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
	    unsigned char b{ 255 }, g{ 255 }, r{ 255 }, a{ 255 };
    };

    tga_image(unsigned width = 0, unsigned height = 0, unsigned bpp = 4);

    void flip_x();
    
    const color& get(unsigned x, unsigned y) const { return m_data[y * m_width + x]; }
    void set(unsigned x, unsigned y, const color& c) { m_data[y * m_width + x] = c; }
    point size() const { return { (int)m_width, (int)m_height }; }

    void load(const char* file);
    void store(const char* file) const;
private:

    unsigned m_width;
    unsigned m_height;
    unsigned m_bpp;

    std::vector<color> m_data;
};