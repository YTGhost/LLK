#ifndef RANK_H
#define RANK_H

#include <QDialog>

namespace Ui {
class Rank;
}

class Rank : public QDialog
{
    Q_OBJECT

public:
    explicit Rank(QWidget *parent = nullptr);
    ~Rank();

private:
    Ui::Rank *ui;

private slots:
    void showRank();
};

#endif // RANK_H
