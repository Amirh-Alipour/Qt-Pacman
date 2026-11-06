#include <QString>
#include <QFile>
#include <QRandomGenerator>
#include "game.h"
#define W (GameEntity::Width)

int GHOST_RELEASE_TIME[] = {0, 200, 400, 600};

Game::Game(int x, int y, int map_w, int map_h, QString mapSrc)
    : QGraphicsScene(x, y, W * map_w, W * map_h)
{
    geo_x = x;
    geo_y = y;
    stat = Playing;

    /* Initialize map pointers */
    mapSize = map_w * map_h;
    mapWidth = map_w;
    mapHeight = map_h;
    map = new GameEntity**[mapHeight];
    for (int i = 0; i < mapHeight; i++) {
        map[i] = new GameEntity*[mapWidth];
        for (int j = 0; j < mapWidth; j++)
            map[i][j] = nullptr;
    }

    /* Initialize map graphics */
    ballNum = eatNum = score = 0;
    int ghost_i = 0;
    QPixmap wallpix(":/game_objects/map_objects/wall.png");
    QPixmap ballpix(":/game_objects/map_objects/dot.png");
    QPixmap powerpelletpix(":/game_objects/map_objects/power_ball.png");
    QPixmap gatepix(":/game_objects/map_objects/gate.png");
    QPixmap blankpix;
    QFile mapfile(mapSrc);
    mapfile.open(QIODevice::ReadOnly|QIODevice::Text);

    pacman = new Pacman();

    for (int i = 0; i < map_h; i++) {
        QByteArray line = mapfile.readLine();
        for (int j = 0; j < map_w; j++) {
            int tmp_x = x + (j * W);
            int tmp_y = y + (i * W);
            switch (line[j]) {
            case '1':
                map[i][j] = new GameEntity(GameEntity::Wall, wallpix);
                map[i][j]->setPos(tmp_x, tmp_y);
                addItem(map[i][j]);
                break;
            case '0':
                map[i][j] = new GameEntity(GameEntity::Ball, ballpix);
                map[i][j]->setScore(BALL_SCORE);
                map[i][j]->setPos(tmp_x, tmp_y);
                addItem(map[i][j]);
                ballNum++;
                break;
            case '4':
                map[i][j] = new GameEntity(GameEntity::PowerPellet, powerpelletpix);
                map[i][j]->setScore(POWERPELLET_SCORE);
                map[i][j]->setPos(tmp_x, tmp_y);
                addItem(map[i][j]);
                powerpellet.push_back(map[i][j]);
                ballNum++;
                break;
            case '3':
                map[i][j] = new GameEntity(GameEntity::Blank, blankpix);
                break;
            case '2':
                gate = new GameEntity(GameEntity::Gate, gatepix);
                gate->_x = j;
                gate->_y = i;
                gate->setPos(tmp_x, tmp_y);
                addItem(gate);
                map[i][j] = gate;
                break;
            case 'p':
                pacman = new Pacman();
                pacman->game = this;
                pacman->setZValue(4);
                pacman->setPos(tmp_x, tmp_y);
                addItem(pacman);
                map[i][j] = pacman;
                break;
            case 'g':
                map[i][j] = new GameEntity(GameEntity::Blank, blankpix);
                ghost[ghost_i] = new Ghost(ghost_i);
                ghost[ghost_i]->game = this;
                ghost[ghost_i]->setZValue(2);
                ghost[ghost_i]->releaseTime = GHOST_RELEASE_TIME[ghost_i];
                ghost[ghost_i]->_x = j;
                ghost[ghost_i]->_y = i;
                ghost[ghost_i]->setScore(BONUS_SCORE);
                ghost[ghost_i]->setPos(tmp_x, tmp_y);
                addItem(ghost[ghost_i]);
                ghost_i++;
                break;
            }
        }
    }

    ghost[Ghost::Blinky]->chaseStrategy = &strategy1;
    ghost[Ghost::Pinky]->chaseStrategy = &strategy2;
    ghost[Ghost::Inky]->chaseStrategy = &strategy3;
    ghost[Ghost::Clyde]->chaseStrategy = &strategy4;
}

void Game::start()
{
    powerpelletFlashTimer = new QTimer(this);
    connect(powerpelletFlashTimer, SIGNAL(timeout()), this , SLOT(powerpelletFlash()));
    powerpelletFlashTimer->start(FLASH_INTERVAL);

    pacmanTimer = new QTimer(this);
    connect(pacmanTimer, SIGNAL(timeout()), this , SLOT(pacmanHandler()));
    pacmanTimer->start(PACMAN_MOVE_INTERVAL);

    for (int i = 0; i < Ghost::GhostNum; i++) {
        ghostTimer[i] = new QTimer(this);
        // Pass ghost id to ghostHandler
        connect(ghostTimer[i], &QTimer::timeout, [=](){return ghostHandler(i);} );
        ghostTimer[i]->start(NORMAL_INTERVAL);
    }
}

void Game::stop()
{
    pacmanTimer->stop();
    powerpelletFlashTimer->stop();
    for (int i = 0; i < Ghost::GhostNum; i++) {
        ghostTimer[i]->stop();
    }
}

void Game::pacmanHandler()
{
    pacman->move();
    if (stat == Win) {
        stop();
    }
}

void Game::ghostHandler(int ghostId)
{
    ghost[ghostId]->move();
    if (stat == Lose) {
        stop();
    }
}

void Game::powerpelletFlash()
{
    if (powerpellet.empty()) {
        powerpelletFlashTimer->stop();
        return;
    }

    if (flashTick) {
        for (int i = 0; i < powerpellet.size(); i++) {
            powerpellet.at(i)->hide();
        }
        flashTick = false;
    }
    else {
        for (int i = 0; i < powerpellet.size(); i++) {
            powerpellet.at(i)->show();
        }
        flashTick = true;
    }
}

void Game::pacmanNextDirection(GameEntity::Direction d)
{
    pacman->setNextDir(d);
}

int Game::getScore()
{
    return score;
}

Game::~Game()
{
    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            if (map[i][j] != nullptr)
                delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
    delete pacmanTimer;
    delete powerpelletFlashTimer;
    for (int i = 0; i < Ghost::GhostNum; i++) {
        delete ghostTimer[i];
    }
}
