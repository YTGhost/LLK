#include "login.h"
#include "ui_login.h"

#include <QPropertyAnimation>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    // 连接数据库
    helper = new databasehelper();

    // 设置GUI样式
    QString dialogQSS = "QDialog {background:qlineargradient(spread:pad,x1:1,y1:0,x2:0,y2:0,stop:0 #c31399,stop:1 #9118fd)}";
    QString widgetQSS = "QWidget {background-color: white; border-radius: 10px}";
    QString btnQSS = "QPushButton {background-color: #9013fb; color: #FFFFFF; border-radius: 8px; font-weight:bold;}";
    QString StyleSheetUser = "QLineEdit { background-image: url(:/pic/icon/user.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;} QLineEdit:focus { background-image: url(:/pic/icon/user-focused.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left; }";
    QString StyleSheetPassword = "QLineEdit { background-image: url(:/pic/icon/lock.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;} QLineEdit:focus { background-image: url(:/pic/icon/lock-focused.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left; }";

    QGraphicsScene *scene = new QGraphicsScene();
    scene->addPixmap(QPixmap(":/pic/background/name.jpg").scaled(200, 80, Qt::KeepAspectRatio));
    ui->graphicsView->setScene(scene);
    ui->userLineEdit->setPlaceholderText("User");
    ui->passwordLineEdit->setPlaceholderText("Password");
    ui->userLineEdit->setClearButtonEnabled(true);
    ui->passwordLineEdit->setClearButtonEnabled(true);
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);

    this->setStyleSheet(dialogQSS);
    ui->widget->setStyleSheet(widgetQSS);
    ui->loginBtn->setStyleSheet(btnQSS);
    ui->registerBtn->setStyleSheet(btnQSS);
    ui->forgetBtn->setStyleSheet(btnQSS);
    ui->userLineEdit->setStyleSheet(StyleSheetUser);
    ui->passwordLineEdit->setStyleSheet(StyleSheetPassword);

}

Login::~Login()
{
    delete ui;
}

void Login::on_loginBtn_clicked()
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

void Login::on_registerBtn_clicked()
{
    emit toRegister();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(2000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, [=] {
        this->hide();
    });
}
