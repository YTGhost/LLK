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
    timeBarImage = QImage(":/pic/background/progress.png");
    ui->label->hide();
    connect(worker, &playworker::updateLcdNumber, this, &Play::updateLcdNumber);
    connect(worker, &playworker::updateProgressBar, this, &Play::updateProgressBar);
    connect(worker, SIGNAL(goonPlay(int)), this, SLOT(goonPlay(int)));
    connect(worker, SIGNAL(toHint(int)), this, SLOT(getHint(int)));
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

    if (level == 1){
        worker->setDifficulty(2, 2, 8);
    }else if(level == 2){
        worker->setDifficulty(6, 6, 8);
    }
    else if (level == 3){
        worker->setDifficulty(8, 10, 11);
    }
    else if(level == 4){
        worker->setDifficulty(8, 14, 15);
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
        time->terminate();
        time->destroyed();
        if(time->isPause){
            // 取消遮住游戏区
            ui->label->hide();
            // 自动解题，重置和提示按钮恢复
            ui->solveBtn->setEnabled(true);
            ui->resetBtn->setEnabled(true);
            ui->remind->setEnabled(true);
        }
    }

    //设置场景
    this->setBlocks();

    // 检查是否有解
    if(!worker->gameCheck()){
        ui->resetHint->setText("有僵局");
    }else{
        ui->resetHint->setText("无僵局");
    }

    /*初始化时间条*/
    int max = 60;
    ui->timeBar->setRange(0, max);
    ui->timeBar->setValue(max);
    time = new time_thread();
    connect(time, SIGNAL(updateUI(int)), this, SLOT(updateTime(int)));
    connect(time, SIGNAL(timeEnd()), worker, SLOT(fail()));
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
    QString qss= "QProgressBar{"
                   "border: 1px solid rgb(16, 135, 209);"
                   "background: rgba(248,248,255,180);"
                   "border-radius: 6px; }"
                   "QProgressBar::chunk:enabled {"
                   "border-radius: 4px; "
                   "background: qlineargradient(x1:0, y1:0, x2:1, y2:0" ;
    double EndColor = static_cast<double>(time) / 60;
    for(int i = 0; i < 100; i++)
    {
        double Current = EndColor*i/100;
        QRgb rgb = timeBarImage.pixel((timeBarImage.width()-1)*Current,timeBarImage.height()/2);
        QColor c(rgb);
        qss.append(QString(",stop:%1  rgb(%2,%3,%4)").arg(i/100.0).arg(c.red()).arg(c.green()).arg(c.blue()));
    }
    qss.append(");}");
    ui->timeBar->setStyleSheet(qss);
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
    QString qss= "QProgressBar{"
                   "border: 1px solid rgb(16, 135, 209);"
                   "background: rgba(248,248,255,180);"
                   "border-radius: 6px; }"
                   "QProgressBar::chunk:enabled {"
                   "border-radius: 4px; "
                   "background: qlineargradient(x1:0, y1:0, x2:1, y2:0" ;
    double EndColor = static_cast<double>(value) / 100;
    for(int i = 0; i < 100; i++)
    {
        double Current = EndColor*i/100;
        QRgb rgb = timeBarImage.pixel((timeBarImage.width()-1)*Current,timeBarImage.height()/2);
        QColor c(rgb);
        qss.append(QString(",stop:%1  rgb(%2,%3,%4)").arg(i/100.0).arg(c.red()).arg(c.green()).arg(c.blue()));
    }
    qss.append(");}");
    ui->progressBar->setStyleSheet(qss);
}

void Play::goonPlay(int score)
{
    QString str = QString("select * from rank where username = '%1'").arg(username);
    QSqlQuery query;
    query.exec(str);
    // 如果排行榜中有该用户了
    if(query.next()){
        // 获取该用户的最高分，若是这次得分比最高分高，则更新最高分
        int max = query.value(2).toInt();
        if(score > max){
            str = QString("update rank set score = '%1' where username = '%2'").arg(score).arg(username);
            query.exec(str);
        }
    }else{
        str = QString("insert into rank (username, score) values ('%1', '%2')").arg(username).arg(score);
        query.exec(str);
    }

    QMessageBox::StandardButton result =
            QMessageBox::question(this, QString("完成"),
                          QString("完成！\n再来一局？"),
                          QMessageBox::Yes | QMessageBox::No,
                          QMessageBox::NoButton);
    // 再玩一局
    if(result==QMessageBox::Yes)
    {
        showPlay(level);
    }
    else // 跳回选择难度页面
    {
        emit returnChoose();
        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
        animation->setDuration(2000);
        animation->setStartValue(1);
        animation->setEndValue(0);
        animation->start();
        connect(animation, &QPropertyAnimation::finished, [=] {
            this->hide();
        });
    }

}

void Play::on_pauseBtn_clicked()
{
    bool isPause = time->isPause;
    if(isPause){
        time->isPause = false;
        // 继续计时
        time->start();
        worker->pTimer->start();
        // 取消遮住游戏区
        ui->label->hide();
        // 自动解题，重置和提示按钮恢复
        ui->solveBtn->setEnabled(true);
        ui->resetBtn->setEnabled(true);
        if(worker->times != 0) ui->remind->setEnabled(true);
    }else{
        // 暂停时停止计时
        time->isPause = true;
        time->terminate();
        worker->pTimer->stop();
        // 暂停时遮住游戏区
        ui->label->show();
        // 暂停的时候禁止自动解题，重置和提示
        ui->solveBtn->setEnabled(false);
        ui->resetBtn->setEnabled(false);
        ui->remind->setEnabled(false);
    }
}

void Play::getPoint()
{
    // 得分时增加时间（暂定1秒）
    curTime++;
    time->time = curTime;
    ui->timeBar->setValue(curTime);
}

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
    if(!worker->gameCheck()){
        return;
    }
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

void Play::getInfo(QString username)
{
    this->username = username;
}

void Play::on_resetBtn_clicked()
{
    if(worker->gameCheck()) worker->score -= 5;
    worker->remakeMap(ui->gridLayout);
    while(!worker->gameCheck()){
        worker->remakeMap(ui->gridLayout);
    }
    ui->resetHint->setText("无僵局");
    return;
}

void Play::on_solveBtn_clicked()
{
    // 开始自动解题前，禁用返回选择难度，提示，暂停，重置按钮
    ui->backBtn->setEnabled(false);
    ui->remind->setEnabled(false);
    ui->pauseBtn->setEnabled(false);
    ui->resetBtn->setEnabled(false);
    worker->autoSolve();
}

void Play::getHint(int sign)
{
    if(sign == 1){
        ui->resetHint->setText("有僵局");
    }else{
        ui->resetHint->setText("无僵局");
    }
}

void Play::on_backBtn_clicked()
{
    emit returnChoose();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(2000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, [=] {
        this->hide();
    });
}
