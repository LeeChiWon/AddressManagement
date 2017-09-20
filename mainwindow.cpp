#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SettingInit();
    DBInit();
    TreeWidgetInit();
}

MainWindow::~MainWindow()
{    
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{    
    this->close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();

    int ret = QMessageBox::warning(this, tr("Application Exit"),
                                   tr("Do you want exit application?"),
                                   QMessageBox::Ok| QMessageBox::Cancel,
                                   QMessageBox::Ok);
    switch(ret)
    {
    case QMessageBox::Ok:       
        DBBackup();
        Setting->setValue("Configuration/Geometry",saveGeometry());       
        break;    
    }
    event->accept();
}

void MainWindow::SettingInit()
{
    Setting=new QSettings("EachOne","AddressManagement",this);
    restoreGeometry(Setting->value("Configuration/Geometry").toByteArray());
}

void MainWindow::DBBackup()
{
    QString FilePath=QApplication::applicationDirPath()+"/DBBackup";
    QDir Dir(FilePath);

    if(!Dir.exists())
    {
        Dir.mkdir(FilePath);
    }
    QFile File(QApplication::applicationDirPath()+QString("/DBBackup/%1.db").arg(QDate::currentDate().toString("yyyy-MM-dd")));

    if(File.exists())
    {
        File.remove();
    }
    QFile::copy(QApplication::applicationDirPath()+"/AddressManagement.db",
                QApplication::applicationDirPath()+QString("/DBBackup/%1.db").arg(QDate::currentDate().toString("yyyy-MM-dd")));
}

void MainWindow::TreeWidgetInit()
{
    ui->treeWidget->clear();
    QStringList TreeText=QStringList()<<tr("AllAddress")<<tr("NoNameAddress");
    QStringList GroupText;
    QTreeWidgetItem *item,*child;
    QSqlDatabase DB=QSqlDatabase::database("MainDB");

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            DBInit();
        }

        QSqlQuery query(DB);
        query.exec(QString("select * from group_management"));

        while(query.next())
        {
            GroupText.append(query.value("groupname").toString());
        }

        for(int i=0; i<TreeText.count(); i++)
        {
            item=new QTreeWidgetItem();
            item->setText(0,TreeText.at(i));
            ui->treeWidget->addTopLevelItem(item);
            if(i==ALL)
            {
                for(int j=0; j<GroupText.count(); j++)
                {
                    child=new QTreeWidgetItem();
                    child->setText(0,GroupText.at(j));
                    item->addChild(child);
                }
            }
        }
        ui->treeWidget->expandAll();
        DB.close();
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}
void MainWindow::on_actionAddressAdd_triggered()
{
    AddressAddDialog AddressAddDlg;
    connect(&AddressAddDlg,SIGNAL(DBInit()),this,SLOT(DBInit()));
    AddressAddDlg.exec();
}

void MainWindow::on_actionAddressDelete_triggered()
{

}

void MainWindow::on_actionGroupAdd_triggered()
{

}

void MainWindow::on_actionGroupDelete_triggered()
{

}

void MainWindow::on_actionUpload_triggered()
{

}

void MainWindow::on_actionDownload_triggered()
{

}

void MainWindow::on_actionDuplicateCheck_triggered()
{

}

void MainWindow::DBInit()
{
    QSqlDatabase DB=QSqlDatabase::addDatabase("QSQLITE","MainDB");
    DB.setDatabaseName(QApplication::applicationDirPath()+"/AddressManagement.db");

    try
    {
        if(!DB.open())
        {
            switch(DB.lastError().number())
            {
            case -1:
                QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database open error!"),tr("Database is not exist!")),QMessageBox::Ok);
                break;
            default:
                QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database open error!"),DB.lastError().text()),QMessageBox::Ok);
                break;
            }
            exit(0);
            QSqlDatabase::removeDatabase("MainDB");
            return;
        }

        QSqlQuery query(DB);
        query.exec(QString("create table if not exists address_management (name text, phonenumber text, phonenumber2 text, phonenumber3 text, email text, email2 text, email3 text"
                           "group text, companyname text, department text, position text, addresstype text, addressnumber text, address text, "
                           "addresstype2 text, addressnumber2 text, address2 text, addresstype3 text, addressnumber3 text, address3 text, memo text)"));
        query.exec(QString("create table if not exists group_management (groupname text not null primary key)"));
        DB.close();
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
        return;
    }
}
