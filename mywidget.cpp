#include "mywidget.h"

myWidget::myWidget(QWidget* parent, View& v, Present& p)
    :
    QWidget(parent),
    m_view(v),
    m_present(p)
{
}

myWidget::~myWidget()
{

}

void myWidget::mousePressEvent(QMouseEvent* event)
{
    // ���ϴε����tile�Ĺ��ȥ��
    QPoint& p = m_present.from_tile_index_to_axis();
    update(p.x() * PIXEL - m_view.get_scroll_x(), p.y() * PIXEL - m_view.get_scroll_y(), PIXEL, PIXEL);

    // ת���� tile ������
    uint tx = (event->x() + m_view.get_scroll_x()) / PIXEL;
    uint ty = (event->y() + m_view.get_scroll_y()) / PIXEL;
    /* ������ɼ�����ʱ������tile���꣬���ö�Ӧ��tileΪѡ��״̬ */
    m_present.click_tile_event_execute(tx, ty);

    update(tx * PIXEL - m_view.get_scroll_x(), ty * PIXEL - m_view.get_scroll_y(), PIXEL, PIXEL);
    // std::cout << tx << ", " << ty << std::endl;
}

void myWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    uint scroll_x = m_view.get_scroll_x();
    uint scroll_y = m_view.get_scroll_y();

    // ���㵱ǰӦ���ɼ��� tile �� first λ��
    uint fx = scroll_x / PIXEL;
    uint fy = scroll_y / PIXEL;

    // ���㵱ǰ�ɼ�������������
    uint curr_col = scroll_x % PIXEL == 0 ? m_view.view_col :
        (m_view.view_col + 1);
    uint curr_row = scroll_y % PIXEL == 0 ? m_view.view_row :
        (m_view.view_row + 1);
    uint col = (fx + curr_col) > COL ? COL : (fx + curr_col);
    uint row = (fy + curr_row) > ROW ? ROW : (fy + curr_row);

    for (uint i = fy; i < row; i++) {
        for (uint j = fx; j < col; j++)
        {
            // ���㵱ǰtile�ڿɼ�ͼ���е�����
            uint x = j * PIXEL - scroll_x;
            uint y = i * PIXEL - scroll_y;
            QPoint point(x, y);

            // ��ȡ��ǰtile��ͼƬ�б������ɻ�ͼ
            std::vector<image> image_list;
            image_list = m_present.get_tile_image_list(image_list, j, i);
            for (const auto& it : image_list) {
                QImage qimage(
                    (uchar*)it.get_img_data(),
                    PIXEL,
                    PIXEL,
                    QImage::Format_ARGB32);
                painter.setCompositionMode(QPainter::CompositionMode_SourceOver);   // ���ػ��
                painter.drawImage(point, qimage.mirrored());
            }

            // ��tile��Χ��һ������
            QRect rect(x, y, PIXEL, PIXEL);
            painter.drawRect(rect);
        }
    }
}