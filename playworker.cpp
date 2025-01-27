﻿#include "playworker.h"
#include "play.h"

playworker::playworker(QObject *parent) : QObject(parent)
{
    times = 3;
}

void playworker::setDifficulty(int setHb, int setWb, int setFigure)
{
    hb = setHb;
    wb = setWb;
    figures = setFigure;
}

void playworker::initImages()
{
    types = nullptr;
    btns = nullptr;
    for(int i=0; i<=15; i++)
        icons[i].load(QString::asprintf(":/pic/pattern/%d.png", i));
    imageLine[0].load(":/pic/pattern/line.jpg");
    QMatrix matrix;
    matrix.rotate(90.0);
    imageLine[1] = imageLine[0].transformed(matrix,
                                            Qt::FastTransformation);
    imageTurn[0].load(":/pic/pattern/turn.jpg");
    for(int i=1; i<=3; i++)
        imageTurn[i] = imageTurn[i-1].transformed(matrix,
                                                  Qt::FastTransformation);
}

void playworker::initSounds()
{
    clickSound.setSource(QUrl::fromLocalFile(":/sounds/clicks.wav"));
    endsSound.setSource(QUrl::fromLocalFile(":/sounds/ends.wav"));
    failSound.setSource(QUrl::fromLocalFile(":/sounds/fail.wav"));
}

void playworker::stageClear()
{
    isStarted = false;

    lastClickedH = lastClickedW = 0;
    if(pTimer!=nullptr) delete pTimer;
    if(pTime!=nullptr) delete pTime;

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
}

void playworker::setBlocks(QGridLayout *uiGridLayout)
{
    m_uiGridLayout = uiGridLayout;
    //重置剩余块
    remains = hb * wb;

    //初始化计时器
    pTimer = new QTimer;
    pTime = new QTime;
    pTimer->setInterval(60);
    connect(pTimer, SIGNAL(timeout()), this, SLOT(onTimeOut()));

    //初始化按钮数组，按钮类型。长宽加2，边上留空
    btns = new QPushButton **[hb+2];
    types = new int *[hb+2];
    for(int i=0; i<=hb+1; i++) {
        types[i] = new int [wb+2];
        btns[i] = new QPushButton *[wb+2];
    }
    int *typeList = new int [hb*wb];
    for(int i=0; i<hb*wb; i+=2)
        typeList[i] = typeList[i+1] = i/2 % figures + 1;

    //生成随机地图
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i=0; i<hb*wb; i++) {
        int idx=qrand()%(hb*wb-i)+i;
        int t=typeList[idx];
        typeList[idx] = typeList[i];
        typeList[i] = t;
        types[i/wb+1][i%wb+1] = typeList[i];
    }

    //设置地图
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
            uiGridLayout->addWidget(btns[i][j],
                                      i, j, 1, 1,
                                      Qt::AlignCenter | Qt::AlignHCenter);
        }
}

