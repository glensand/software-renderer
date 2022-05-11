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
    TValue x, y, z;

    vector3(TValue in_x = 0, TValue in_y = 0, TValue in_z = 0)
        : x(in_x)
        , y(in_y)
        , z(in_z) {

    }

    template<typename TRValue>
    vector3(const vector3<TRValue>& rhs) {
        x = TValue(rhs.x);
        y = TValue(rhs.y);
        z = TValue(rhs.z);
    }

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
        auto length = std::sqrt((double)dot(self, self));
        x /= length;
        y /= length;
        z /= length;
    }
};

using vector3i = vector3<int>;
using vector3f = vector3<double>;

template<typename TValue>
struct vector2 final { 
    TValue x, y;

    friend vector2 operator-(const vector2& a, const vector2& b) {
        return { a.x - b.x, a.y - b.y};
    }

    friend vector2 operator+(const vector2& a, const vector2& b) {
        return { a.x + b.x, a.y + b.y};
    }

    template<typename TMult>
    friend vector2 operator*(const vector2& a, TMult m) {
        return { TValue(a.x * m), TValue(a.y * m) };
    }

    friend double dot(const vector2& a, const vector2& b) {
        return a.x * b.x + a.y * b.y;
    }
};

using point = vector2<int>;
using vector2f = vector2<double>;