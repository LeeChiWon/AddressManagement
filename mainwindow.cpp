#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow *pMain;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pMain=this;
    SettingInit();
    DBInit();
    TreeWidgetInit();
    TableWidgetInit();
    TableWidgetContextMenuInit();
    ui->lineEdit_Search->setPlaceholderText(tr("Address Search"));
    connect(ui->tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(sectionClicked(int)));
}

MainWindow::~MainWindow()
{    
    delete ui;
}

void MainWindow::sectionClicked(int column)
{
    ui->tableWidget->setSortingEnabled(true);

    if(!bSortChanged)
    {
        ui->tableWidget->sortByColumn(column,Qt::AscendingOrder);
        bSortChanged=true;
    }

    else
    {
        ui->tableWidget->sortByColumn(column,Qt::DescendingOrder);
        bSortChanged=false;
    }
    ui->tableWidget->setSortingEnabled(false);
}

void MainWindow::TableWidgetUpdate()
{
    TableWidgetShow(QString("select name, phonenumber, email, companyname, department, position from address_management where name=''"));
}

void MainWindow::TableWidgetContextMenuShow(const QPoint &pos)
{
    if(ui->tableWidget->rowCount()>0)
    {
        TableWidgetContextMenu->exec(ui->tableWidget->viewport()->mapToGlobal(pos));
    }
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
        event->accept();
        break;
    }    
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