void playworker::btnsClicked() {
    //获取被点击的按钮的坐标
    QObject *obj = qobject_cast<QPushButton *>(sender());
    int thisH=0, thisW=0;
    for(int i=1; i<=hb; i++)
        for(int j=1; j<=wb; j++)
            if(btns[i][j]==obj) {
                thisH = i;
                thisW = j;
                break;
            }

    //点击已经消除的无效
    if(types[thisH][thisW]==0)
        return ;

    //如果没有已经点击的按钮，就记录当前按钮为第一个点击的按钮
    if (!lastClickedH && !lastClickedW) {
        lastClickedH = thisH;
        lastClickedW = thisW;
        return ;
    }

    //点击已经点过的无效
    if(lastClickedH==thisH && lastClickedW==thisW)
        return ;

    //点击不同类型的无效
    if(types[lastClickedH][lastClickedW]!=types[thisH][thisW]) {
        lastClickedH = thisH;
        lastClickedW = thisW;
        return ;
    }

    //判定这次点击配对的合法性
    LinkPoints lp;
    if(canNoCorner(lastClickedH, lastClickedW, thisH, thisW, lp)
            || canOneCorner(lastClickedH, lastClickedW, thisH, thisW, lp)
            || canTwoCorner(lastClickedH, lastClickedW, thisH, thisW, lp)) {
        //如果没有开始游戏，就开始游戏。同时开始计时
        if(!isStarted) {
            isStarted = true;
            pTime->start();
            pTimer->start();
        }

        //点击音效
        clickSound.play();
        if(flag==1){
            score=score+1;
        }
        if(flag==2){
            score=score+2;
        }
        if(flag==3){
            score=score+3;
        }

        //画线连接两张图片
        drawLines(lp);

        //设置两张图片为被点击了
        types[lastClickedH][lastClickedW] = types[thisH][thisW] = 0;
        btns[thisH][thisW]->setIcon(icons[0]);
        m_uiGridLayout->addWidget(btns[thisH][thisW],
                                  thisH, thisW, 1, 1,
                                  Qt::AlignCenter | Qt::AlignHCenter);
        btns[lastClickedH][lastClickedW]->setIcon(icons[0]);
        m_uiGridLayout->addWidget(btns[lastClickedH][lastClickedW],
                                  lastClickedH, lastClickedW, 1, 1,
                                  Qt::AlignCenter | Qt::AlignHCenter);

        //重置点击按钮
        lastClickedH = lastClickedW = 0;

        //修改进度
        remains -= 2;
        emit updateProgressBar((100*(hb*wb-remains)/(hb*wb)));

        // 无解的话提示玩家重置
        if(!gameCheck()){
            emit toHint(1);
        }else{
            emit toHint(0);
        }

        //结束了，要不要继续玩
        if (remains<=0) {
            pTimer->stop();
            endsSound.play();
            emit goonPlay(score, 1);
        }
    }
    else {
        //不合法的配对
        lastClickedH = lastClickedW = 0;
    }
}

void playworker::onTimeOut()
{
    //获取流逝的时间并更新lcd
    int elapsedMSec = pTime->elapsed();
    if(elapsedMSec>60000)
        emit updateLcdNumber((QString::asprintf("%d:%d.%3d",
                                                 elapsedMSec / 60000,
                                                 (elapsedMSec / 1000) % 60,
                                                 elapsedMSec % 1000)));
    else
        emit updateLcdNumber(QString::asprintf("%d.%3d",
                                                 elapsedMSec / 1000,
                                                 elapsedMSec % 1000));
}

bool playworker::canNoCorner(int lstH, int lstW,
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
        flag=1;
        return true;
    }
    else if (lstH==thisH) {
        int minW = lstW<thisW?lstW:thisW;
        int maxW = lstW + thisW - minW;
        for(int i=minW+1; i<=maxW-1; i++)
            if(types[lstH][i])
                return false;
        flag=1;
        return true;
    }
    return false;
}

bool playworker::canOneCorner(int lstH, int lstW,
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
                && canNoCorner(lstH, thisW, thisH, thisW, rubbish)){
            flag=2;
            return true;
        }
    }
    if(types[thisH][lstW]==0) {
        lp.pt[1][0] = thisH; lp.pt[1][1] = lstW;
        if(canNoCorner(lstH, lstW, thisH, lstW, rubbish)
                && canNoCorner(thisH, lstW, thisH, thisW, rubbish)){
            flag=2;
            return true;
        }

    }
    return false;
}

bool playworker::canTwoCorner(int lstH, int lstW,
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
            flag=3;
            return true;
        }

        if(i!=thisH && i!=lstH
                && !types[i][thisW] && !types[i][lstW]
                && canOneCorner(lstH, lstW, i, thisW, rubbish)
                && canNoCorner(i, thisW, thisH, thisW, rubbish)) {
            lp.pt[1][0] = i; lp.pt[1][1] = thisW;
            lp.pt[2][0] = i; lp.pt[2][1] = lstW;
            flag=3;
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
            flag=3;
            return true;
        }
        if(i!=thisW && i!=lstW
                && !types[thisH][i] && !types[lstH][i]
                && canOneCorner(lstH, lstW, thisH, i, rubbish)
                && canNoCorner(thisH, i, thisH, thisW, rubbish)) {
            lp.pt[1][0] = thisH; lp.pt[1][1] = i;
            lp.pt[2][0] = lstH; lp.pt[2][1] = i;
            flag=3;
            return true;
        }
    }
    return false;
}

