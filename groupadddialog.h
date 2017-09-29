#ifndef GROUPADDDIALOG_H
#define GROUPADDDIALOG_H

#include <QDialog>
#include "define.h"


namespace Ui {
class GroupAddDialog;
}

class GroupAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GroupAddDialog(QWidget *parent = 0);
    ~GroupAddDialog();

    bool Combobox;
    void MessageShow();

signals:
    void TreeWidgetInit();
    void DBInit();
    void ComboInit();

private slots:
    void on_pushButton_Close_clicked();
    void on_pushButton_Add_clicked();

private:
    Ui::GroupAddDialog *ui;

    void UIInit();
};

#endif // GROUPADDDIALOG_H
