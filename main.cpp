#include "mainwindow.h"
#include "login.h"
#include "register.h"
#include "play.h"
#include "choose.h"
#include "rank.h"
#include "forget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Login l;
    Register r;
    Play p;
    Choose c;
    Rank rk;
    Forget f;
    l.setWindowTitle(QString::fromLocal8Bit("登录"));
    r.setWindowTitle(QString::fromLocal8Bit("注册"));
    w.setWindowTitle(QString::fromLocal8Bit("主界面"));
    p.setWindowTitle(QString::fromLocal8Bit("游戏"));
    c.setWindowTitle(QString::fromLocal8Bit("选择难度"));
    rk.setWindowTitle(QString::fromLocal8Bit("排行榜"));
    f.setWindowTitle(QString::fromLocal8Bit("忘记密码"));
   // l.show();
    c.show();
    // 跳转到下一个页面
    QObject::connect(&l, SIGNAL(toMain()), &w, SLOT(showMain()));
    QObject::connect(&r, SIGNAL(toMain()), &w, SLOT(showMain()));
    QObject::connect(&l, SIGNAL(toRegister()), &r, SLOT(showRegister()));
    QObject::connect(&w, SIGNAL(toChoose()), &c, SLOT(showChoose()));
    QObject::connect(&w, SIGNAL(toRank()), &rk, SLOT(showRank()));
    QObject::connect(&c, SIGNAL(toPlay(int)), &p, SLOT(showPlay(int)));
    QObject::connect(&l, SIGNAL(toForget()), &f, SLOT(showForget()));

    // 返回上一个页面
    QObject::connect(&r, SIGNAL(toLogin()), &l, SLOT(showLogin()));
    QObject::connect(&f, SIGNAL(toLogin()), &l, SLOT(showLogin()));
    QObject::connect(&w, SIGNAL(toLogin()), &l, SLOT(showLogin()));
    QObject::connect(&c, SIGNAL(toMain()), &w, SLOT(showMain()));
    QObject::connect(&rk, SIGNAL(toMain()), &w, SLOT(showMain()));
    return a.exec();
}
