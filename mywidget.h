#pragma once
#include <iostream>
#include <qwidget.h>
#include <qpainter.h>
#include <QMouseEvent>

#include "view.h"
#include "present.h"
#include "image.h"
#include "config.h"

class myWidget : public QWidget
{
public:

    myWidget(QWidget* parent, View& v, Present& p);
    ~myWidget();

private:
    View& m_view;
    Present& m_present;

    /* 鼠标点击tile时响应 */
    void mousePressEvent(QMouseEvent* event);
    
    /* 画图painter响应事件，调用
    时会触发该事件 */
    void paintEvent(QPaintEvent* event);
};

