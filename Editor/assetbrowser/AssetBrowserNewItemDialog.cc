//
// Created by Marcell on 25.12.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AssetBrowserNewItemDialog.h" resolved

#include "AssetBrowserNewItemDialog.hh"
#include "ui_AssetBrowserNewItemDialog.h"


AssetBrowserNewItemDialog::AssetBrowserNewItemDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AssetBrowserNewItemDialog)
{
  ui->setupUi(this);
  ui->btnOK->setDefault(true);

  connect(ui->btnOK, &QPushButton::clicked, this, &AssetBrowserNewItemDialog::onOK);
  connect(ui->btnCancel, &QPushButton::clicked, this, &AssetBrowserNewItemDialog::onCancel);
}

std::string AssetBrowserNewItemDialog::GetName() const
{
  return ui->leName->text().toStdString();
}

AssetBrowserNewItemDialog::~AssetBrowserNewItemDialog()
{
  delete ui;
}

void AssetBrowserNewItemDialog::onCancel()
{
  reject();
}


void AssetBrowserNewItemDialog::onOK()
{
  accept();
}
