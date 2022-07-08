#include "wolfdialog.h"
#include "ui_wolfdialog.h"

WolfDialog::WolfDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WolfDialog)
{
    ui->setupUi(this);
    ready=0;
    idendity=0;
    start=0;//游戏开始的标志
    m_mainlayout=new QVBoxLayout;
    m_mainlayout->setContentsMargins(0,0,0,0);
    m_mainlayout->setSpacing(5);
    //设置一个水平布局的画布，可以向这里边添加控件。
      ui->wdg_list->setLayout(m_mainlayout);
      m_mainlayout2=new QVBoxLayout;
      m_mainlayout2->setContentsMargins(0,0,0,0);
      m_mainlayout2->setSpacing(5);
      //设置一个水平布局的画布，可以向这里边添加控件。
     ui->wdg_list_2->setLayout(m_mainlayout2);
     QString c="wowowo";
     savepeople=1;
     wich=0;
     prohit=0;




}
void WolfDialog::slot_addUserShow(UserShow *user_show)
{
    m_mainlayout->addWidget(user_show);
    m_mapidToUserShow[user_show->m_id]=user_show;

}
void WolfDialog::slot_setInfo(QString roomid)
{
    QString title=QString("房间号id:%1").arg(roomid);
    setWindowTitle(title);
    //ui->lb_title->setText(title);

}


void WolfDialog::slot_removeUserShow(UserShow *user_show)
{
    user_show->hide();
    m_mainlayout->removeWidget(user_show);
}

int WolfDialog::slot_removeUserShow(int id)
{
    int c=-1;
    if(m_mapidToUserShow.count(id)>0)
    {
        UserShow *user=m_mapidToUserShow[id];
        c=user->id;
        slot_removeUserShow(user);
    }
    return c;
}
void WolfDialog::slot_clearUserShow()
{
    for(auto ite=m_mapidToUserShow.begin();ite!=m_mapidToUserShow.end();ite++)
    {
        slot_removeUserShow(ite->second);
    }
}


void WolfDialog::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::question(this,"退出提示","是否退出狼人杀房间")==QMessageBox::Yes)
    {
        //发送退出房间信号
        Q_EMIT SIG_close();
        event->accept();
        return ;
    }
    event->ignore();
}

WolfDialog::~WolfDialog()
{
    delete ui;
}

void WolfDialog::on_pb_jump_clicked()
{
      if(start!=0)//在游戏开始以后
      {

      }
}

void WolfDialog::on_cb_audio_clicked()
{
    if(ui->cb_audio->isChecked())
    {
        Q_EMIT SIG_audiostart();


    }
    else
    {
        Q_EMIT SIG_audiopause();

    }
}


void WolfDialog::on_pb_ready_clicked()
{
    if(ready==0)
    {
     Q_EMIT SIG_READY();
    }
}

