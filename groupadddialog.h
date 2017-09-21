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

signals:
    void TreeWidgetInit();
    void DBInit();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_Close_clicked();

private:
    Ui::GroupAddDialog *ui;
};

#endif // GROUPADDDIALOG_H
