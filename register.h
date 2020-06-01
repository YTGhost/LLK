#ifndef REGISTER_H
#define REGISTER_H
#define TRUE 1
#define FALSE 0

#include <QDialog>

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

private slots:
    void showRegister();
    void userValidator();
    void passwordValidator();
    void emailValidator();
    void confirmValidator();
    void on_sendcodeBtn_clicked();
    void on_registerBtn_clicked();
};

#endif // REGISTER_H
