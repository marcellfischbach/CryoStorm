//
// Created by mcel on 17.09.2024.
//

#include <assetbrowser/AssetBrowserDockWidget.hh>
#include <assetbrowser/AssetBrowserWidget.hh>


AssetBrowserDockWidget::AssetBrowserDockWidget (QWidget* parent)
: QDockWidget (QString ("Asset browser"), parent)
{
  m_assetBrowser = new AssetBrowserWidget(this);

  setWidget(m_assetBrowser);
}