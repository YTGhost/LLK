#ifndef REGISTER_H
#define REGISTER_H

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
};

#endif // REGISTER_H
