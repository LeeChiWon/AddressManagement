#ifndef ADDRESSADDDIALOG_H
#define ADDRESSADDDIALOG_H

#include <QDialog>
#include "define.h"
#include "groupadddialog.h"

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

signals:
    void DBInit();
    void TreeWidgetInit();
    void TableWidgetUpdate();
    void ShowMessage(QString,int);

private slots:   
    void on_pushButton_Save_clicked();
    void on_pushButton_Cancel_clicked();
    void on_pushButton_GroupAdd_clicked();
    void ComboInit();

private:
    Ui::AddressAddDialog *ui;
    bool IsReject;

    void UIInit();
    void DBSave();
};

#endif // ADDRESSADDDIALOG_H
