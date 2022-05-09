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

template<typename TValue>
struct vector3 final {
    TValue x{ 0 }, y{ 0 }, z{ 0 };

    friend vector3 cross(const vector3& a, const vector3& b) {
        return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
    }

    friend vector3 operator-(const vector3& a, const vector3& b) {
        return { a.x - b.x, a.y - b.y, a.z - b.z };
    }

    friend vector3 operator+(const vector3& a, const vector3& b) {
        return { a.x + b.x, a.y + b.y, a.z + b.z };
    }

    friend double dot(const vector3& a, const vector3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    template<typename TMult>
    friend vector3 operator*(const vector3& v, TMult a){
        return { TValue(v.x * a), TValue(v.y * a), TValue(v.z * a) };
    }

    void normalize() {
        auto&& self = *this;
        auto length = std::sqrt(dot(self, self));
        x /= length;
        y /= length;
        z /= length;
    }
};

using vector3i = vector3<int>;
using vector3f = vector3<double>;

struct point final { 
    int x, y;
};