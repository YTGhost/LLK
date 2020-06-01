#include "play.h"
#include "ui_play.h"
#include <QPropertyAnimation>
#include <QMessageBox>

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

    if (level == 1 || level == 2){
        setDifficulty(6, 6, 8);
    }
    else if (level == 4){
        setDifficulty(10, 14, 15);
    }
    else if(level == 3){
        setDifficulty(8, 10, 11);
    }

    //��ʼ��ͼƬ����Ч
    types = nullptr;
    btns = nullptr;
    for(int i=0; i<=16; i++)
        icons[i].load(QString::asprintf(":/images/%d.jpg", i));
    imageLine[0].load(":/images/line.jpg");
    QMatrix matrix;
    matrix.rotate(90.0);
    imageLine[1] = imageLine[0].transformed(matrix,
                                            Qt::FastTransformation);
    imageTurn[0].load(":/images/turn.jpg");
    for(int i=1; i<=3; i++)
        imageTurn[i] = imageTurn[i-1].transformed(matrix,
                                                  Qt::FastTransformation);
    clickSound.setSource(QUrl::fromLocalFile(":/sounds/clicks.wav"));
    endsSound.setSource(QUrl::fromLocalFile(":/sounds/ends.wav"));

    //����grid����
    ui->gridLayout->setHorizontalSpacing(1);
    ui->gridLayout->setVerticalSpacing(1);
    ui->gridLayout->setSizeConstraint(QLayout::SetFixedSize);
    ui->gridLayout->activate();

    //������
    stageClear();

    //���ó���
    setBlocks();


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

// test

void Play::btnsClicked() {
    //��ȡ������İ�ť������
    QObject *obj = qobject_cast<QPushButton *>(sender());
    int thisH=0, thisW=0;
    for(int i=1; i<=hb; i++)
        for(int j=1; j<=wb; j++)
            if(btns[i][j]==obj) {
                thisH = i;
                thisW = j;
                break;
            }

    //����Ѿ���������Ч
    if(types[thisH][thisW]==0)
        return ;

    //���û���Ѿ�����İ�ť���ͼ�¼��ǰ��ťΪ��һ������İ�ť
    if (!lastClickedH && !lastClickedW) {
        lastClickedH = thisH;
        lastClickedW = thisW;
        return ;
    }

    //����Ѿ��������Ч
    if(lastClickedH==thisH && lastClickedW==thisW)
        return ;

    //�����ͬ���͵���Ч
    if(types[lastClickedH][lastClickedW]!=types[thisH][thisW]) {
        lastClickedH = thisH;
        lastClickedW = thisW;
        return ;
    }

    //�ж���ε����ԵĺϷ���
    LinkPoints lp;
    if(canNoCorner(lastClickedH, lastClickedW, thisH, thisW, lp)
            || canOneCorner(lastClickedH, lastClickedW, thisH, thisW, lp)
            || canTwoCorner(lastClickedH, lastClickedW, thisH, thisW, lp)) {
        //���û�п�ʼ��Ϸ���Ϳ�ʼ��Ϸ��ͬʱ��ʼ��ʱ
        if(!isStarted) {
            isStarted = true;
            pTime->start();
            pTimer->start();
        }

        //�����Ч
        clickSound.play();

        //������������ͼƬ
        drawLines(lp);

        //��������ͼƬΪ�������
        types[lastClickedH][lastClickedW] = types[thisH][thisW] = 0;
        btns[thisH][thisW]->setIcon(icons[0]);
        ui->gridLayout->addWidget(btns[thisH][thisW],
                                  thisH, thisW, 1, 1,
                                  Qt::AlignCenter | Qt::AlignHCenter);
        btns[lastClickedH][lastClickedW]->setIcon(icons[0]);
        ui->gridLayout->addWidget(btns[lastClickedH][lastClickedW],
                                  lastClickedH, lastClickedW, 1, 1,
                                  Qt::AlignCenter | Qt::AlignHCenter);

        //���õ����ť
        lastClickedH = lastClickedW = 0;

        //�޸Ľ���
        remains -= 2;
        ui->progressBar->setValue(100*(hb*wb-remains)/(hb*wb));

        //�����ˣ�Ҫ��Ҫ������
        if (remains<=0) {
            pTimer->stop();
            endsSound.play();
            QMessageBox::StandardButton result =
                    QMessageBox::question(this, QString::fromLocal8Bit("���"),
                                  QString::fromLocal8Bit("��ɣ�\n����һ�֣�"),
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::NoButton);
            if(result==QMessageBox::No)
                exit(0);
//            on_actNewGame_triggered();
        }
    }
    else {
        //���Ϸ������
        lastClickedH = lastClickedW = 0;
    }
}

void Play::setDifficulty(int setHb, int setWb, int setFigure)
{
    hb = setHb;
    wb = setWb;
    figures = setFigure;
}

