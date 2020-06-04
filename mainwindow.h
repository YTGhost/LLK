#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

signals:
    void toChoose();
    void toRank();
    void toLogin();

private slots:
    void showMain();
    void on_playBtn_clicked();
    void on_rankBtn_clicked();
    void on_backBtn_clicked();
};
#endif // MAINWINDOW_H
