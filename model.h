/* Copyright (C) 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/software-renderer
 */

#pragma once

#include <vector>
#include <tuple>
#include "core/rmath.h"

class model final {
public:
    using verticies_t = std::vector<vector3f>;
    using triangles_t = std::vector<std::tuple<unsigned, unsigned, unsigned>>;

    model(const char* file);
    
    const verticies_t& get_vert() const { return m_verticies; }
    const triangles_t& get_trian() const { return m_triangles; }

private:
    verticies_t m_verticies;
    triangles_t m_triangles;
};