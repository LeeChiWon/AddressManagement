#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "define.h"
#include "addressadddialog.h"
#include "groupadddialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionExit_triggered();
    void on_actionAddressAdd_triggered();
    void on_actionAddressDelete_triggered();
    void on_actionGroupAdd_triggered();
    void on_actionGroupDelete_triggered();
    void on_actionUpload_triggered();
    void on_actionDownload_triggered();
    void on_actionDuplicateCheck_triggered();
    void DBInit();
    void TreeWidgetInit();
    void TableWidgetInit();
    void on_pushButton_Search_clicked();

private:

    QSettings *Setting;

    void SettingInit();
    void DBBackup();
    void GroupDelete(int);
};

#endif // MAINWINDOW_H
