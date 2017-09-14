#include "addressadddialog.h"
#include "ui_addressadddialog.h"

AddressAddDialog::AddressAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddressAddDialog)
{
    ui->setupUi(this);
}

AddressAddDialog::~AddressAddDialog()
{
    delete ui;
}