int playworker::getDirection(int lstH, int lstW, int thisH,
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

void playworker::drawALine(int lstH, int lstW, int thisH,int thisW, bool isW)
{
    if(lstW==thisW) {
        int minH=lstH<thisH?lstH:thisH;
        int maxH=lstH+thisH-minH;
        for(int i=minH+1; i<=maxH-1; i++) {
            btns[i][lstW]->setIcon(isW?icons[0]:imageLine[0]);
            m_uiGridLayout->addWidget(btns[i][lstW], i, lstW,
                                      1, 1,
                                      Qt::AlignCenter | Qt::AlignHCenter);
        }
    }
    else {
        int minW=lstW<thisW?lstW:thisW;
        int maxW=lstW+thisW-minW;
        for(int i=minW+1; i<=maxW-1; i++) {
            btns[lstH][i]->setIcon(isW?icons[0]:imageLine[1]);
            m_uiGridLayout->addWidget(btns[lstH][i], lstH, i,
                                      1, 1,
                                      Qt::AlignCenter | Qt::AlignHCenter);
        }
    }
}

void playworker::drawLines(LinkPoints lp)
{
    //画线连接
    for(int i=0; i<=lp.cnt; i++)
        drawALine(lp.pt[i][0], lp.pt[i][1],
                lp.pt[i+1][0], lp.pt[i+1][1], false);
    for(int i=1; i<=lp.cnt; i++) {
        btns[lp.pt[i][0]][lp.pt[i][1]]->setIcon(imageTurn[
                getDirection(lp.pt[i-1][0],
                lp.pt[i-1][1],
                lp.pt[i+1][0], lp.pt[i+1][1],
                lp.pt[i][0], lp.pt[i][1])]);
        m_uiGridLayout->addWidget(btns[lp.pt[i][0]][lp.pt[i][1]],
                lp.pt[i][0], lp.pt[i][1],
                1, 1,
                Qt::AlignCenter | Qt::AlignHCenter);
    }

    //等待0.2s
    QTest::qWait(200);

    //取消画线（画白块）
    for(int i=0; i<=lp.cnt; i++)
        drawALine(lp.pt[i][0], lp.pt[i][1],
                lp.pt[i+1][0], lp.pt[i+1][1], true);
    for(int i=1; i<=lp.cnt; i++) {
        btns[lp.pt[i][0]][lp.pt[i][1]]->setIcon(icons[0]);
        m_uiGridLayout->addWidget(btns[lp.pt[i][0]][lp.pt[i][1]],
                lp.pt[i][0], lp.pt[i][1],
                1, 1,
                Qt::AlignCenter | Qt::AlignHCenter);
    }
}

void playworker::gameRemind()
{
    for(int i=1; i<=hb; i++)
    {

        for(int j=1; j<=wb; j++)
        {
            if(types[i][j]==0)
                continue;

            for (int k = i; k <= hb; ++k)
            {
                int l = 0;
                if (k > i) l = 1;
                else l = 1 + j;
                for (; l <= wb; ++l)
                {
                    if(types[k][l]==0)
                        continue;

                    if(types[i][j]!=types[k][l])
                        continue;

                    LinkPoints lp;
                    if(canNoCorner(i, j, k, l, lp)
                            || canOneCorner(i, j, k, l, lp)
                            || canTwoCorner(i, j, k, l, lp)) {

                        btns[i][j]->hide();
                        btns[k][l]->hide();

                        QTest::qWait(100);

                        btns[i][j]->show();
                        btns[k][l]->show();

                        QTest::qWait(50);

                        btns[i][j]->hide();
                        btns[k][l]->hide();

                        QTest::qWait(100);

                        btns[i][j]->show();
                        btns[k][l]->show();

                        drawLines(lp);

                        return;
                    }
                }
            }
        }
    }
}

int playworker::gameCheck()
{
    for(int i=1; i<=hb; i++)
    {

        for(int j=1; j<=wb; j++)
        {
            if(types[i][j]==0)
                continue;

            for (int k = i; k <= hb; ++k)
            {
                int l = 0;
                if (k > i) l = 1;
                else l = 1 + j;
                for (; l <= wb; ++l)
                {
                    if(types[k][l]==0)
                        continue;

                    if(types[i][j]!=types[k][l])
                        continue;

                    LinkPoints lp;
                    if(canNoCorner(i, j, k, l, lp)
                            || canOneCorner(i, j, k, l, lp)
                            || canTwoCorner(i, j, k, l, lp)) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

vector<int> playworker::gameRemind2(LinkPoints &lpp)
{
    for(int i=1; i<=hb; i++)
    {

        for(int j=1; j<=wb; j++)
        {
            if(types[i][j]==0)
                continue;

            for (int k = i; k <= hb; ++k)
            {
                int l = 0;
                if (k > i) l = 1;
                else l = 1 + j;
                for (; l <= wb; ++l)
                {
                    if(types[k][l]==0)
                        continue;

                    if(types[i][j]!=types[k][l])
                        continue;

                    LinkPoints lp;
                    if(canNoCorner(i, j, k, l, lp)
                            || canOneCorner(i, j, k, l, lp)
                            || canTwoCorner(i, j, k, l, lp)) {

                        btns[i][j]->hide();
                        btns[k][l]->hide();

                        QTest::qWait(100);

                        btns[i][j]->show();
                        btns[k][l]->show();

                        QTest::qWait(50);

                        btns[i][j]->hide();
                        btns[k][l]->hide();

                        QTest::qWait(100);

                        btns[i][j]->show();
                        btns[k][l]->show();

                        drawLines(lp);

                        lpp = lp;
                        return vector<int>{i, j, k, l};
                    }
                }
            }
        }
    }
    return vector<int>{};
}

int playworker::getTimes()
{
    return this->times;
}

void playworker::setTimes(int num)
{
    times = num;
}

void playworker::remakeMap(QGridLayout *uiMyGridLayout)
{
    int still = remains;
    int remaining[still];  //用来存放没消掉的blocks
    memset(remaining, 0, sizeof(remaining));
    int k = 0;

    //把没消掉的blocks存入数组
    for(int i=0;i<=hb+1;i++){
        for(int j=0;j<=wb+1;j++){
            if(types[i][j]!=0){
                remaining[k] = types[i][j];
                k++;
            }
        }
    }

    //将blocks打乱放入另一个数组
    std::random_shuffle(remaining,remaining+still);

    //将存放的blocks重新放入地图
    int a = 0;
    for(int i=0;i<=hb+1;i++){
        for(int j=0;j<=wb+1;j++){
            if(types[i][j]!=0){
                types[i][j] = remaining[a];
                a++;
            }
        }
    }

    //设置地图
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
            uiMyGridLayout->addWidget(btns[i][j],
                                      i, j, 1, 1,
                                      Qt::AlignCenter | Qt::AlignHCenter);
        }
}

void playworker::autoSolve()
{   
    // 若开始自动解题时没有解，重置到有解
    if(!gameCheck()){
        while(!gameCheck()){
            remakeMap(m_uiGridLayout);
        }
    }

    while(1)
    {
        LinkPoints lp;
        vector<int> t = gameRemind2(lp);
        int lastClickedH = t[0];
        int lastClickedW = t[1];
        int thisH = t[2];
        int thisW = t[3];
        if(t.size() != 0){
            //点击音效
            clickSound.play();

            //画线连接两张图片
            drawLines(lp);

            //设置两张图片为被点击了
            types[lastClickedH][lastClickedW] = types[thisH][thisW] = 0;
            btns[thisH][thisW]->setIcon(icons[0]);
            m_uiGridLayout->addWidget(btns[thisH][thisW],
                                      thisH, thisW, 1, 1,
                                      Qt::AlignCenter | Qt::AlignHCenter);
            btns[lastClickedH][lastClickedW]->setIcon(icons[0]);
            m_uiGridLayout->addWidget(btns[lastClickedH][lastClickedW],
                                      lastClickedH, lastClickedW, 1, 1,
                                      Qt::AlignCenter | Qt::AlignHCenter);

            //重置点击按钮
            lastClickedH = lastClickedW = 0;

            //修改进度
            remains -= 2;
            emit updateProgressBar((100*(hb*wb-remains)/(hb*wb)));

            //结束了，要不要继续玩
            if (remains<=0) {
                pTimer->stop();
                endsSound.play();
                emit goonPlay(score, 1);
                return;
            }
            // 消完一对后若没有解，重置到有解的状态
            if(!gameCheck()){
                while(!gameCheck()){
                    remakeMap(m_uiGridLayout);
                }
            }
        }
    }
}

void playworker::fail()
{
    pTimer->stop();
    // 失败音乐
    failSound.play();
    emit goonPlay(score, 0);
    return;
}
