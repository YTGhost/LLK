#include "mainwindow.h"
#include "login.h"
#include "register.h"
#include "play.h"
#include "choose.h"
#include "rank.h"
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
    l.setWindowTitle(QString("Login"));
    r.setWindowTitle(QString("Register"));
    p.setWindowTitle(QString("Play"));
    c.setWindowTitle(QString("Choose"));
    rk.setWindowTitle(QString("Rank"));
    l.show();
    QObject::connect(&l, SIGNAL(toMain()), &w, SLOT(showMain()));
    QObject::connect(&l, SIGNAL(toRegister()), &r, SLOT(showRegister()));
    QObject::connect(&w, SIGNAL(toChoose()), &c, SLOT(showChoose()));
    QObject::connect(&w, SIGNAL(toRank()), &rk, SLOT(showRank()));
    QObject::connect(&c, SIGNAL(toPlay(int)), &p, SLOT(showPlay(int)));
    return a.exec();
}
