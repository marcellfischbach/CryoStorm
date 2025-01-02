//
// Created by Marcell on 02.01.2025.
//

#include <editors/EditorRegistry.hh>

using namespace cs;

EditorRegistry &EditorRegistry::Get()
{
  static EditorRegistry registry;
  return registry;
}



EditorRegistry &EditorRegistry::RegisterEditor(iEditorFactory *editor)
{
  m_editors.push_back(editor);
  return *this;
}

iEditorFactory *EditorRegistry::GetEditor(const csResourceLocator &locator) const
{
  for (auto editor: m_editors)
  {
    if (editor->CanEdit(locator))
    {
      return editor;
    }
  }
  return nullptr;
}