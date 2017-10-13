#ifndef ADDRESSDOWNLOADDIALOG_H
#define ADDRESSDOWNLOADDIALOG_H

#include <QDialog>
#include "define.h"

namespace Ui {
class AddressDownloadDialog;
}

class AddressDownloadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddressDownloadDialog(QWidget *parent = 0);
    ~AddressDownloadDialog();
signals:
    void DBInit();
    void ShowMessage(QString,int);

private slots:
    void on_checkBox_All_clicked(bool checked);
    void on_pushButton_Add_clicked();
    void on_pushButton_Delete_clicked();
    void on_pushButton_Download_clicked();

private:
    Ui::AddressDownloadDialog *ui;

    void ListWidgetInit();
    QString ListSelect();
    int ListCount;
    QStringList ListNames;
};

#endif // ADDRESSDOWNLOADDIALOG_H
