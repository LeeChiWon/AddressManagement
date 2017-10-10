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
            ui->lineEdit_PhoneNumber_2->setText(query.value("phonenumber2").toString());
            ui->lineEdit_PhoneNumber_3->setText(query.value("phonenumber3").toString());
            ui->lineEdit_EMail->setText(query.value("email").toString());
            ui->lineEdit_EMail_2->setText(query.value("email2").toString());
            ui->lineEdit_EMail_3->setText(query.value("email3").toString());
            ui->comboBox_Group->setCurrentText(query.value("grouping").toString());
            ui->lineEdit_CompanyName->setText(query.value("companyname").toString());
            ui->lineEdit_CompanyDepartment->setText(query.value("department").toString());
            ui->lineEdit_CompanyPosition->setText(query.value("position").toString());
            ui->comboBox_Address->setCurrentText(query.value("addresstype").toString());
            ui->comboBox_Address_2->setCurrentText(query.value("addresstype2").toString());
            ui->comboBox_Address_3->setCurrentText(query.value("addresstype3").toString());
            ui->lineEdit_AddressNumber->setText(query.value("addressnumber").toString());
            ui->lineEdit_AddressNumber_2->setText(query.value("addressnumber2").toString());
            ui->lineEdit_AddressNumber_3->setText(query.value("addressnumber3").toString());
            ui->textEdit_Address->setText(query.value("address").toString());
            ui->textEdit_Address_2->setText(query.value("address2").toString());
            ui->textEdit_Address_3->setText(query.value("address3").toString());
            ui->textEdit_Memo->setText(query.value("memo").toString());
            BeforeName=ui->lineEdit_Name->text();
            BeforePhoneNumber=ui->lineEdit_PhoneNumber->text();
        }
        DB.commit();
        DB.close();
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
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
        query.exec(QString("update address_management set name='%1',phonenumber='%2',phonenumber2='%3',phonenumber3='%4',email='%5',email2='%6',email3='%7',grouping='%8'"
                           ",companyname='%9',department='%10',position='%11',addresstype='%12',addressnumber='%13',address='%14'"
                           ",addresstype2='%15',addressnumber2='%16',address2='%17',addresstype3='%18',addressnumber3='%19',address3='%20',memo='%21' where name='%22' and phonenumber='%23'")
                   .arg(ui->lineEdit_Name->text(),ui->lineEdit_PhoneNumber->text(),ui->lineEdit_PhoneNumber_2->text(),ui->lineEdit_PhoneNumber_3->text(),ui->lineEdit_EMail->text())
                   .arg(ui->lineEdit_EMail_2->text(),ui->lineEdit_EMail_3->text(),ui->comboBox_Group->currentText(),ui->lineEdit_CompanyName->text(),ui->lineEdit_CompanyDepartment->text())
                   .arg(ui->lineEdit_CompanyPosition->text(),ui->comboBox_Address->currentText(),ui->lineEdit_AddressNumber->text(),ui->textEdit_Address->toPlainText())
                   .arg(ui->comboBox_Address_2->currentText(),ui->lineEdit_AddressNumber_2->text(),ui->textEdit_Address_2->toPlainText())
                   .arg(ui->comboBox_Address_3->currentText(),ui->lineEdit_AddressNumber_3->text(),ui->textEdit_Address_3->toPlainText
                        (),ui->textEdit_Memo->toPlainText())
                   .arg(BeforeName,BeforePhoneNumber));
        DB.commit();
        DB.close();
        QMessageBox::information(this,tr("Update Address"),tr("Address is Updated."),QMessageBox::Ok);
        emit TableWidgetUpdate();
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void AddressDetailDialog::on_pushButton_Cancel_clicked()
{
    this->close();
}

void AddressDetailDialog::UIInit()
{
    QStringList AddressType=QStringList()<<tr("Home")<<tr("Company")<<tr("Etc");
    ui->comboBox_Address->addItems(AddressType);
    ui->comboBox_Address_2->addItems(AddressType);
    ui->comboBox_Address_3->addItems(AddressType);

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
        ui->comboBox_Group->addItem("");
        while(query.next())
        {
            ui->comboBox_Group->addItem(query.value("groupname").toString());
        }
        DB.commit();
        DB.close();
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

