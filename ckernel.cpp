#include "ckernel.h"
#include<qdebug.h>
#include<iostream>
#include"md5.h"
#include<QBuffer>
#include"audioread.h"
#define NetPackmap(a) m_netpackmap[a-DEF_PACK_BASE]
#define MD5_KET 1234
void waitSec(int s)
{
    QTime t;
    t.start();
    while(t.elapsed()<1000*s)
        QCoreApplication::processEvents();
}
void Ckernel::setNetpackmap()
{
    memset(m_netpackmap,0,sizeof(m_netpackmap));
    NetPackmap(DEF_PACK_LOGIN_RS)=&Ckernel::slot_dealloginRs;
    NetPackmap(DEF_PACK_REGISTER_RS)=&Ckernel::slot_dealRegisRs;
    NetPackmap(DEF_PACK_CREATEROOM_RS)=&Ckernel::slot_dealCreateRs;
    NetPackmap(DEF_PACK_JOINROOM_RS)=&Ckernel::slot_dealJoinRs;
    NetPackmap(DEF_PACK_ROOM_MEMBER)=&Ckernel::slot_dealRoomMember;
    NetPackmap(DEF_PACK_LEAVEROOM_RQ)=&Ckernel::slot_dealLeaveRs;
    NetPackmap(DEF_PACK_AUDIO_FRAME)=&Ckernel::slot_dealAudioFrame;
    NetPackmap(DEF_PACK_VIDEO_FRAME)=&Ckernel::slot_dealVideoFrameRq;
    NetPackmap(DEF_PACK_CREATEROOM_WOLF_RS)=&Ckernel::slot_dealWfCreate;
    NetPackmap(DEF_PACK_JOINROOM_WOLF_RS)=&Ckernel::slot_dealWolfJoinRs;
    NetPackmap(DEF_PACK_LEAVEWOLFROOM_RQ)=&Ckernel::slot_dealWolfLeaveRs;
    NetPackmap(DEF_PACK_WOLF_ROOM_MEMBER)=&Ckernel::slot_dealWolfMemberRs;
    NetPackmap(DEF_PACK_WOLF_START)=&Ckernel::slot_getIdendity;
    NetPackmap(DEF_PACK_DAY_DARK)=&Ckernel::slot_dealDaydark;
    NetPackmap(DEF_PACK_WOLF_KILL)=&Ckernel::slot_dealWolfKill;
    NetPackmap(DEF_PACK_WITCH_SAVE)=&Ckernel::slot_dealWitchSave;
    NetPackmap(DEF_PACK_DIE)=&Ckernel::slot_dealDie;
    NetPackmap(DEF_PACK_DEAL_WATCH)=&Ckernel::slot_dealWatch;
    NetPackmap(DEF_PACK_DEAL_WAKE)=&Ckernel::slot_dealWake;
    NetPackmap(DEF_PACK_SAYSOMETHING)=&Ckernel::slot_dealsaysm;
    NetPackmap(DEF_PACK_STOPSAY)=&Ckernel::slot_dealstopsay;
    NetPackmap(DEF_PACK_NORVOTE)=&Ckernel::slot_dealNormalnote;
    NetPackmap(DEF_PACK_GAMEOVER)=&Ckernel::dealGameover;
    NetPackmap(DEF_PACK_ADD_RESULT)=&Ckernel::dealaddresult;
    NetPackmap(DEF_PACK_GetListRS)=&Ckernel::getlistrs;
    NetPackmap(DEF_PACK_GetMemberLikeRS)=&Ckernel::getlikefriend;
    NetPackmap(DEF_PACK_SEEROOMRS)=&Ckernel::getdealseeroom;
}

#include<QInputDialog>
#include<QString>
#include<QMessageBox>
#include"QRegExp"
void Ckernel::slot_Roomjoin() //加入房间
{
    //判断是否在房间内 m_roomid
    if(m_roomid!=0||m_wolfrommid!=0)
    {
        QMessageBox::about(we_chat,"提示","已经在房间内,无法加入,请先退出");
        return;
    }
    //弹出窗口 填房间号
    QString strroom=QInputDialog::getText(we_chat,"加入房间","请输入房间号");


    // 合理化判断
    QRegExp reg("^[0-9]\{1,8\}$");
    if(!reg.exactMatch(strroom))
    {
        QMessageBox::about(we_chat,"提示","房间号不合理,长度不可超过8位数字");
        return;
    }
    qDebug()<<strroom;
    STRU_JOINROOM_RQ joinrq;
    joinrq.m_UserID=m_id;
    joinrq.m_RoomID=strroom.toInt();
    m_inet->SendData(0,(char*)&joinrq,sizeof(joinrq));
    m_roomdlg->set_check(false);
    //发命令 加入房间
}

void Ckernel::slot_friendlike()
{
    STRU_GetMemberLike rq;
    rq.m_id=m_id;
     m_inet->SendData(0,(char*)&rq,sizeof(rq));
}
void Ckernel::slot_WOLFRoomjoin() //加入房间
{
    //判断是否在房间内 m_roomid
    if(m_wolfrommid!=0||m_roomid!=0)
    {
        QMessageBox::about(we_chat,"提示","已经在房间内,无法加入,请先退出");
        return;
    }
    //弹出窗口 填房间号
    QString strroom=QInputDialog::getText(we_chat,"加入房间","请输入房间号");


    // 合理化判断
    QRegExp reg("^[0-9]\{1,8\}$");
    if(!reg.exactMatch(strroom))
    {
        QMessageBox::about(we_chat,"提示","房间号不合理,长度不可超过8位数字");
        return;
    }
    qDebug()<<strroom;
    STRU_JOINROOM_WOLF_RQ joinrq;
    joinrq.m_UserID=m_id;
    joinrq.m_RoomID=strroom.toInt();
    m_inet->SendData(0,(char*)&joinrq,sizeof(joinrq));

    //发命令 加入房间
}

void Ckernel::slot_readywolf()
{
    STRU_WOLF_READY rq;
    rq.m_id=m_id;
    rq.m_roomid=m_wolfrommid;
    m_inet->SendData(0,(char*)&rq,sizeof(rq));
}

void Ckernel::slot_addfriend()
{
    STRU_ADD_FRIEND rq;
    QString strmember=QInputDialog::getText(we_chat,"请选择","要加的好友账号");
    int id=strmember.toInt();
    rq.m_id=m_id;
    rq.m_bid=id;
    m_inet->SendData(0,(char*)&rq,sizeof(rq));


}

