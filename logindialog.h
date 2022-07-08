#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include"QCloseEvent"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT
signals:
    void SIG_login_commit(QString tel,QString password);
    void SIG_rs_commit(QString tel,QString password,QString name);
    void SIG_close();
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    void closeEvent(QCloseEvent*zevent);

private slots:


    void on_clear_zc_clicked();

    void on_commit_zc_clicked();





    void on_commit_dl_clicked();

    void on_clear_dl_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
