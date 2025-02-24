//
// Created by MCEL on 03.02.2025.
//

#include <csEditor/editors/image/ImageWidget.hh>
#include <QPainter>

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent)
    , m_image(256, 256, QImage::Format_ARGB32)
{

}


void ImageWidget::SetImage(const QImage &image)
{
  m_image = image;
  resize(image.width(), image.height());
  repaint();
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(rect(), QBrush(QColor(0, 0, 0, 0xff)));

  // Draw the generated image with alpha blending
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  painter.drawImage(0, 0, m_image);
}