void Ckernel::getdealseeroom(uint sock, char *buf, int nlen)
{
    STRU_SEEROOMRS *rq=(STRU_SEEROOMRS*)buf;
    int roomid=rq->m_roomid;
    if(roomid==0)
    {
          QMessageBox::about(m_wolfplaydlg,"提示","此人不在房间中或者你没有该人好友");
          return ;
    }
    char arr[30];
    sprintf(arr,"%d",rq->m_roomid);
    string c=arr;
    const QString s=QString::fromStdString(c);
     const QString p="提示";
        QMessageBox::about(m_wolfplaydlg,p,s);
}

void Ckernel::slot_GetList()
{
    STRU_GetList rq;
    rq.m_id=m_id;
    m_inet->SendData(0,(char*)&rq,sizeof(rq));
}

void Ckernel::slot_Roomcreate()//创建房间
{
    //判断是否在房间内 m_roomid
    if(m_roomid!=0||m_wolfrommid!=0)
    {
        QMessageBox::about(we_chat,"提示","已经在房间内,无法创建,请先退出");
        return;
    }
    UserShow *user=new UserShow;
    connect(user,SIGNAL(SIG_itemCliked(int,QString)),m_roomdlg,SLOT(slot_setBigImageid(int,QString)));

    user->set_info(m_id,m_name);
    m_roomdlg->slot_addUserShow(user);

    //发命令 创建房间
    STRU_CREATEROOM_RQ roomrq;
    roomrq.m_UserID=m_id;
    m_inet->SendData(0,(char*)&roomrq,sizeof(roomrq));
     m_roomdlg->set_check(false);


}
void  Ckernel::slot_wolfroomcreate()
{
    if(m_wolfrommid!=0)
    {
        QMessageBox::about(m_wolfplaydlg,"提示","已经在房间内,无法创建,请先退出");
        return;
    }
//    UserShow *user=new UserShow;
//    user->set_info(m_id,m_name);
//    m_roomdlg->slot_addUserShow(user);
   //QMessageBox::about(m_wolfplaydlg,"提示","加入成功");
    //发命令 创建房间
    STRU_CRATEROOM_WOLF_RQ roomrq;
    roomrq.m_UserID=m_id;
    m_inet->SendData(0,(char*)&roomrq,sizeof(roomrq));

}


static std::string GetMD5STRING(QString ctr)
{
    QString str=QString("%1_%2").arg(ctr).arg(MD5_KET);
    std::string strsrc=str.toStdString();
    MD5 md5(strsrc);
    return md5.toString();
}
void Ckernel::slot_dealdata(uint sock, char *buf, int nlen)
{

    int type = *(int*)buf;
     if( (type >= DEF_PACK_BASE) && ( type < DEF_PACK_BASE + DEF_PACK_COUNT) )
       {
             PFUN pf = NetPackmap( type );
             if( pf )
             {
                 //(this->*pf)
                 (this->*pf)( sock, buf , nlen);
             }
     }
     delete []buf;
}
#include<QSettings>
#include<QCoreApplication>//核心应用头文件 exe目录
#include<QFileInfo>//判断一个文件是否存在使用文件信息
void Ckernel::Initconfig()
{
    strcpy( m_serverip , _DEF_SERVER_IP );
    //配置文件
    QString path = QCoreApplication::applicationDirPath() +"/config.ini";

    QFileInfo info(path);
    if( info.exists() ){
        //如果存在, 加载
        //打开配置文件
        QSettings setting(path , QSettings::IniFormat , NULL);

        //跳转到 组 net
        setting.beginGroup( "net");
        // 获取 ip对应的value值
        QVariant var = setting.value( "ip");
        QString strip = var.toString();
        if( !strip.isEmpty() )
            strcpy( m_serverip , strip.toStdString().c_str() );
        //跳出 Net
        setting.endGroup();

    }else{
        //如果不存在, 创建 并写入默认值
        QSettings setting(path , QSettings::IniFormat , NULL);

        //跳转到 组 net
        setting.beginGroup( "net");
        // 写入默认 ip
        setting.setValue( "ip" , QString::fromStdString( m_serverip ) );
        //跳出 Net
        setting.endGroup();
    }

    qDebug() << "server ip:"<<m_serverip;

}

void Ckernel::slot_dealloginRs(uint sock, char *buf, int nlen)
{
   qDebug()<<__func__;
   STRU_LOGIN_RS* rs = (STRU_LOGIN_RS*)buf;
   //根据返回结果, 得到不同信息
   switch( rs->m_lResult )
   {
     case userid_no_exist:
       QMessageBox::about(m_plogindlg,"提示","用户不存在，登陆失败");
       break;
     case password_error:
       QMessageBox::about(m_plogindlg,"提示","密码错误，登录失败");
       break;
      case 10:
       {
       QMessageBox::about(m_plogindlg,"提示","有人异地登录");
          slot_destory();
       }
     case login_sucess:
     {

       qDebug()<<rs->m_userID;
       m_id=rs->m_userID;
       //QString strName =QString("用户[%1]登录成功").arg( rs->m_szName );
       //QMessageBox::about(  m_plogindlg , "提示", strName );//模态对话框，必须要点不然阻塞
       m_name=QString::fromStdString(rs->m_szName);
       QMessageBox::about(m_plogindlg,"提示","登录成功");
       STRU_AUDIO_REGISTER rq_audio;
       rq_audio.m_userid = m_id;



       STRU_VIDEO_REGISTER rq_video;
       rq_video.m_userid = m_id;

       m_pAVClient[audio_client]->SendData(0,(char*)&rq_audio,sizeof(rq_audio));
       m_pAVClient[video_client]->SendData(0,(char*)&rq_video,sizeof(rq_video));

       we_chat->setinfo(m_name);

       m_timer->start(20);
       m_plogindlg->hide();
       m_hdlg->showNormal();
       //we_chat->showNormal();

       break;
     }
     default:
       break;

   }
}

