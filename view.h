#pragma once
#ifndef VIEW_H
#define VIEW_H
#include "config.h"
class Present;

/*
定义 view 视图基类
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

     //////////////////// 定义所有与视图有关的变量 //////////////////
     unsigned int view_row               = 20;                   // 可见图像行数
     unsigned int view_col               = 20;                   // 可见图像列数
     unsigned int view_height            = view_col * PIXEL;     // 可见视图 32 * 13 像素高，每个单元 32 像素
     unsigned int view_width             = view_row * PIXEL;     // 可见视图 32 * 13 像素宽
     unsigned int h_max                  = COL * PIXEL - view_width;     // 水平滚动条的最大范围
     unsigned int v_max                  = ROW * PIXEL - view_height;    // 垂直滚动条的最大范围

};

#endif // !VIEW_H