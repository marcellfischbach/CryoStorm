//
// Created by MCEL on 03.02.2025.
//

#pragma once

#include <csEditor/editors/iEditorFactory.hh>

class ImageEditorFactory : public iEditorFactory
{
public:

  bool CanEdit(const cs::csAssetLocator &locator) const override;

  EditorWidget *Create(const cs::csAssetLocator &locator) const override;
};
