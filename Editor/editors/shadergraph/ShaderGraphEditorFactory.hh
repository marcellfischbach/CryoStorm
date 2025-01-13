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

  bool CanEdit(const cs::csAssetLocator &locator) const override;

  void Edit(const cs::csAssetLocator &locator, QWidget* path = nullptr) const override;



};
