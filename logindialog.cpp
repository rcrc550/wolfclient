#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("注册&&登录");
    ui->tw_page->setCurrentIndex(0);//第一时间显示登录
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::closeEvent(QCloseEvent *zevent)
{
    zevent->ignore();
    Q_EMIT  SIG_close();
}
#include<QRegExp>//正则表达式
#include<QMessageBox>


void LoginDialog::on_clear_zc_clicked()//清空注册
{
  ui->mm_edit_2zc->setText("");
  ui->mm_edit_zc->setText("");
  ui->tel_edit_zc->setText("");
  ui->nc_edit_zc->setText("");

}




void LoginDialog::on_commit_dl_clicked()//登录申请
{
    QString strtel=ui->tel_edit_dl->text();
    QString strpassword=ui->mm_edit_dl->text();
    //校验 1.手机号  正则表达式 清掉空格后不能是空字符串2.密码 长度不过20  （非空校验）
    QString tmptel=strtel, tmppassword=strpassword;
    if(tmptel.remove(" ").isEmpty()||tmppassword.remove(" ").isEmpty())
    {
      QMessageBox::about(this,"提示","手机密码不能为空");//调用静态方法来提示
      return;
    }

    QRegExp reg("^1[3-8][0-9]\{6,9\}$");
    bool res=reg.exactMatch(tmptel);
    if(!res)
    {
        QMessageBox::about(this,"提示","手机号格式错误");//调用静态方法来提示
        return;

    }
    if(tmppassword.length()>20)
    {
        QMessageBox::about(this,"提示","密码过长");//调用静态方法来提示
        return;
    }
    Q_EMIT SIG_login_commit(strtel,strpassword);
}

void LoginDialog::on_clear_dl_clicked()
{
    ui->mm_edit_dl->setText("");
    ui->tel_edit_dl->setText("");

}

void LoginDialog::on_commit_zc_clicked()
{
    QString strtel=ui->tel_edit_zc->text();
    QString strpassword2=ui->mm_edit_2zc->text();
    QString strpassword=ui->mm_edit_zc->text();
    QString strname=ui->nc_edit_zc->text();
    //校验 1.手机号  正则表达式 清掉空格后不能是空字符串2.密码 长度不过20  （非空校验）
    QString tmptel=strtel, tmppassword=strpassword,tmppassword2=strpassword2,strrname=strname;
    if(tmptel.remove(" ").isEmpty()||tmppassword.remove(" ").isEmpty()||tmppassword2.remove("").isEmpty()||strrname.remove("").isEmpty())
    {
      QMessageBox::about(this,"提示","手机密码昵称确认密码不能为空");//调用静态方法来提示
      return;
    }

    QRegExp reg("^1[3-8][0-9]\{6,9\}$");
    bool res=reg.exactMatch(tmptel);
    if(!res)
    {
        QMessageBox::about(this,"提示","手机号格式错误");//调用静态方法来提示
        return;

    }
    if(tmppassword.length()>20||strname.length()>10)
    {
        QMessageBox::about(this,"提示","密码或者名称过长");//调用静态方法来提示
        return;
    }
    if(tmppassword!=tmppassword2)
    {
        QMessageBox::about(this,"提示","两次密码不一致");//调用静态方法来提示
        return;
    }
    Q_EMIT SIG_rs_commit(strtel,strpassword,strname);
}

