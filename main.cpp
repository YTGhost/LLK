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
    l.setWindowTitle(QString::fromLocal8Bit("��¼"));
    r.setWindowTitle(QString::fromLocal8Bit("ע��"));
    w.setWindowTitle(QString::fromLocal8Bit("������"));
    p.setWindowTitle(QString::fromLocal8Bit("��Ϸ"));
    c.setWindowTitle(QString::fromLocal8Bit("ѡ���Ѷ�"));
    rk.setWindowTitle(QString::fromLocal8Bit("���а�"));
    f.setWindowTitle(QString::fromLocal8Bit("��������"));
   // l.show();
    c.show();
    // ��ת����һ��ҳ��
    QObject::connect(&l, SIGNAL(toMain()), &w, SLOT(showMain()));
    QObject::connect(&r, SIGNAL(toMain()), &w, SLOT(showMain()));
    QObject::connect(&l, SIGNAL(toRegister()), &r, SLOT(showRegister()));
    QObject::connect(&w, SIGNAL(toChoose()), &c, SLOT(showChoose()));
    QObject::connect(&w, SIGNAL(toRank()), &rk, SLOT(showRank()));
    QObject::connect(&c, SIGNAL(toPlay(int)), &p, SLOT(showPlay(int)));
    QObject::connect(&l, SIGNAL(toForget()), &f, SLOT(showForget()));

    // ������һ��ҳ��
    QObject::connect(&r, SIGNAL(toLogin()), &l, SLOT(showLogin()));
    QObject::connect(&f, SIGNAL(toLogin()), &l, SLOT(showLogin()));
    QObject::connect(&w, SIGNAL(toLogin()), &l, SLOT(showLogin()));
    QObject::connect(&c, SIGNAL(toMain()), &w, SLOT(showMain()));
    QObject::connect(&rk, SIGNAL(toMain()), &w, SLOT(showMain()));
    return a.exec();
}
