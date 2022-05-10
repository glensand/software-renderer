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

    m_data.resize(w * h * bpp);
}

bool tga_image::load(const char* file){
    std::ifstream in;
	in.open (file, std::ios::binary);
	if (!in.is_open()) {
		std::cerr << "can't open file " << file << "\n";
		in.close();
		return false;
	}

	header header;
	in.read((char *)&header, sizeof(header));
	if (!in.good()) {
		in.close();
		std::cerr << "an error occured while reading the header\n";
		return false;
	}

	m_width   = header.width;
	m_height  = header.height;
	m_bpp = header.bitsperpixel >> 3;
	unsigned long nbytes = m_bpp * m_width * m_height;
    m_data.resize(nbytes);
	if (3 == header.datatypecode || 2==header.datatypecode) {
		in.read((char *)m_data.data(), nbytes);
		if (!in.good()) {
			in.close();
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
	} else if (10 == header.datatypecode || 11 == header.datatypecode) {
		if (!load_rle(in)) {
			in.close();
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
	} else {
		in.close();
		std::cerr << "unknown file format " << (int)header.datatypecode << "\n";
		return false;
	}
	if (!(header.imagedescriptor & 0x20)) {
		flip_x();
	}
	if (header.imagedescriptor & 0x10) {
		// flip_horizontally(); todo
	}
	std::cerr << m_width << "x" << m_height << "/" << m_bpp * 8 << "\n";
	in.close();
	return true;
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
            std::swap(get(i, j), get(m_width - i - 1, m_height - j - 1));
        }
    }
}

bool tga_image::load_rle(std::istream& in) {
    unsigned long pixelcount = m_width * m_height;
	unsigned long currentpixel = 0;
	unsigned long currentbyte  = 0;
	tga_image::color colorbuffer;
	do {
		unsigned char chunkheader = 0;
		chunkheader = in.get();
		if (!in.good()) {
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
		if (chunkheader<128) {
			chunkheader++;
			for (int i=0; i<chunkheader; i++) {
				in.read((char *)&colorbuffer, m_bpp);
				if (!in.good()) {
					std::cerr << "an error occured while reading the header\n";
					return false;
				}
				for (int t = 0; t < m_bpp; t++)
					m_data[currentbyte++] = colorbuffer.bgra[t];
				currentpixel++;
				if (currentpixel>pixelcount) {
					std::cerr << "Too many pixels read\n";
					return false;
				}
			}
		} else {
			chunkheader -= 127;
			in.read((char *)colorbuffer.bgra, m_bpp);
			if (!in.good()) {
				std::cerr << "an error occured while reading the header\n";
				return false;
			}
			for (int i=0; i<chunkheader; i++) {
				for (int t=0; t < m_bpp; t++)
					m_data[currentbyte++] = colorbuffer.bgra[t];
				currentpixel++;
				if (currentpixel > pixelcount) {
					std::cerr << "Too many pixels read\n";
					return false;
				}
			}
		}
	} while (currentpixel < pixelcount);
	return true;
}