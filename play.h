#ifndef PLAY_H
#define PLAY_H

#include <QDialog>
#include "time_thread.h"
#include <QTime>

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

    void getPoint();// 难度：1为入门，2为初级，3为中级，4为高级
    int level;
    time_thread *time;
    int curTime;
    int isInit = 1;

    // test
    //设置游戏场景
    void setBlocks();

    //清场
    void stageClear();

private:
    Ui::Play *ui;

    playworker *worker;

private slots:
    void showPlay(int level);
    void on_pauseBtn_clicked();
    void updateTime(int time);
    void updateLcdNumber(QString value);
    void updateProgressBar(int value);
    void goonPlay();
    void on_remind_clicked();
};

#endif // PLAY_H

