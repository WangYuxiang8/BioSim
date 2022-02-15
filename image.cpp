#include "image.h"

image::image()
{
}

image::~image()
{
}

image image::load_tga(const std::string& filename)
{
    image img;
    if (filename.empty()) {
        const std::string msg = "Invalid image file path: " + filename;
        throw std::exception(msg.c_str());
    }

    // 判断是否是TGA图片
    if (filename.substr(filename.size() - 4, 4) != ".tga") {
        const std::string msg = "Not TGA image file: " + filename;
        throw std::exception(msg.c_str());
    }

    std::ifstream infile;
    infile.open(filename, std::ios::in | std::ios::binary);
    if (!infile.is_open()) {
        const std::string msg = "Fail to open image file: " + filename;
        throw std::exception(msg.c_str());
    }

    // 读取头部
    infile.read((char *) &img.texture.header, sizeof(TGA_header));
    if (infile.fail()) {
        infile.close();
        throw std::exception("Read image header data failed!");
    }

    // 判断头部是否合法
    if (img.texture.header.bpp != 32 || img.texture.header.image_type != 2) {
        infile.close();
        throw std::exception("Image type error!");
    }

    // 读取 BGRA 数据
    unsigned int byte_per_pixel = img.texture.header.bpp / 8;
    unsigned int img_size = img.texture.header.width * img.texture.header.height * byte_per_pixel;
    img.texture.img_size = img_size;
    img.texture.image_data.resize(img_size);
    infile.read((char *) &img.texture.image_data[0], img_size);
    if (infile.fail() && !infile.eof()) {
        infile.close();
        throw std::exception("Read image color data failed!");
    }

    infile.close();
    return img;
}

const unsigned char* image::get_img_data() const
{
    return &texture.image_data[0];
}

void image::create_tga_image(const std::string& filename) const
{
    std::ofstream outfile;
    if (filename.empty())
        throw std::exception("Invalid image filename!");

    outfile.open(filename, std::ios::out | std::ios::binary);
    if (!outfile.is_open())
        throw std::exception("Open image file failed!");

    outfile.write((char *) &texture.header, sizeof(TGA_header));
    if (outfile.fail()) {
        outfile.close();
        throw std::exception("Write image header data failed!");
    }

    outfile.write((char*)&texture.image_data[0], texture.img_size);
    if (outfile.fail()) {
        outfile.close();
        throw std::exception("Read image color data failed!");
    }

    outfile.close();
}

void image::print_image_data() const
{
    for (auto it = texture.image_data.begin(); it != texture.image_data.end(); it += 4)
    {
        printf("%d %d %d %d\n", *it, *(it + 1), *(it + 2), *(it + 3));
    }
}