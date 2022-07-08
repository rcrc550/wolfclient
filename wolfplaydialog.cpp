#include "wolfplaydialog.h"
#include "ui_wolfplaydialog.h"

WolfPlayDialog::WolfPlayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WolfPlayDialog)
{
    ui->setupUi(this);
}

WolfPlayDialog::~WolfPlayDialog()
{
    delete ui;
}

void WolfPlayDialog::on_pb_create_clicked()
{
    Q_EMIT SIG_Createe();

}


void WolfPlayDialog::on_pb_join_clicked()
{
     Q_EMIT SIG_Join();
}

void WolfPlayDialog::on_pb_start_clicked()
{
      Q_EMIT SIG_Match();
}


void WolfPlayDialog::on_pb_watch_clicked()
{
    Q_EMIT SIG_GetFriendRoom();
}

