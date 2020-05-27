#include "play.h"
#include "ui_play.h"
#include <QPropertyAnimation>

Play::Play(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Play)
{
    ui->setupUi(this);
}

Play::~Play()
{
    delete ui;
}

void Play::showPlay(int level)
{
    this->level = level;

    /*��ʱ����ʱ��ʼ��ʱ����*/
    int maxNum = 100; // ��ʱΪ100��
    ui->timeBar->setRange(0, maxNum);
    ui->timeBar->setValue(maxNum);
    time = new time_thread(maxNum);
    connect(time, SIGNAL(updateUI(int)), this, SLOT(updateTime(int)));
    time->start();
    //-------------------------------//

    setWindowOpacity(0);
    this->show();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(2000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
}

void Play::updateTime(int time)
{
    curTime = time;
    ui->timeBar->setValue(time);
}

void Play::on_pauseBtn_clicked()
{
    bool isPause = time->isPause;
    if(isPause){
        time->isPause = false;
        time->start();
    }else{
        time->isPause = true;
        time->terminate();
    }
}

void Play::getPoint()
{
    // �÷�ʱ����ʱ�䣨�ݶ�1�룩
    curTime++;
    time->time = curTime;
    ui->timeBar->setValue(curTime);
    //�÷ֹ��ܴ���������
}