void MainWindow::GroupDelete(int Select)
{
    QSqlDatabase DB=QSqlDatabase::database("MainDB");
    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            DBInit();
        }

        QSqlQuery query(DB);
        query.exec(QString("delete from group_management where groupname='%1'").arg(ui->treeWidget->currentItem()->text(0)));

        switch (Select)
        {
        case ALL_DELETE:
            query.exec(QString("delete from address_management where grouping='%1'").arg(ui->treeWidget->currentItem()->text(0)));
            QMessageBox::information(this,tr("Group Delete"),tr("Group & Includes Addresses is Deleted."),QMessageBox::Ok);
            break;
        case GROUP_DELETE:
            query.exec(QString("update address_management set grouping='' where grouping='%1'").arg(ui->treeWidget->currentItem()->text(0)));
            QMessageBox::information(this,tr("Group Delete"),tr("Group is Deleted."),QMessageBox::Ok);
            break;
        }

        TreeWidgetInit();
        DB.close();
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void MainWindow::TableWidgetShow(QString QueryStr)
{
    QSqlDatabase DB=QSqlDatabase::database("MainDB");
    TableWidgetInit();
    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            DBInit();
        }

        QSqlQuery query(DB);
        query.exec(QueryStr);

        while(query.next())
        {
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(query.value("name").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(query.value("phonenumber").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,new QTableWidgetItem(query.value("email").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,new QTableWidgetItem(query.value("companyname").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,4,new QTableWidgetItem(query.value("department").toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,5,new QTableWidgetItem(query.value("position").toString()));
        }

        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->resizeRowsToContents();

        DB.close();
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void MainWindow::TableWidgetContextMenuInit()
{
    connect(ui->tableWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(TableWidgetContextMenuShow(QPoint)));
    TableWidgetContextMenu=new QMenu();
    TableWidgetContextMenu->addAction(ui->actionAddressAdd);
    TableWidgetContextMenu->addAction(ui->actionAddressDelete);
    TableWidgetContextMenu->addAction(ui->actionPrinter);
}

void MainWindow::DBDelete(QString Name, QString PhoneNumber)
{
    QSqlDatabase DB=QSqlDatabase::database("MainDB");

    try
    {
        if(!DB.isOpen())
        {
            QSqlDatabase::removeDatabase("MainDB");
            DBInit();
        }

        QSqlQuery query(DB);
        query.exec(QString("delete from address_management where name='%1' and phonenumber='%2'").arg(Name,PhoneNumber));
        qDebug()<<query.lastQuery()<<query.lastError();
        DB.close();
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void MainWindow::TreeWidgetInit()
{
    ui->treeWidget->clear();
    ui->treeWidget->header()->setVisible(false);
    ui->treeWidget->setHeaderLabels(QStringList()<<tr("GroupName")<<tr("Count"));
    ui->treeWidget->setColumnWidth(1,5);
    QStringList TreeText=QStringList()<<tr("AllAddress")<<tr("NoNameAddress");
    QStringList GroupText;

    QTreeWidgetItem *item,*subitem;
    QBrush brush (Qt::red);
    QSqlDatabase DB=QSqlDatabase::database("MainDB");
    QMap<QString,int> GroupMap;
    QString Count;
    int AllCount,NoNameCount;
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


        query.exec(QString("select grouping, count(*) as count from address_management group by grouping"));

        while(query.next())
        {
            GroupMap.insert(query.value("grouping").toString(),query.value("count").toInt());
        }
        query.exec(QString("select sum(case when name not null then 1 end) as allname, sum(case when name=null then 1 end) as noname from address_management"));
        query.next();

        AllCount=query.value("allname").toInt();
        NoNameCount=query.value("noname").toInt();

        for(int i=0; i<TreeText.count(); i++)
        {
            item=new QTreeWidgetItem();
            item->setForeground( 1 , brush );
            switch(i)
            {
            case ALL:
                item->setText(0,TreeText.at(i));
                item->setText(1,QString::number(AllCount));

                for(int j=0; j<GroupText.count(); j++)
                {
                    Count=QString::number(GroupMap.value(GroupText.at(j)));
                    subitem=new QTreeWidgetItem();
                    subitem->setText(0,GroupText.at(j));
                    subitem->setText(1,Count);
                    subitem->setForeground( 1 , brush );
                    item->addChild(subitem);
                }
                break;
            case NONAME:
                item->setText(0,TreeText.at(i));
                item->setText(1,QString::number(NoNameCount));
                item->setForeground( 1 , brush );
                break;
            }
            ui->treeWidget->addTopLevelItem(item);
        }
        ui->treeWidget->expandAll();
        ui->treeWidget->resizeColumnToContents(0);
        DB.close();
    }
    catch(QException &e)
    {
        QMessageBox::warning(this,tr("Warning"),QString("%1\n%2").arg(tr("Database Error!"),e.what()),QMessageBox::Ok);
        QSqlDatabase::removeDatabase("MainDB");
    }
}

void MainWindow::TableWidgetInit()
{
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("Name")<<tr("PhoneNumber")<<tr("EMail")<<tr("Company")<<tr("Department")<<tr("Position"));
    ui->tableWidget->setRowCount(0);
}
void MainWindow::on_actionAddressAdd_triggered()
{
    AddressAddDialog AddressAddDlg;
    connect(&AddressAddDlg,SIGNAL(DBInit()),this,SLOT(DBInit()));
    connect(&AddressAddDlg,SIGNAL(TreeWidgetInit()),this,SLOT(TreeWidgetInit()));
    connect(&AddressAddDlg,SIGNAL(TableWidgetUpdate()),this,SLOT(TableWidgetUpdate()));
    AddressAddDlg.exec();
}

void MainWindow::on_actionAddressDelete_triggered()
{
    int ret = QMessageBox::warning(this, tr("Address Delete"),
                                   tr("Do you want delete address?"),
                                   QMessageBox::Ok| QMessageBox::Cancel,
                                   QMessageBox::Ok);
    switch(ret)
    {
    case QMessageBox::Ok:
        QModelIndexList indexes = ui->tableWidget->selectionModel()->selectedRows(0);
        QModelIndexList indexes2 = ui->tableWidget->selectionModel()->selectedRows(1);

        for (int i=indexes.count()-1; i>=0; i--)
        {
            DBDelete(indexes.at(i).data().toString(),indexes2.at(i).data().toString());
            ui->tableWidget->removeRow(indexes.at(i).row());
        }
        ui->tableWidget->resizeColumnsToContents();
        ui->tableWidget->resizeRowsToContents();
        TreeWidgetInit();
        break;
    }
}

void MainWindow::on_actionGroupAdd_triggered()
{
    GroupAddDialog GroupAddDlg;
    GroupAddDlg.exec();
}

void MainWindow::on_actionGroupDelete_triggered()
{
    if(!ui->treeWidget->currentItem()->text(0).isEmpty() && ui->treeWidget->currentItem()->text(0)!=tr("AllAddress") &&  ui->treeWidget->currentItem()->text(0)!=tr("NoNameAddress"))
    {
        int ret = QMessageBox::warning(this, tr("Group Delete"),
                                       tr("Do you want to delete the includes Addresses?\nYes=All, No=only Group, Cancel=Cancel"),
                                       QMessageBox::Yes| QMessageBox::No|QMessageBox::Cancel,
                                       QMessageBox::Yes);
        switch(ret)
        {
        case QMessageBox::Yes:
            GroupDelete(ALL_DELETE);
            break;
        case QMessageBox::No:
            GroupDelete(GROUP_DELETE);
            break;
        }
        TreeWidgetInit();
    }
}

void MainWindow::on_actionUpload_triggered()
{

}

void MainWindow::on_actionDownload_triggered()
{
    AddressDownloadDialog AddressDownloadDlg;
    AddressDownloadDlg.exec();
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
        query.exec(QString("create table if not exists address_management (name text, phonenumber text, phonenumber2 text, phonenumber3 text, email text, email2 text, email3 text,"
                           " grouping text, companyname text, department text, position text, addresstype text, addressnumber text, address text, "
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

void MainWindow::on_pushButton_Search_clicked()
{
    if(ui->lineEdit_Search->text().isEmpty())
    {
        QMessageBox::information(this,tr("Search"),tr("Input the search word."),QMessageBox::Ok);
        return;
    }

    TableWidgetShow(QString("select name, phonenumber, email, companyname, department, position from address_management where name like '\%%1\%' or phonenumber like '\%%1\%' "
                            "or email like '\%%1\%' or companyname like '\%%1\%' or department like '\%%1\%' or position like '\%%1\%'"
                            "or phonenumber2 like '\%%1\%' or phonenumber3 like '\%%1\%' or email2 like '\%%1\%' or email3 like '\%%1\%'").arg(ui->lineEdit_Search->text()));
    if(ui->tableWidget->rowCount()<=0)
    {
        QMessageBox::information(this,tr("Search Result"),tr("Search word is nothing."),QMessageBox::Ok);
    }
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item->text(0)==tr("AllAddress"))
    {
        TableWidgetShow(QString("select name, phonenumber, email, companyname, department, position from address_management where name not null"));
    }

    else if(item->text(0)==tr("NoNameAddress"))
    {
        TableWidgetShow(QString("select name, phonenumber, email, companyname, department, position from address_management where name=''"));
    }
    else
    {
        TableWidgetShow(QString("select name, phonenumber, email, companyname, department, position from address_management where grouping='%1'").arg(item->text(0)));
    }
}

void MainWindow::on_lineEdit_Search_returnPressed()
{
    on_pushButton_Search_clicked();
}

void MainWindow::on_actionPrinter_triggered()
{
    QPrintPreviewDialog dialog;
    dialog.setWindowTitle(tr("Preview"));
    connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(TableWidgetPrint(QPrinter*)));
    dialog.exec();
}

void MainWindow::TableWidgetPrint(QPrinter *printer)
{
    QPainter painter;
    if(!painter.begin(printer)) {
        QMessageBox::warning(this,tr("Warning"),tr("can't start printer"),QMessageBox::Ok);
        return;
    }
    // print table
    TablePrinter tablePrinter(&painter, printer);
    QVector<int> columnStretch = QVector<int>() << 3 << 3 << 5 << 3 << 3 << 3;
    if(!tablePrinter.printTable(ui->tableWidget->model(), columnStretch)) {
        QMessageBox::warning(this,tr("Error"),tablePrinter.lastError(),QMessageBox::Ok);
    }
    painter.end();
}

void MainWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    AddressDetailDialog AddressDetailDlg;
    connect(&AddressDetailDlg,SIGNAL(DBInit()),this,SLOT(DBInit()));
    connect(&AddressDetailDlg,SIGNAL(TableWidgetUpdate()),this,SLOT(TableWidgetUpdate()));
    AddressDetailDlg.DBShow(QString("select * from address_management where name='%1' and phonenumber='%2'").arg(ui->tableWidget->item(item->row(),0)->text(),ui->tableWidget->item(item->row(),1)->text()));
    AddressDetailDlg.exec();
}
