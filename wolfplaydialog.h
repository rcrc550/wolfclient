#ifndef WOLFPLAYDIALOG_H
#define WOLFPLAYDIALOG_H

#include <QDialog>
#include <QObject>
#include <QCloseEvent>
#include<QMessageBox>
namespace Ui {
class WolfPlayDialog;
}

class WolfPlayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WolfPlayDialog(QWidget *parent = nullptr);
    ~WolfPlayDialog();
signals:
    void SIG_Createe();
    void SIG_Join();
    void SIG_Match();
    void SIG_GetFriendRoom();


private slots:
    void on_pb_create_clicked();

    void on_pb_join_clicked();

    void on_pb_start_clicked();

    void on_pb_watch_clicked();

private:
    Ui::WolfPlayDialog *ui;
};

#endif // WOLFPLAYDIALOG_H
