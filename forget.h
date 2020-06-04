#ifndef FORGET_H
#define FORGET_H

#include <QDialog>
#include <QMessageBox>
#include <QTime>
#include <iostream>
using namespace std;

namespace Ui {
class Forget;
}

class Forget : public QDialog
{
    Q_OBJECT

public:
    explicit Forget(QWidget *parent = nullptr);
    ~Forget();
    QString code = "";
    int checkEmail = 0;
    int checkPassword = 0;
    int checkConfirm = 0;

private:
    Ui::Forget *ui;

signals:
    void toLogin();

private slots:
    void showForget();
    void emailValidator();
    void on_sendBtn_clicked();
    void on_changeBtn_clicked();
    void passwordValidator();
    void confirmValidator();
    void on_backBtn_clicked();
};

#endif // FORGET_H
