#ifndef RANK_H
#define RANK_H

#include <QDialog>
#include <QSqlQuery>
#include <qdebug.h>
#include <vector>
using namespace std;
namespace Ui {
class Rank;
}

class Rank : public QDialog
{
    Q_OBJECT

public:
    explicit Rank(QWidget *parent = nullptr);
    ~Rank();
    void select();
    vector<pair<QString, int>> list;

private:
    Ui::Rank *ui;

signals:
    void toMain();

private slots:
    void showRank();
    void on_backBtn_clicked();
};

#endif // RANK_H
