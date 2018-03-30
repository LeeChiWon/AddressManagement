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
    ui->checkBox_Group->setChecked(checked);
    ui->checkBox_Memo->setChecked(checked);
    ui->checkBox_Name->setChecked(checked);
    ui->checkBox_PhoneNumber->setChecked(checked);
}

void AddressDownloadDialog::on_pushButton_Add_clicked()
{
    QList<QListWidgetItem *> items = ui->listWidget_Group->selectedItems();

    foreach(QListWidgetItem *item, items)
    {
        QList<QListWidgetItem *>findItem = ui->listWidget_OutputGroup->findItems(item->text(), Qt::MatchFixedString);
        if(findItem.count()>0)
        {
            continue;
        }
        ui->listWidget_OutputGroup->addItem(item->text());
    }
}

void AddressDownloadDialog::on_pushButton_Delete_clicked()
{
    QList<QListWidgetItem*> items = ui->listWidget_OutputGroup->selectedItems();
    foreach(QListWidgetItem *item, items)
    {
        delete ui->listWidget_OutputGroup->takeItem(ui->listWidget_OutputGroup->row(item));
    }
}

void AddressDownloadDialog::on_pushButton_Download_clicked()
{
    QFileDialog FileDialog;
    QString FileName=FileDialog.getSaveFileName(this,tr("Save File"),"c://",tr("Excel (*.xlsx)"));

    QSqlDatabase DB=QSqlDatabase::database("MainDB");
    QString QueryStr="select ";
    int RowCount=0;
    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            emit DBInit();
        }
        DB.transaction();
        QSqlQuery query(DB);
        QueryStr.append(ListSelect());
        QList<QListWidgetItem *> items=ui->listWidget_OutputGroup->findItems(tr("All"),Qt::MatchFixedString);
        QueryStr.append(" from address_management");
        if(ui->listWidget_OutputGroup->count()<=0 || items.count()>0)
        {
            query.exec(QueryStr);
        }
        else
        {
            QueryStr.append(" where");
            for(int i=0; i<ui->listWidget_OutputGroup->count(); i++)
            {
                if(i>0)
                {
                    QueryStr.append(" or");
                }
                QueryStr.append(QString(" grouping='%1'").arg(ui->listWidget_OutputGroup->item(i)->text()));
            }
            query.exec(QueryStr);
        }

        QFile File(FileName);

        if(File.exists())
        {
            File.remove();
        }
        QXlsx::Document xlsx(FileName);
        QXlsx::Format format;

        if(!FileName.isEmpty() || !FileName.isNull())
        {
            while(query.next())
            {
                format.setBorderStyle(QXlsx::Format::BorderThin);
                format.setFontSize(10);
                format.setFontBold(false);

                for(int j=0; j<ListCount; j++)
                {
                    xlsx.write(RowCount+2,j+1,query.value(j).toString(),format);
                }

                format.setBottomBorderStyle(QXlsx::Format::BorderDouble);
                format.setFontBold(true);
                format.setFontSize(13);

                for(int i=1; i<=ListCount; i++)
                {
                    xlsx.setColumnWidth(i,25);
                    xlsx.write(1,i,ListNames.at(i-1),format);
                }
                RowCount++;
            }
            if(xlsx.save())
            {
                QMessageBox::information(this,tr("Save"),tr("Excel File is Saved."),QMessageBox::Ok);
            }
        }
        xlsx.deleteLater();
        DB.commit();
        DB.close();

    /*    QAxObject* excel = new QAxObject( "Excel.Application", 0 );
        QAxObject* workbooks = excel->querySubObject( "Workbooks" );
        QAxObject* workbook = workbooks->querySubObject( "Open(const QString&",FileName);
        QAxObject* worksheets = workbook->querySubObject( "Worksheets");
        QAxObject* worksheet = worksheets->querySubObject( "Item(int)",1);
        worksheet->dynamicCall("PrintOut()");
        workbook->dynamicCall("Close()");
        excel->dynamicCall("Quit()");
        worksheet->deleteLater();
        worksheets->deleteLater();
        workbook->deleteLater();
        workbooks->deleteLater();
        excel->deleteLater();*/

    }
    catch(QException &e)
    {
        ShowMessage(QString("%1\n%2").arg(tr("Database Error!"),e.what()),0);
        QSqlDatabase::removeDatabase("MainDB");
    }
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
        DB.transaction();
        QSqlQuery query(DB);
        query.exec(QString("select * from group_management"));

        while(query.next())
        {
            ui->listWidget_Group->addItem(query.value("groupname").toString());
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

QString AddressDownloadDialog::ListSelect()
{
    QString QueryStr;
    ListCount=0;
    ListNames.clear();
    if(ui->checkBox_All->isChecked())
    {
        QueryStr.append("*");
        ListCount=4;
        ListNames=QStringList()<<tr("Name")<<tr("PhoneNumber")<<tr("Grouping")<<tr("Memo");
        return QueryStr;
    }
    else
    {
        if(ui->checkBox_Name->isChecked())
        {
            QueryStr.append("name");
            ListCount++;
            ListNames.append(tr("Name"));
        }

        if(ui->checkBox_PhoneNumber->isChecked())
        {
            if(!QueryStr.isEmpty())
            {
                QueryStr.append(",");
            }
            QueryStr.append("phonenumber");
            ListCount++;
            ListNames.append(tr("PhoneNumber"));

        }

        if(ui->checkBox_Group->isChecked())
        {
            if(!QueryStr.isEmpty())
            {
                QueryStr.append(",");
            }
            QueryStr.append("grouping");
            ListCount++;
            ListNames.append(tr("Grouping"));
        }

        if(ui->checkBox_Memo->isChecked())
        {
            if(!QueryStr.isEmpty())
            {
                QueryStr.append(",");
            }
            QueryStr.append("memo");
            ListCount++;
            ListNames.append(tr("Memo"));
        }
    }
    return QueryStr;
}
