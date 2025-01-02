//
// Created by Marcell on 02.01.2025.
//

#pragma once

#include <editors/iEditorFactory.hh>

class ShaderGraphEditorFactory : public iEditorFactory
{
public:
  ShaderGraphEditorFactory() = default;
  ~ShaderGraphEditorFactory() override = default;

  bool CanEdit (const cs::csResourceLocator &locator) const override;

  void Edit (const cs::csResourceLocator &locator, QWidget* path = nullptr) const override;



};
