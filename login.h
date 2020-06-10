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
    void toMain();  // ��תmainwindow���źţ����͵�mainwindow��
    void toRegister();  // ��ת��register���ź�(���͵�register)
    void toForget();    // ��ת��forget���ź�(���͵�forget)
    void sendInfo(QString);    // ���͵�¼���û�����Play��
private slots:
    void on_loginBtn_clicked();
    void on_registerBtn_clicked();
    void on_forgetBtn_clicked();
    void showLogin();
};

#endif // LOGIN_H
