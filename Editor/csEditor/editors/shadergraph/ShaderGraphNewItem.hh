
#pragma once

#include <csEditor/assetbrowser/iAssetBrowserNewItem.hh>

class ShaderGraphNewItem : public iAssetBrowserNewItem
{
public:
  std::string &GetName() const override;

  void Create(const std::fs::path &path, const std::string &fileName) override;

};
