#include "play.h"
#include "ui_play.h"
#include <QPropertyAnimation>
#include <QMessageBox>

Play::Play(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Play),
    worker(new playworker(this))
{
    ui->setupUi(this);

    connect(worker, &playworker::updateLcdNumber, this, &Play::updateLcdNumber);
    connect(worker, &playworker::updateProgressBar, this, &Play::updateProgressBar);
    connect(worker, &playworker::goonPlay, this, &Play::goonPlay);
}

Play::~Play()
{
    delete ui;
}
void Play::showPlay(int level)
{
    if (nullptr == worker)
    {
        qDebug() << "nullptr == worker";
        return;
    }

    this->level = level;

    if (level == 1 || level == 2){
        worker->setDifficulty(6, 6, 8);
    }
    else if (level == 4){
        worker->setDifficulty(10, 14, 15);
    }
    else if(level == 3){
        worker->setDifficulty(8, 10, 11);
    }

    //初始化图片、音效
    worker->initImages();
    worker->initSounds();

    //设置grid布局
    ui->gridLayout->setHorizontalSpacing(1);
    ui->gridLayout->setVerticalSpacing(1);
    ui->gridLayout->setSizeConstraint(QLayout::SetFixedSize);
    ui->gridLayout->activate();

    //清理场景
    if(isInit == 1){
        isInit = 0;
    }else{
        this->stageClear();
    }

    //设置场景
    this->setBlocks();


    /*暂时测试时初始化时间条*/
    int maxNum = 100; // 暂时为100秒
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

void Play::updateLcdNumber(QString value)
{
    ui->lcdNumber->display(value);
    ui->lcdNumber_2->display(QString::number(worker->score));
    if(worker->flag!=0){
        getPoint();
    }
    worker->flag=0;
}

void Play::updateProgressBar(int value)
{
    ui->progressBar->setValue(value);
}

void Play::goonPlay()
{
    QMessageBox::StandardButton result =
            QMessageBox::question(this, QString::fromLocal8Bit("完成"),
                          QString::fromLocal8Bit("完成！\n再来一局？"),
                          QMessageBox::Yes | QMessageBox::No,
                          QMessageBox::NoButton);
    if(result==QMessageBox::No)
    {
        exit(0);
    }
    else
    {
//            on_actNewGame_triggered();
    }

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
    // 得分时增加时间（暂定1秒）
    curTime++;
    time->time = curTime;
    ui->timeBar->setValue(curTime);
    //得分功能待做。。。
}

// test

void Play::setBlocks()
{
    worker->setBlocks(ui->gridLayout);
}

void Play::stageClear()
{
    ui->progressBar->setValue(0);
    ui->lcdNumber->display(0);

    worker->stageClear();
}

void Play::on_remind_clicked()
{
    int times = worker->getTimes();

    times--;
    worker->setTimes(times);
    worker->gameRemind();

    QString text = "提示次数：" + QString::number(times);
    ui->remind->setText(text);

    if (0 == times) {
        ui->remind->setEnabled(false);
    }
}

void Play::on_pushButton_clicked()
{
    worker->remakeMap(ui->gridLayout);
}
