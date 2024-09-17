//
// Created by mcel on 17.09.2024.
//

#include <assetbrowser/assetbrowserdockwidget.hh>
#include <assetbrowser/assetbrowserwidget.hh>


AssetBrowserDockWidget::AssetBrowserDockWidget (QWidget* parent)
: QDockWidget (QString ("Asset browser"), parent)
{
  m_assetBrowser = new AssetBrowserWidget(this);

  setWidget(m_assetBrowser);
}