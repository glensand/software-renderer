#include "model.h"

#include <fstream>
#include <sstream>

model::model(const char* model_file, const char* diffuse_file) { 
    m_diffuse_texture.load(diffuse_file);
    m_diffuse_texture.flip_x();
    
    std::ifstream in(model_file, std::ios::binary);

    if(!in.is_open()) return;
    
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            vector3f v;
            iss >> v.x >> v.y >> v.z; 
            m_verticies.push_back(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            vector3f n;
            iss >> n.x >> n.y >> n.z; 
            m_normals.push_back(n);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            vector2f uv;
            iss >> uv.x >> uv.y; 
            m_uvs.push_back(uv);
        } else if (!line.compare(0, 2, "f ")) {
            auto&& triangle = m_triangles.emplace_back();
            auto&& indicies = triangle.indicies;
            unsigned vert_index = 0;
            iss >> trash;
            while (iss >> indicies[vert_index].normal >> trash 
                    >> indicies[vert_index].uv >> trash >> indicies[vert_index].vert)  {
                indicies[vert_index].uv--; 
                indicies[vert_index].normal--;
                indicies[vert_index].vert--;

                ++vert_index;
            }
        }
    }
}

tga_image::color model::diffuse(const point& uv) const{
    return m_diffuse_texture.get(uv.x, uv.y);
}

void model::load_texture(std::string_view texture_name){
    m_diffuse_texture.load(texture_name.data());
}

vector2f model::uv(unsigned triangle_index, unsigned vert_index){
    auto&& triangle = m_triangles[triangle_index];
    auto&& uv_index = triangle.indicies[vert_index].uv;

    auto&& size = m_diffuse_texture.size();
    return { m_uvs[uv_index].x * size.x, m_uvs[uv_index].y * size.y };
}