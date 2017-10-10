#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "define.h"
#include "addressadddialog.h"
#include "groupadddialog.h"
#include "tableprinter.h"
#include "addressdetaildialog.h"
#include "addressdownloaddialog.h"
#include "groupselectdialog.h"
#include "duplicationdialog.h"

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
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_lineEdit_Search_returnPressed();
    void sectionClicked(int column);
    void TableWidgetUpdate();
    void TableWidgetContextMenuShow(const QPoint &pos);
    void on_actionPrinter_triggered();
    void TableWidgetPrint(QPrinter*);
    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);
    void on_actionGroupMove_triggered();

private:
    QSettings *Setting;
    QMenu *TableWidgetContextMenu;

    void SettingInit();
    void DBBackup();
    void GroupDelete(int);
    void TableWidgetShow(QString);
    bool bSortChanged;
    void TableWidgetContextMenuInit();
    void DBDelete(QString,QString);
    void ExcelUpload(QString);
    void DBGroupMove(QString);
};

#endif // MAINWINDOW_H
