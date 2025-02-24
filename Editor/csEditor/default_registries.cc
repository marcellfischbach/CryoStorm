
#include <csEditor/default_registries.hh>

#include <csEditor/assetbrowser/AssetBrowserNewItemRegistry.hh>
#include <csEditor/editors/shadergraph/ShaderGraphNewItem.hh>

#include <csEditor/editors/EditorRegistry.hh>
#include <csEditor/editors/image/ImageEditorFactory.hh>
#include <csEditor/editors/shadergraph/ShaderGraphEditorFactory.hh>


void register_defaults()
{

  AssetBrowserNewItemRegistry::Get()
      .RegisterNewItem(new ShaderGraphNewItem());

  EditorRegistry::Get()
      .RegisterEditor(new ImageEditorFactory())
      .RegisterEditor(new ShaderGraphEditorFactory())
      ;


}