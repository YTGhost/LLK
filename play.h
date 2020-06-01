#ifndef PLAY_H
#define PLAY_H

#include <QDialog>
#include "time_thread.h"
#include <QTime>
#include <QSoundEffect>
#include <QDebug>
#include <QtTest/QTest>

namespace Ui {
class Play;
}

class Play : public QDialog
{
    Q_OBJECT

public:
    explicit Play(QWidget *parent = nullptr);
    ~Play();
    void getPoint();
    // 难度：1为入门，2为初级，3为中级，4为高级
    int level;
    time_thread *time;
    int curTime;

    // test
    QPushButton ***btns;//pushbutton数组
    int hb, wb;//高多少个格子，宽多少个格子
    int lastClickedH, lastClickedW;//上一个点击的坐标
    int remains;//剩余多少个格子
    int **types;//格子的类型的数组。0代表被清理了，1-15格子代表一种格子（图片）
    int figures;//多少种格子（图片）
    QPixmap icons[16];//图片数组
    QPixmap imageLine[2], imageTurn[4];//连线的图片的数组
    QTimer *pTimer;//定时器
    QTime *pTime;//计时器
    QSoundEffect clickSound;//点击音效
    QSoundEffect endsSound;//游戏结束音效
    bool isStarted;//游戏开始了吗

    //点击两个配对点后，配对点和中间端点构成的结构体
    struct LinkPoints{
        int cnt;
        int pt[4][2];//0是第一个端点，cnt+1是第二个端点，其他是中间点
    };

    //设置游戏高、宽、人数
    void setDifficulty(int setHb, int setWb, int setFigure);

    //设置游戏场景
    void setBlocks();

    //判定能否不拐、一拐、两拐连接
    bool canNoCorner(int lstH, int lstW, int thisH, int thisW, LinkPoints &lp);
    bool canOneCorner(int lstH, int lstW, int thisH, int thisW, LinkPoints &lp);
    bool canTwoCorner(int lstH, int lstW, int thisH, int thisW, LinkPoints &lp);

    //清场
    void stageClear();

    //获得一个点到另一个点的经过的中间点的方向
    int getDirection(int lstH, int lstW, int thisH, int thisW, int midH, int midW);

    //画一条线和一组线
    void drawALine(int lstH, int lstW, int thisH, int thisW, bool isW);
    void drawLines(LinkPoints lp);

private:
    Ui::Play *ui;

private slots:
    void onTimeOut();
    void btnsClicked();
    void showPlay(int level);
    void on_pauseBtn_clicked();
    void updateTime(int time);
};

#endif // PLAY_H
