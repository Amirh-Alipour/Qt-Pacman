#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QString>
#include <QKeyEvent>
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void initLabels();
    void keyPressEvent(QKeyEvent*) override;

private slots:
    void updateScore();

private:
    Ui::MainWindow* ui;
    QLabel* scoreTitle;
    QLabel* score;
    QLabel* winLabel, *loseLabel;
    QTimer* scoreTimer;
    Game* game;
};
#endif // MAINWINDOW_H