void Ckernel::slot_dealRegisRs(uint sock, char *buf, int nlen)//处理服务器过来的注册回复
{
     STRU_REGISTER_RS *rsrs=(STRU_REGISTER_RS *)buf;
     switch(rsrs->m_lResult)
     {
      case userid_is_exist:
         QMessageBox::about(m_plogindlg,"提示","用户电话已经存在");
         break;
      case register_sucess:
          QMessageBox::about(m_plogindlg,"提示","注册成功");
         break;
      case name_is_exist:

          QMessageBox::about(m_plogindlg,"提示","昵称已存在");

         break;
     default:
         break;
     }

}
void Ckernel::slot_dealWolfJoinRs(uint sock, char *buf, int nlen)
{
    STRU_JOINROOM_WOLF_RS* rs = (STRU_JOINROOM_WOLF_RS*)buf;
    //房间号 显示到界面 跳转
    if(rs->m_lResult==0)
    {
       QMessageBox::about(m_plogindlg,"提示","加入房间失败,没有这个房间");
       return;
    }
    if(rs->m_lResult==-3)
    {
       QMessageBox::about(m_plogindlg,"提示","加入房间失败,游戏已开始");
       return;
    }
    if(rs->m_lResult==-2)
    {
       QMessageBox::about(m_plogindlg,"提示","加入房间失败,房间人物已经满");
       return;
    }
    if(rs->m_lResult==-4)
    {
       QMessageBox::about(m_plogindlg,"提示","加入房间失败,没有空闲房间");
       return;
    }
    QMessageBox::about(m_plogindlg,"提示","加入成功");
    m_wolfdog->savepeople=1;
    m_wolfdog->slot_setInfo( QString::number(rs->m_RoomID ) );


   //成功，跳转。
    //we_chat->hide();


    m_wolfrommid = rs->m_RoomID;
    m_wolfdog->showNormal();
}
void Ckernel::slot_dealJoinRs(uint sock, char *buf, int nlen)
{

    //拆包
    STRU_JOINROOM_RS* rs = (STRU_JOINROOM_RS*)buf;
    //房间号 显示到界面 跳转
    if(rs->m_lResult==0)
    {
       QMessageBox::about(m_plogindlg,"提示","加入房间失败,没有这个房间");
       return;
    }
    QMessageBox::about(m_plogindlg,"提示","加入成功");
    m_roomdlg->slot_setInfo( QString::number(rs->m_RoomID ) );


   //成功，跳转。
    //we_chat->hide();


    m_roomid = rs->m_RoomID;
    m_roomdlg->showNormal();

}

void Ckernel::slot_dealCreateRs(uint sock, char *buf, int nlen)
{

    //拆包
    STRU_CREATEROOM_RS* rs = (STRU_CREATEROOM_RS*)buf;
    //房间号 显示到界面 跳转
    if(rs->m_nType==0)
    {
       QMessageBox::about(m_plogindlg,"提示","创建失败");
       return;
    }
    m_roomdlg->slot_setInfo( QString::number(rs->m_RoomID ) );
    //服务器没有把个人信息发送给你, 作为第一个进入房间的

    //把自己的信息放到房间里 作显示 todo
    //we_chat->hide();


    m_roomid = rs->m_RoomID;
    m_roomdlg->showNormal();
}
//处理离开的请求
void Ckernel::slot_dealLeaveRs(uint sock, char *buf, int nlen)
{
    STRU_LEAVEROOM_RQ *rs=(STRU_LEAVEROOM_RQ *)buf;
    if(rs->m_RoomId==m_roomid)
    {
      m_roomdlg->slot_removeUserShow(rs->m_nUserId);
    }
    //去掉对应的音频
    if(m_mapidToAudiowrite.count(rs->m_nUserId)>0)
    {
        AudioWrite *wr=m_mapidToAudiowrite[rs->m_nUserId];
        delete wr;
        m_mapidToAudiowrite[rs->m_nUserId]=NULL;
        m_mapidToAudiowrite.erase(rs->m_nUserId);



    }

}

void Ckernel::slot_dealWolfLeaveRs(uint sock, char *buf, int nlen)
{
    STRU_LEAVEWOLFROOM_RQ *rs=(STRU_LEAVEWOLFROOM_RQ *)buf;
    //QMessageBox::about(m_plogindlg,"提示","离开房间");

    if(rs->m_RoomId==m_wolfrommid)
    {
      int c;
      c=m_wolfdog->slot_removeUserShow(rs->m_nUserId);
      if(c!=-1)
      {
          idstore.erase(c);
      }
    }
    wolf_audio=0;
    for(int i=0;i<m_wolfdog->myvec.size();i++)
    {
        QWidget*l=m_wolfdog->myvec[i];
        m_wolfdog->m_mainlayout2->removeWidget(l);
        delete l;
    }
    //去掉对应的音频
    if(m_mapidToWolfAudiowrite.count(rs->m_nUserId)>0)
    {
        AudioWrite *wr=m_mapidToWolfAudiowrite[rs->m_nUserId];
        delete wr;
        m_mapidToWolfAudiowrite[rs->m_nUserId]=NULL;
        m_mapidToWolfAudiowrite.erase(rs->m_nUserId);
        wolfmembername.erase(rs->m_nUserId);


    }


    //去掉对应的音频


}
//处理音频数据
void Ckernel::slot_dealAudioFrame(uint sock, char *buf, int nlen)
{
    //反序列化 读取数据到音频设备中

    //拆包
    //音频帧
    /// int type;
    /// int userId;
    /// int roomId;
    /// int min;
    /// int sec;
    /// int msec;
    /// QByteArray audioFrame;

    //反序列化
    char* tmp = buf;
    int userId ;
    int roomId ;

    tmp += sizeof(int);

    userId = *(int*)tmp; //按照整型取
    tmp += sizeof(int);

    roomId = *(int*)tmp ;
    tmp += sizeof(int);

    //跳过时间
    tmp += sizeof(int);

    tmp += sizeof(int);

    tmp += sizeof(int);

    int nbufLen = nlen - 6*sizeof(int);
    QByteArray ba( tmp , nbufLen);
    if(m_roomid==roomId)
    {
        if(m_mapidToAudiowrite.count(userId)>0)
        {
            AudioWrite *wr=m_mapidToAudiowrite[userId];

            wr->slot_playAudio(ba);
        }
    }
    else if(roomId==m_wolfrommid)
    {
        qDebug()<<"come on";
        if(m_mapidToWolfAudiowrite.count(userId)>0)
        {
            AudioWrite *wr=m_mapidToWolfAudiowrite[userId];

            wr->slot_playAudio(ba);
        }
    }


}
void Ckernel::slot_dealWfCreate(uint sock, char *buf, int nlen)
{

            if(m_wolfrommid!=0||m_roomid!=0)
            {
                QMessageBox::about(we_chat,"提示","已经在房间内,无法加入,请先退出");
                return;
            }
            //拆包
            STRU_CREATEROOM_WOLF_RS* rs = (STRU_CREATEROOM_WOLF_RS*)buf;
            //房间号 显示到界面 跳转
            if(rs->m_nType==0)
            {
               QMessageBox::about(m_plogindlg,"提示","创建失败,你已经在房间中");
               return;
            }
            m_wolfdog->slot_setInfo( QString::number(rs->m_RoomID ) );
            //服务器没有把个人信息发送给你, 作为第一个进入房间的
             //m_wolfdog
            //把自己的信息放到房间里 作显示 todo
            //we_chat->hide();
//             UserShow *wo=new UserShow;
//             wo->set_info(m_id,m_name);
//             m_wolfdog->slot_addUserShow(wo);

            m_wolfrommid = rs->m_RoomID;
           // qDebug("%d\n",m_wolfrommid);
            m_wolfdog->showNormal();
}

