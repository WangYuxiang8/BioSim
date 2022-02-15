#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <uchar.h>

#pragma pack( push, 1 )     //禁止字节自动对齐

/* tga 文件头部信息结构体 */
struct TGA_header
{
    unsigned char           desc_len;
    unsigned char           cmap_type;
    unsigned char           image_type;
    short                   cmap_start;
    unsigned short          cmap_entries;
    unsigned char           cmap_bits;
    unsigned short          x_offset;
    unsigned short          y_offset;
    unsigned short          width;
    unsigned short          height;
    unsigned char           bpp;
    unsigned char           attrib;
};

/* tga 文件所有信息结构体 */
struct tex_image
{
    TGA_header header;
    std::vector<unsigned char> image_data;
    unsigned int img_size;
};
#pragma pack( pop )

/*
* 定义图像类，包含读取tga图像的方法，获取图像像素数据的方法，
* 提供图像重叠计算像素的方法，以及打印相关图像信息的方法。
*/
class image
{
public:

    image();
	~image();

    /* 读取图像文件的静态函数 */
	static image load_tga(const std::string& filename);
    /* 获取图像像素信息，用于绘图 */
    const unsigned char* get_img_data() const;

    /* 测试 */
   	void create_tga_image(const std::string& filename) const;
	void print_image_data() const;

private:

	tex_image texture;

};