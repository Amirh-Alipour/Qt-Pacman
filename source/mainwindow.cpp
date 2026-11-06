#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Pac-Man"));

    /* Initialize graphicsview and game scene */
    ui->graphicsView->setStyleSheet("QGraphicsView {border: none;}");
    ui->graphicsView->setBackgroundBrush(Qt::black);
    ui->graphicsView->setFocusPolicy(Qt::NoFocus);

    int mapHeight = 20, mapWidth = 29;
    int x = 50, y = 50;
    int w = (mapWidth * GameEntity::Width);
    int h = (mapHeight * GameEntity::Width);

    ui->graphicsView->setGeometry(x, y, w, h);
    game = new Game(x, y, mapWidth, mapHeight, ":/game_objects/map_objects/map.txt");
    ui->graphicsView->setScene(game);
    initLabels();
    game->start();
}


/* Initialize UI */
void MainWindow::initLabels()
{
    scoreTitle = new QLabel(this);
    scoreTitle->setText("score");
    scoreTitle->setStyleSheet("QLabel {font-family: Fixedsys;color: white;font-size: 16px;}");
    scoreTitle->setGeometry(50, 12, 60, 26);

    score = new QLabel(this);
    score->setIndent(-80);
    score->setText("0");
    score->setStyleSheet("QLabel {font-family: Fixedsys;color: white;font-size: 16px;}");
    score->setGeometry(110, 12, 150, 26);

    winLabel = new QLabel(this);
    winLabel->hide();
    winLabel->setText("You Win");
    winLabel->setStyleSheet("QLabel {font-family: Fixedsys;color: yellow;font-size: 20px;}");
    winLabel->setGeometry(290, 12, 150, 26);

    loseLabel = new QLabel(this);
    loseLabel->hide();
    loseLabel->setText("Game Over");
    loseLabel->setStyleSheet("QLabel {font-family: Fixedsys;color: red;font-size: 20px;}");
    loseLabel->setGeometry(280, 12, 150, 26);

    scoreTimer = new QTimer(this);
    scoreTimer->start(25);
    connect(scoreTimer, SIGNAL(timeout()), this , SLOT(updateScore()));
}


/* Update score UI */
void MainWindow::updateScore()
{
    score->setText(QString::number(game->getScore()));
    if (game->stat == Game::Win) {
        winLabel->show();
        scoreTimer->stop();
    }
    else if (game->stat == Game::Lose) {
        loseLabel->show();
        scoreTimer->stop();
    }
}


void MainWindow::keyPressEvent(QKeyEvent* event) {
    if ((event->key() == Qt::Key_Up) || (event->key() == Qt::Key_W))
        game->pacmanNextDirection(GameEntity::Up);
    else if ((event->key() == Qt::Key_Down) || (event->key() == Qt::Key_S))
        game->pacmanNextDirection(GameEntity::Down);
    else if ((event->key() == Qt::Key_Left) || (event->key() == Qt::Key_A))
        game->pacmanNextDirection(GameEntity::Left);
    else if ((event->key() == Qt::Key_Right) || (event->key() == Qt::Key_D))
        game->pacmanNextDirection(GameEntity::Right);
}


MainWindow::~MainWindow()
{
    delete ui;
}
