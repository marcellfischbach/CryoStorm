//
// Created by MCEL on 03.02.2025.
//

#pragma once

#include <csCore/csRef.hh>

#include <editors/EditorWidget.hh>

namespace cs
{
class csImage;
}

class ImageWidget;

class QComboBox;
class QLabel;
class QPushButton;
class QPixmap;
class QSpinBox;

class ImageEditorWidget : public EditorWidget
{
  Q_OBJECT;
public:
  ImageEditorWidget(cs::csImage* image, QWidget* parent = nullptr);
  ~ImageEditorWidget() override;

private slots:
  void onLayerChanged(int);
  void onLayerTransformationChanged(int);
  void onColorButtonChanged(bool);

private:
  void UpdateImage();

  QImage CreateImage();
  QImage CreateRGBImage ();
  QImage CreateARGBImage ();

  uint32_t FixValue (uint32_t argb, bool r, bool g, bool b, bool a);

  cs::csRef<cs::csImage> m_image;

  QPixmap *m_pixmap;
  ImageWidget *m_imageWidget;

  QPushButton* m_btnR;
  QPushButton* m_btnG;
  QPushButton* m_btnB;
  QPushButton* m_btnA;
  QSpinBox* m_spinBoxLayer;
  QComboBox* m_comboLayerTransformation;
};