void Play::onTimeOut()
{
    //��ȡ���ŵ�ʱ�䲢����lcd
    int elapsedMSec = pTime->elapsed();
    if(elapsedMSec>60000)
        ui->lcdNumber->display(QString::asprintf("%d:%d.%3d",
                                                 elapsedMSec / 60000,
                                                 (elapsedMSec / 1000) % 60,
                                                 elapsedMSec % 1000));
    else
        ui->lcdNumber->display(QString::asprintf("%d.%3d",
                                                 elapsedMSec / 1000,
                                                 elapsedMSec % 1000));
}

void Play::setBlocks()
{
    //����ʣ���
    remains = hb * wb;

    //��ʼ����ʱ��
    pTimer = new QTimer;
    pTime = new QTime;
    pTimer->setInterval(60);
    connect(pTimer, SIGNAL(timeout()), this, SLOT(onTimeOut()));

    //��ʼ����ť���飬��ť���͡������2����������
    btns = new QPushButton **[hb+2];
    types = new int *[hb+2];
    for(int i=0; i<=hb+1; i++) {
        types[i] = new int [wb+2];
        btns[i] = new QPushButton *[wb+2];
    }
    int *typeList = new int [hb*wb];
    for(int i=0; i<hb*wb; i+=2)
        typeList[i] = typeList[i+1] = i/2 % figures + 1;

    //���������ͼ
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i=0; i<hb*wb; i++) {
        int idx=qrand()%(hb*wb-i)+i;
        int t=typeList[idx];
        typeList[idx] = typeList[i];
        typeList[i] = t;
        types[i/wb+1][i%wb+1] = typeList[i];
    }

    //���õ�ͼ
    for(int i=0; i<=hb+1; i++)
        for(int j=0; j<=wb+1; j++) {
            btns[i][j] = new QPushButton;
            if(i==0 || j==0 || i==hb+1 || j==wb+1) {
                types[i][j] = 0;
                btns[i][j]->setIcon(icons[types[i][j]]);
            }
            else {
                btns[i][j]->setIcon(icons[types[i][j]]);
                connect(btns[i][j], SIGNAL(clicked()),
                        this, SLOT(btnsClicked()));
            }
            btns[i][j]->setMinimumWidth(60);
            btns[i][j]->setMinimumHeight(60);
            btns[i][j]->setMaximumWidth(60);
            btns[i][j]->setMaximumHeight(60);
            btns[i][j]->setIconSize(QSize(60,60));
            ui->gridLayout->addWidget(btns[i][j],
                                      i, j, 1, 1,
                                      Qt::AlignCenter | Qt::AlignHCenter);
        }
}

bool Play::canNoCorner(int lstH, int lstW,
                             int thisH, int thisW, LinkPoints &lp)
{
    lp.cnt = 0;
    lp.pt[0][0] = lstH; lp.pt[0][1] = lstW;
    lp.pt[1][0] = thisH; lp.pt[1][1] = thisW;
    if (lstW==thisW) {
        int minH = lstH<thisH?lstH:thisH;
        int maxH = lstH + thisH - minH;
        for(int i=minH+1; i<=maxH-1; i++)
            if(types[i][lstW])
                return false;
        return true;
    }
    else if (lstH==thisH) {
        int minW = lstW<thisW?lstW:thisW;
        int maxW = lstW + thisW - minW;
        for(int i=minW+1; i<=maxW-1; i++)
            if(types[lstH][i])
                return false;
        return true;
    }
    return false;
}

bool Play::canOneCorner(int lstH, int lstW,
                              int thisH, int thisW, LinkPoints &lp)
{
    lp.cnt = 1;
    lp.pt[0][0] = lstH; lp.pt[0][1] = lstW;
    lp.pt[2][0] = thisH; lp.pt[2][1] = thisW;
    if(lstH==thisH || lstW==thisW)  return false;
    LinkPoints rubbish;
    if(types[lstH][thisW]==0) {
        lp.pt[1][0] = lstH; lp.pt[1][1] = thisW;
        if(canNoCorner(lstH, lstW, lstH, thisW, rubbish)
                && canNoCorner(lstH, thisW, thisH, thisW, rubbish))
            return true;
    }
    if(types[thisH][lstW]==0) {
        lp.pt[1][0] = thisH; lp.pt[1][1] = lstW;
        if(canNoCorner(lstH, lstW, thisH, lstW, rubbish)
                && canNoCorner(thisH, lstW, thisH, thisW, rubbish))
            return true;
    }
    return false;
}