void Ckernel::slot_dealRoomMember(uint sock, char *buf, int nlen)
{
    STRU_ROOM_MEMBER_RQ *rq= (STRU_ROOM_MEMBER_RQ *)buf;
    UserShow *u_ser=new UserShow;
     connect(u_ser,SIGNAL(SIG_itemCliked(int,QString)),m_roomdlg,SLOT(slot_setBigImageid(int,QString)));
    u_ser->set_info(rq->m_UserID,QString::fromStdString(rq->m_szUser));
    m_roomdlg->slot_addUserShow(u_ser);
    if(m_mapidToAudiowrite.count(rq->m_UserID)==0)
    {
        //为每个人创建一个播放音频的对象
        AudioWrite *aut=new AudioWrite;
        m_mapidToAudiowrite[rq->m_UserID]=aut;
    }

}
void Ckernel::slot_dealWolfMemberRs(uint sock, char *buf, int nlen)
{
    STRU_ROOM_WOLF_MEMBER_RQ *rq= (STRU_ROOM_WOLF_MEMBER_RQ *)buf;
    UserShow *u_ser=new UserShow;
    QString l=QString::fromStdString(rq->m_szUser);
    //idstore.size();
    int size;
    while(1)
    {
        size=rand()%20;
        if(idstore.count(size)==0)
        {
            idstore[size]=rq->m_UserID;
            break;
        }
    }
    QString c=QString::number(size);
    QString p="号：";
    QString k=QString::fromStdString(rq->m_szUser);
    c.append(p);
    c.append(k);
     wolfmembername[rq->m_UserID]=rq->m_szUser;
    u_ser->set_info(rq->m_UserID,c);
    u_ser->id=size;
    m_wolfdog->slot_addUserShow(u_ser);
    if(m_mapidToAudiowrite.count(rq->m_UserID)==0)
    {
        //为每个人创建一个播放音频的对象
        AudioWrite *aut=new AudioWrite;
        m_mapidToWolfAudiowrite[rq->m_UserID]=aut;
        std::string s;
        s=rq->m_szUser;


    }
}

void Ckernel::slot_getIdendity(uint sock, char *buf, int nlen)
{
    qDebug()<<"get iden";
    //QMessageBox::about(m_wolfdog,"提示","你是狼人");
    STRU_ROOM_START_RQ *rq=(STRU_ROOM_START_RQ*)buf;
    m_wolfdog->idendity=rq->m_idtype;
    std::string c;
    if(rq->m_idtype==1)
    {
       c="你,";
       for(int i=0;i<3;i++)
       {
          std::string  k=wolfmembername[rq->wolfmember[i]];
          c+=k;
          c+="_";
       }
       c+="狼人";
    }
    else if(rq->m_idtype==2)
    {
          c="你是预言家";
          m_wolfdog->prohit=1;
    }
        else if(rq->m_idtype==3)
    {
         c="你是村民";
    }
     else if(rq->m_idtype==4)
    {
        c="你是女巫";
        m_wolfdog->wich=1;
    }
    else if(rq->m_idtype==5)
   {
          c="你是猎人";
   }
    QString f=QString::fromStdString(c);
    QWidget *l=new QLabel(f);
     m_wolfdog->myvec.push_back(l);
    m_wolfdog->m_mainlayout2->addWidget(l);
}

void Ckernel::slot_dealDaydark(uint sock, char *buf, int nlen)
{
    STRU_DAY_DARK*rq=(STRU_DAY_DARK*)buf;
    if(rq->m_id==m_id)
    {
        if(rq->m_roomid==m_wolfrommid)
        {
            if(m_wolfdog->idendity!=Wolf)
            wolf_audio=1;
            else
              wolf_audio=0;
        }
    }
    QMessageBox::about(m_wolfdog,"提示","天黑了");
    if( m_wolfdog->wich>0)
    {
    if(m_wolfdog->savepeople>0)
    {
    QString ff="你要是使用毒药么";

     const QString qq=ff;
     const QString ll="提示";
    if(QMessageBox::question(m_wolfdog,ll,qq)==QMessageBox::Yes)
    {
            m_wolfdog->savepeople=0;//毒药数量
            STRU_WITCH_KILL rq;
            rq.m_roomid=m_wolfrommid;
            QString strmember=QInputDialog::getText(m_wolfdog,"请选择","要毒杀的人的号码");
            int id=strmember.toInt();
            int number_id=0;
            if(idstore.count(id)>0)
            number_id=idstore[id];
            rq.m_id=number_id;
             m_inet->SendData(0,(char *)&rq,sizeof(rq));

    }

    }
    }
    else if( m_wolfdog->prohit>0)
    {



              STRU_PROHIT_WATCH  rq;
            rq.m_roomid=m_wolfrommid;
            QString strmember=QInputDialog::getText(m_wolfdog,"请选择","要查验的号码");
            int id=strmember.toInt();
            int number_id=0;
            if(idstore.count(id)>0)
            number_id=idstore[id];
            rq.m_id=number_id;
             m_inet->SendData(0,(char *)&rq,sizeof(rq));



    }

}

void Ckernel::slot_dealWolfKill(uint sock, char *buf, int nlen)
{
    //QMessageBox::about(m_wolfdog,"提示","选人");
     QString strmember=QInputDialog::getText(m_wolfdog,"请选择","要杀的人的号码");
     int id=strmember.toInt();
     STRU_KILL_VOTE rq;
     int number_id=30;
     if(idstore.count(id)>0)
     number_id=idstore[id];
     rq.m_id=number_id;
     rq.m_roomid=m_wolfrommid;
     m_inet->SendData(0,(char *)&rq,sizeof(rq));

}

void Ckernel::slot_dealWitchSave(uint sock, char *buf, int nlen)
{
    STRU_SWTICH_SAVE*rq=(STRU_SWTICH_SAVE*)buf;
    if(rq->m_roomid==m_wolfrommid)
    {
        std::string s;
        if(rq->m_id!=m_id)
        {

         s=wolfmembername[rq->m_id];
        }
        else
        {
            s="你";
        }
          const QString c="提示";
         QString f=QString::fromStdString(s);
         f.append("要死了，你要救他么");
         const QString q=f;
         qDebug()<<"nw save";
          STRU_SAVE_CHOOSE rs;
         if(QMessageBox::question(m_wolfdog,c,q)==QMessageBox::Yes)
         {

             rs.result=1;
             rs.m_roomid=m_wolfrommid;
         }
         else
         {

              rs.m_roomid=m_wolfrommid;
         }
          m_inet->SendData(0,(char *)&rs,sizeof(rs));

    }
}

