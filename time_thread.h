#ifndef TIME_THREAD_H
#define TIME_THREAD_H
#include <QThread>
#include <QTimer>

class time_thread:public QThread
{
    Q_OBJECT
public:
    explicit time_thread();
    time_thread(int time);
    void run();
    int time;
    bool isPause;
    QTimer* timer;

private slots:
    void qtimeSlot();

signals:
    void updateUI(int time);
};

#endif // TIME_THREAD_H
