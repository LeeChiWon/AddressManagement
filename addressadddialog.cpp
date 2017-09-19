#include "addressadddialog.h"
#include "ui_addressadddialog.h"


AddressAddDialog::AddressAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddressAddDialog),
    IsReject(false),PhoneNumberCount(1),EMailCount(1),AddressNumberCount(1)
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
    ui->pushButton_AddressNumberDelete->setVisible(false);
    ui->pushButton_EMailDelete->setVisible(false);
    ui->pushButton_PhoneNumberDelete->setVisible(false);
}

int AddressAddDialog::Calculation(int Value, int Select)
{
    switch(Select)
    {
    case PLUS:
        Value++;
        break;
    case MINUS:
        Value--;
        break;
    }

    return Value;
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

void AddressAddDialog::on_pushButton_PhoneNumberAdd_clicked()
{
    PhoneNumberCount=Calculation(PhoneNumberCount,PLUS);
    switch(PhoneNumberCount)
    {
    case 3:
        ui->label_4->setVisible(true);
        ui->lineEdit_PhoneNumber_3->setVisible(true);
        ui->pushButton_PhoneNumberAdd->setVisible(false);
        break;
    case 2:
        ui->label_3->setVisible(true);
        ui->lineEdit_PhoneNumber_2->setVisible(true);
        ui->pushButton_PhoneNumberDelete->setVisible(true);
        break;
    }
}

void AddressAddDialog::on_pushButton_PhoneNumberDelete_clicked()
{
    PhoneNumberCount=Calculation(PhoneNumberCount,MINUS);
    switch(PhoneNumberCount)
    {
    case 1:
        ui->pushButton_PhoneNumberDelete->setVisible(false);
        ui->label_3->setVisible(false);
        ui->lineEdit_PhoneNumber_2->setVisible(false);
        ui->lineEdit_PhoneNumber_2->setText("");
        break;
    case 2:
        ui->pushButton_PhoneNumberAdd->setVisible(true);
        ui->label_4->setVisible(false);
        ui->lineEdit_PhoneNumber_3->setVisible(false);
        ui->lineEdit_PhoneNumber_3->setText("");
        break;
    }
}

void AddressAddDialog::on_pushButton_EMailAdd_clicked()
{
    EMailCount=Calculation(EMailCount,PLUS);
    switch(EMailCount)
    {
    case 3:
        ui->label_7->setVisible(true);
        ui->lineEdit_EMail_3->setVisible(true);
        ui->pushButton_EMailAdd->setVisible(false);
        break;
    case 2:
        ui->pushButton_EMailDelete->setVisible(true);
        ui->label_6->setVisible(true);
        ui->lineEdit_EMail_2->setVisible(true);
        break;
    }
}

void AddressAddDialog::on_pushButton_EMailDelete_clicked()
{
    EMailCount=Calculation(EMailCount,MINUS);
    switch(EMailCount)
    {
    case 1:
        ui->pushButton_EMailDelete->setVisible(false);
        ui->label_6->setVisible(false);
        ui->lineEdit_EMail_2->setVisible(false);
        ui->lineEdit_EMail_2->setText("");
        break;
    case 2:
        ui->pushButton_EMailAdd->setVisible(true);
        ui->label_7->setVisible(false);
        ui->lineEdit_EMail_3->setVisible(false);
        ui->lineEdit_EMail_3->setText("");
        break;
    }
}

void AddressAddDialog::on_pushButton_AddressNumberAdd_clicked()
{
    AddressNumberCount=Calculation(AddressNumberCount,PLUS);
    switch(AddressNumberCount)
    {
    case 3:
        ui->label_12->setVisible(true);
        ui->textEdit_Address_3->setVisible(true);
        ui->comboBox_Address_3->setVisible(true);
        ui->lineEdit_AddressNumber_3->setVisible(true);
        ui->pushButton_AddressNumberAdd->setVisible(false);
        break;
    case 2:
        ui->label_11->setVisible(true);
        ui->textEdit_Address_2->setVisible(true);
        ui->comboBox_Address_2->setVisible(true);
        ui->lineEdit_AddressNumber_2->setVisible(true);
        ui->pushButton_AddressNumberDelete->setVisible(true);
        break;
    }
}

void AddressAddDialog::on_pushButton_AddressNumberDelete_clicked()
{
    AddressNumberCount=Calculation(AddressNumberCount,MINUS);
    switch(AddressNumberCount)
    {
    case 1:
        ui->pushButton_AddressNumberDelete->setVisible(false);
        ui->label_11->setVisible(false);
        ui->lineEdit_AddressNumber_2->setVisible(false);
        ui->lineEdit_AddressNumber_2->setText("");
        ui->textEdit_Address_2->setVisible(false);
        ui->textEdit_Address_2->setText("");
        ui->comboBox_Address_2->setCurrentIndex(-1);
        ui->comboBox_Address_2->setVisible(false);
        break;
    case 2:
        ui->pushButton_AddressNumberAdd->setVisible(true);
        ui->label_12->setVisible(false);
        ui->lineEdit_AddressNumber_3->setVisible(false);
        ui->lineEdit_AddressNumber_3->setText("");
        ui->textEdit_Address_3->setVisible(false);
        ui->textEdit_Address_3->setText("");
        ui->comboBox_Address_3->setCurrentIndex(-1);
        ui->comboBox_Address_3->setVisible(false);
        break;
    }
}

void AddressAddDialog::on_pushButton_Save_clicked()
{

}

void AddressAddDialog::on_pushButton_Cancel_clicked()
{
    this->close();
}
