#include "time_thread.h"

time_thread::time_thread()
{

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
    time--;
    emit updateUI(time);
}
