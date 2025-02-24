//
// Created by Marcell on 25.12.2024.
//

#include <csEditor/assetbrowser/AssetBrowserNewItemRegistry.hh>
#include <csEditor/assetbrowser/iAssetBrowserNewItem.hh>


AssetBrowserNewItemRegistry &AssetBrowserNewItemRegistry::Get()
{
  static AssetBrowserNewItemRegistry registry;
  return registry;
}

AssetBrowserNewItemRegistry &AssetBrowserNewItemRegistry::RegisterNewItem(iAssetBrowserNewItem *item)
{
  m_items.push_back(item);
  return *this;
}

const std::vector<iAssetBrowserNewItem *> &AssetBrowserNewItemRegistry::GetItems() const
{
  return m_items;
}
