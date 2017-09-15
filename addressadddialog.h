#ifndef ADDRESSADDDIALOG_H
#define ADDRESSADDDIALOG_H

#include <QDialog>
#include "define.h"

namespace Ui {
class AddressAddDialog;
}

class AddressAddDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddressAddDialog(QWidget *parent = 0);
    ~AddressAddDialog();

public slots:
    void reject();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_lineEdit_CompanyDepartment_selectionChanged();

    void on_pushButton_AddressNumber_clicked();

private:
    Ui::AddressAddDialog *ui;
    void UIInit();
    bool IsReject;
};

#endif // ADDRESSADDDIALOG_H
