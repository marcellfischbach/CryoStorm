//
// Created by mcel on 17.09.2024.
//

#include <csEditor/assetbrowser/AssetBrowserDockWidget.hh>
#include <csEditor/assetbrowser/AssetBrowserWidget.hh>


AssetBrowserDockWidget::AssetBrowserDockWidget (QWidget* parent)
: QDockWidget (QString ("Asset browser"), parent)
{
  m_assetBrowser = new AssetBrowserWidget(this);

  setWidget(m_assetBrowser);
}