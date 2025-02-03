//
// Created by MCEL on 03.02.2025.
//

#pragma once

#include <QWidget>
#include <QImage>

class QImage;
class ImageWidget : public QWidget
{
  Q_OBJECT;
public:
  ImageWidget(QWidget* parent);

  void SetImage (const QImage &image);

  void paintEvent(QPaintEvent* event) override;

private:
  QImage m_image;

};
