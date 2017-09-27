#include "addressdownloaddialog.h"
#include "ui_addressdownloaddialog.h"

AddressDownloadDialog::AddressDownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddressDownloadDialog)
{
    ui->setupUi(this);
    ListWidgetInit();
}

AddressDownloadDialog::~AddressDownloadDialog()
{
    delete ui;
}

void AddressDownloadDialog::on_checkBox_All_clicked(bool checked)
{
    ui->checkBox_Address->setChecked(checked);
    ui->checkBox_CompanyDepartment->setChecked(checked);
    ui->checkBox_CompanyName->setChecked(checked);
    ui->checkBox_CompanyPosition->setChecked(checked);
    ui->checkBox_EMail->setChecked(checked);
    ui->checkBox_Group->setChecked(checked);
    ui->checkBox_Memo->setChecked(checked);
    ui->checkBox_Name->setChecked(checked);
    ui->checkBox_PhoneNumber->setChecked(checked);
}

void AddressDownloadDialog::on_pushButton_Add_clicked()
{
    QModelIndexList indexes = ui->listWidget_Group->selectionModel()->selectedRows();

    for (int i=0; i<indexes.count(); i++)
    {
        QList<QListWidgetItem *> items = ui->listWidget_OutputGroup->findItems(indexes.at(i).data().toString(), Qt::MatchFixedString);
        if(items.count()>0)
        {
            continue;
        }
        ui->listWidget_OutputGroup->addItem(indexes.at(i).data().toString());
    }
}

void AddressDownloadDialog::on_pushButton_Delete_clicked()
{

}

void AddressDownloadDialog::on_pushButton_Download_clicked()
{

}

void AddressDownloadDialog::ListWidgetInit()
{
    ui->listWidget_Group->addItem(tr("All"));

    QSqlDatabase DB=QSqlDatabase::database("MainDB");

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            emit DBInit();
        }

        QSqlQuery query(DB);
        query.exec(QString("select * from group_management"));

        while(query.next())
        {
            ui->listWidget_Group->addItem(query.value("groupname").toString());
        }
        DB.close();
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}
