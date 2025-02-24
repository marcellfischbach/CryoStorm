//
// Created by MCEL on 03.02.2025.
//

#include <csEditor/editors/image/ImageEditorWidget.hh>
#include <csEditor/editors/image/ImageWidget.hh>
#include <csCore/graphics/csImage.hh>

#include <QComboBox>
#include <QGridLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSpacerItem>
#include <QSpinBox>


static QString ImageFormatString(cs::ePixelFormat format)
{
  switch (format)
  {
    case cs::ePF_R:
      return "R 8bit";
    case cs::ePF_RG:
      return "RG 16bit";
    case cs::ePF_RGB:
      return "RGB 24bit";
    case cs::ePF_RGBA:
      return "RGBA 32bit";
    case cs::ePF_RGB10A2:
      return "RGBA 10x10x10x2bit";
    case cs::ePF_Depth:
      return "Depth";
    case cs::ePF_DepthStencil:
      return "Depth & Stencil";
  }
  return "Unknown";
}

ImageEditorWidget::ImageEditorWidget(cs::csImage *image, QWidget *parent)
    : EditorWidget(parent)
    , m_image(image)
    , m_pixmap(nullptr)
{
  image->GenerateMipMaps(cs::csImage::eMipMapProcedure::eMMP_Linear4x4);
  setWindowTitle("Image editor");
  resize(1024, 768);

  QGridLayout *layout = new QGridLayout(this);

  QHBoxLayout *toolbarLayout = new QHBoxLayout();
  m_btnR = new QPushButton("R", this);
  m_btnG = new QPushButton("G", this);
  m_btnB = new QPushButton("B", this);
  m_btnA = new QPushButton("A", this);
  QLabel *channels = new QLabel("Channels: ", this);
  QLabel *layers   = new QLabel("    Layers ", this);
  QLabel *ofLayers = new QLabel(QString(" / %1    ").arg(m_image->GetNumberOfLayers()), this);
  m_spinBoxLayer = new QSpinBox(this);
  m_spinBoxLayer->setValue(1);
  m_spinBoxLayer->setMinimum(1);
  m_spinBoxLayer->setMaximum(m_image->GetNumberOfLayers());

  m_comboLayerTransformation = new QComboBox(this);
  m_comboLayerTransformation->addItem("Original");
  m_comboLayerTransformation->addItem("Point");
  m_comboLayerTransformation->addItem("Linear");


  QString info = QString("    %1 x %2 %3")
      .arg(m_image->GetWidth())
      .arg(m_image->GetHeight())
      .arg(ImageFormatString(m_image->GetPixelFormat()));

  QLabel *imageInformation = new QLabel(info, this);


  toolbarLayout->addWidget(channels);
  toolbarLayout->addWidget(m_btnR);
  toolbarLayout->addWidget(m_btnG);
  toolbarLayout->addWidget(m_btnB);
  toolbarLayout->addWidget(m_btnA);
  toolbarLayout->addWidget(layers);
  toolbarLayout->addWidget(m_spinBoxLayer);
  toolbarLayout->addWidget(ofLayers);
  toolbarLayout->addWidget(m_comboLayerTransformation);
  toolbarLayout->addWidget(imageInformation);

  toolbarLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred));

  layout->addItem(toolbarLayout, 0, 0);


  QScrollArea *area = new QScrollArea(this);
  layout->addWidget(area, 1, 0);

  m_imageWidget = new ImageWidget(this);
  area->setWidget(m_imageWidget);


  m_btnR->setStyleSheet("QPushButton { background-color: #a06060; } QPushButton:checked { background-color: #a00000; }");
  m_btnG->setStyleSheet("QPushButton { background-color: #60a060; } QPushButton:checked { background-color: #00a000; }");
  m_btnB->setStyleSheet("QPushButton { background-color: #6060a0; } QPushButton:checked { background-color: #0000a0; }");
  m_btnA->setStyleSheet("QPushButton { background-color: #303030; } QPushButton:checked { background-color: #a0a0a0; }");

  m_btnR->setCheckable(true);
  m_btnG->setCheckable(true);
  m_btnB->setCheckable(true);
  m_btnA->setCheckable(true);

  m_btnR->setChecked(true);
  m_btnG->setChecked(true);
  m_btnB->setChecked(true);
  m_btnA->setChecked(true);


  connect(m_btnR, &QPushButton::toggled, this, &ImageEditorWidget::onColorButtonChanged);
  connect(m_btnG, &QPushButton::toggled, this, &ImageEditorWidget::onColorButtonChanged);
  connect(m_btnB, &QPushButton::toggled, this, &ImageEditorWidget::onColorButtonChanged);
  connect(m_btnA, &QPushButton::toggled, this, &ImageEditorWidget::onColorButtonChanged);
  connect(m_spinBoxLayer, &QSpinBox::valueChanged, this, &ImageEditorWidget::onLayerChanged);
  connect(m_comboLayerTransformation,
          &QComboBox::currentIndexChanged,
          this,
          &ImageEditorWidget::onLayerTransformationChanged);

  UpdateImage();
}

