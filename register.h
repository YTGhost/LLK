#ifndef REGISTER_H
#define REGISTER_H
#define TRUE 1
#define FALSE 0

#include <QDialog>

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private:
    Ui::Register *ui;

private slots:
    void showRegister();
    void userValidator();
    void passwordValidator();
    void emailValidator();
    void confirmValidator();
    void on_sendcodeBtn_clicked();
    void on_registerBtn_clicked();
};

#endif // REGISTER_H
