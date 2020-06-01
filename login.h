#ifndef LOGIN_H
#define LOGIN_H
#include "databasehelper.h"
#include <QDialog>
#include<QSqlQuery>
#include<qdebug.h>
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
    QString select(QString username);
private:
    Ui::Login *ui;

signals:
    void toMain();  // 跳转mainwindow的信号（发送到mainwindow）
    void toRegister();  // 跳转到register的信号(发送到register)
private slots:
    void on_loginBtn_clicked();
    void on_registerBtn_clicked();
};

#endif // LOGIN_H
