/* Copyright (C) 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/software-renderer
 */

#pragma once

#include "render/renderer.h"

class wireframe_renderer final : public renderer {
public:
    wireframe_renderer(tga_image& back_buffer);
    virtual ~wireframe_renderer() = default;

    virtual void draw(const model& in_model) override;
private:
    tga_image& m_back_buffer;
};
