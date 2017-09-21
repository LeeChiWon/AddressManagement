#include "groupadddialog.h"
#include "ui_groupadddialog.h"

GroupAddDialog::GroupAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupAddDialog)
{
    ui->setupUi(this);
}

GroupAddDialog::~GroupAddDialog()
{
    delete ui;
}

void GroupAddDialog::on_pushButton_clicked()
{
    QSqlDatabase DB=QSqlDatabase::database("MainDB");
    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            emit DBInit();
        }

        QSqlQuery query(DB);
        query.exec(QString("insert into group_management(groupname) select '%1' where not exists(select * from group_management where groupname='%1')").arg(ui->lineEdit->text()));

        DB.close();
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void GroupAddDialog::on_pushButton_Close_clicked()
{
    this->close();
}
