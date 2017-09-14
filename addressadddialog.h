#ifndef ADDRESSADDDIALOG_H
#define ADDRESSADDDIALOG_H

#include <QDialog>

namespace Ui {
class AddressAddDialog;
}

class AddressAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddressAddDialog(QWidget *parent = 0);
    ~AddressAddDialog();

private:
    Ui::AddressAddDialog *ui;
};

#endif // ADDRESSADDDIALOG_H
