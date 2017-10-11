#include "duplicationdialog.h"
#include "ui_duplicationdialog.h"

DuplicationDialog::DuplicationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DuplicationDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton_Clean,SIGNAL(clicked(bool)),this,SLOT(accept()));
    TableWidgetInit();
    DBShow();
    if(ui->tableWidget->rowCount()<1)
    {
        ui->pushButton_Clean->setEnabled(false);
    }
}

DuplicationDialog::~DuplicationDialog()
{
    delete ui;
}

void DuplicationDialog::TableWidgetInit()
{
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("Name")<<tr("PhoneNumber")<<tr("GroupName"));
    ui->tableWidget->setRowCount(0);
}

void DuplicationDialog::DBShow()
{
    TableWidgetInit();
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
        query.exec(QString("select name,phonenumber,grouping from address_management where phonenumber IN (select phonenumber from address_management group by phonenumber having count(*)>1) order by phonenumber asc"));
        qDebug()<<query.lastError();
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
        }
        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->resizeRowsToContents();
        DB.commit();
        DB.close();

    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void DuplicationDialog::DBDelete(QString QueryStr)
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
        qDebug()<<query.lastError()<<query.lastQuery();
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
            QRadioButton *RadioBox = qobject_cast<QRadioButton*>(ui->tableWidget->cellWidget(i,0));
            if(!RadioBox->isChecked())
            {
                DBDelete(QString("delete from address_management where name='%1' and phonenumber='%2'").arg(RadioBox->text(),ui->tableWidget->item(i,1)->text()));
            }
        }
    }
    QMessageBox::information(this,tr("Duplication Clean"),tr("Duplication address is Cleaned."),QMessageBox::Ok);
    this->close();
}
