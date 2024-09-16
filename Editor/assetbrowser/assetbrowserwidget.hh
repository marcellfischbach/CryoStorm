//
// Created by Marcell on 16.09.2024.
//

#ifndef CRIMSONEDGE_ASSETBROWSERWIDGET_HH
#define CRIMSONEDGE_ASSETBROWSERWIDGET_HH

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui
{
class AssetBrowserWidget;
}
QT_END_NAMESPACE

class AssetBrowserWidget : public QWidget
{
Q_OBJECT

public:
  explicit AssetBrowserWidget(QWidget *parent = nullptr);
  ~AssetBrowserWidget() override;

private:
  Ui::AssetBrowserWidget *ui;
};


#endif //CRIMSONEDGE_ASSETBROWSERWIDGET_HH
