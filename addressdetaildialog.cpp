#include "addressdetaildialog.h"
#include "ui_addressdetaildialog.h"

AddressDetailDialog::AddressDetailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddressDetailDialog)
{
    ui->setupUi(this);
    UIInit();
}

AddressDetailDialog::~AddressDetailDialog()
{
    delete ui;
}

void AddressDetailDialog::DBShow(QString QueryStr)
{
    QSqlDatabase DB=QSqlDatabase::database("MainDB");

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            emit DBInit();
        }
        DB.transaction();
        QSqlQuery query(DB);
        query.exec(QueryStr);

        while(query.next())
        {
            ui->lineEdit_Name->setText(query.value("name").toString());
            ui->lineEdit_PhoneNumber->setText(query.value("phonenumber").toString());          
            ui->comboBox_Group->setCurrentText(query.value("grouping").toString());          
            ui->textEdit_Memo->setText(query.value("memo").toString());
            BeforeName=ui->lineEdit_Name->text();
            BeforePhoneNumber=ui->lineEdit_PhoneNumber->text();
        }
        DB.commit();
        DB.close();
    }
    catch(QException &e)
    {
        ShowMessage(QString("%1\n%2").arg(tr("Database Error!"),e.what()),0);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void AddressDetailDialog::on_pushButton_Modify_clicked()
{
    QSqlDatabase DB=QSqlDatabase::database("MainDB");

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            emit DBInit();
        }
        DB.transaction();
        QSqlQuery query(DB);
        query.exec(QString("update address_management set name='%1',phonenumber='%2',grouping='%3',memo='%4' where name='%5' and phonenumber='%6'")
                   .arg(ui->lineEdit_Name->text(),ui->lineEdit_PhoneNumber->text(),ui->comboBox_Group->currentText(),ui->textEdit_Memo->toPlainText())
                   .arg(BeforeName,BeforePhoneNumber));
        DB.commit();
        DB.close();
        QMessageBox::information(this,tr("Update Address"),tr("Address is Updated."),QMessageBox::Ok);

        emit TableWidgetUpdate();
        emit TreeWidgetInit();
    }
    catch(QException &e)
    {
        ShowMessage(QString("%1\n%2").arg(tr("Database Error!"),e.what()),0);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void AddressDetailDialog::on_pushButton_Cancel_clicked()
{
    this->close();
}

void AddressDetailDialog::UIInit()
{    
    QSqlDatabase DB=QSqlDatabase::database("MainDB");

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            emit DBInit();
        }
        DB.transaction();
        QSqlQuery query(DB);
        query.exec(QString("select * from group_management"));

        while(query.next())
        {
            ui->comboBox_Group->addItem(query.value("groupname").toString());
        }
        DB.commit();
        DB.close();
    }
    catch(QException &e)
    {
        ShowMessage(QString("%1\n%2").arg(tr("Database Error!"),e.what()),0);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

