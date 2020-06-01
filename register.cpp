#include "register.h"
#include "ui_register.h"
#include <QPropertyAnimation>

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

    // ����GUI��ʽ
    QString dialogQSS = "QDialog {background:qlineargradient(spread:pad,x1:1,y1:0,x2:0,y2:0,stop:0 #0082bc,stop:1 #01d8db)}";
    QString widgetQSS = "QWidget {background-color: white; border-radius: 10px}";
    QString btnQSS = "QPushButton {background-color: #0082bc; color: #FFFFFF; border-radius: 8px; font-weight:bold;}";
    QString btnqss = "QPushButton {background-color: #0082bc; color: #FFFFFF; border-radius: 8px; font-weight:bold;}";
    QString StyleSheetUser = "QLineEdit { background-image: url(:/pic/icon/user.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;} QLineEdit:focus { background-image: url(:/pic/icon/user-focused.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left; }";
    QString StyleSheetPassword = "QLineEdit { background-image: url(:/pic/icon/lock.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;} QLineEdit:focus { background-image: url(:/pic/icon/lock-focused.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left; }";
    QString StyleSheetEmail = "QLineEdit { background-image: url(:/pic/icon/email.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;} QLineEdit:focus { background-image: url(:/pic/icon/email-focused.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left; }";
    QString StyleSheetCode = "QLineEdit { background-image: url(:/pic/icon/confirm.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;} QLineEdit:focus { background-image: url(:/pic/icon/confirm-focused.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left; }";

    ui->userLineEdit->setPlaceholderText(QString::fromLocal8Bit("�������û���"));
    ui->passwordLineEdit->setPlaceholderText(QString::fromLocal8Bit("��������"));
    ui->confirmLineEdit->setPlaceholderText(QString::fromLocal8Bit("���ٴ���������"));
    ui->emailLineEdit->setPlaceholderText(QString::fromLocal8Bit("����������"));
    ui->codeLineEdit->setPlaceholderText(QString::fromLocal8Bit("��д��֤��"));

    ui->userLineEdit->setClearButtonEnabled(true);
    ui->passwordLineEdit->setClearButtonEnabled(true);
    ui->confirmLineEdit->setClearButtonEnabled(true);
    ui->emailLineEdit->setClearButtonEnabled(true);

    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->confirmLineEdit->setEchoMode(QLineEdit::Password);

    this->setStyleSheet(dialogQSS);
    ui->widget->setStyleSheet(widgetQSS);
    ui->registerBtn->setStyleSheet(btnQSS);
    ui->sendcodeBtn->setStyleSheet(btnqss);
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
    QString content = QString::fromLocal8Bit("�û�������Ϊ1~10");
    QString color = "red";
    QString pattern("^.{1,10}$");
    QRegExp rx(pattern);
    username = "";
    if(ui->userLineEdit->text().contains(rx)){
        image = "correct";
        content = QString::fromLocal8Bit("������ȷ��");
        color = "green";
        username = ui->userLineEdit->text();
    }
    ui->userLabel->setText(content);
    ui->userLabel->setStyleSheet(QString("QLabel {color: %1; background-image: url(:/pic/icon/%2.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;}").arg(color).arg(image));
}

void Register::passwordValidator()
{
    QString image = "error";
    QString content = QString::fromLocal8Bit("��8��16λ��ĸ���������");
    QString color = "red";
    QString pattern("^(?![0-9]+$)(?![a-zA-Z]+$)[0-9A-Za-z]{8,16}$");
    QRegExp rx(pattern);
    if(ui->passwordLineEdit->text().contains(rx)){
        image = "correct";
        content = QString::fromLocal8Bit("������ȷ��");
        color = "green";
    }
    ui->passwordLabel->setText(content);
    ui->passwordLabel->setStyleSheet(QString("QLabel {color: %1; background-image: url(:/pic/icon/%2.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;}").arg(color).arg(image));
}

void Register::confirmValidator()
{
    QString image = "error";
    QString content = QString::fromLocal8Bit("�ٴ��������");
    QString color = "red";
    password = "";
    if(ui->confirmLineEdit->text() == ui->passwordLineEdit->text()){
        image = "correct";
        content = QString::fromLocal8Bit("������ȷ��");
        color = "green";
        password = ui->confirmLineEdit->text();
    }
    ui->confirmLabel->setText(content);
    ui->confirmLabel->setStyleSheet(QString("QLabel {color: %1; background-image: url(:/pic/icon/%2.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;}").arg(color).arg(image));
}

void Register::emailValidator()
{
    QString image = "error";
    QString content = QString::fromLocal8Bit("�����䲻�Ϸ�");
    QString color = "red";
    QString pattern("^[a-z0-9A-Z]+[- | a-z0-9A-Z . _]+@([a-z0-9A-Z]+(-[a-z0-9A-Z]+)?\\.)+[a-z]{2,}$");
    QRegExp rx(pattern);
    email = "";
    if(ui->emailLineEdit->text().contains(rx)){
        image = "correct";
        content = QString::fromLocal8Bit("������ȷ��");
        color = "green";
        email = ui->emailLineEdit->text();
    }
    ui->emailLabel->setText(content);
    ui->emailLabel->setStyleSheet(QString("QLabel {color: %1; background-image: url(:/pic/icon/%2.png); padding: 2 2 2 30; background-repeat: no-repeat; background-position: left;}").arg(color).arg(image));
}

void Register::on_sendcodeBtn_clicked()
{
    //����8λ��֤�룬���ִ�Сд
    srand((int)time(0));
    for(int i = 0;i<8;i++){
        switch (rand()%3) {
        case 0:
            temp = rand()%10 + '0';
            break;
        case 1:
           temp = rand()%26 + 'a';
            break;
        case 2:
            temp = rand()%26 + 'A';
            break;
        }
        ch[i] = temp;
    }

    if(email!=""){  //�����������ȷ����

        //�����ݿ��в����Ƿ��������
        QString s=QString("select email from user_info");
        QSqlQuery query;
        QString pas;
        query.exec(s);
        while (query.next())
        {
            pas=query.value(0).toString() ;
            if(email==pas){   //�������
                ui->emailLineEdit->setText(QString::fromLocal8Bit("�������Ѵ��ڣ�"));
                return;
            }
        }

        //�����ھͷ����ʼ�
        //send email  //send email  //send email


    }
}

void Register::on_registerBtn_clicked()
{    
    QString code;
    code = ui->codeLineEdit->text();
    QByteArray ba = password.toLatin1();
    char* chcode = ba.data();
    if(strcmp(ch,chcode)==0){
        codeflag = TRUE;
    }
    else{
        ui->codeLineEdit->setText(QString::fromLocal8Bit("��֤�����"));
    }

    if(email!=""&&username!=""&&password!=""&&codeflag){

        //�������ݿ�
        QString s = QString("insert into user_info values(" + username + "," + password + "," + email + ")");
        QSqlQuery query;
        int result = query.exec(s);
        if(result){
            //��ת����Ϸ����
        }
    }
}
