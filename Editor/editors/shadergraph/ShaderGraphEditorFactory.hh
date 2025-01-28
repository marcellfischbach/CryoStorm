//
// Created by Marcell on 02.01.2025.
//

#pragma once

#include <editors/iEditorFactory.hh>
#include <QDoubleSpinBox>

class ShaderGraphEditorFactory : public iEditorFactory
{
public:
  ShaderGraphEditorFactory() = default;
  ~ShaderGraphEditorFactory() override = default;

  bool CanEdit(const cs::csAssetLocator &locator) const override;

  void Edit(const cs::csAssetLocator &locator, QWidget *path = nullptr) const override;


};

class MySpinBox : public QDoubleSpinBox
{
Q_OBJECT;
public:
  MySpinBox(QWidget *parent, QString styleSheet);

protected:
  void focusInEvent(QFocusEvent *evt) override;
  void focusOutEvent(QFocusEvent *evt) override;

signals:
  void focusGained ();
  void focusLost ();

private:
  QString m_styleSheet;
};
