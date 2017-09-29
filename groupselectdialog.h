#ifndef GROUPSELECTDIALOG_H
#define GROUPSELECTDIALOG_H

#include <QDialog>
#include "define.h"

namespace Ui {
class GroupSelectDialog;
}

class GroupSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GroupSelectDialog(QWidget *parent = 0);
    ~GroupSelectDialog();
    QString ComboSelected();
    void ComboInit(QStringList);

private:
    Ui::GroupSelectDialog *ui;


};

#endif // GROUPSELECTDIALOG_H
