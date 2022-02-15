#include "BioSim.h"

BioSim::BioSim(Present& present, QWidget *parent)
    :
    m_present(present),
    QWidget(parent)
{
    ui.setupUi(this);
    ui.groupBox_2->setFixedWidth(500);

    if (view_col > COL) {
        view_col = COL;
        view_width = view_col * PIXEL;
        h_max = COL * PIXEL - view_width;
    }
    if (view_row > ROW) {
        view_row = ROW;
        view_height = view_row * PIXEL;
        v_max = ROW * PIXEL - view_height;
    }

    init_scrollbar(h_max, view_width, v_max, view_height);
    init_my_widget(view_width, view_height);
}

void BioSim::start()
{
    m_present.place_bioinfo_list_execute();
}

uint BioSim::get_scroll_x() const
{
    return scroll_x;
}

uint BioSim::get_scroll_y() const
{
    return scroll_y;
}

void BioSim::update(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    widget->update(x * PIXEL - scroll_x, y * PIXEL - scroll_y, width, height);
}

void BioSim::update()
{
    widget->update();
}

void BioSim::bioinfo_selection_changed(
    unsigned int strength,
    unsigned int speed,
    unsigned int life,
    const std::string& attr)
{
    ui.text_strength->setText(QString::fromStdString(std::to_string(strength)));
    ui.text_speed->setText(QString::fromStdString(std::to_string(speed)));
    ui.text_life->setText(QString::fromStdString(std::to_string(life)));
    ui.text_attribute->setText(QString::fromStdString(attr));
}

void BioSim::show_message_box(const std::string& message, const std::string& title)
{
    QMessageBox::information(this, QString::fromStdString(title), QString::fromStdString(message),
        QMessageBox::Ok, QMessageBox::NoButton);
}

void BioSim::add_item_to_bio_combox(const std::string& name)
{
    ui.comboBox->addItem(QString::fromStdString(name));
}

void BioSim::on_comboBox_currentIndexChanged(int index)
{
    m_present.bioinfo_selection_changed_execute(index);
    curr_select_bio_index = index;
}

void BioSim::on_btnSet_clicked()
{
    m_present.set_bio_image_execute(curr_select_bio_index);
    QPoint& p = m_present.from_tile_index_to_axis();
    widget->update(p.x() * PIXEL - scroll_x, p.y() * PIXEL - scroll_y, PIXEL, PIXEL);
}

void BioSim::on_btnStart_clicked()
{
    //QPoint& start = m_present.from_last_tile_index_to_axis();
    //QPoint& target = m_present.from_tile_index_to_axis();
    //if (m_present.find_shortest_path(start.x(), start.y(), target.x(), target.y()))
        //widget->update();
    //else 
    //    show_message_box_execute("No path!", "Error");
    m_present.simulate(TIME_INTERVAL, false);
}

void BioSim::on_btnPause_clicked()
{
    m_present.set_sim_stop(true);
}

void BioSim::on_btnStep_clicked()
{
    m_present.simulate(TIME_INTERVAL, true);
}

void BioSim::on_vertical_scrollbar_valueChanged(int num)
{
    scroll_y = num;
    widget->update();
}

void BioSim::on_horizontal_scrollbar_valueChanged(int num)
{
    scroll_x = num;
    widget->update();
}


void BioSim::wheelEvent(QWheelEvent* event)
{
    //��������ֵĹ�������para��para<0���¹�����>0���Ϲ���
    int para = event->angleDelta().y();
    int curr_value = ui.vertical_scrollbar->value() - para;
    if (para < 0)
    {
        //���¹������趨������ÿ����һ����λ��������ƶ�20����λ
        if (curr_value <= ui.vertical_scrollbar->maximum())
        {
            //����verticalScrollBar���ź�valueChange��value+20�������ӵ�������֮ǰд��slot_ScrollWidget(int)����
            emit ui.vertical_scrollbar->valueChanged(curr_value);
            //�趨����֮��Ļ���λ��
            ui.vertical_scrollbar->setSliderPosition(curr_value);
        }
        else
        {
            emit ui.vertical_scrollbar->valueChanged(ui.vertical_scrollbar->maximum());
            ui.vertical_scrollbar->setSliderPosition(ui.vertical_scrollbar->maximum());
        }
    }
    else
    {
        //���Ϲ���
        if (curr_value >= ui.vertical_scrollbar->minimum())
        {
            emit ui.vertical_scrollbar->valueChanged(curr_value);
            ui.vertical_scrollbar->setSliderPosition(curr_value);
        }
        else
        {
            emit ui.vertical_scrollbar->valueChanged(ui.vertical_scrollbar->minimum());
            ui.vertical_scrollbar->setSliderPosition(ui.vertical_scrollbar->minimum());
        }
    }
}


void BioSim::resizeEvent(QResizeEvent* event)
{
    unsigned int curr_width = widget->width();
    unsigned int curr_height = widget->height();

    // ������ǰ�ɼ���Χ��ߣ��Լ��������ķ�Χ
    // ���������������ʱ������Ϊ�����
    view_col = curr_width > (COL * PIXEL) ? COL :
        curr_width % PIXEL == 0 ? curr_width / PIXEL : curr_width / PIXEL + 1;
    view_row = curr_height > (ROW * PIXEL) ? ROW :
        curr_height % PIXEL == 0 ? curr_height / PIXEL : curr_height / PIXEL + 1;
    view_width = curr_width > (COL * PIXEL) ? (COL * PIXEL) : curr_width;
    view_height = curr_height > (ROW * PIXEL) ? (ROW * PIXEL) : curr_height;
    h_max = COL * PIXEL - view_width;
    v_max = ROW * PIXEL - view_height;

    init_scrollbar(h_max, view_width, v_max, view_height);
    widget->resize(view_width, view_height);
    widget->update();
}

void BioSim::closeEvent(QCloseEvent* event)
{
    m_present.set_sim_stop(true);
}

void BioSim::init_my_widget(uint width, uint height)
{
    widget = new myWidget(ui.groupBox, *this, m_present);
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setMinimumSize(QSize(width, height));
    ui.gridLayout_2->addWidget(widget, 0, 0, 1, 1);
}

/*
����������������أ����Ա�֤ƽ���Ĺ���
*/
void BioSim::init_scrollbar(
    unsigned int h_max,
    unsigned int h_pagestep,
    unsigned int v_max,
    unsigned int v_pagestep) const
{
    ui.vertical_scrollbar->setMaximum(v_max);
    ui.vertical_scrollbar->setPageStep(v_pagestep);
    ui.horizontal_scrollbar->setMaximum(h_max);
    ui.horizontal_scrollbar->setPageStep(h_pagestep);
}