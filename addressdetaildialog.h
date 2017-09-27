#ifndef ADDRESSDETAILDIALOG_H
#define ADDRESSDETAILDIALOG_H

#include <QDialog>
#include "define.h"

namespace Ui {
class AddressDetailDialog;
}

class AddressDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddressDetailDialog(QWidget *parent = 0);
    ~AddressDetailDialog();
    void DBShow(QString);
signals:
    void DBInit();
    void TableWidgetUpdate();

private slots:
    void on_pushButton_Modify_clicked();
    void on_pushButton_Cancel_clicked();

private:
    Ui::AddressDetailDialog *ui;
    QString BeforeName,BeforePhoneNumber;

    void UIInit();
};

#endif // ADDRESSDETAILDIALOG_H
