#ifndef DUPLICATIONDIALOG_H
#define DUPLICATIONDIALOG_H

#include <QDialog>
#include "define.h"

namespace Ui {
class DuplicationDialog;
}

class DuplicationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DuplicationDialog(QWidget *parent = 0);
    ~DuplicationDialog();
signals:
    void DBInit();

private slots:
    void on_pushButton_Clean_clicked();

private:
    Ui::DuplicationDialog *ui;

    void TableWidgetInit();
    void DBShow();    
    void DBDelete(QString);
};

#endif // DUPLICATIONDIALOG_H
