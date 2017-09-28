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
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
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
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
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
        ListCount=21;
        ListNames=QStringList()<<tr("Name")<<tr("PhoneNumber")<<tr("PhoneNumber2")<<tr("PhoneNumber3")<<tr("Email")<<tr("Email2")<<tr("Email3")<<tr("Grouping")<<tr("CompanyName")
                              <<tr("Department")<<tr("Position")<<tr("AddressType")<<tr("AddressNumber")<<tr("Address")<<tr("AddressType2")<<tr("AddressNumber2")<<tr("Address2")
                             <<tr("AddressType3")<<tr("AddressNumber3")<<tr("Address3")<<tr("Memo");
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
            QueryStr.append("phonenumber, phonenumber2, phonenumber3");
            ListCount+=3;
            ListNames.append(tr("PhoneNumber"));
            ListNames.append(tr("PhoneNumber2"));
            ListNames.append(tr("PhoneNumber3"));
        }
        if(ui->checkBox_EMail->isChecked())
        {
            if(!QueryStr.isEmpty())
            {
                QueryStr.append(",");
            }
            QueryStr.append("email, email2, email3");
            ListCount+=3;
            ListNames.append(tr("Email"));
            ListNames.append(tr("Email2"));
            ListNames.append(tr("Email3"));
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
        if(ui->checkBox_CompanyName->isChecked())
        {
            if(!QueryStr.isEmpty())
            {
                QueryStr.append(",");
            }
            QueryStr.append("companyname");
            ListCount++;
            ListNames.append(tr("CompanyName"));
        }
        if(ui->checkBox_CompanyDepartment->isChecked())
        {
            if(!QueryStr.isEmpty())
            {
                QueryStr.append(",");
            }
            QueryStr.append("department");
            ListNames.append(tr("Department"));
            ListCount++;
        }
        if(ui->checkBox_CompanyPosition->isChecked())
        {
            if(!QueryStr.isEmpty())
            {
                QueryStr.append(",");
            }
            QueryStr.append("position");
            ListNames.append(tr("Position"));
            ListCount++;
        }
        if(ui->checkBox_Address->isChecked())
        {
            if(!QueryStr.isEmpty())
            {
                QueryStr.append(",");
            }
            QueryStr.append("addresstype,addressnumber,address,addresstype2,addressnumber2,address2,addresstype3,addressnumber3,address3");
            ListCount+=9;
            ListNames.append(tr("AddressType"));
            ListNames.append(tr("AddressNumber"));
            ListNames.append(tr("Address"));
            ListNames.append(tr("AddressType2"));
            ListNames.append(tr("AddressNumber2"));
            ListNames.append(tr("Address2"));
            ListNames.append(tr("AddressType3"));
            ListNames.append(tr("AddressNumber3"));
            ListNames.append(tr("Address3"));
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
