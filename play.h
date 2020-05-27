#ifndef PLAY_H
#define PLAY_H

#include <QDialog>
#include "time_thread.h"

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

private:
    Ui::Play *ui;

private slots:
    void showPlay(int level);
    void on_pauseBtn_clicked();
    void updateTime(int time);
};

#endif // PLAY_H
