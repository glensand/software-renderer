/* Copyright (C) 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/software-renderer
 */

#pragma once

#include <vector>
#include <cmath>

struct point final {
    int x;
    int y;
};

namespace line {

    template<typename TPointSetter>
    void brezenhaim(point p1, point p2, TPointSetter&& setter) {
        bool steep = false;

        if (std::abs(p1.x - p2.x) < std::abs(p1.y - p2.y)) { // if the line is steep, we transpose the image
            std::swap(p1.x, p1.y);
            std::swap(p2.x, p2.y);
            steep = true;
        }
        if (p1.x > p2.x) { // make it left-to-right
            std::swap(p1, p2);
        }

        for (auto x{ p1.x }; x <= (p2.x); ++x) {
            float t = ( x - p1.x) / (float)(p2.x - p1.x);
            int y = p1.y * (1. - t) + p2.y * t;
            if (steep) {
                setter(y, x); // if transposed, de-transpose
            } else {
                setter(x, y);
            }
        }
    }

}