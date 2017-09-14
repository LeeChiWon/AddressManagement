#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    Setting->deleteLater();
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
       /* if(TrayIcon->isVisible())
        {
            TrayIcon->hide();
        }*/
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
void MainWindow::on_actionAddressAdd_triggered()
{

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
