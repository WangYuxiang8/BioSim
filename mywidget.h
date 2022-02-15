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

    /* �����tileʱ��Ӧ */
    void mousePressEvent(QMouseEvent* event);
    
    /* ��ͼpainter��Ӧ�¼�������
    ʱ�ᴥ�����¼� */
    void paintEvent(QPaintEvent* event);
};

