#ifndef REGISTER_H
#define REGISTER_H
#define TRUE 1
#define FALSE 0

#include <QDialog>
#include<time.h>
#include <QSqlQuery>
#include "smtp.h"
#include <QMessageBox>
#include <string>

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();
    //用于存放从文本框得到的注册信息
    QString username = "",email = "",password = "";
    //用于判断验证码是否正确
    int codeflag = FALSE;
    //用于存放随机生成的验证码
    char ch[8];
    char temp;

private:
    Ui::Register *ui;

signals:
    void toMain();  // 跳转mainwindow的信号（发送到mainwindow）
    void toLogin(); // 跳转到login的信号

private slots:
    void showRegister();
    void userValidator();
    void passwordValidator();
    void emailValidator();
    void confirmValidator();
    void on_sendcodeBtn_clicked();
    void on_registerBtn_clicked();
    void on_backBtn_clicked();
};

#endif // REGISTER_H
