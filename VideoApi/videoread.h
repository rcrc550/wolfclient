#ifndef VIDEOREAD_H
#define VIDEOREAD_H

#include <QObject>
#include <QImage>
#include<QTimer>
#include"common.h"
#include"threadworker.h"
#include<QDebug>
#include "myfacedetect.h"
//图片的宽高
#define IMAGE_WIDTH (320)
#define IMAGE_HEIGHT (240)



class Videoworkder;
class VideoRead : public QObject
{
    Q_OBJECT
public:
    explicit VideoRead(QObject *parent = nullptr);
    ~VideoRead();
signals:
    void SIG_sendVideoFrame(QImage img);
public slots:
    void slot_getVideoFrame();
    void slot_openVideo();
    void slot_closeVideo();
    void slot_setMoji(int i);
private:
    QTimer *m_timer;

    QSharedPointer<Videoworkder> m_shared_worker;

    //opencv获取图片的对象
    cv::VideoCapture cap;
     std::vector<Rect> m_vecLastFace;
     enum moji_type { moji_tuer = 1 , moji_hat };

     int m_moji; // 用于存储当前的萌拍效果 1 兔耳朵 2 帽子
     QImage m_tuer;
     QImage m_hat;
};
class Videoworkder:public ThreadWorker
{
     Q_OBJECT
public slots:
    void slot_setinfo(VideoRead*c)
    {
        m_pread=c;
    }
    //定时器到时运行
    void slot_dowork()
    {
        m_pread->slot_getVideoFrame();
        qDebug() << QThread::currentThreadId();
    }

private:
    VideoRead *m_pread;
};

#endif // VIDEOREAD_H
