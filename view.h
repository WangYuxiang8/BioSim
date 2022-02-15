#pragma once
#ifndef VIEW_H
#define VIEW_H
#include "config.h"
class Present;

/*
���� view ��ͼ����
*/
class View 
{
public:

	virtual void bioinfo_selection_changed(
        unsigned int strength,
        unsigned int speed,
        unsigned int life,
        const std::string& attr) = 0;
	virtual void show_message_box(const std::string& message, const std::string& title) = 0;
	virtual void add_item_to_bio_combox(const std::string& name) = 0;
    virtual uint get_scroll_x() const = 0;
    virtual uint get_scroll_y() const = 0;
    virtual void update(unsigned int x, unsigned int y, unsigned int width = PIXEL, unsigned int height = PIXEL) = 0;
    virtual void update() = 0;

     //////////////////// ������������ͼ�йصı��� //////////////////
     unsigned int view_row               = 20;                   // �ɼ�ͼ������
     unsigned int view_col               = 20;                   // �ɼ�ͼ������
     unsigned int view_height            = view_col * PIXEL;     // �ɼ���ͼ 32 * 13 ���ظߣ�ÿ����Ԫ 32 ����
     unsigned int view_width             = view_row * PIXEL;     // �ɼ���ͼ 32 * 13 ���ؿ�
     unsigned int h_max                  = COL * PIXEL - view_width;     // ˮƽ�����������Χ
     unsigned int v_max                  = ROW * PIXEL - view_height;    // ��ֱ�����������Χ

};

#endif // !VIEW_H