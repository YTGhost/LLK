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

    void getPoint();// 闅惧害锛涓哄叆闂紝2涓哄垵绾э紝3涓轰腑绾э紝4涓洪珮绾
    int level;
    time_thread *time;
    int curTime;
    int isInit = 1;
    QString username;
    QImage timeBarImage;
    struct LinkPoints{
        int cnt;
        int pt[4][2];//0是第一个端点，cnt+1是第二个端点，其他是中间点
    };
    // test
    //璁剧疆娓告垙鍦烘櫙
    void setBlocks();

    //娓呭満
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