void Ckernel::slot_dealDie(uint sock, char *buf, int nlen)
{
    STRU_DIE *rq=(STRU_DIE*)buf;
    if(rq->m_roomid==m_wolfrommid)
    {
      std::string s=wolfmembername[rq->m_id];
      if(m_id==rq->m_id)
      {
          s="你死亡";
          wolf_audio=1;
          m_wolfdog->prohit=0;
          m_wolfdog->wich=0;
      }
      else
      {
      s=wolfmembername[rq->m_id];
      s+="玩家死亡";
      }
       QWidget *l=new QLabel(QString::fromStdString(s));
        m_wolfdog->myvec.push_back(l);
       m_wolfdog->m_mainlayout2->addWidget(l);
    }
}

void Ckernel::slot_dealWatch(uint sock, char *buf, int nlen)
{
   STRU_DEAL_WATCH*rq=(STRU_DEAL_WATCH*)buf;
   if(rq->m_result==1)
       QMessageBox::about(m_wolfdog,"提示","他是狼人");
   else if(rq->m_result==2)
       QMessageBox::about(m_wolfdog,"提示","他是好人");
   else
       QMessageBox::about(m_wolfdog,"提示","没查到这个人");
}

void Ckernel::slot_dealWake(uint sock, char *buf, int nlen)
{
    STRU_WAKE*rq=(STRU_WAKE*)buf;
    if(rq->m_roomid==m_wolfrommid)
    {
        QMessageBox::about(m_wolfdog,"提示","天亮了");
    }
}

void Ckernel::slot_dealsaysm(uint sock, char *buf, int nlen)
{
    STRU_SAY*rq=(STRU_SAY*)buf;
    if(rq->m_roomid==m_wolfrommid)
    {
        if(rq->m_id==m_id)
        {
          wolf_audio=0;
          QMessageBox::about(m_wolfdog,"提示","开始发言");
        }
        else
        {
            std::string str=wolfmembername[rq->m_id];
            str+="开始发言";
            QString f=QString::fromStdString(str);
             QWidget *l=new QLabel(f);
             m_wolfdog->myvec.push_back(l);
            m_wolfdog->m_mainlayout2->addWidget(l);

        }

    }
}

void Ckernel::slot_dealstopsay(uint sock, char *buf, int nlen)
{
    STRU_STOPSAY*rq=(STRU_STOPSAY*)buf;
    if(rq->m_roomid==m_wolfrommid)
    {
        wolf_audio=1;
        QMessageBox::about(m_wolfdog,"提示","停止发言");
    }

}

void Ckernel::slot_dealNormalnote(uint sock, char *buf, int nlen)
{
    STRU_NORVOTE*rq=(STRU_NORVOTE*)buf;
    if(rq->m_roomid==m_wolfrommid)
    {
        QString strmember=QInputDialog::getText(m_wolfdog,"请选择","要投票的人的号码");
        int id=strmember.toInt();
        STRU_VOTE_WHO rq;
        int number_id=30;
        if(idstore.count(id)>0)
        number_id=idstore[id];
        rq.m_id=number_id;
        rq.m_roomid=m_wolfrommid;
        m_inet->SendData(0,(char *)&rq,sizeof(rq));
    }

}

void Ckernel::dealGameover(uint sock, char *buf, int nlen)
{
    STRU_GAMEOVER*rq=(STRU_GAMEOVER*)buf;
     wolf_audio=0;
    if(rq->result==1)
     QMessageBox::about(m_wolfdog,"提示","游戏结束好人胜利");
    else if(rq->result==2)
     QMessageBox::about(m_wolfdog,"提示","游戏结束狼人胜利");
    m_wolfdog->wich=0;
    m_wolfdog->prohit=0;
}

void Ckernel::dealaddresult(uint sock, char *buf, int nlen)
{
    STRU_ADD_RESULT *rs=(STRU_ADD_RESULT*)buf;
    if(rs->m_result==-1)
        {
          QMessageBox::about(we_chat,"提示","没有这个账号，请重试");
          return;
        }
    else if(rs->m_result==-2)
        {
          QMessageBox::about(we_chat,"提示","你已经有这个好友了");
          return;
        }
    else if(rs->m_result==-1)
        {
          QMessageBox::about(we_chat,"提示","添加成功");
          return;
        }


}

void Ckernel::getlistrs(uint sock, char *buf, int nlen)
{
    STRU_GetListRs*rq=(STRU_GetListRs*)buf;



     for(int i=0;i<rq->size;i++)
     {
         char ar[50];
          sprintf(ar,"%d",rq->m_id[i]);
          string s;
          s=ar;
          QString str=QString::fromStdString(s);
          UserShow *u_ser=new UserShow;
          u_ser->set_info(rq->m_id[i],str);
          friendlist->slot_addUserShow(u_ser);
     }
     friendlist->showNormal();

}

void Ckernel::getlikefriend(uint sock, char *buf, int nlen)
{
    STRU_GetMembero*rq=(STRU_GetMembero*)buf;
    if(rq->m_id[0]==0)
    {
          QMessageBox::about(we_chat,"提示","没有类似的人");
          return ;
    }
    int i=0;
   while(rq->m_id[i]!=0&&i<50)
   {
        char ar[50];
         sprintf(ar,"%d",rq->m_id[i]);
         string s;
         s=ar;
         QString str=QString::fromStdString(s);
         UserShow *u_ser=new UserShow;
         u_ser->set_info(rq->m_id[i],str);
         friendlikelist->slot_addUserShow(u_ser);
         i++;
    }
    friendlikelist->showNormal();

}
//退出房间
void Ckernel::slot_roomquit()
{

    //发退出包
    STRU_LEAVEROOM_RQ rq;
    rq.m_nUserId=m_id;
    rq.m_RoomId=m_roomid;
    m_roomid=0;
    std::string name=m_name.toStdString();
    strcpy(rq.szUserName,name.c_str());
    //音频视频关掉  todo
    m_paudioread->pause();
     m_screenread->slot_closeVideo();
    m_roomdlg->set_check(false);

    for(auto ite=m_mapidToAudiowrite.begin();ite!=m_mapidToAudiowrite.end();)
    {
        AudioWrite *pwrite=ite->second;
        delete pwrite;
        ite->second=NULL;
        ite=m_mapidToAudiowrite.erase(ite);

    }



    m_inet->SendData(0,(char *)&rq,sizeof(rq));

    //回收资源
    m_roomdlg->slot_clearUserShow();
}

