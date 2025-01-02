
#include <default_registries.hh>

#include <assetbrowser/AssetBrowserNewItemRegistry.hh>
#include <editors/shadergraph/ShaderGraphNewItem.hh>

#include <editors/EditorRegistry.hh>
#include <editors/shadergraph/ShaderGraphEditorFactory.hh>


void register_defaults()
{

  AssetBrowserNewItemRegistry::Get()
      .RegisterNewItem(new ShaderGraphNewItem());


  EditorRegistry::Get()
      .RegisterEditor(new ShaderGraphEditorFactory());

}