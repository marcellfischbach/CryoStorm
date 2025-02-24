//
// Created by Marcell on 02.01.2025.
//

#pragma once

#include <csEditor/editors/iEditorFactory.hh>

class ShaderGraphEditorFactory : public iEditorFactory
{
public:
  ShaderGraphEditorFactory() = default;
  ~ShaderGraphEditorFactory() override = default;

  bool CanEdit(const cs::csAssetLocator &locator) const override;

  EditorWidget *Create(const cs::csAssetLocator &locator) const override;


};
