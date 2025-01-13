//
// Created by Marcell on 02.01.2025.
//

#pragma once


#include <editors/iEditorFactory.hh>

class EditorRegistry
{
public:
    static EditorRegistry &Get();

    EditorRegistry &RegisterEditor(iEditorFactory *editor);

    iEditorFactory *GetEditor(const cs::csAssetLocator &locator) const;

private:
    EditorRegistry() = default;

    std::vector<iEditorFactory *> m_editors;
};
