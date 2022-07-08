#ifndef USERSHOW_H
#define USERSHOW_H

#include <QWidget>
#include <QPaintEvent>
#include<QImage>
#include<QMouseEvent>
#include<QTimer>
#include<QTime>

namespace Ui {
class UserShow;
}

class UserShow : public QWidget
{
    Q_OBJECT

public:
    explicit UserShow(QWidget *parent = nullptr);

    ~UserShow();
signals:
    void SIG_itemCliked(int mid,QString username);
public slots:
    void set_info(int id,QString username);
    void paintEvent(QPaintEvent *event);
    void slot_setImage(QImage &img);
    void mousePressEvent(QMouseEvent *event);
    void slot_checkTimer();
private:
    Ui::UserShow *ui;
    QString m_username;
    QImage m_img;
public:
    friend class roomDialog;
    int m_id;
    QImage m_defaultImg;
    QTimer m_timer;
    QTime m_lastTime;
    int id;
};

#endif // USERSHOW_H
