//
// Created by MCEL on 03.02.2025.
//

#pragma once

#include <QDialog>
#include <csEditor/editors/EditorWidget.hh>
#include <csEditor/editors/iEditorFactory.hh>

class EditorDialog : public QDialog
{
  Q_OBJECT;
public:
  EditorDialog(EditorWidget* editor, QWidget* parent = nullptr);
  ~EditorDialog() override;


};
