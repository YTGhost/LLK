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
    // �Ѷȣ�1Ϊ���ţ�2Ϊ������3Ϊ�м���4Ϊ�߼�
    int level;
    time_thread *time;
    int curTime;

    // test
    QPushButton ***btns;//pushbutton����
    int hb, wb;//�߶��ٸ����ӣ�����ٸ�����
    int lastClickedH, lastClickedW;//��һ�����������
    int remains;//ʣ����ٸ�����
    int **types;//���ӵ����͵����顣0���������ˣ�1-15���Ӵ���һ�ָ��ӣ�ͼƬ��
    int figures;//�����ָ��ӣ�ͼƬ��
    QPixmap icons[16];//ͼƬ����
    QPixmap imageLine[2], imageTurn[4];//���ߵ�ͼƬ������
    QTimer *pTimer;//��ʱ��
    QTime *pTime;//��ʱ��
    QSoundEffect clickSound;//�����Ч
    QSoundEffect endsSound;//��Ϸ������Ч
    bool isStarted;//��Ϸ��ʼ����

    //���������Ե����Ե���м�˵㹹�ɵĽṹ��
    struct LinkPoints{
        int cnt;
        int pt[4][2];//0�ǵ�һ���˵㣬cnt+1�ǵڶ����˵㣬�������м��
    };

    //������Ϸ�ߡ�������
    void setDifficulty(int setHb, int setWb, int setFigure);

    //������Ϸ����
    void setBlocks();

    //�ж��ܷ񲻹ա�һ�ա���������
    bool canNoCorner(int lstH, int lstW, int thisH, int thisW, LinkPoints &lp);
    bool canOneCorner(int lstH, int lstW, int thisH, int thisW, LinkPoints &lp);
    bool canTwoCorner(int lstH, int lstW, int thisH, int thisW, LinkPoints &lp);

    //�峡
    void stageClear();

    //���һ���㵽��һ����ľ������м��ķ���
    int getDirection(int lstH, int lstW, int thisH, int thisW, int midH, int midW);

    //��һ���ߺ�һ����
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
