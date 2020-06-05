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

    ui->emailLineEdit->setPlaceholderText(QString::fromLocal8Bit("�����������ַ"));
    ui->codeLineEdit->setPlaceholderText(QString::fromLocal8Bit("��������֤��"));
    ui->passwordLineEdit->setPlaceholderText(QString::fromLocal8Bit("����������"));
    ui->confirmLineEdit->setPlaceholderText(QString::fromLocal8Bit("��ȷ������"));

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
    // ��������������ʽ�Ƿ���ȷ
    if(checkEmail != 1){
        QMessageBox::warning(NULL, QString::fromLocal8Bit("����ʧ��"), QString::fromLocal8Bit("��������ȷ�������ַ"), QString::fromLocal8Bit("ȷ��"));
        return;
    }
    QString email = ui->emailLineEdit->text();
    // ��ѯ���ݿ��Ƿ���ڸ�����
    QString str = QString("select * from user_info where email = '%1'").arg(email);
    QSqlQuery query;
    query.exec(str);
    // ���û���ҵ�������
    if(!query.next()){
        QMessageBox::warning(NULL, QString::fromLocal8Bit("��֤ʧ��"), QString::fromLocal8Bit("��������ȷ�������ַ"), QString::fromLocal8Bit("ȷ��"));
        return;
    }
    // �������������
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    // ������֤��
    code = "";
    // ������֤��
    for(int i = 0; i < 6; i++)
    {
        code.append(QString::number(qrand() % 9));
    }
    // ���������֤��
    cout << code.toLocal8Bit().toStdString() << endl;
    Smtp *smtp = new Smtp();
    smtp->send(email.toUtf8(), QString::fromLocal8Bit("�������֤��"), QString::fromLocal8Bit("������֤��Ϊ:%1,ף����Ϸ���!^_^").arg(code));
    QMessageBox::information(NULL, QString::fromLocal8Bit("���ͳɹ�"), QString::fromLocal8Bit("��ǰ������鿴��֤��"), QString::fromLocal8Bit("ȷ��"));
}

void Forget::on_changeBtn_clicked()
{
    QString inputCode = ui->codeLineEdit->text();
    QString email = ui->emailLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    if(inputCode == "" || inputCode != code){
        QMessageBox::warning(NULL, QString::fromLocal8Bit("��֤ʧ��"), QString::fromLocal8Bit("��������ȷ����֤��"), QString::fromLocal8Bit("ȷ��"));
        return;
    }
    if(checkPassword == 0){
        QMessageBox::warning(NULL, QString::fromLocal8Bit("�����ʽ����"), QString::fromLocal8Bit("��������Ϲ��������"), QString::fromLocal8Bit("ȷ��"));
        return;
    }
    if(checkConfirm == 0){
        QMessageBox::warning(NULL, QString::fromLocal8Bit("�������벻һ��"), QString::fromLocal8Bit("�����¼������"), QString::fromLocal8Bit("ȷ��"));
        return;
    }
    QString str = QString("update user_info set password = '%1' where email = '%2'").arg(password).arg(email);
    QSqlQuery query;
    query.exec(str);
    QMessageBox::information(NULL, QString::fromLocal8Bit("�޸�����"), QString::fromLocal8Bit("�޸ĳɹ���������ʹ����������е�¼�ˣ�"), QString::fromLocal8Bit("ȷ��"));
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
