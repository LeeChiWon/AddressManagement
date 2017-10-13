#include "addressadddialog.h"
#include "ui_addressadddialog.h"

AddressAddDialog::AddressAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddressAddDialog),
    IsReject(false)
{
    ui->setupUi(this);
    ComboInit();
    UIInit();
}

AddressAddDialog::~AddressAddDialog()
{
    delete ui;
}

void AddressAddDialog::UIInit()
{
    ui->lineEdit_Name->clear();   
    ui->lineEdit_PhoneNumber->clear();  
    ui->textEdit_Memo->clear();  
    ui->comboBox_Group->setCurrentIndex(0);
}

void AddressAddDialog::DBSave()
{
    QSqlDatabase DB=QSqlDatabase::database("MainDB");
    QString QueryStr;
    QString DuplicationData; 
    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            emit DBInit();
        }
        DB.transaction();
        QSqlQuery query(DB);

        if(!ui->lineEdit_PhoneNumber->text().isEmpty())
        {
            QueryStr.append(QString("select * from address_management where phonenumber='%1'").arg(ui->lineEdit_PhoneNumber->text()));
        }

        query.exec(QueryStr);       

        if(!query.next())
        {
            query.exec(QString("insert into address_management(name,phonenumber,grouping,memo) values('%1','%2','%3','%4')")
                       .arg(ui->lineEdit_Name->text(),ui->lineEdit_PhoneNumber->text(),ui->comboBox_Group->currentText(),ui->textEdit_Memo->toPlainText()));
        }

        else
        {
            DuplicationData=QString("%1: %2 -> %3\n%4: %5 -> %6\n%7: %8 -> %9\n%10: %11 -> %12").arg(tr("Name"),query.value("name").toString(),ui->lineEdit_Name->text(),tr("PhoneNumber"),query.value("phonenumber").toString(),ui->lineEdit_PhoneNumber->text())
                    .arg(tr("GroupName"),query.value("grouping").toString(),ui->comboBox_Group->currentText())
                    .arg(tr("Memo"),query.value("memo").toString(),ui->textEdit_Memo->toPlainText());

            int ret = QMessageBox::information(this, tr("Duplicate PhoneNumber"),
                                               DuplicationData+tr("\n\nDo you want Overwrite?\nYes:Overwrite, No:Cancel"),
                                               QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
            switch(ret)
            {
            case QMessageBox::Yes:
                query.exec(QString("update address_management set name='%1', phonenumber='%2,'grouping='%3',memo='%4' where name='%5' and phonenumber='%6'")
                           .arg(ui->lineEdit_Name->text(),ui->lineEdit_PhoneNumber->text(),ui->comboBox_Group->currentText())
                           .arg(ui->textEdit_Memo->toPlainText(),query.value("name").toString(),query.value("phonenumber").toString()));
                break;           
            default:
                DB.commit();
                DB.close();
                return;
                break;
            }
        }
        QMessageBox::information(this,tr("DB Save"),tr("DB is Saved."),QMessageBox::Ok);
        UIInit();
        emit TreeWidgetInit();
        emit TableWidgetUpdate();
        DB.commit();
        DB.close();
    }
    catch(QException &e)
    {
        ShowMessage(QString("%1\n%2").arg(tr("Database Error!"),e.what()),0);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void AddressAddDialog::ComboInit()
{
    ui->comboBox_Group->clear();    
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

void AddressAddDialog::on_pushButton_Save_clicked()
{    
    DBSave();
}

void AddressAddDialog::on_pushButton_Cancel_clicked()
{
    this->close();
}

void AddressAddDialog::on_pushButton_GroupAdd_clicked()
{
    GroupAddDialog GroupAddDlg;
    connect(&GroupAddDlg,SIGNAL(ComboInit()),this,SLOT(ComboInit()));
    GroupAddDlg.Combobox=true;
    GroupAddDlg.exec();
}
