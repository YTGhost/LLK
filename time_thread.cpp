#include "time_thread.h"

time_thread::time_thread()
{
    this->time = 60;
    isPause = false;
}

time_thread::time_thread(int time)
{
    this->time = time;
    isPause = false;
}

void time_thread::run()
{
    timer = new QTimer();
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(qtimeSlot()));
    timer->start();
    this->exec();
}

void time_thread::qtimeSlot()
{
//    if(time > 60 || time < 0) time = 60;
    time--;
    // 时间耗尽时结束游戏
    if(time == 0) emit timeEnd();
    emit updateUI(time);
}
