#ifndef WOLFDIALOG_H
#define WOLFDIALOG_H

#include <QDialog>
#include<QVBoxLayout>
#include<usershow.h>
#include<QMessageBox>
#include<QCloseEvent>
#include<QLabel>
namespace Ui {
class WolfDialog;
}

class WolfDialog : public QDialog
{
    Q_OBJECT
signals:

    void SIG_close();
    void SIG_audiopause();
    void SIG_audiostart();
    void SIG_READY();
public:
    explicit WolfDialog(QWidget *parent = nullptr);
    ~WolfDialog();
public slots:
    void slot_addUserShow(UserShow *user_show);
    void slot_setInfo(QString roomid);
     void closeEvent(QCloseEvent *event);
     void slot_clearUserShow();
     void slot_removeUserShow(UserShow *user_show);
     int slot_removeUserShow(int id);

private slots:
     void on_pb_jump_clicked();

     void on_cb_audio_clicked();

     void on_pb_ready_clicked();
public:
     int idendity;//身份
     QVBoxLayout *m_mainlayout;
     QVBoxLayout *m_mainlayout2;
private:
    Ui::WolfDialog *ui;

     std::map<int,UserShow*>m_mapidToUserShow;

      int start;
      int ready;
public:
      int savepeople;
      int wich;
      int prohit;
       std::vector<QWidget*>myvec;
};

#endif // WOLFDIALOG_H
