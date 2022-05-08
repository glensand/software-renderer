#include "model.h"

#include <fstream>
#include <sstream>

model::model(const char* file) { 
    std::ifstream in(file, std::ios::binary);

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
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }
            if(f.size() == 3) {
                m_triangles.emplace_back(f[0], f[1], f[2]);
            }
        }
    }
}