void Ckernel::slot_wolfmatch()
{
      if(m_wolfrommid!=0)
      {
            QMessageBox::about(m_wolfplaydlg,"提示","您已在房间中不可再进行匹配");
      }

       STRU_MATCH rs;
       rs.m_id=m_id;
       m_inet->SendData(0,(char *)&rs,sizeof(rs));


}
void Ckernel::slot_wolfroomquit()
{
    STRU_LEAVEWOLFROOM_RQ rq;
    rq.m_nUserId=m_id;
    rq.m_RoomId=m_wolfrommid;
    m_wolfrommid=0;
    std::string name=m_name.toStdString();
    strcpy(rq.szUserName,name.c_str());
    //音频视频关掉  todo
    for(auto ite=m_mapidToAudiowrite.begin();ite!=m_mapidToAudiowrite.end();)
    {
        AudioWrite *pwrite=ite->second;
        delete pwrite;
        ite->second=NULL;
        ite=m_mapidToAudiowrite.erase(ite);

    }
     m_inet->SendData(0,(char *)&rq,sizeof(rq));

     //回收资源
     m_wolfdog->slot_clearUserShow();
}

void Ckernel::slot_wolfroom2quit()
{
    friendlist->slot_clearUserShow();
}

void Ckernel::slot_wolfroom3quit()
{
    friendlikelist->slot_clearUserShow();
}
void Ckernel::slot_startAudio()
{
    m_paudioread->start();
}
void Ckernel::slot_startAudio2()
{
    m_paudioread->start();
}


void Ckernel::slot_pauseAudio()
{
    m_paudioread->pause();
}

void Ckernel::slot_wolfgetfriendroom()
{
      QString strmember=QInputDialog::getText(m_wolfplaydlg,"请写出","你要查看好友的账号");
      STRU_SEEROOM rq;
      rq.m_id=strmember.toInt();
        m_inet->SendData(0,(char*)&rq,sizeof(rq));
}

void Ckernel::slot_ChooseEnjoy()
{
    STRU_CHOOSE_ADDRESS rq;
    QString strmember=QInputDialog::getText(we_chat,"请写出","你的爱好");
    std::string str=strmember.toStdString();
    char arr[MAX_SIZE];
    strcpy( rq.m_address,str.c_str());

    rq.m_id=m_id;
    m_inet->SendData(0,(char*)&rq,sizeof(rq));
}
//发送音频帧
void Ckernel::slot_audioFrame(QByteArray ba)
{
    int nPackSize = 6*sizeof(int) + ba.size();
    char* buf = new char[nPackSize];
    char * tmp = buf;
    //序列化
    int type = DEF_PACK_AUDIO_FRAME;

    int userId = m_id;
    int roomId = m_roomid;
    QTime tm = QTime::currentTime();
    int min = tm.minute();
    int sec = tm.second();
    int msec = tm.msec();

    *(int*)tmp = type; ////按照整型存
    tmp += sizeof(int);

    *(int*)tmp = userId;
    tmp += sizeof(int);
    if(m_roomid!=0)
    {
     *(int*)tmp = m_roomid;
    }
    else if(m_wolfrommid!=0)
    {
        *(int*)tmp = m_wolfrommid;
        qDebug()<<"send";
    }
    else
    {
       qDebug()<<"no send";
    }

    tmp += sizeof(int);

    *(int*)tmp = min;
    tmp += sizeof(int);

    *(int*)tmp = sec;
    tmp += sizeof(int);

    *(int*)tmp = msec;
    tmp += sizeof(int);

    memcpy( tmp , ba.data() , ba.size() );


    //Q_EMIT SIG_sendaudio(buf,len);
    if(wolf_audio==0)
    m_pAVClient[audio_client]->SendData(0,buf,nPackSize);
    delete []buf;




}
///视频数据帧
/// 成员描述
/// int type;
/// int userId;
/// int roomId;
/// int min;
/// int sec;
/// int msec;
/// QByteArray videoFrame;
///
//发送视频帧
void Ckernel::slot_senvediodata(QImage image)
{
    //先显示自己
    slot_refreshvideo(m_id,image);
    // 压缩
    //压缩图片从RGB24格式压缩到JPEG格式, 发送出去
    QByteArray ba;
    QBuffer qbuf(&ba); // QBuffer 与 QByteArray 字节数组联立联系

    image.save( &qbuf , "JPEG" , 50 );  //将图片的数据写入 ba
    //使用ba对象, 可以获取图片对应的缓冲区

    //可以使用ba.data() , ba.size()将缓冲区发送出去

    // 写视频帧 发送
    ///视频数据帧
    /// 成员描述
    /// int type;
    /// int userId;
    /// int roomId;
    /// int min;
    /// int sec;
    /// int msec;
    /// QByteArray videoFrame;
    int nPackSize = 6*sizeof(int) + ba.size();
    char * buf = new char[nPackSize];
    char* tmp = buf;

    *(int*)tmp = DEF_PACK_VIDEO_FRAME;
    tmp +=sizeof(int);
    *(int*)tmp = m_id;
    tmp +=sizeof(int);
    if(m_roomid!=0)
    {
     *(int*)tmp = m_roomid;
    }
    else if(m_wolfrommid!=0)
    {
        *(int*)tmp = m_wolfrommid;
    }
    else
    {
        QMessageBox::about(we_chat,"提示","你当前不在房间内无法发送音频");
        return;
    }
    tmp +=sizeof(int);

    //用于延迟过久舍弃一些帧的参考时间
    QTime tm = QTime::currentTime();
    *(int*)tmp = tm.minute();
    tmp +=sizeof(int);
    *(int*)tmp = tm.second();
    tmp +=sizeof(int);
    *(int*)tmp = tm.msec();
    tmp +=sizeof(int);


    memcpy( tmp , ba.data() , ba.size() );
    //发送是一个阻塞函数如果服务器接受缓冲区由于数据量大，没能及时取走缓冲区数据 滑动窗口变小send函数阻塞，使得用户界面卡顿，出现未响应问题

    //m_inet->SendData( 0 , buf , nPackSize );
   // delete []buf;
    Q_EMIT SIG_sendvedio(buf,nPackSize);

}

void Ckernel::slot_vediopause()
{
    m_vedioread->slot_closeVideo();
}

void Ckernel::slot_chooseaddress()
{
      STRU_CHOOSE_ADDRESS rq;
      QString strmember=QInputDialog::getText(we_chat,"请写出","居住的地址");
      std::string str=strmember.toStdString();
      char arr[MAX_SIZE];
      strcpy(rq.m_address,str.c_str());

      rq.m_id=m_id;
      m_inet->SendData(0,(char*)&rq,sizeof(rq));
}

void Ckernel::slot_vediostart()
{
    m_vedioread->slot_openVideo();
}

