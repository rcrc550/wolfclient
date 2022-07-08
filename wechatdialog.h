#ifndef WECHATDIALOG_H
#define WECHATDIALOG_H
#include"QCloseEvent"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class wechatDialog; }
QT_END_NAMESPACE

class wechatDialog : public QDialog
{
    Q_OBJECT
signals:
    void SIG_CLOSE();
    void SIG_Roomcreate();
    void SIG_Roomjoin();
    void SIG_Addfriend();
     void SIG_ChooseAdress();
      void  SIG_GetFriendList();
      void SIG_ChooseEnjoy();
       void SIG_GetLikeFriendList();
public:
    wechatDialog(QWidget *parent = nullptr);
    ~wechatDialog();

public:
   void closeEvent(QCloseEvent *p);
   void setinfo( QString name,int icon=1);
private slots:
   void on_room_create_clicked();

   void on_room_join_clicked();




   void on_pb_add_clicked();

   void on_pushButton_2_clicked();

   void on_pushButton_clicked();

   void on_room_no_clicked();

   void on_pushButton_4_clicked();

private:
    Ui::wechatDialog *ui;
};
#endif // WECHATDIALOG_H
