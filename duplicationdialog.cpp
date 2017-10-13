#include "duplicationdialog.h"
#include "ui_duplicationdialog.h"

DuplicationDialog::DuplicationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DuplicationDialog),
    Upload(false)
{
    ui->setupUi(this);
    connect(ui->pushButton_Clean,SIGNAL(clicked(bool)),this,SLOT(accept()));
    TableWidgetInit();
}

DuplicationDialog::~DuplicationDialog()
{
    delete ui;
}

void DuplicationDialog::Init()
{
    DBShow();
    if(ui->tableWidget->rowCount()<1)
    {
        ui->pushButton_Clean->setEnabled(false);
    }
}

void DuplicationDialog::Init(QMultiMap<QString, QStringList> List)
{
    QButtonGroup *ButtonGroup;
    QRadioButton *RadioButton;
    QColor Rgb;
    qsrand(QTime::currentTime().msecsSinceStartOfDay());
    QString BeforeKey;

    for(int i=0; i<List.keys().count(); i++)
    {
        if(List.keys().at(i)!=BeforeKey)
        {
            BeforeKey=List.keys().at(i);

            for(int j=0; j<List.values(List.keys().at(i)).count(); j++)
            {
                if(j==0)
                {
                    ButtonGroup=new QButtonGroup();
                    Rgb.setRgb(qrand()*255/RAND_MAX,qrand()*255/RAND_MAX,qrand()*255/RAND_MAX);
                }
                ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
                RadioButton=new QRadioButton(List.values(List.keys().at(i)).at(j).at(0));
                ButtonGroup->addButton(RadioButton);
                ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,0,RadioButton);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(BeforeKey));
                ui->tableWidget->item(ui->tableWidget->rowCount()-1,1)->setBackgroundColor(Rgb);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,new QTableWidgetItem(List.values(List.keys().at(i)).at(j).at(1)));
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,new QTableWidgetItem(List.values(List.keys().at(i)).at(j).at(2)));

            }
        }
    }
    Upload=true;
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}

void DuplicationDialog::TableWidgetInit()
{
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("Name")<<tr("PhoneNumber")<<tr("GroupName")<<tr("Memo"));
    ui->tableWidget->setRowCount(0);
}

void DuplicationDialog::DBShow()
{    
    QButtonGroup *ButtonGroup;
    QRadioButton *RadioButton;
    QString PhoneNumber;
    QColor Rgb;

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
        query.exec(QString("select name,phonenumber,grouping,memo from address_management where phonenumber IN (select phonenumber from address_management group by phonenumber having count(*)>1) order by phonenumber asc"));
        while(query.next())
        {
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
            RadioButton=new QRadioButton(query.value("name").toString());

            if(PhoneNumber!=query.value("phonenumber").toString())
            {
                ButtonGroup=new QButtonGroup();
                qsrand(QTime::currentTime().msecsSinceStartOfDay());
                Rgb.setRgb(qrand()*255/RAND_MAX,qrand()*255/RAND_MAX,qrand()*255/RAND_MAX);
            }

            ButtonGroup->addButton(RadioButton);
            ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,0,RadioButton);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(query.value("phonenumber").toString()));
            ui->tableWidget->item(ui->tableWidget->rowCount()-1,1)->setBackgroundColor(Rgb);
            PhoneNumber=query.value("phonenumber").toString();
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,new QTableWidgetItem(query.value("grouping").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,new QTableWidgetItem(query.value("memo").toString()));
        }
        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->resizeRowsToContents();
        DB.commit();
        DB.close();

    }
    catch(QException &e)
    {
        ShowMessage(QString("%1\n%2").arg(tr("Database Error!"),e.what()),0);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void DuplicationDialog::DBCommand(QString QueryStr)
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
        DB.commit();
        DB.close();

    }
    catch(QException &e)
    {
        ShowMessage(QString("%1\n%2").arg(tr("Database Error!"),e.what()),0);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void DuplicationDialog::on_pushButton_Clean_clicked()
{
    if(Upload)
    {
        for(int i=0; i<ui->tableWidget->rowCount(); i++)
        {
            if(ui->tableWidget->cellWidget(i,0)!=NULL)
            {
                QRadioButton *RadioBox = qobject_cast<QRadioButton*>(ui->tableWidget->cellWidget(i,0));

                if(RadioBox->isChecked())
                {
                    DBCommand(QString("update address_management set name='%1', phonenumber='%2',grouping='%3',memo='%4' where phonenumber='%2'")
                              .arg(RadioBox->text(),ui->tableWidget->item(i,1)->text(),ui->tableWidget->item(i,2)->text(),ui->tableWidget->item(i,3)->text()));
                }
            }
        }
    }
    else
    {
        for(int i=0; i<ui->tableWidget->rowCount(); i++)
        {
            if(ui->tableWidget->cellWidget(i,0)!=NULL)
            {
                QRadioButton *RadioBox = qobject_cast<QRadioButton*>(ui->tableWidget->cellWidget(i,0));
                if(!RadioBox->isChecked())
                {
                    DBCommand(QString("delete from address_management where name='%1' and phonenumber='%2'").arg(RadioBox->text(),ui->tableWidget->item(i,1)->text()));
                }
            }
        }
    }
    QMessageBox::information(this,tr("Duplication Clean"),tr("Duplication address is Cleaned."),QMessageBox::Ok);
    this->close();
}

