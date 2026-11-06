#ifndef GAME_H
#define GAME_H

#include <QGraphicsScene>
#include <QTimer>
#include <QVector>
#include "ghost.h"
#include "pacman.h"


/* game options */
#define BALL_SCORE            10        // score of eating balls
#define POWERPELLET_SCORE     50        // score of eating powerpellets
#define BONUS_SCORE           200       // score of eating pannic ghosts
#define PACMAN_MOVE_INTERVAL  15        // move interval of pacman
#define NORMAL_INTERVAL       17        // move interval of normal ghosts
#define PANNIC_INTERVAL       20        // move interval of pannic ghosts
#define RUNNING_INTERVAL      5         // move interval of running eyes of ghosts
#define PANNIC_TIME           500       // interval number of pannic ghosts
#define FLASH_INTERVAL        250       // flash interval of powerpellets

/* Game control class */
class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    enum GameStatus {
        Playing,
        Win,
        Lose,
    };

    Game(int, int, int, int, QString);
    void start();
    void stop();
    void pacmanNextDirection(GameEntity::Direction);
    int getScore();
    ~Game();

    GameEntity*** map;                  // the map of pacman game
    GameEntity* gate;                   // pointer of the gate of ghost's cage
    Pacman* pacman;                     // pointer of pacman
    Ghost* ghost[Ghost::GhostNum];      // pointers of ghosts
    QVector<GameEntity*> powerpellet;   // pointers of powerpellets
    GameStatus stat;

    friend class Pacman;
    friend class Ghost;

private slots:
    void pacmanHandler();
    void powerpelletFlash();
    void ghostHandler(int);

private:
    int mapHeight, mapWidth;
    int mapSize;
    int ballNum, eatNum, score;
    int geo_x, geo_y;

    QTimer* ghostTimer[Ghost::GhostNum];
    QTimer* pacmanTimer;
    QTimer* powerpelletFlashTimer;
    bool flashTick;
};

#endif // GAME_H
