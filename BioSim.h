#pragma once

#include <QtWidgets/QWidget>
#include <qmessagebox.h>
#include <QCloseEvent>

#include "ui_BioSim.h"
#include "view.h"
#include "present.h"
#include "mywidget.h"
#include "config.h"

class BioSim : public QWidget, public View
{
    Q_OBJECT

public:
    BioSim(Present& present, QWidget *parent = Q_NULLPTR);

    // 当选择某个生物的名称时，更新对应的生物信息列表
    virtual void bioinfo_selection_changed(
        unsigned int strength,
        unsigned int speed,
        unsigned int life,
        const std::string& attr);
    virtual void show_message_box(const std::string& message, const std::string& title);
    virtual void add_item_to_bio_combox(const std::string& name);
    virtual uint get_scroll_x() const;
    virtual uint get_scroll_y() const;
    virtual void update(unsigned int x, unsigned int y, unsigned int width = PIXEL, unsigned int height = PIXEL);
    virtual void update();

    void start();

private:

    Ui::BioSimClass ui;
    Present& m_present;
    myWidget* widget;

    uint scroll_x = 0;                    // 水平滚动条位置
    uint scroll_y = 0;                    // 垂直滚动条位置

    uint curr_select_bio_index = 0;     // 当前被选中的生物的下标

    /* 初始化自定义的widget，也就是可见图景区域 */
    void init_my_widget(uint width, uint height);
    /* max 表示滚动条的总长度，pagestep 表示单位滑动长度 */
    void init_scrollbar(
        unsigned int h_max,
        unsigned int h_pagestep,
        unsigned int v_max,
        unsigned int v_pagestep) const;
 
private slots:

    // 生物信息输入框里面选中变化时响应，传入索引
    void on_comboBox_currentIndexChanged(int index);
    void on_btnSet_clicked();
    void on_btnStart_clicked();
    void on_btnPause_clicked();
    void on_btnStep_clicked();

    // 两个滚动条的滚动位置发生变化时响应，传入当前的位置
    void on_vertical_scrollbar_valueChanged(int num);
    void on_horizontal_scrollbar_valueChanged(int num);
    void wheelEvent(QWheelEvent* event);

    /* 窗口大小变化时响应 */
    void resizeEvent(QResizeEvent* event);

    // 关闭窗口事件
    void closeEvent(QCloseEvent* event);
};
