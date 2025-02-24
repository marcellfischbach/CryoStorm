//
// Created by MCEL on 03.02.2025.
//

#include <csEditor/editors/EditorDialog.hh>
#include <csEditor/editors/EditorWidget.hh>
#include <QGridLayout>

EditorDialog::EditorDialog(EditorWidget *editor, QWidget *parent)
    : QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint)
{
  QGridLayout *layout = new QGridLayout(this);
  layout->addWidget(editor, 0, 0);
  layout->setContentsMargins(0, 0, 0, 0);
  editor->setParent(this);

  setAttribute(Qt::WA_DeleteOnClose);
  resize(1024, 768);
}

EditorDialog::~EditorDialog()
{
}

