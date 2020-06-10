#ifndef PLAY_H
#define PLAY_H

#include <QDialog>
#include "time_thread.h"
#include <QTime>
#include <QSqlQuery>
#include <QDebug>
#include <QtTest/QTest>
#include "playworker.h"

namespace Ui {
class Play;
}

class Play : public QDialog
{
    Q_OBJECT

public:
    explicit Play(QWidget *parent = nullptr);
    ~Play();

    void getPoint();// 难度�为入门，2为初级，3为中级，4为高�
    int level;
    time_thread *time;
    int curTime;
    int isInit = 1;
    QString username;
    QImage timeBarImage;
    struct LinkPoints{
        int cnt;
        int pt[4][2];//0�ǵ�һ���˵㣬cnt+1�ǵڶ����˵㣬�������м��
    };
    // test
    //设置游戏场景
    void setBlocks();

    //清场
    void stageClear();

private:
    Ui::Play *ui;

    playworker *worker;

signals:
    void returnChoose();

private slots:
    void showPlay(int level);
    void on_pauseBtn_clicked();
    void updateTime(int time);
    void updateLcdNumber(QString value);
    void updateProgressBar(int value);
    void goonPlay(int score);
    void on_remind_clicked();
    void getInfo(QString);
    void on_resetBtn_clicked();
    void on_solveBtn_clicked();
    void getHint(int);
    void on_backBtn_clicked();
};

#endif // PLAY_H

