//
// Created by Marcell on 25.12.2024.
//

#ifndef CRYOSTORM_ASSETBROWSERNEWITEMDIALOG_HH
#define CRYOSTORM_ASSETBROWSERNEWITEMDIALOG_HH

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui
{
class AssetBrowserNewItemDialog;
}
QT_END_NAMESPACE

class AssetBrowserNewItemDialog : public QDialog
{
Q_OBJECT

public:
  explicit AssetBrowserNewItemDialog(QWidget *parent = nullptr);
  ~AssetBrowserNewItemDialog() override;

  std::string GetName () const;

private:
  void onCancel();
  void onOK();

private:
  Ui::AssetBrowserNewItemDialog *ui;
};


#endif //CRYOSTORM_ASSETBROWSERNEWITEMDIALOG_HH
