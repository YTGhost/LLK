#include "rank.h"
#include "ui_rank.h"
#include <QPropertyAnimation>

Rank::Rank(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Rank)
{
    ui->setupUi(this);
}

Rank::~Rank()
{
    delete ui;

}

void Rank::showRank()
{
    select();
    setWindowOpacity(0);
    this->show();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(2000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
}

void Rank::select(){
    list.clear();
    QString str = QString("select * from rank");
    QSqlQuery query;
    query.exec(str);
    while(query.next())
    {
        list.push_back(make_pair(query.value(1).toString(), query.value(2).toInt()));
    }
    sort(list.begin(), list.end(), [](pair<QString, int> a, pair<QString, int> b)
    {
        return a.second > b.second;
    });
    for(int i = 0; i < list.size(); i++)
    {
        ui->textBrowser->insertPlainText("\t"+QString::number(i+1)+"\t\t"+list[i].first+"\t\t"+QString::number(list[i].second)+"\n");
    }
}

void Rank::on_backBtn_clicked()
{
    emit toMain();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(2000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, [=] {
        this->hide();
    });
}
