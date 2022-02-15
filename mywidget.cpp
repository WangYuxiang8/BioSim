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
    // 把上次点击的tile的光标去掉
    QPoint& p = m_present.from_tile_index_to_axis();
    update(p.x() * PIXEL - m_view.get_scroll_x(), p.y() * PIXEL - m_view.get_scroll_y(), PIXEL, PIXEL);

    // 转换到 tile 的坐标
    uint tx = (event->x() + m_view.get_scroll_x()) / PIXEL;
    uint ty = (event->y() + m_view.get_scroll_y()) / PIXEL;
    /* 鼠标点击可见环境时，传入tile坐标，设置对应的tile为选中状态 */
    m_present.click_tile_event_execute(tx, ty);

    update(tx * PIXEL - m_view.get_scroll_x(), ty * PIXEL - m_view.get_scroll_y(), PIXEL, PIXEL);
    // std::cout << tx << ", " << ty << std::endl;
}

void myWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    uint scroll_x = m_view.get_scroll_x();
    uint scroll_y = m_view.get_scroll_y();

    // 计算当前应当可见的 tile 的 first 位置
    uint fx = scroll_x / PIXEL;
    uint fy = scroll_y / PIXEL;

    // 计算当前可见的行数和列数
    uint curr_col = scroll_x % PIXEL == 0 ? m_view.view_col :
        (m_view.view_col + 1);
    uint curr_row = scroll_y % PIXEL == 0 ? m_view.view_row :
        (m_view.view_row + 1);
    uint col = (fx + curr_col) > COL ? COL : (fx + curr_col);
    uint row = (fy + curr_row) > ROW ? ROW : (fy + curr_row);

    for (uint i = fy; i < row; i++) {
        for (uint j = fx; j < col; j++)
        {
            // 计算当前tile在可见图景中的坐标
            uint x = j * PIXEL - scroll_x;
            uint y = i * PIXEL - scroll_y;
            QPoint point(x, y);

            // 获取当前tile的图片列表，并集成绘图
            std::vector<image> image_list;
            image_list = m_present.get_tile_image_list(image_list, j, i);
            for (const auto& it : image_list) {
                QImage qimage(
                    (uchar*)it.get_img_data(),
                    PIXEL,
                    PIXEL,
                    QImage::Format_ARGB32);
                painter.setCompositionMode(QPainter::CompositionMode_SourceOver);   // 像素混合
                painter.drawImage(point, qimage.mirrored());
            }

            // 在tile周围画一个矩形
            QRect rect(x, y, PIXEL, PIXEL);
            painter.drawRect(rect);
        }
    }
}