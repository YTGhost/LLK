#ifndef CHOOSE_H
#define CHOOSE_H

#include <QDialog>

namespace Ui {
class Choose;
}

class Choose : public QDialog
{
    Q_OBJECT

public:
    explicit Choose(QWidget *parent = nullptr);
    ~Choose();

private:
    Ui::Choose *ui;

signals:
    void toPlay(int level);

private slots:
    void showChoose();
    void on_beginnerBtn_clicked();
    void on_intermediateBtn_clicked();
    void on_mediumBtn_clicked();
    void on_advancedBtn_clicked();
};

#endif // CHOOSE_H
