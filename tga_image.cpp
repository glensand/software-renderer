#include "tga_image.h"

#include <fstream>
#include <iostream>

namespace {

#pragma pack(push,1)
    struct header final {
        char idlength;
        char colormaptype;
        char datatypecode;
        short colormaporigin;
        short colormaplength;
        char colormapdepth;
        short x_origin;
        short y_origin;
        short width;
        short height;
        char bitsperpixel;
        char imagedescriptor;
    };
#pragma pack(pop)

    const unsigned char developer_area_ref[4] = {0, 0, 0, 0};
    const unsigned char extension_area_ref[4] = {0, 0, 0, 0};
    const unsigned char footer[18] = {'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0'};

}

tga_image::tga_image(unsigned w, unsigned h, unsigned bpp)
    : m_height(h)
    , m_width(w)
    , m_bpp(bpp) {

    m_data.resize(w * h);
}

void tga_image::load(const char* file){
    // todo: implement
}

void tga_image::store(const char* filename) const {    
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) {
    	std::cout << "can't open file " << filename << "\n";
    	out.close();
    	return;
    }
    header header_obj;
    memset((void *)&header_obj, 0, sizeof(header));
    header_obj.bitsperpixel = m_bpp << 3;
    header_obj.width  = m_width;
    header_obj.height = m_height;
    header_obj.datatypecode = 2; // 4 bpp rgba
    header_obj.imagedescriptor = 0x20; // top-left origin
    out.write((char *)&header_obj, sizeof(header));

    out.write((char *)m_data.data(), m_width * m_height * m_bpp);

    out.write((char *)developer_area_ref, sizeof(developer_area_ref));
    out.write((char *)extension_area_ref, sizeof(extension_area_ref));
    out.write((char *)footer, sizeof(footer));

    out.close();
}

void tga_image::flip_x() {
    for(unsigned i{ 0 }; i < m_height / 2; ++i){
        for(unsigned j{ 0 }; j < m_width; ++j){
            std::swap(m_data[i * m_width + j], m_data[m_width * (m_height - i) + j]);
        }
    }
}