#include "forget.h"
#include "ui_forget.h"
#include <QPropertyAnimation>
#include "smtp.h"
#include <QSqlQuery>

Forget::Forget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Forget)
{
    ui->setupUi(this);

    ui->emailLineEdit->setPlaceholderText(QString::fromLocal8Bit("请输入邮箱地址"));
    ui->codeLineEdit->setPlaceholderText(QString::fromLocal8Bit("请输入验证码"));
    ui->passwordLineEdit->setPlaceholderText(QString::fromLocal8Bit("请输入密码"));
    ui->confirmLineEdit->setPlaceholderText(QString::fromLocal8Bit("请确认密码"));

    ui->passwordLineEdit->setClearButtonEnabled(true);
    ui->confirmLineEdit->setClearButtonEnabled(true);
    ui->emailLineEdit->setClearButtonEnabled(true);

    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->confirmLineEdit->setEchoMode(QLineEdit::Password);

    connect(ui->emailLineEdit, SIGNAL(textChanged(QString)), this, SLOT(emailValidator()));
    connect(ui->passwordLineEdit, SIGNAL(textChanged(QString)), this, SLOT(passwordValidator()));
    connect(ui->confirmLineEdit, SIGNAL(textChanged(QString)), this, SLOT(confirmValidator()));
}

Forget::~Forget()
{
    delete ui;
}

void Forget::emailValidator()
{
    QString image = "error";
    QString content = "Illegal mailbox";
    QString color = "red";
    checkEmail = 0;
    QString pattern("^[a-z0-9A-Z]+[- | a-z0-9A-Z . _]+@([a-z0-9A-Z]+(-[a-z0-9A-Z]+)?\\.)+[a-z]{2,}$");
    QRegExp rx(pattern);
    if(ui->emailLineEdit->text().contains(rx)){
        image = "correct";
        content = "Verified!";
        color = "green";
        checkEmail = 1;
    }
    ui->emailLabel->setText(content);
    ui->emailLabel->setStyleSheet(QString("QLabel {color: %1; background-image: url(:/pic/icon/%2.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;}").arg(color).arg(image));
}

void Forget::passwordValidator()
{
    QString image = "error";
    QString content = "8~16 digits letters and numbers";
    QString color = "red";
    checkPassword = 0;
    QString pattern("^(?![0-9]+$)(?![a-zA-Z]+$)[0-9A-Za-z]{8,16}$");
    QRegExp rx(pattern);
    if(ui->passwordLineEdit->text().contains(rx)){
        image = "correct";
        content = "Verified!";
        color = "green";
        checkPassword = 1;
    }
    ui->passwordLabel->setText(content);
    ui->passwordLabel->setStyleSheet(QString("QLabel {color: %1; background-image: url(:/pic/icon/%2.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;}").arg(color).arg(image));
}

void Forget::confirmValidator()
{
    QString image = "error";
    QString content = "Passwords are inconsistent";
    QString color = "red";
    checkConfirm = 0;
    if(ui->confirmLineEdit->text() == ui->passwordLineEdit->text()){
        image = "correct";
        content = "Verified!";
        color = "green";
        checkConfirm = 1;
    }
    ui->confirmLabel->setText(content);
    ui->confirmLabel->setStyleSheet(QString("QLabel {color: %1; background-image: url(:/pic/icon/%2.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;}").arg(color).arg(image));
}

void Forget::showForget()
 {
     setWindowOpacity(0);
     this->show();
     QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
     animation->setDuration(2000);
     animation->setStartValue(0);
     animation->setEndValue(1);
     animation->start();
 }

void Forget::on_sendBtn_clicked()
{
    // 检验输入的邮箱格式是否正确
    if(checkEmail != 1){
        QMessageBox::warning(NULL, QString::fromLocal8Bit("发送失败"), QString::fromLocal8Bit("请输入正确的邮箱地址"), QString::fromLocal8Bit("确定"));
        return;
    }
    QString email = ui->emailLineEdit->text();
    // 查询数据库是否存在该邮箱
    QString str = QString("select * from user_info where email = '%1'").arg(email);
    QSqlQuery query;
    query.exec(str);
    // 如果没有找到该邮箱
    if(!query.next()){
        QMessageBox::warning(NULL, QString::fromLocal8Bit("验证失败"), QString::fromLocal8Bit("请输入正确的邮箱地址"), QString::fromLocal8Bit("确定"));
        return;
    }
    // 设置随机数种子
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    // 重置验证码
    code = "";
    // 生成验证码
    for(int i = 0; i < 6; i++)
    {
        code.append(QString::number(qrand() % 9));
    }
    // 调试输出验证码
    cout << code.toLocal8Bit().toStdString() << endl;
    Smtp *smtp = new Smtp();
    smtp->send(email.toUtf8(), QString::fromLocal8Bit("请查收验证码"), QString::fromLocal8Bit("您的验证码为:%1,祝您游戏愉快!^_^").arg(code));
    QMessageBox::information(NULL, QString::fromLocal8Bit("发送成功"), QString::fromLocal8Bit("请前往邮箱查看验证码"), QString::fromLocal8Bit("确定"));
}

void Forget::on_changeBtn_clicked()
{
    QString inputCode = ui->codeLineEdit->text();
    QString email = ui->emailLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    if(inputCode == "" || inputCode != code){
        QMessageBox::warning(NULL, QString::fromLocal8Bit("验证失败"), QString::fromLocal8Bit("请输入正确的验证码"), QString::fromLocal8Bit("确定"));
        return;
    }
    if(checkPassword == 0){
        QMessageBox::warning(NULL, QString::fromLocal8Bit("密码格式错误"), QString::fromLocal8Bit("请输入符合规则的密码"), QString::fromLocal8Bit("确定"));
        return;
    }
    if(checkConfirm == 0){
        QMessageBox::warning(NULL, QString::fromLocal8Bit("两次密码不一致"), QString::fromLocal8Bit("请重新检查输入"), QString::fromLocal8Bit("确定"));
        return;
    }
    QString str = QString("update user_info set password = '%1' where email = '%2'").arg(password).arg(email);
    QSqlQuery query;
    query.exec(str);
    QMessageBox::information(NULL, QString::fromLocal8Bit("修改密码"), QString::fromLocal8Bit("修改成功，您可以使用新密码进行登录了！"), QString::fromLocal8Bit("确定"));
    return;
}

void Forget::on_backBtn_clicked()
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