void Ckernel::slot_refreshvideo(int id,QImage image)
{

    m_roomdlg->slot_refreusershow(id, image);


}

void Ckernel::slot_deskstart()
{

    m_screenread->slot_openVideo();

}

void Ckernel::slot_deskpause()
{
    m_screenread->slot_closeVideo();

}
//这个是在一个线程里发送的，上个信号发过来线程会一直处理然后再处理第二个信号，在上一个信号中阻塞太久就丢包。
void Ckernel::slot_sendScreenFrame(QImage image)
{
    //先显示自己

        //先显示自己
        slot_refreshvideo(m_id,image);
        // 压缩
        //压缩图片从RGB24格式压缩到JPEG格式, 发送出去
        QByteArray ba;
        QBuffer qbuf(&ba); // QBuffer 与 QByteArray 字节数组联立联系

        image.save( &qbuf , "JPEG" , 50 );  //将图片的数据写入 ba
        //使用ba对象, 可以获取图片对应的缓冲区

        //可以使用ba.data() , ba.size()将缓冲区发送出去

        // 写视频帧 发送
        ///视频数据帧
        /// 成员描述
        /// int type;
        /// int userId;
        /// int roomId;
        /// int min;
        /// int sec;
        /// int msec;
        /// QByteArray videoFrame;
        int nPackSize = 6*sizeof(int) + ba.size();
        char * buf = new char[nPackSize];
        char* tmp = buf;

        *(int*)tmp = DEF_PACK_VIDEO_FRAME;
        tmp +=sizeof(int);
        *(int*)tmp = m_id;
        tmp +=sizeof(int);
        *(int*)tmp = m_roomid;
        tmp +=sizeof(int);

        //用于延迟过久舍弃一些帧的参考时间
        QTime tm = QTime::currentTime();
        *(int*)tmp = tm.minute();
        tmp +=sizeof(int);
        *(int*)tmp = tm.second();
        tmp +=sizeof(int);
        *(int*)tmp = tm.msec();
        tmp +=sizeof(int);


        memcpy( tmp , ba.data() , ba.size() );
        //发送是一个阻塞函数如果服务器接受缓冲区由于数据量大，没能及时取走缓冲区数据 滑动窗口变小send函数阻塞，使得用户界面卡顿，出现未响应问题

        //m_inet->SendData( 0 , buf , nPackSize );
        //delete []buf;
        Q_EMIT SIG_sendvedio2(buf,nPackSize);

    }



void Ckernel::slot_sendaudio(char *buf, int nlen)
{
    if(wolf_audio!=1)
    m_pAVClient[audio_client]->SendData(0,buf,nlen);
    delete []buf;

}

void Ckernel::slot_sendvedio(char *buf, int nlen)
{
    char *tmp=buf;
    tmp+=sizeof(int);

    tmp+=sizeof(int);

    tmp+=sizeof(int);
    QTime ctm=QTime::currentTime();
    int min=*(int*)tmp;
    tmp+=sizeof(int);
     int sec=*(int*)tmp;
    tmp+=sizeof(int);
    int mec=*(int*)tmp;
    QTime tm(ctm.hour(),min,sec,mec);
    //发送数据报延迟超过300ms就舍弃
    if(tm.msecsTo(ctm)>300)
    {
        //qDebug()<<"sq";
        delete []buf;
        return ;
    }


     m_pAVClient[video_client]->SendData(0,buf,nlen);
    delete []buf;


}

void Ckernel::slot_showwechat()
{

    we_chat->showNormal();
}





void Ckernel::slot_logincommit(QString tel, QString password)
{
    std::string strtel=tel.toStdString();
    int i=sizeof(STRU_LOGIN_RQ)+sizeof(STRU_REGISTER_RQ)+sizeof(int);

    STRU_LOGIN_RQ rq;
    strcpy(rq.m_tel,strtel.c_str());


    std::string md5pass=GetMD5STRING(password);
    strcpy(rq.m_szPassword,md5pass.c_str());






    m_inet->SendData(0,(char *)&rq,sizeof(rq));
     qDebug()<<md5pass.c_str();
}

void Ckernel::slot_rscommit(QString tel, QString password,QString name)
{
    std::string strtel=tel.toStdString();
    std::string strname=name.toStdString();//默认utf8 unclode 兼容中文  （控件里写的） 但是数据库里不是utf8
    
    STRU_REGISTER_RQ rq;
    strcpy(rq.m_tel,strtel.c_str());
    
    strcpy(rq.m_name,strname.c_str());


    std::string md5pass=GetMD5STRING(password);
    strcpy(rq.m_Password,md5pass.c_str());


    m_inet->SendData(0,(char *)&rq,sizeof(rq));
    qDebug()<<md5pass.c_str();

}
void Ckernel::slot_alive()
{

       STRU_ALIVE rq;
       rq.id=m_id;

       m_inet->SendData(0,(char *)&rq,sizeof(rq));
      // qDebug()<<"aoligei";


}