ImageEditorWidget::~ImageEditorWidget()
{
}

void ImageEditorWidget::onLayerChanged(int)
{
  UpdateImage();
}


void ImageEditorWidget::onLayerTransformationChanged(int)
{
  UpdateImage();
}

void ImageEditorWidget::onColorButtonChanged(bool)
{

  UpdateImage();
}

void ImageEditorWidget::UpdateImage()
{
  QImage image = CreateImage();

  int index = m_comboLayerTransformation->currentIndex();
  if (index == 1)
  {
    // Point interpolation
    image = image.scaled(m_image->GetWidth(), m_image->GetHeight(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
  }
  else if (index == 2)
  {
    // Point interpolation
    image = image.scaled(m_image->GetWidth(), m_image->GetHeight(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  }


  m_imageWidget->SetImage(image);
}

QImage ImageEditorWidget::CreateImage()
{
  if (m_image->GetPixelFormat() == cs::ePF_RGB)
  {
    return CreateRGBImage();
  }
  if (m_image->GetPixelFormat() == cs::ePF_RGBA)
  {
    return CreateARGBImage();
  }
  return QImage(256, 256, QImage::Format_RGB32);
}

QImage ImageEditorWidget::CreateRGBImage()
{
  size_t        layer   = m_spinBoxLayer->value() - 1;
  uint16_t      width   = m_image->GetWidth(layer);
  uint16_t      height  = m_image->GetHeight(layer);
  const uint8_t *buffer = m_image->GetData(layer);

  bool r = m_btnR->isChecked();
  bool g = m_btnG->isChecked();
  bool b = m_btnB->isChecked();
  bool a = m_btnA->isChecked();

  QImage   image = QImage(width, height, QImage::Format_RGB32);
  size_t   idx   = 0;
  for (int y     = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      uint argb = 0xff000000
                  | (static_cast<uint32_t>(buffer[idx]) << 16) & 0x00ff0000
                  | (static_cast<uint32_t>(buffer[idx + 1]) << 8) & 0x0000ff00
                  | (static_cast<uint32_t>(buffer[idx + 2])) & 0x000000ff;
      argb = FixValue(argb, r, g, b, a);

      idx += 3;
      image.setPixel(x, y, argb);
    }
  }

  return image;
}
QImage ImageEditorWidget::CreateARGBImage()
{
  size_t        layer   = m_spinBoxLayer->value() - 1;
  uint16_t      width   = m_image->GetWidth(layer);
  uint16_t      height  = m_image->GetHeight(layer);
  const uint8_t *buffer = m_image->GetData(layer);


  bool r = m_btnR->isChecked();
  bool g = m_btnG->isChecked();
  bool b = m_btnB->isChecked();
  bool a = m_btnA->isChecked();


  QImage   image = QImage(width, height, QImage::Format_ARGB32);
  size_t   idx   = 0;
  for (int y     = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      uint argb = (static_cast<uint32_t>(buffer[idx + 3]) << 24) & 0xff000000
                  | (static_cast<uint32_t>(buffer[idx + 0]) << 16) & 0x00ff0000
                  | (static_cast<uint32_t>(buffer[idx + 1]) << 8) & 0x0000ff00
                  | (static_cast<uint32_t>(buffer[idx + 2])) & 0x000000ff;
      argb = FixValue(argb, r, g, b, a);
      idx += 4;
      image.setPixel(x, y, argb);
    }
  }

  return image;

}

uint32_t ImageEditorWidget::FixValue(uint32_t argb, bool r, bool g, bool b, bool a)
{
  uint32_t ca = (argb & 0xff000000) >> 24;
  uint32_t cr = argb & 0x00ff0000;
  uint32_t cg = argb & 0x0000ff00;
  uint32_t cb = argb & 0x000000ff;

  if (!r && !g && !b && a)
  {
    return 0xff000000
           | ca << 16
           | ca << 8
           | ca;
  }

  uint32_t res = 0x00;
  if (a)
  {
    res |= ca << 24;
  }
  else
  {
    res |= 0xff000000;
  }
  if (r)
  {
    res |= cr;
  }
  if (g)
  {
    res |= cg;
  }
  if (b)
  {
    res |= cb;
  }


  return res;
}