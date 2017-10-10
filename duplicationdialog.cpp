#include "duplicationdialog.h"
#include "ui_duplicationdialog.h"

DuplicationDialog::DuplicationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DuplicationDialog)
{
    ui->setupUi(this);
    TableWidgetInit();
    DBShow();
}

DuplicationDialog::~DuplicationDialog()
{
    delete ui;
}

void DuplicationDialog::TableWidgetInit()
{
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("")<<tr("Name")<<tr("PhoneNumber")<<tr("GroupName"));
    ui->tableWidget->setRowCount(0);
}

void DuplicationDialog::DBShow()
{
    TableWidgetInit();
    QButtonGroup *ButtonGroup;
    QRadioButton *RadioButton;
    QString PhoneNumber;

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
        query.exec(QString("select name,phonenumber,grouping from address_management where phonenumber IN (select phonenumber from address_management group by phonenumber having count(*)>1) order by phonenumber asc"));
        qDebug()<<query.lastError();
        while(query.next())
        {
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
            RadioButton=new QRadioButton(query.value("name").toString());

            if(PhoneNumber!=query.value("phonenumber").toString())
            {
                ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,0,new QCheckBox());
                ButtonGroup=new QButtonGroup();
            }

            ButtonGroup->addButton(RadioButton);
            ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,1,RadioButton);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,new QTableWidgetItem(query.value("phonenumber").toString()));
            PhoneNumber=query.value("phonenumber").toString();
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,new QTableWidgetItem(query.value("grouping").toString()));
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

void DuplicationDialog::on_pushButton_Clean_clicked()
{
    for(int i=0; i<ui->tableWidget->rowCount(); i++)
    {
        qDebug()<<ui->tableWidget->cellWidget(i,0);
        if(ui->tableWidget->cellWidget(i,0)!=NULL)
        {
            QCheckBox *CheckBox = qobject_cast<QCheckBox*>(ui->tableWidget->cellWidget(i,0));
            qDebug()<<CheckBox->isChecked();
        }
    }

    /*QSqlDatabase DB=QSqlDatabase::database("MainDB");
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
    }*/
}
