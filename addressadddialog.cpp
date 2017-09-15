#include "addressadddialog.h"
#include "ui_addressadddialog.h"


AddressAddDialog::AddressAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddressAddDialog),
    IsReject(false)
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
    ui->label_11->setVisible(false);
    ui->label_12->setVisible(false);
    ui->lineEdit_AddressNumber_2->setVisible(false);
    ui->lineEdit_AddressNumber_3->setVisible(false);
    ui->comboBox_Address_2->setVisible(false);
    ui->comboBox_Address_3->setVisible(false);
    ui->textEdit_Address_2->setVisible(false);
    ui->textEdit_Address_3->setVisible(false);
    ui->lineEdit_EMail_2->setVisible(false);
    ui->lineEdit_EMail_3->setVisible(false);
    ui->lineEdit_PhoneNumber_2->setVisible(false);
    ui->lineEdit_PhoneNumber_3->setVisible(false);
    ui->lineEdit_CompanyDepartment->setPlaceholderText(tr("CompanyDepartment"));
}

void AddressAddDialog::on_lineEdit_CompanyDepartment_selectionChanged()
{
    ui->lineEdit_CompanyDepartment->setText("");
}

void AddressAddDialog::reject()
{
    if(IsReject)
    {
        QDialog::reject();
    }
}

void AddressAddDialog::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void AddressAddDialog::on_pushButton_AddressNumber_clicked()
{
    QDesktopServices::openUrl(QUrl(QString("https://search.naver.com/search.naver?query=%1").arg(tr("AddressNumberSearch"))));
}
