//
// Created by MCEL on 03.02.2025.
//

#pragma once

#include <QWidget>

class EditorWidget : public QWidget
{
  Q_OBJECT;
public:
  EditorWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
  ~EditorWidget() override;

};
