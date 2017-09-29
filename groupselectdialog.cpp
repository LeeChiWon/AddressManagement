#include "groupselectdialog.h"
#include "ui_groupselectdialog.h"

GroupSelectDialog::GroupSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupSelectDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton_Ok,SIGNAL(clicked(bool)),this,SLOT(accept()));
}

GroupSelectDialog::~GroupSelectDialog()
{
    delete ui;
}

QString GroupSelectDialog::ComboSelected()
{
    return ui->comboBox_Group->currentText();
}

void GroupSelectDialog::ComboInit(QStringList List)
{
    ui->comboBox_Group->addItem("");

    foreach (QString ListName, List)
    {
        ui->comboBox_Group->addItem(ListName);
    }
}
