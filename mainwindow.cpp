#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    musicPlayerList = new QMediaPlaylist;
    musicPlayerList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

    videoWidget = new QVideoWidget();
    musicPlayer = new QMediaPlayer(this);

    musicPlayer->setVideoOutput(videoWidget);
    musicPlayerList->addMedia(QUrl("qrc:/media/background.mp3"));
    musicPlayer->setPlaylist(musicPlayerList);
    musicPlayer->setVolume(ui->musicSlider->value());
    musicPlayer->play();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMain()
{
    setWindowOpacity(0);
    this->show();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(2000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
}

void MainWindow::on_playBtn_clicked()
{
    emit toChoose();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(2000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, [=] {
        this->hide();
    });
}

void MainWindow::on_rankBtn_clicked()
{
    emit toRank();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(2000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, [=] {
        this->hide();
    });
}

void MainWindow::on_backBtn_clicked()
{
    emit toLogin();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(2000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, [=] {
        this->hide();
    });
}

void MainWindow::on_musicSlider_sliderMoved(int position)
{
    musicPlayer->setVolume(position);
}

