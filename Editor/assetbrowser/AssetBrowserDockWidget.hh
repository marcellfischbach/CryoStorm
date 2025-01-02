//
// Created by mcel on 17.09.2024.
//

#pragma once

#include <QDockWidget>


class AssetBrowserWidget;
class AssetBrowserDockWidget : public QDockWidget
{
public:
  AssetBrowserDockWidget(QWidget* parent = nullptr);

  AssetBrowserWidget *GetAssetBrowser ();
  const AssetBrowserWidget *GetAssetBrowser () const;

private:
  AssetBrowserWidget *m_assetBrowser;
};
