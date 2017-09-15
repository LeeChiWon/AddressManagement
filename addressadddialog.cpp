#include "addressadddialog.h"
#include "ui_addressadddialog.h"

AddressAddDialog::AddressAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddressAddDialog)
{
    ui->setupUi(this);
    UIInit();
}

AddressAddDialog::~AddressAddDialog()
{
    delete ui;
}

void AddressAddDialog::UIInit()
{
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_7->setVisible(false);
    ui->lineEdit_EMail_2->setVisible(false);
    ui->lineEdit_EMail_3->setVisible(false);
    ui->lineEdit_PhoneNumber_2->setVisible(false);
    ui->lineEdit_PhoneNumber_3->setVisible(false);
}
