/* Copyright (C) 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/software-renderer
 */

#pragma once

class tga_image;
class model;

class renderer {
public:
    virtual ~renderer() = default;
    virtual void draw(const model& in_model, tga_image& in_image) = 0;
};
