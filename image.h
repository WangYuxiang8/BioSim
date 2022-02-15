#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <uchar.h>

#pragma pack( push, 1 )     //��ֹ�ֽ��Զ�����

/* tga �ļ�ͷ����Ϣ�ṹ�� */
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

/* tga �ļ�������Ϣ�ṹ�� */
struct tex_image
{
    TGA_header header;
    std::vector<unsigned char> image_data;
    unsigned int img_size;
};
#pragma pack( pop )

/*
* ����ͼ���࣬������ȡtgaͼ��ķ�������ȡͼ���������ݵķ�����
* �ṩͼ���ص��������صķ������Լ���ӡ���ͼ����Ϣ�ķ�����
*/
class image
{
public:

    image();
	~image();

    /* ��ȡͼ���ļ��ľ�̬���� */
	static image load_tga(const std::string& filename);
    /* ��ȡͼ��������Ϣ�����ڻ�ͼ */
    const unsigned char* get_img_data() const;

    /* ���� */
   	void create_tga_image(const std::string& filename) const;
	void print_image_data() const;

private:

	tex_image texture;

};