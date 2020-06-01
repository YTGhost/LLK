#include "rank.h"
#include "ui_rank.h"
#include <QPropertyAnimation>

Rank::Rank(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Rank)
{
    ui->setupUi(this);
    select();
}

Rank::~Rank()
{
    delete ui;

}

void Rank::showRank()
{
    setWindowOpacity(0);
    this->show();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(2000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
}

void Rank::select(){
    QString s=QString("select * from rank ");
    QSqlQuery query;
    int score[10],i=1,k=1;
    query.exec(s);
    while (query.next())
        {
             score[i]=query.value(2).toInt();
             //qDebug()<<score[i];
             i++;
        }
    while(k<i){
        int temp=0;
        int count=0;
        for(int j=1;j<i;j++){
            if(temp<score[j]){
                temp=score[j];
                count=j;
            }
        }

        QString s=QString("select username from rank where id=%1").arg(count);
        QSqlQuery query;
        query.exec(s);
        QString name;
        while (query.next())
            {
                 name=query.value(0).toString();
                 ui->textBrowser->insertPlainText("\t"+QString::number(k)+"\t\t"+name+"\t\t"+QString::number(temp)+"\n");
            }
        score[count]=0;
        k++;
    }

}
