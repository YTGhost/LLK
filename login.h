#ifndef LOGIN_H
#define LOGIN_H
#include "databasehelper.h"
#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    databasehelper *helper;

private:
    Ui::Login *ui;

signals:
    void toMain();  // ��תmainwindow���źţ����͵�mainwindow��
    void toRegister();  // ��ת��register���ź�(���͵�register)
private slots:
    void on_loginBtn_clicked();
    void on_registerBtn_clicked();
};

#endif // LOGIN_H
