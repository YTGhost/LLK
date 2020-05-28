#include "register.h"
#include "ui_register.h"
#include <QPropertyAnimation>

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

    // ÉèÖÃGUIÑùÊ½
    QString dialogQSS = "QDialog {background:qlineargradient(spread:pad,x1:1,y1:0,x2:0,y2:0,stop:0 #0082bc,stop:1 #01d8db)}";
    QString widgetQSS = "QWidget {background-color: white; border-radius: 10px}";
    QString btnQSS = "QPushButton {background-color: #0082bc; color: #FFFFFF; border-radius: 8px; font-weight:bold;}";
    QString StyleSheetUser = "QLineEdit { background-image: url(:/pic/icon/user.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;} QLineEdit:focus { background-image: url(:/pic/icon/user-focused.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left; }";
    QString StyleSheetPassword = "QLineEdit { background-image: url(:/pic/icon/lock.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;} QLineEdit:focus { background-image: url(:/pic/icon/lock-focused.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left; }";
    QString StyleSheetEmail = "QLineEdit { background-image: url(:/pic/icon/email.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;} QLineEdit:focus { background-image: url(:/pic/icon/email-focused.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left; }";
    QString StyleSheetCode = "QLineEdit { background-image: url(:/pic/icon/confirm.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;} QLineEdit:focus { background-image: url(:/pic/icon/confirm-focused.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left; }";

    ui->userLineEdit->setPlaceholderText("User");
    ui->passwordLineEdit->setPlaceholderText("Password");
    ui->confirmLineEdit->setPlaceholderText("confirm password");
    ui->emailLineEdit->setPlaceholderText("email");
    ui->codeLineEdit->setPlaceholderText("confirm code");

    ui->userLineEdit->setClearButtonEnabled(true);
    ui->passwordLineEdit->setClearButtonEnabled(true);
    ui->confirmLineEdit->setClearButtonEnabled(true);
    ui->emailLineEdit->setClearButtonEnabled(true);

    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->confirmLineEdit->setEchoMode(QLineEdit::Password);

    this->setStyleSheet(dialogQSS);
    ui->widget->setStyleSheet(widgetQSS);
    ui->registerBtn->setStyleSheet(btnQSS);
    ui->userLineEdit->setStyleSheet(StyleSheetUser);
    ui->passwordLineEdit->setStyleSheet(StyleSheetPassword);
    ui->confirmLineEdit->setStyleSheet(StyleSheetPassword);
    ui->emailLineEdit->setStyleSheet(StyleSheetEmail);
    ui->codeLineEdit->setStyleSheet(StyleSheetCode);

    connect(ui->userLineEdit, SIGNAL(textChanged(QString)), this, SLOT(userValidator()));
    connect(ui->passwordLineEdit, SIGNAL(textChanged(QString)), this, SLOT(passwordValidator()));
    connect(ui->confirmLineEdit, SIGNAL(textChanged(QString)), this, SLOT(confirmValidator()));
    connect(ui->emailLineEdit, SIGNAL(textChanged(QString)), this, SLOT(emailValidator()));
}

Register::~Register()
{
    delete ui;
}

void Register::showRegister()
{
    setWindowOpacity(0);
    this->show();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(2000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
}

void Register::userValidator()
{
    QString image = "error";
    QString content = "Length of 1~10";
    QString color = "red";
    QString pattern("^.{1,10}$");
    QRegExp rx(pattern);
    if(ui->userLineEdit->text().contains(rx)){
        image = "correct";
        content = "Verified!";
        color = "green";
    }
    ui->userLabel->setText(content);
    ui->userLabel->setStyleSheet(QString("QLabel {color: %1; background-image: url(:/pic/icon/%2.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;}").arg(color).arg(image));
}

void Register::passwordValidator()
{
    QString image = "error";
    QString content = "8~16 digits composed of letters and numbers";
    QString color = "red";
    QString pattern("^(?![0-9]+$)(?![a-zA-Z]+$)[0-9A-Za-z]{8,16}$");
    QRegExp rx(pattern);
    if(ui->passwordLineEdit->text().contains(rx)){
        image = "correct";
        content = "Verified!";
        color = "green";
    }
    ui->passwordLabel->setText(content);
    ui->passwordLabel->setStyleSheet(QString("QLabel {color: %1; background-image: url(:/pic/icon/%2.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;}").arg(color).arg(image));
}

void Register::confirmValidator()
{
    QString image = "error";
    QString content = "Passwords are inconsistent";
    QString color = "red";
    if(ui->confirmLineEdit->text() == ui->passwordLineEdit->text()){
        image = "correct";
        content = "Verified!";
        color = "green";
    }
    ui->confirmLabel->setText(content);
    ui->confirmLabel->setStyleSheet(QString("QLabel {color: %1; background-image: url(:/pic/icon/%2.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;}").arg(color).arg(image));
}

void Register::emailValidator()
{
    QString image = "error";
    QString content = "Illegal mailbox";
    QString color = "red";
    QString pattern("^[a-z0-9A-Z]+[- | a-z0-9A-Z . _]+@([a-z0-9A-Z]+(-[a-z0-9A-Z]+)?\\.)+[a-z]{2,}$");
    QRegExp rx(pattern);
    if(ui->emailLineEdit->text().contains(rx)){
        image = "correct";
        content = "Verified!";
        color = "green";
    }
    ui->emailLabel->setText(content);
    ui->emailLabel->setStyleSheet(QString("QLabel {color: %1; background-image: url(:/pic/icon/%2.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;}").arg(color).arg(image));
}
