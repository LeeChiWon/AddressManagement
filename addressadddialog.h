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
    void on_pushButton_PhoneNumberAdd_clicked();

    void on_pushButton_PhoneNumberDelete_clicked();

    void on_pushButton_EMailAdd_clicked();

    void on_pushButton_EMailDelete_clicked();

    void on_pushButton_AddressNumberAdd_clicked();

    void on_pushButton_AddressNumberDelete_clicked();

    void on_pushButton_Save_clicked();

    void on_pushButton_Cancel_clicked();

private:
    Ui::AddressAddDialog *ui;
    bool IsReject;
    int PhoneNumberCount;
    int EMailCount;
    int AddressNumberCount;

    void UIInit();
    int Calculation(int,int);
};

#endif // ADDRESSADDDIALOG_H
