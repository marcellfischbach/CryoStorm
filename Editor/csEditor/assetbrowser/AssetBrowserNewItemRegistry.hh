
#pragma once

#include <vector>


struct iAssetBrowserNewItem;

class AssetBrowserNewItemRegistry
{
public:
  static AssetBrowserNewItemRegistry &Get();

  AssetBrowserNewItemRegistry &RegisterNewItem(iAssetBrowserNewItem *item);
  const std::vector<iAssetBrowserNewItem *> &GetItems() const;

private:
    AssetBrowserNewItemRegistry() = default;

    std::vector<iAssetBrowserNewItem *> m_items;
};