bool Play::canTwoCorner(int lstH, int lstW,
                              int thisH, int thisW, LinkPoints &lp)
{
    lp.cnt = 2;
    lp.pt[0][0] = lstH; lp.pt[0][1] = lstW;
    lp.pt[3][0] = thisH; lp.pt[3][1] = thisW;
    LinkPoints rubbish;
    for(int i=0; i<=hb+1; i++) {
        if(i!=lstH && i!=thisH
                && !types[i][lstW] && !types[i][thisW]
                && canNoCorner(lstH, lstW, i, lstW, rubbish)
                && canOneCorner(i, lstW, thisH, thisW, rubbish)) {
            lp.pt[1][0] = i; lp.pt[1][1] = lstW;
            lp.pt[2][0] = i; lp.pt[2][1] = thisW;
            return true;
        }

        if(i!=thisH && i!=lstH
                && !types[i][thisW] && !types[i][lstW]
                && canOneCorner(lstH, lstW, i, thisW, rubbish)
                && canNoCorner(i, thisW, thisH, thisW, rubbish)) {
            lp.pt[1][0] = i; lp.pt[1][1] = thisW;
            lp.pt[2][0] = i; lp.pt[2][1] = lstW;
            return true;
        }
    }
    for(int i=0; i<=wb+1; i++) {
        if(i!=lstW && i!=thisW
                && !types[lstH][i] && !types[thisH][i]
                && canNoCorner(lstH, lstW, lstH, i, rubbish)
                && canOneCorner(lstH, i, thisH, thisW, rubbish)) {
            lp.pt[1][0] = lstH; lp.pt[1][1] = i;
            lp.pt[2][0] = thisH; lp.pt[2][1] = i;
            return true;
        }
        if(i!=thisW && i!=lstW
                && !types[thisH][i] && !types[lstH][i]
                && canOneCorner(lstH, lstW, thisH, i, rubbish)
                && canNoCorner(thisH, i, thisH, thisW, rubbish)) {
            lp.pt[1][0] = thisH; lp.pt[1][1] = i;
            lp.pt[2][0] = lstH; lp.pt[2][1] = i;
            return true;
        }
    }
    return false;
}

void Play::stageClear()
{
    lastClickedH = lastClickedW = 0;
    if(types==nullptr || btns==nullptr)
        return ;
    for(int i=0; i<=hb+1; i++) {
        if(btns[i]!=nullptr) {
            for(int j=0; j<=wb+1; j++) {
                if(btns[i][j]!=nullptr)
                    delete btns[i][j];
            }
            delete btns[i];
        }
        if(types[i]!=nullptr)   delete types[i];
    }
    delete types;
    delete btns;
    isStarted = false;
    if(pTimer!=nullptr) delete pTimer;
    if(pTime!=nullptr) delete pTime;
    ui->progressBar->setValue(0);
    ui->lcdNumber->display(0);
}

int Play::getDirection(int lstH, int lstW, int thisH,
                             int thisW, int midH, int midW)
{
    Q_UNUSED(midW)
    if(lstH>thisH) {
        int t=lstH; lstH = thisH;   thisH = t;
        t = lstW;   lstW = thisW;   thisW = t;
    }
    if(lstW<thisW) {
        if(midH==lstH)  return 2;
        else    return 0;
    }
    else {
        if(midH==lstH)  return 1;
        else    return 3;
    }
}

void Play::drawALine(int lstH, int lstW, int thisH,
                           int thisW, bool isW)
{
    if(lstW==thisW) {
        int minH=lstH<thisH?lstH:thisH;
        int maxH=lstH+thisH-minH;
        for(int i=minH+1; i<=maxH-1; i++) {
            btns[i][lstW]->setIcon(isW?icons[0]:imageLine[0]);
            ui->gridLayout->addWidget(btns[i][lstW], i, lstW,
                                      1, 1,
                                      Qt::AlignCenter | Qt::AlignHCenter);
        }
    }
    else {
        int minW=lstW<thisW?lstW:thisW;
        int maxW=lstW+thisW-minW;
        for(int i=minW+1; i<=maxW-1; i++) {
            btns[lstH][i]->setIcon(isW?icons[0]:imageLine[1]);
            ui->gridLayout->addWidget(btns[lstH][i], lstH, i,
                                      1, 1,
                                      Qt::AlignCenter | Qt::AlignHCenter);
        }
    }
}

void Play::drawLines(Play::LinkPoints lp)
{
    //��������
    for(int i=0; i<=lp.cnt; i++)
        drawALine(lp.pt[i][0], lp.pt[i][1],
                lp.pt[i+1][0], lp.pt[i+1][1], false);
    for(int i=1; i<=lp.cnt; i++) {
        btns[lp.pt[i][0]][lp.pt[i][1]]->setIcon(imageTurn[
                getDirection(lp.pt[i-1][0],
                lp.pt[i-1][1],
                lp.pt[i+1][0], lp.pt[i+1][1],
                lp.pt[i][0], lp.pt[i][1])]);
        ui->gridLayout->addWidget(btns[lp.pt[i][0]][lp.pt[i][1]],
                lp.pt[i][0], lp.pt[i][1],
                1, 1,
                Qt::AlignCenter | Qt::AlignHCenter);
    }

    //�ȴ�0.2s
    QTest::qWait(200);

    //ȡ�����ߣ����׿飩
    for(int i=0; i<=lp.cnt; i++)
        drawALine(lp.pt[i][0], lp.pt[i][1],
                lp.pt[i+1][0], lp.pt[i+1][1], true);
    for(int i=1; i<=lp.cnt; i++) {
        btns[lp.pt[i][0]][lp.pt[i][1]]->setIcon(icons[0]);
        ui->gridLayout->addWidget(btns[lp.pt[i][0]][lp.pt[i][1]],
                lp.pt[i][0], lp.pt[i][1],
                1, 1,
                Qt::AlignCenter | Qt::AlignHCenter);
    }
}
