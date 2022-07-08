#ifndef ROOMDIALOG_H
#define ROOMDIALOG_H

#include <QDialog>
#include<QVBoxLayout>
#include"usershow.h"
namespace Ui {
class roomDialog;
}

class roomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit roomDialog(QWidget *parent = nullptr);
    ~roomDialog();
signals:
    void SIG_close();
    void SIG_audiopause();
    void SIG_audiostart();
    void SIG_vediostart();
    void SIG_vediopause();
    void SIG_deskstart();
    void SIG_deskpause();
    void SIG_setMoji(int i);
public slots:
    void slot_setInfo(QString roomid);
    void slot_addUserShow(UserShow *user_show);
    void slot_removeUserShow(UserShow *user_show);
    void slot_removeUserShow(int id);
    void slot_refreusershow(int id,QImage image);
    void slot_clearUserShow();
    void set_check(bool check);
    void slot_setBigImageid(int id,QString username);
private slots:
    void on_pb_close_clicked();

    void on_pb_quit_clicked();


    void closeEvent(QCloseEvent *event);
    void on_cb_audio_clicked();

    void on_cb_video_clicked();

    void on_cb_desk_clicked();

    void on_cb_choice_currentIndexChanged(int index);

private:
    Ui::roomDialog *ui;
    QVBoxLayout *m_mainlayout;
     std::map<int,UserShow*>m_mapidToUserShow;
};

#endif // ROOMDIALOG_H
