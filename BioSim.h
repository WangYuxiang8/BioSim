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

    // ��ѡ��ĳ�����������ʱ�����¶�Ӧ��������Ϣ�б�
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

    uint scroll_x = 0;                    // ˮƽ������λ��
    uint scroll_y = 0;                    // ��ֱ������λ��

    uint curr_select_bio_index = 0;     // ��ǰ��ѡ�е�������±�

    /* ��ʼ���Զ����widget��Ҳ���ǿɼ�ͼ������ */
    void init_my_widget(uint width, uint height);
    /* max ��ʾ���������ܳ��ȣ�pagestep ��ʾ��λ�������� */
    void init_scrollbar(
        unsigned int h_max,
        unsigned int h_pagestep,
        unsigned int v_max,
        unsigned int v_pagestep) const;
 
private slots:

    // ������Ϣ���������ѡ�б仯ʱ��Ӧ����������
    void on_comboBox_currentIndexChanged(int index);
    void on_btnSet_clicked();
    void on_btnStart_clicked();
    void on_btnPause_clicked();
    void on_btnStep_clicked();

    // �����������Ĺ���λ�÷����仯ʱ��Ӧ�����뵱ǰ��λ��
    void on_vertical_scrollbar_valueChanged(int num);
    void on_horizontal_scrollbar_valueChanged(int num);
    void wheelEvent(QWheelEvent* event);

    /* ���ڴ�С�仯ʱ��Ӧ */
    void resizeEvent(QResizeEvent* event);

    // �رմ����¼�
    void closeEvent(QCloseEvent* event);
};