Ckernel::Ckernel(QObject *parent) : QObject(parent),m_id(0),m_roomid(0),m_wolfrommid(0)
{
    wolf_audio=0;
    qDebug()<<__func__;
   setNetpackmap();
   Initconfig();
   we_chat=new wechatDialog;
   //we_chat->show();
   m_plogindlg=new LoginDialog;
   m_wolfplaydlg=new WolfPlayDialog;
   m_hdlg=new hallDialog;
   friendlist=new WolfDialog;
   friendlikelist=new WolfDialog;
   m_wolfdog=new WolfDialog;
   m_roomdlg=new roomDialog;
   m_paudioread=new AudioRead;
   m_vedioread=new VideoRead;
   m_screenread=new ScreenRead;
   m_audio_worker = QSharedPointer<audioworkder>(new audioworkder);

   m_vedio_worker= QSharedPointer<vedioworkder>(new vedioworkder);
   m_vedio_worker2= QSharedPointer<vedioworkder>(new vedioworkder);
   for( int i = 0 ; i < 2 ; i++){
       m_pAVClient[i] = new TcpClientMediator;
       m_pAVClient[i]->OpenNet( /*_DEF_SERVERIP*/m_serverip , _DEF_TCP_PORT );
       connect( m_pAVClient[i] , SIGNAL( SIG_ReadyData(uint,char*,int))
                , this , SLOT( slot_dealData(uint,char*,int) ) );
   }
   connect( this , SIGNAL(SIG_sendvedio(char*,int))
            , m_vedio_worker.data() , SLOT(slot_vedio(char*,int)  ));
   connect( this , SIGNAL(SIG_sendvedio2(char*,int))
            , m_vedio_worker2.data() , SLOT(slot_vedio(char*,int)  ));
   m_timer=new QTimer;
   connect(m_timer,SIGNAL(timeout()),this,SLOT(slot_alive()));
   connect( this , SIGNAL(SIG_sendaudio(char*,int))
            , m_audio_worker.data() , SLOT(slot_audio(char*,int)  ));
   connect(m_plogindlg,SIGNAL(SIG_login_commit(QString ,QString )),this,SLOT(slot_logincommit(QString,QString)));
   connect(m_plogindlg,SIGNAL(SIG_rs_commit(QString ,QString ,QString)),this,SLOT(slot_rscommit(QString,QString,QString)));
   connect(we_chat,SIGNAL(SIG_Roomcreate()),this,SLOT(slot_Roomcreate()));
     connect(we_chat,SIGNAL(SIG_GetLikeFriendList()),this,SLOT(slot_friendlike()));
    connect(m_wolfplaydlg,SIGNAL(SIG_Createe()),this,SLOT(slot_wolfroomcreate()));
     connect(m_wolfplaydlg,SIGNAL(SIG_Match()),this,SLOT(slot_wolfmatch()));
       connect(m_wolfplaydlg,SIGNAL(SIG_GetFriendRoom()),this,SLOT(slot_wolfgetfriendroom()));

   connect(we_chat,SIGNAL(SIG_Roomjoin()),this,SLOT(slot_Roomjoin()));
   connect(we_chat,SIGNAL(SIG_CLOSE()),this,SLOT(slot_destory()));
   connect(m_plogindlg,SIGNAL(SIG_close()),this,SLOT(slot_destory()));
   connect(m_roomdlg,SIGNAL(SIG_close()),this,SLOT(slot_roomquit()));
    connect(m_wolfdog,SIGNAL(SIG_close()),this,SLOT(slot_wolfroomquit()));
    connect(friendlist,SIGNAL(SIG_close()),this,SLOT(slot_wolfroom2quit()));
     connect(friendlikelist,SIGNAL(SIG_close()),this,SLOT(slot_wolfroom3quit()));
   connect(m_roomdlg,SIGNAL(SIG_audiopause()),this,SLOT(slot_pauseAudio()));
   connect(m_wolfplaydlg,SIGNAL(SIG_Join()),this,SLOT(slot_WOLFRoomjoin()));
   connect(m_roomdlg,SIGNAL(SIG_audiostart()),this,SLOT(slot_startAudio()));
   connect(m_wolfdog,SIGNAL(SIG_audiostart()),this,SLOT(slot_startAudio2()));
    connect(m_wolfdog,SIGNAL(SIG_audiopause()),this,SLOT(slot_pauseAudio()));
    connect(m_wolfdog,SIGNAL(SIG_READY()),this,SLOT(slot_readywolf()));
   connect(m_paudioread,SIGNAL(SIG_audioFrame(QByteArray)),this,SLOT(slot_audioFrame(QByteArray)));
   connect(m_vedioread,SIGNAL(SIG_sendVideoFrame(QImage)),this,SLOT(slot_senvediodata(QImage)));
   connect(m_roomdlg,SIGNAL(SIG_vediostart()),this,SLOT(slot_vediostart()));
   connect(m_roomdlg,SIGNAL(SIG_vediopause()),this,SLOT(slot_vediopause()));
   connect(m_roomdlg,SIGNAL(SIG_deskstart()),this,SLOT(slot_deskstart()));
   connect(m_roomdlg,SIGNAL(SIG_deskpause()),this,SLOT(slot_deskpause()));
    connect(m_screenread,SIGNAL(SIG_getScreenFrame(QImage)),this,SLOT(slot_sendScreenFrame(QImage)));
    connect(m_hdlg,SIGNAL(SIG_friend_commit()),this,SLOT(slot_showwechat()));
    connect(we_chat,SIGNAL(  SIG_Addfriend()),this,SLOT(slot_addfriend()));
     connect(we_chat,SIGNAL( SIG_ChooseAdress()),this,SLOT(slot_chooseaddress()));
     connect(we_chat,SIGNAL( SIG_ChooseEnjoy()),this,SLOT(slot_ChooseEnjoy()));
       connect(we_chat,SIGNAL(SIG_GetFriendList()),this,SLOT(slot_GetList()));


    connect(m_hdlg,SIGNAL(SIG_wolf_commit()),this,SLOT(slot_showwolf()));
    connect(m_roomdlg,SIGNAL(SIG_setMoji(int)),m_vedioread,SLOT(slot_setMoji(int)));
   m_plogindlg->show();
   m_inet=new TcpClientMediator;

   m_inet->OpenNet(m_serverip,_DEF_TCP_PORT);

   connect(m_inet,SIGNAL(SIG_ReadyData(uint,char*,int)),this,SLOT(slot_dealdata(uint,char*,int)));
}

void Ckernel::slot_destory()
{
    if(we_chat)
    {
        we_chat->hide();
        delete we_chat;
        we_chat=NULL;

    }
    if(m_plogindlg)
    {
        m_plogindlg->hide();
        delete m_plogindlg;
        m_plogindlg=NULL;

    }
    if(m_hdlg)
    {
        m_hdlg->hide();
        delete m_hdlg;
        m_hdlg=NULL;

    }
    if(m_roomdlg)
    {
        m_roomdlg->hide();
        delete m_roomdlg;
        m_roomdlg=NULL;

    }
    if(m_paudioread)
    {
        m_paudioread->pause();
        delete m_paudioread;
        m_paudioread=NULL;
    }
    if(m_wolfplaydlg)
    {
        m_wolfplaydlg->hide();
        delete m_wolfplaydlg;
        m_wolfplaydlg=NULL;
    }


    exit(0);
}

void Ckernel::slot_showwolf()
{

    m_wolfplaydlg->showNormal();
}
void Ckernel::slot_dealVideoFrameRq(uint sock, char *buf, int nlen)
{
    //拆包
    ///视频数据帧
    /// 成员描述
    /// int type;
    /// int userId;
    /// int roomId;
    /// int min;
    /// int sec;
    /// int msec;
    /// QByteArray videoFrame;
    char * tmp = buf;
    tmp += sizeof( int );
    int userId = *(int*) tmp;
    tmp += sizeof( int );
    int roomId = *(int*) tmp;
    tmp += sizeof( int );

    tmp += sizeof( int );
    tmp += sizeof( int );
    tmp += sizeof( int );

    int datalen = nlen - 6*sizeof(int);
    QByteArray bt(tmp , datalen);
    QImage img;
    img.loadFromData( bt );
    printf("slove this video data");
    if( m_roomid == roomId )
        m_roomdlg->slot_refreusershow(userId ,  img);

}


