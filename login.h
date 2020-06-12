#ifndef LOGIN_H
#define LOGIN_H
#include "databasehelper.h"
#include <QDialog>
#include <QSqlQuery>
#include "music.h"
#include <qdebug.h>
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
    Music *music;
    QString select(QString username);
private:
    Ui::Login *ui;

signals:
    void toMain();  // 跳转mainwindow的信号（发送到mainwindow）
    void toRegister();  // 跳转到register的信号(发送到register)
    void toForget();    // 跳转到forget的信号(发送到forget)
    void sendInfo(QString);    // 发送登录的用户名到Play类
private slots:
    void on_loginBtn_clicked();
    void on_registerBtn_clicked();
    void on_forgetBtn_clicked();
    void showLogin();
};

#endif // LOGIN_H
