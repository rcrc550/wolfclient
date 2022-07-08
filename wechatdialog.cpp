#include "wechatdialog.h"
#include "ui_wechatdialog.h"

#include<QMessageBox>
#include<qdebug.h>


wechatDialog::wechatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::wechatDialog)
{
    ui->setupUi(this);
    setWindowTitle("交友空间");
}

wechatDialog::~wechatDialog()
{
    delete ui;
}

void wechatDialog::closeEvent(QCloseEvent *p)
{
    qDebug()<<__func__;
    if(QMessageBox::question(this,"提示","是否关闭")==QMessageBox::Yes)
    {
        Q_EMIT SIG_CLOSE(); //发送信号
        p->accept();
    }
    else
        p->ignore();//忽略
}

void wechatDialog::setinfo(QString name, int icon)
{
    //ui->lb_name->setText("name");

}


void wechatDialog::on_room_create_clicked()//创建会议
{
    Q_EMIT SIG_Roomcreate();
}

void wechatDialog::on_room_join_clicked()//加入会议
{
    Q_EMIT SIG_Roomjoin();
}


void wechatDialog::on_pb_add_clicked()
{
     Q_EMIT SIG_Addfriend();
}


void wechatDialog::on_pushButton_2_clicked()
{
    Q_EMIT SIG_ChooseAdress();
}


void wechatDialog::on_pushButton_clicked()
{
     Q_EMIT SIG_ChooseEnjoy();
}


void wechatDialog::on_room_no_clicked()
{
     Q_EMIT SIG_GetFriendList();
}


void wechatDialog::on_pushButton_4_clicked()
{
     Q_EMIT SIG_GetLikeFriendList();
}

