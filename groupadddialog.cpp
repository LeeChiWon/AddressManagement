#include "groupadddialog.h"
#include "ui_groupadddialog.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
extern MainWindow *pMain;


GroupAddDialog::GroupAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupAddDialog),Combobox(false)
{
    ui->setupUi(this);
    connect(this,SIGNAL(DBInit()),pMain,SLOT(DBInit()));
    connect(this,SIGNAL(TreeWidgetInit()),pMain,SLOT(TreeWidgetInit()));
}

GroupAddDialog::~GroupAddDialog()
{
    delete ui;
}

void GroupAddDialog::on_pushButton_Close_clicked()
{
    if(Combobox)
    {
        emit ComboInit();
    }
    this->close();
}

void GroupAddDialog::on_pushButton_Add_clicked()
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

        QMessageBox::information(this,tr("Group Add"),QString("%1 %2").arg(ui->lineEdit->text(),tr(" is in Group.")));
        emit TreeWidgetInit();

        DB.close();
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}
