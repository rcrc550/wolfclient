#include "roomdialog.h"
#include "ui_roomdialog.h"
#include<QMessageBox>
#include<QDebug>
#include<QThread>
roomDialog::roomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::roomDialog)
{
    ui->setupUi(this);
    m_mainlayout=new QVBoxLayout;
    m_mainlayout->setContentsMargins(0,0,0,0);
    m_mainlayout->setSpacing(5);
    //设置一个水平布局的画布，可以向这里边添加控件。
    ui->wdg_list->setLayout(m_mainlayout);
   /* for(int i=0;i<5;i++)
    {
    UserShow *ig=new UserShow;

    ig->set_info(i,QString("测试：%1:").arg(i));
    slot_addUserShow(ig);
    }*/
}

roomDialog::~roomDialog()
{
    delete ui;
}

void roomDialog::slot_setInfo(QString roomid)
{
    QString title=QString("房间号id:%1").arg(roomid);
    setWindowTitle(title);
    ui->lb_title->setText(title);

}

void roomDialog::slot_addUserShow(UserShow *user_show)
{
    m_mainlayout->addWidget(user_show);
    m_mapidToUserShow[user_show->m_id]=user_show;

}

void roomDialog::slot_removeUserShow(UserShow *user_show)
{
    user_show->hide();
    m_mainlayout->removeWidget(user_show);
}

void roomDialog::slot_removeUserShow(int id)
{
    if(m_mapidToUserShow.count(id)>0)
    {
        UserShow *user=m_mapidToUserShow[id];
        slot_removeUserShow(user);
    }
}

void roomDialog::slot_refreusershow(int id, QImage image)
{
    if(ui->wdg_usershow->m_id==id)
    {
        ui->wdg_usershow->slot_setImage(image);
    }
    if(m_mapidToUserShow.count(id)>0)
    {
        UserShow *p=m_mapidToUserShow[id];
        p->slot_setImage(image);
    }
}

void roomDialog::slot_clearUserShow()
{
    for(auto ite=m_mapidToUserShow.begin();ite!=m_mapidToUserShow.end();ite++)
    {
        slot_removeUserShow(ite->second);
    }
}
//退出房间
void roomDialog::on_pb_close_clicked()
{
    this->close();

}
//退出房间
void roomDialog::on_pb_quit_clicked()
{
    this->close();
}

void roomDialog::set_check(bool check)
{
    ui->cb_audio->setChecked(check);
    ui->cb_video->setChecked(check);
    ui->cb_desk->setChecked(check);;
}

void roomDialog::slot_setBigImageid(int id,QString username)
{
    ui->wdg_usershow->set_info(id,username);

}

void roomDialog::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::question(this,"退出提示","是否退出会议")==QMessageBox::Yes)
    {
        //发送退出房间信号
        Q_EMIT SIG_close();
        event->accept();
        return ;
    }
    event->ignore();

}
//开启或关闭音频
void roomDialog::on_cb_audio_clicked()
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

void roomDialog::on_cb_video_clicked()
{
    qDebug() << QThread::currentThreadId();
    if(ui->cb_video->isChecked())
    {

         ui->cb_desk->setChecked(false);
         Q_EMIT SIG_deskpause();
        Q_EMIT SIG_vediostart();


    }
    else
    {
        Q_EMIT SIG_vediopause();

    }
}


void roomDialog::on_cb_desk_clicked()
{
    if(ui->cb_desk->isChecked())
    {
        ui->cb_video->setChecked(false);
         Q_EMIT SIG_vediopause();
        Q_EMIT SIG_deskstart();


    }
    else
    {
        Q_EMIT SIG_deskpause();

    }

}


void roomDialog::on_cb_choice_currentIndexChanged(int index)
{
       Q_EMIT SIG_setMoji( index );
}

