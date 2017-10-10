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

void GroupSelectDialog::LabelInit(QString String)
{
    ui->label->setVisible(false);
    ui->label_2->setText(String);
}

void GroupSelectDialog::LabelInit(QStringList List)
{
    QString Str;
    for(int i=0; i<List.count(); i++)
    {
        Str.append(List.at(i));
        if(i<List.count()-1)
        {
            Str.append("\n");
        }
    }
    ui->label->setVisible(true);
    ui->label_2->setText(Str);
}
