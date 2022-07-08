#ifndef CKERNEL_H
#define CKERNEL_H

#include <QObject>
#include"wechatdialog.h"
#include<QTime>
#include"TcpClientMediator.h"
#include"packdef.h"
#include"logindialog.h"
#include"audioread.h"
#include"audiowrite.h"
#include"roomdialog.h"
#include"videoread.h"
#include"screenread.h"
#include"halldialog.h"
#include"wolfplaydialog.h"
#include"wolfdialog.h"
#include"vector"


class Ckernel;
typedef void(Ckernel::*PFUN)(uint sock,char *buf,int nlen);
void waitSec(int s);

class audioworkder;
class vedioworkder;
class aliveworkder;
class Ckernel : public QObject
{
    Q_OBJECT
public:
    explicit Ckernel(QObject *parent = nullptr);
    //单例
    static Ckernel *Getinstance()
    {
        static Ckernel kernel;
        return &kernel;   //没法回收
    }
signals:
    void SIG_sendaudio(char *buf,int nlen);
    void SIG_sendvedio(char *buf,int nlen);
    void SIG_sendvedio2(char *buf,int nlen);
    void SIG_Alive();


public slots:
    void slot_destory();
    void slot_showwolf();
    void slot_dealdata(uint sock,char *buf,int nlen);
    void Initconfig();
    //处理不同的回复
    void slot_dealloginRs(uint sock,char *buf,int nlen);
    void slot_dealRegisRs(uint sock,char *buf,int nlen);
    void slot_dealJoinRs(uint sock,char *buf,int nlen);
    void slot_dealCreateRs(uint sock,char *buf,int nlen);
    void slot_dealLeaveRs(uint sock,char *buf,int nlen);
    void slot_dealAudioFrame(uint sock,char *buf,int nlen);
    void slot_dealVideoFrameRq(uint sock,char *buf,int nlen);
    void slot_dealWfCreate(uint sock,char *buf,int nlen);
    void slot_dealWolfJoinRs(uint sock, char *buf, int nlen);
    void slot_dealWolfLeaveRs(uint sock, char *buf, int nlen);
    void slot_dealWolfMemberRs(uint sock, char *buf, int nlen);
    void slot_getIdendity(uint sock, char *buf, int nlen);
    void slot_dealDaydark(uint sock, char *buf, int nlen);
    void slot_dealWolfKill(uint sock, char *buf, int nlen);
    void slot_dealWitchSave(uint sock, char *buf, int nlen);
    void slot_dealDie(uint sock, char *buf, int nlen);
    void slot_dealWatch(uint sock, char *buf, int nlen);
    void slot_dealWake(uint sock, char *buf, int nlen);
    void slot_dealsaysm(uint sock, char *buf, int nlen);
    void slot_dealstopsay(uint sock, char *buf, int nlen);
    void slot_dealNormalnote(uint sock, char *buf, int nlen);
    void dealGameover(uint sock, char *buf, int nlen);
    void dealaddresult(uint sock, char *buf, int nlen);
    void  getlistrs(uint sock, char *buf, int nlen);
    void  getlikefriend(uint sock, char *buf, int nlen);
    void  getdealseeroom(uint sock, char *buf, int nlen);
    //点击提交的槽函数
    void slot_rscommit(QString tel,QString password,QString name);
    void slot_logincommit(QString tel,QString password);
    void setNetpackmap();
    void slot_Roomjoin();
    void slot_friendlike();
    void slot_Roomcreate();
    void slot_dealRoomMember(uint sock, char *buf, int nlen);
    void slot_roomquit();
    void slot_wolfmatch();
    void slot_startAudio();
    void slot_pauseAudio();
    void slot_wolfgetfriendroom();
    void slot_ChooseEnjoy();
    void slot_audioFrame(QByteArray array);
    void slot_senvediodata(QImage image);
    void slot_vediopause();
    void slot_chooseaddress();
    void slot_vediostart();
    void slot_refreshvideo(int id,QImage image);
    void slot_deskstart();
    void slot_deskpause();
    void slot_sendScreenFrame(QImage);
    void slot_sendaudio(char *buf,int nlen);
    void slot_sendvedio(char *buf,int nlen);
    void slot_showwechat();
    void slot_WOLFRoomjoin();
    void slot_readywolf();
    void slot_addfriend();
    void  slot_GetList();
    void slot_wolfroomquit();
    void slot_wolfroom2quit();
    void slot_wolfroom3quit();
public slots:
    void slot_wolfroomcreate();
    void slot_startAudio2();
    void slot_alive();




private:
    wechatDialog *we_chat;
    INetMediator *m_inet;
    WolfDialog *m_wolfdog;
    LoginDialog *m_plogindlg;
    WolfPlayDialog *m_wolfplaydlg;
    hallDialog *m_hdlg;
     WolfDialog *friendlist;
     WolfDialog *friendlikelist;
    char m_serverip[20];
    PFUN m_netpackmap[DEF_PACK_COUNT];
    int m_id;
    int m_roomid;
    int m_wolfrommid;
    QString m_name ;
    QTimer *m_timer;
    roomDialog *m_roomdlg;
    /////////////////
    /// 音频一个采集 多个播放 房间内成员列表 每一个房间成员1:1映射map
    ///
    AudioRead *m_paudioread;
    std::map<int,AudioWrite*>m_mapidToAudiowrite;
    std::map<int,AudioWrite*>m_mapidToWolfAudiowrite;

    /////////////////
    /// 视频采集
    ///
    VideoRead *m_vedioread;
    ScreenRead *m_screenread;
    QSharedPointer<audioworkder> m_audio_worker;
     QSharedPointer<aliveworkder> m_alive;
     QSharedPointer<vedioworkder> m_vedio_worker;
      QSharedPointer<vedioworkder> m_vedio_worker2;
      enum client_type{ audio_client = 0 , video_client };
      INetMediator * m_pAVClient[2];
      std::map<int,std::string>wolfmembername;
      std::map<int,int>idstore;
      int wolf_audio;




private:

};
class audioworkder:public ThreadWorker
{
     Q_OBJECT
public slots:

    //定时器到时运行

    void slot_audio(char *buf,int nlen)
    {
       Ckernel::Getinstance()->slot_sendaudio( buf, nlen);
    }
private:

};
class vedioworkder:public ThreadWorker
{
     Q_OBJECT
public slots:

    //定时器到时运行

    void slot_vedio(char *buf,int nlen)
    {
       Ckernel::Getinstance()->slot_sendvedio( buf, nlen);
    }
private:

};



#endif // CKERNEL_H
