#include "halldialog.h"
#include "ui_halldialog.h"

hallDialog::hallDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::hallDialog)
{
    ui->setupUi(this);
}

hallDialog::~hallDialog()
{
    delete ui;
}



void hallDialog::on_pb_friend_clicked()
{
    Q_EMIT SIG_friend_commit();
}


void hallDialog::on_pb_wolf_clicked()
{
     Q_EMIT SIG_wolf_commit();
}

