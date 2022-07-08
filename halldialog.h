#ifndef HALLDIALOG_H
#define HALLDIALOG_H

#include <QDialog>

namespace Ui {
class hallDialog;
}

class hallDialog : public QDialog
{
    Q_OBJECT

public:
    explicit hallDialog(QWidget *parent = nullptr);
    ~hallDialog();
signals:
    void SIG_friend_commit();
    SIG_wolf_commit();
private slots:


    void on_pb_friend_clicked();

    void on_pb_wolf_clicked();

private:
    Ui::hallDialog *ui;
};

#endif // HALLDIALOG_H
