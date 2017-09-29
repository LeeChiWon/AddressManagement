#include "addressadddialog.h"
#include "ui_addressadddialog.h"

AddressAddDialog::AddressAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddressAddDialog),
    IsReject(false),PhoneNumberCount(1),EMailCount(1),AddressNumberCount(1)
{
    ui->setupUi(this);
    UIInit();
    ComboInit();
    ui->comboBox_Address->addItems(QStringList()<<tr("Home")<<tr("Company")<<tr("Etc"));
    ui->comboBox_Address_2->addItems(QStringList()<<tr("Home")<<tr("Company")<<tr("Etc"));
    ui->comboBox_Address_3->addItems(QStringList()<<tr("Home")<<tr("Company")<<tr("Etc"));

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
    ui->lineEdit_CompanyName->setPlaceholderText(tr("CompanyName"));
    ui->lineEdit_CompanyPosition->setPlaceholderText(tr("CompanyPosition"));
    ui->lineEdit_CompanyDepartment->setPlaceholderText(tr("CompanyDepartment"));
    ui->lineEdit_AddressNumber->setPlaceholderText(tr("AddressNumber"));
    ui->lineEdit_AddressNumber_2->setPlaceholderText(tr("AddressNumber"));
    ui->lineEdit_AddressNumber_3->setPlaceholderText(tr("AddressNumber"));
    ui->textEdit_Address->setPlaceholderText(tr("Detail Address"));
    ui->textEdit_Address_2->setPlaceholderText(tr("Detail Address"));
    ui->textEdit_Address_3->setPlaceholderText(tr("Detail Address"));
    ui->pushButton_AddressNumberDelete->setVisible(false);
    ui->pushButton_EMailDelete->setVisible(false);
    ui->pushButton_PhoneNumberDelete->setVisible(false);
    ui->lineEdit_AddressNumber->clear();
    ui->lineEdit_AddressNumber_2->clear();
    ui->lineEdit_AddressNumber_3->clear();
    ui->lineEdit_Name->clear();
    ui->lineEdit_CompanyDepartment->clear();
    ui->lineEdit_CompanyName->clear();
    ui->lineEdit_CompanyPosition->clear();
    ui->lineEdit_EMail->clear();
    ui->lineEdit_EMail_2->clear();
    ui->lineEdit_EMail_3->clear();
    ui->lineEdit_PhoneNumber->clear();
    ui->lineEdit_PhoneNumber_2->clear();
    ui->lineEdit_PhoneNumber_3->clear();
    ui->textEdit_Address->clear();
    ui->textEdit_Address_2->clear();
    ui->textEdit_Address_3->clear();
    ui->textEdit_Memo->clear();
    ui->comboBox_Address->setCurrentIndex(-1);
    ui->comboBox_Address_2->setCurrentIndex(-1);
    ui->comboBox_Address_3->setCurrentIndex(-1);
    ui->comboBox_Group->setCurrentIndex(-1);
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

void AddressAddDialog::DBSave()
{
    QSqlDatabase DB=QSqlDatabase::database("MainDB");
    QString QueryStr="select * from address_management where ";
    QString DuplicationData;
    bool bAddStr=false;
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
            QueryStr.append(QString("phonenumber='%1' or phonenumber2='%1' or phonenumber3='%1'").arg(ui->lineEdit_PhoneNumber->text()));
            bAddStr=true;
        }
        if(!ui->lineEdit_PhoneNumber_2->text().isEmpty())
        {
            if(bAddStr)
            {
                QueryStr.append(" or ");
            }
            QueryStr.append(QString("phonenumber='%1' or phonenumber2='%1' or phonenumber3='%1'").arg(ui->lineEdit_PhoneNumber_2->text()));
        }
        if(!ui->lineEdit_PhoneNumber_3->text().isEmpty())
        {
            if(bAddStr)
            {
                QueryStr.append(" or ");
            }
            QueryStr.append(QString("phonenumber='%1' or phonenumber2='%1' or phonenumber3='%1'").arg(ui->lineEdit_PhoneNumber_3->text()));
        }
        query.exec(QueryStr);

        if(!query.next())
        {
            query.exec(QString("insert into address_management(name,phonenumber,phonenumber2,phonenumber3,email,email2,email3,grouping,companyname,department,position,"
                               "addresstype,addressnumber,address,addresstype2,addressnumber2,address2,addresstype3,addressnumber3,address3,memo)"
                               " values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14','%15','%16','%17','%18','%19','%20','%21')")
                       .arg(ui->lineEdit_Name->text(),ui->lineEdit_PhoneNumber->text(),ui->lineEdit_PhoneNumber_2->text(),ui->lineEdit_PhoneNumber_3->text(),ui->lineEdit_EMail->text())
                       .arg(ui->lineEdit_EMail_2->text(),ui->lineEdit_EMail_3->text(),ui->comboBox_Group->currentText(),ui->lineEdit_CompanyName->text(),ui->lineEdit_CompanyDepartment->text())
                       .arg(ui->lineEdit_CompanyPosition->text(),ui->comboBox_Address->currentText(),ui->lineEdit_AddressNumber->text(),ui->textEdit_Address->toPlainText(),ui->comboBox_Address_2->currentText())
                       .arg(ui->lineEdit_AddressNumber_2->text(),ui->textEdit_Address_2->toPlainText(),ui->comboBox_Address_3->currentText(),ui->lineEdit_AddressNumber_3->text(),ui->textEdit_Address_3->toPlainText())
                       .arg(ui->textEdit_Memo->toPlainText()));
        }

        else
        {
            DuplicationData=QString("%1:%2\n%3:%4\n%5:%6\n%7:%8\n%9:%10\n%11:%12\n%13:%14").arg(tr("Name"),query.value("name").toString(),tr("PhoneNumber"),query.value("phonenumber").toString())
                    .arg(tr("Email"),query.value("email").toString(),tr("GroupName"),query.value("grouping").toString(),tr("AddressType"),query.value("addresstype").toString())
                    .arg(tr("AddressNumber"),query.value("addressnumber").toString(),tr("Address"),query.value("address").toString());
            int ret = QMessageBox::information(this, tr("Duplicate PhoneNumber"),
                                               DuplicationData+tr("\nDo you want Overwrite?\nYes:Overwrite, No:Cancel"),
                                               QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
            switch(ret)
            {
            case QMessageBox::Yes:
                query.exec(QString("update address_management set name='%1', phonenumber='%2',phonenumber2='%3',phonenumber3='%4',email='%5',email2='%6',email3='%7',grouping='%8',companyname='%9',department='%10'"
                                   ",position='%11',addresstype='%12',addressnumber='%13',address='%14',addresstype2='%15',addressnumber2='%16',address2='%17',addresstype3='%18',addressnumber3='%19',address3='%20',memo='%21'")
                           .arg(ui->lineEdit_Name->text(),ui->lineEdit_PhoneNumber->text(),ui->lineEdit_PhoneNumber_2->text(),ui->lineEdit_PhoneNumber_3->text(),ui->lineEdit_EMail->text())
                           .arg(ui->lineEdit_EMail_2->text(),ui->lineEdit_EMail_3->text(),ui->comboBox_Group->currentText(),ui->lineEdit_CompanyName->text(),ui->lineEdit_CompanyDepartment->text())
                           .arg(ui->lineEdit_CompanyPosition->text(),ui->comboBox_Address->currentText(),ui->lineEdit_AddressNumber->text(),ui->textEdit_Address->toPlainText(),ui->comboBox_Address_2->currentText())
                           .arg(ui->lineEdit_AddressNumber_2->text(),ui->textEdit_Address_2->toPlainText(),ui->comboBox_Address_3->currentText(),ui->lineEdit_AddressNumber_3->text(),ui->textEdit_Address_3->toPlainText())
                           .arg(ui->textEdit_Memo->toPlainText()));
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
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void AddressAddDialog::ComboInit()
{
    ui->comboBox_Group->clear();
    ui->comboBox_Group->addItem("");
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
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
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
