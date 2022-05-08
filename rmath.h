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

    friend point operator+(const point& a, const point& b){
        return { a.x + b.x, a.y + b.y };
    }

    friend point operator-(const point& a, const point& b){
        return { a.x - b.x, a.y - b.y };
    }

    template<typename T>
    friend point operator*(const point& a, T b){
        return { a.x * (int)b, a.y * (int)b };
    }
};

 struct vector3 final {
    double x, y, z;

    friend vector3 cross(const vector3& a, const vector3& b) {
        return { a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x };
    }

    friend vector3 operator-(const vector3& a, const vector3& b) {
        return { a.x - b.x, a.y - b.y, a.z - b.z };
    }

    friend double operator*(const vector3& a, const vector3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    void normalize() {
        auto&& self = *this;
        auto length = std::sqrt(self * self);
        x /= length;
        y /= length;
        z /= length;
    }
};